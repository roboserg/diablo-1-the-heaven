//#include "..\src\stdafx.h"
/*****************************************************************************/
/* SFileOpenArchive.cpp                       Copyright Ladislav Zezula 1999 */
/*                                                                           */
/* Author : Ladislav Zezula                                                  */
/* E-mail : ladik@zezula.net                                                 */
/* WWW    : www.zezula.net                                                   */
/*---------------------------------------------------------------------------*/
/*                       Archive functions of Storm.dll                      */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* xx.xx.xx  1.00  Lad  The first version of SFileOpenArchive.cpp            */
/* 19.11.03  1.01  Dan  Big endian handling                                  */
/*****************************************************************************/

#define __STORMLIB_SELF__
#include "StormLib.h"
#include "StormPort.h"
#include "SCommon.h"

/*****************************************************************************/
/* Local functions                                                           */
/*****************************************************************************/

static int IsAviFile(TMPQHeader * pHeader)
{
    uint * AviHdr = (uint*)pHeader;

    // Test for 'RIFF', 'AVI ' or 'LIST'
    return (AviHdr[0] == 'FFIR' && AviHdr[2] == ' IVA' && AviHdr[3] == 'TSIL');
}

// This function gets the right positions of the hash table and the block table.
static int RelocateMpqTablePositions(TMPQArchive * ha)
{
    TMPQHeader2 * pHeader = ha->pHeader;
    LARGE_INTEGER FileSize;
    LARGE_INTEGER TempSize;

    // Get the size of the file
    FileSize.LowPart = GetFileSize(ha->hFile, (LPDWORD)&FileSize.HighPart);

    // Set the proper hash table position
    ha->HashTablePos.HighPart = pHeader->wHashTablePosHigh;
    ha->HashTablePos.LowPart = pHeader->dwHashTablePos;
    ha->HashTablePos.QuadPart += ha->MpqPos.QuadPart;
    if(ha->HashTablePos.QuadPart > FileSize.QuadPart)
        return ERROR_BAD_FORMAT;

    // Set the proper block table position
    ha->BlockTablePos.HighPart = pHeader->wBlockTablePosHigh;
    ha->BlockTablePos.LowPart = pHeader->dwBlockTablePos;
    ha->BlockTablePos.QuadPart += ha->MpqPos.QuadPart;
    if(ha->BlockTablePos.QuadPart > FileSize.QuadPart)
        return ERROR_BAD_FORMAT;

    // Set the proper position of the extended block table
    if(pHeader->ExtBlockTablePos.QuadPart != 0)
    {
        ha->ExtBlockTablePos = pHeader->ExtBlockTablePos;
        ha->ExtBlockTablePos.QuadPart += ha->MpqPos.QuadPart;
        if(ha->ExtBlockTablePos.QuadPart > FileSize.QuadPart)
            return ERROR_BAD_FORMAT;
    }

    // Size of MPQ archive is computed as the biggest of
    // (EndOfBlockTable, EndOfHashTable, EndOfExtBlockTable)
    TempSize.QuadPart = ha->HashTablePos.QuadPart + (pHeader->dwHashTableSize * sizeof(TMPQHash));
    if(TempSize.QuadPart > ha->MpqSize.QuadPart)
        ha->MpqSize = TempSize;
    TempSize.QuadPart = ha->BlockTablePos.QuadPart + (pHeader->dwBlockTableSize * sizeof(TMPQBlock));
    if(TempSize.QuadPart > ha->MpqSize.QuadPart)
        ha->MpqSize = TempSize;
    TempSize.QuadPart = ha->ExtBlockTablePos.QuadPart + (pHeader->dwBlockTableSize * sizeof(TMPQBlockEx));
    if(TempSize.QuadPart > ha->MpqSize.QuadPart)
        ha->MpqSize = TempSize;
    
    // MPQ size does not include the bytes before MPQ header
    ha->MpqSize.QuadPart -= ha->MpqPos.QuadPart;
    return ERROR_SUCCESS;
}


/*****************************************************************************/
/* Public functions                                                          */
/*****************************************************************************/

//-----------------------------------------------------------------------------
// SFileGetLocale and SFileSetLocale
// Set the locale for all neewly opened archives and files

LCID __stdcall SFileGetLocale()
{
    return lcLocale;
}

LCID __stdcall SFileSetLocale_lib(LCID lcNewLocale)
{
    lcLocale = lcNewLocale;
    return lcLocale;
}

//-----------------------------------------------------------------------------
// SFileOpenArchiveEx (not a public function !!!)
//
//   szFileName - MPQ archive file name to open
//   dwPriority - When SFileOpenFileEx called, this contains the search priority for searched archives
//   dwFlags    - If contains MPQ_OPEN_NO_LISTFILE, then the internal list file will not be used.
//   phMPQ      - Pointer to store open archive handle
int SFileOpenArchiveEx( char * szMpqName, uint dwPriority, uint dwFlags, HANDLE * phMPQ, uint dwAccessMode)
{
	LARGE_INTEGER TempPos;
    TMPQArchive * curMpq = NULL;            // Archive handle
    HANDLE hFile = INVALID_HANDLE_VALUE;// Opened archive file handle
    uint dwMaxBlockIndex = 0;          // Maximum value of block entry
    uint dwBlockTableSize = 0;         // Block table size.
    uint dwTransferred;                // Number of bytes read
    uint dwBytes = 0;                  // Number of bytes to read
    int nError = ERROR_SUCCESS;   

    // Check the right parameters
    if(nError == ERROR_SUCCESS)
    {
        if(szMpqName == NULL || *szMpqName == 0 || phMPQ == NULL)
            nError = ERROR_INVALID_PARAMETER;
    }

    // Ensure that StormBuffer is allocated
    if(nError == ERROR_SUCCESS)
        nError = PrepareStormBuffer();

    // Open the MPQ archive file
    if(nError == ERROR_SUCCESS)
    {
        hFile = CreateFile(szMpqName, dwAccessMode, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if(hFile == INVALID_HANDLE_VALUE)
            nError = GetLastError();
    }
    
    // Allocate the MPQhandle
    if(nError == ERROR_SUCCESS)
    {
        if((curMpq = ALLOCMEM(TMPQArchive, 1)) == NULL)
            nError = ERROR_NOT_ENOUGH_MEMORY;
    }

    // Initialize handle structure and allocate structure for MPQ header
    if(nError == ERROR_SUCCESS)
    {
        memset(curMpq, 0, sizeof(TMPQArchive));
        strncpy(curMpq->szFileName, szMpqName, strlen(szMpqName));
        curMpq->hFile      = hFile;
        curMpq->dwPriority = dwPriority;
        curMpq->pHeader    = &curMpq->Header;
        curMpq->pListFile  = NULL;
        hFile = INVALID_HANDLE_VALUE;
    }

    // Find the offset of MPQ header within the file
    if(nError == ERROR_SUCCESS)
    {
        LARGE_INTEGER SearchPos = {0};
        LARGE_INTEGER MpqPos = {0};
        uint dwHeaderID;

        for(;;)
        {
            // Invalidate the MPQ ID and read the eventual header
            SetFilePointer(curMpq->hFile, MpqPos.LowPart, &MpqPos.HighPart, FILE_BEGIN);
            ReadFile(curMpq->hFile, curMpq->pHeader, sizeof(TMPQHeader2), (LPDWORD)&dwTransferred, NULL);
            dwHeaderID = BSWAP_INT32_UNSIGNED(curMpq->pHeader->dwID);

            // Special check : Some MPQs are actually AVI files, only with
            // changed extension.
            if(MpqPos.QuadPart == 0 && IsAviFile(curMpq->pHeader))
            {
                nError = ERROR_AVI_FILE;
                break;
            }

            // If different number of bytes read, break the loop
            if(dwTransferred != sizeof(TMPQHeader2))
            {
                nError = ERROR_BAD_FORMAT;
                break;
            }

            // If there is the MPQ shunt signature, process it
            if(dwHeaderID == ID_MPQ_SHUNT && curMpq->pShunt == NULL)
            {
                // Ignore the MPQ shunt completely if the caller wants to open the MPQ as V1.0
                if((dwFlags & MPQ_OPEN_FORCE_MPQ_V1) == 0)
                {
                    // Fill the shunt header
                    curMpq->ShuntPos = MpqPos;
                    curMpq->pShunt = &curMpq->Shunt;
                    memcpy(curMpq->pShunt, curMpq->pHeader, sizeof(TMPQShunt));
                    BSWAP_TMPQSHUNT(curMpq->pShunt);

                    // Set the MPQ pos and repeat the search
                    MpqPos.QuadPart = SearchPos.QuadPart + curMpq->pShunt->dwHeaderPos;
                    continue;
                }
            }

            // There must be MPQ header signature
            if(dwHeaderID == ID_MPQ)
            {
                BSWAP_TMPQHEADER(curMpq->pHeader);

                // Save the position where the MPQ header has been found
                curMpq->MpqPos = MpqPos;

                // If valid signature has been found, break the loop
                if(curMpq->pHeader->wFormatVersion == MPQ_FORMAT_VERSION_1)
                {
                    // W3M Map Protectors set some garbage value into the "dwHeaderSize"
                    // field of MPQ header. This value is apparently ignored by Storm.dll
                    if(curMpq->pHeader->dwHeaderSize != sizeof(TMPQHeader))
                    {
                        curMpq->dwFlags |= MPQ_FLAG_PROTECTED;
                        curMpq->pHeader->dwHeaderSize = sizeof(TMPQHeader);
                    }
					break;
                }

                if(curMpq->pHeader->wFormatVersion == MPQ_FORMAT_VERSION_2)
                {
                    // W3M Map Protectors set some garbage value into the "dwHeaderSize"
                    // field of MPQ header. This value is apparently ignored by Storm.dll
                    if(curMpq->pHeader->dwHeaderSize != sizeof(TMPQHeader2))
                    {
                        curMpq->dwFlags |= MPQ_FLAG_PROTECTED;
                        curMpq->pHeader->dwHeaderSize = sizeof(TMPQHeader2);
                    }
					break;
                }

				//
				// Note: the "dwArchiveSize" member in the MPQ header is ignored by Storm.dll
				// and can contain garbage value ("w3xmaster" protector)
				// 
                
                nError = ERROR_NOT_SUPPORTED;
                break;
            }

            // Move to the next possible offset
            SearchPos.QuadPart += 0x200;
            MpqPos = SearchPos;
        }
    }

    // Relocate tables position
    if(nError == ERROR_SUCCESS)
    {
        // W3x Map Protectors use the fact that War3's StormLib ignores the file shunt,
        // and probably ignores the MPQ format version as well. The trick is to
        // fake MPQ format 2, with an improper hi-word position of hash table and block table
        // We can overcome such protectors by forcing opening the archive as MPQ v 1.0
        if(dwFlags & MPQ_OPEN_FORCE_MPQ_V1)
        {
            curMpq->pHeader->wFormatVersion = MPQ_FORMAT_VERSION_1;
            curMpq->pHeader->dwHeaderSize = sizeof(TMPQHeader);
            curMpq->pShunt = NULL;
        }

        // Clear the fields not supported in older formats
        if(curMpq->pHeader->wFormatVersion < MPQ_FORMAT_VERSION_2)
        {
            curMpq->pHeader->ExtBlockTablePos.QuadPart = 0;
            curMpq->pHeader->wBlockTablePosHigh = 0;
            curMpq->pHeader->wHashTablePosHigh = 0;
        }

        curMpq->dwBlockSize = (0x200 << curMpq->pHeader->wBlockSize);
        nError = RelocateMpqTablePositions(curMpq);
    }

    // Allocate buffers
    if(nError == ERROR_SUCCESS)
    {
        //
        // Note that the block table should be as large as the hash table
        // (For later file additions).
        //
        // I have found a MPQ which has the block table larger than
        // the hash table. We should avoid buffer overruns caused by that.
        //
        
        if(curMpq->pHeader->dwBlockTableSize > curMpq->pHeader->dwHashTableSize)
            curMpq->pHeader->dwBlockTableSize = curMpq->pHeader->dwHashTableSize;
        dwBlockTableSize   = curMpq->pHeader->dwHashTableSize;

        curMpq->pHashTable     = ALLOCMEM(TMPQHash, curMpq->pHeader->dwHashTableSize);
        curMpq->pBlockTable    = ALLOCMEM(TMPQBlock, dwBlockTableSize);
        curMpq->pExtBlockTable = ALLOCMEM(TMPQBlockEx, dwBlockTableSize);
        curMpq->pbBlockBuffer  = ALLOCMEM(BYTE, curMpq->dwBlockSize);

        if(!curMpq->pHashTable || !curMpq->pBlockTable || !curMpq->pExtBlockTable || !curMpq->pbBlockBuffer)
            nError = ERROR_NOT_ENOUGH_MEMORY;
    }

    // Read the hash table into memory
    if(nError == ERROR_SUCCESS)
    {
        dwBytes = curMpq->pHeader->dwHashTableSize * sizeof(TMPQHash);
        SetFilePointer(curMpq->hFile, curMpq->HashTablePos.LowPart, &curMpq->HashTablePos.HighPart, FILE_BEGIN);
        ReadFile(curMpq->hFile, curMpq->pHashTable, dwBytes, (LPDWORD)&dwTransferred, NULL);

        if(dwTransferred != dwBytes)
            nError = ERROR_FILE_CORRUPT;
    }

    // Decrypt hash table and check if it is correctly decrypted
    if(nError == ERROR_SUCCESS)
    {
//      TMPQHash * pHashEnd = ha->pHashTable + ha->pHeader->dwHashTableSize;
//      TMPQHash * pHash;

        // We have to convert the hash table from LittleEndian
        BSWAP_ARRAY32_UNSIGNED((uint *)curMpq->pHashTable, (dwBytes / sizeof(uint)));
        DecryptHashTable((uint *)curMpq->pHashTable, (BYTE *)"(hash table)", (curMpq->pHeader->dwHashTableSize * 4));

        //
        // Check hash table if is correctly decrypted
        // 
        // Ladik: Some MPQ protectors corrupt the hash table by rewriting part of it.
        // To be able to open these, we will not check the entire hash table,
        // but will check it at the moment of file opening.
        // 
    }

    // Now, read the block table
    if(nError == ERROR_SUCCESS)
    {
        memset(curMpq->pBlockTable, 0, dwBlockTableSize * sizeof(TMPQBlock));

        // Carefully check the block table size
        dwBytes = curMpq->pHeader->dwBlockTableSize * sizeof(TMPQBlock);
        SetFilePointer(curMpq->hFile, curMpq->BlockTablePos.LowPart, &curMpq->BlockTablePos.HighPart, FILE_BEGIN);
        ReadFile(curMpq->hFile, curMpq->pBlockTable, dwBytes, (LPDWORD)&dwTransferred, NULL);

        // I have found a MPQ which claimed 0x200 entries in the block table,
        // but the file was cut and there was only 0x1A0 entries.
        // We will handle this case properly, even if that means 
        // omiting another integrity check of the MPQ
        if(dwTransferred < dwBytes)
            dwBytes = dwTransferred;
        BSWAP_ARRAY32_UNSIGNED((uint *)curMpq->pBlockTable, dwBytes / sizeof(uint));

        // If nothing was read, we assume the file is corrupt.
        if(dwTransferred == 0)
            nError = ERROR_FILE_CORRUPT;
    }

    // Decrypt block table.
    // Some MPQs don't have the block table decrypted, e.g. cracked Diablo version
    // We have to check if block table is really encrypted
    if(nError == ERROR_SUCCESS)
    {
        TMPQBlock * pBlockEnd = curMpq->pBlockTable + (dwBytes / sizeof(TMPQBlock));
        TMPQBlock * pBlock = curMpq->pBlockTable;
        int bBlockTableEncrypted = FALSE;

        // Verify all blocks entries in the table
        // The loop usually stops at the first entry
        while(pBlock < pBlockEnd)
        {
            // The lower 8 bits of the MPQ flags are always zero.
            // Note that this may change in next MPQ versions
            if(pBlock->dwFlags & 0x000000FF)
            {
                bBlockTableEncrypted = TRUE;
                break;
            }

            // Move to the next block table entry
            pBlock++;
        }

        if(bBlockTableEncrypted)
        {
            DecryptBlockTable((uint *)curMpq->pBlockTable,
                               (BYTE *)"(block table)",
                                       (dwBytes / sizeof(uint)));
        }
    }

    // Now, read the extended block table.
    // For V1 archives, we still will maintain the extended block table
    // (it will be filled with zeros)
    if(nError == ERROR_SUCCESS)
    {
        memset(curMpq->pExtBlockTable, 0, dwBlockTableSize * sizeof(TMPQBlockEx));

        if(curMpq->pHeader->ExtBlockTablePos.QuadPart != 0)
        {
            dwBytes = curMpq->pHeader->dwBlockTableSize * sizeof(TMPQBlockEx);
            SetFilePointer(curMpq->hFile,
                           curMpq->ExtBlockTablePos.LowPart,
                          &curMpq->ExtBlockTablePos.HighPart,
                           FILE_BEGIN);
            ReadFile(curMpq->hFile, curMpq->pExtBlockTable, dwBytes, (LPDWORD)&dwTransferred, NULL);

            // We have to convert every uint in ha->block from LittleEndian
            BSWAP_ARRAY16_UNSIGNED((USHORT *)curMpq->pExtBlockTable, dwBytes / sizeof(USHORT));

            // The extended block table is not encrypted (so far)
            if(dwTransferred != dwBytes)
                nError = ERROR_FILE_CORRUPT;
        }
    }

    // Verify both block tables (If the MPQ file is not protected)
    if(nError == ERROR_SUCCESS && (curMpq->dwFlags & MPQ_FLAG_PROTECTED) == 0)
    {
        TMPQBlockEx * pBlockEx = curMpq->pExtBlockTable;
        TMPQBlock * pBlockEnd = curMpq->pBlockTable + dwMaxBlockIndex + 1;
        TMPQBlock * pBlock   = curMpq->pBlockTable;

        // If the MPQ file is not protected,
        // we will check if all sizes in the block table is correct.
        // Note that we will not relocate the block table (change from previous versions)
        for(; pBlock < pBlockEnd; pBlock++, pBlockEx++)
        {
            if(pBlock->dwFlags & MPQ_FILE_EXISTS)
            {
                // Get the 64-bit file position
                TempPos.HighPart = pBlockEx->wFilePosHigh;
                TempPos.LowPart = pBlock->dwFilePos;

                if(TempPos.QuadPart > curMpq->MpqSize.QuadPart || pBlock->dwCSize > curMpq->MpqSize.QuadPart)
                {
                    nError = ERROR_BAD_FORMAT;
                    break;
                }
            }
        }
    }

    // If the caller didn't specified otherwise, 
    // include the internal listfile to the TMPQArchive structure
    if(nError == ERROR_SUCCESS)
    {
        if((dwFlags & MPQ_OPEN_NO_LISTFILE) == 0)
        {
            if(nError == ERROR_SUCCESS)
                SListFileCreateListFile(curMpq);

            // Add the internal listfile
            if(nError == ERROR_SUCCESS)
                SFileAddListFile((HANDLE)curMpq, NULL);
        }
    }

    // If the caller didn't specified otherwise, 
    // load the "(attributes)" file
    if(nError == ERROR_SUCCESS && (dwFlags & MPQ_OPEN_NO_ATTRIBUTES) == 0)
    {
        // Ignore the result here. Attrobutes are not necessary,
        // if they are not there, we will just ignore them
        SAttrFileLoad(curMpq);
    }

    // Cleanup and exit
    if(nError != ERROR_SUCCESS)
    {
        FreeMPQArchive(curMpq);
        if(hFile != INVALID_HANDLE_VALUE)
            CloseHandle(hFile);
        SetLastError(nError);
        curMpq = NULL;
    }
    else
    {
        if(pFirstOpen == NULL)
            pFirstOpen = curMpq;
    }
    *phMPQ = curMpq;
    return (nError == ERROR_SUCCESS);
}

int __stdcall SFileOpenArchive_lib( char * szMpqName, uint dwPriority, uint dwFlags, HANDLE * phMPQ)
{
    return SFileOpenArchiveEx(szMpqName, dwPriority, dwFlags, phMPQ, GENERIC_READ);
}

//-----------------------------------------------------------------------------
// int SFileFlushArchive(HANDLE hMpq)
//
// Saves all dirty data into MPQ archive.
// Has similar effect like SFileCLoseArchive, but the archive is not closed.
// Use on clients who keep MPQ archive open even for write operations,
// and terminating without calling SFileCloseArchive might corrupt the archive.
//

int __stdcall SFileFlushArchive(HANDLE hMpq)
{
    TMPQArchive * ha = (TMPQArchive *)hMpq;
    
    // Do nothing if 'hMpq' is bad parameter
    if(!IsValidMpqHandle(ha))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    // If the archive has been changed, update the changes
    // on the disk drive.
    if(ha->dwFlags & MPQ_FLAG_CHANGED)
    {
        SListFileSaveToMpq(ha);
        SAttrFileSaveToMpq(ha);
        SaveMPQTables(ha);
        ha->dwFlags &= ~MPQ_FLAG_CHANGED;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
// int SFileCloseArchive(HANDLE hMPQ);
//

int __stdcall SFileCloseArchive_lib(HANDLE hMPQ)
{
    TMPQArchive * ha = (TMPQArchive *)hMPQ;
    
    // Flush all unsaved data to the storage
    if(!SFileFlushArchive(hMPQ))
        return FALSE;

    // Free all memory used by MPQ archive
    FreeMPQArchive(ha);
    return TRUE;
}

