//#pragma once
#ifndef _SCommon_h_
#define _SCommon_h_
#else
#error reinclude SCommon.h
#endif

/*****************************************************************************/
/* SCommon.h                              Copyright (c) Ladislav Zezula 2003 */
/*---------------------------------------------------------------------------*/
/* Common functions for encryption/decryption from Storm.dll. Included by    */
/* SFile*** functions, do not include and do not use this file directly      */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 24.03.03  1.00  Lad  The first version of SFileCommon.h                   */
/* 12.06.04  1.00  Lad  Renamed to SCommon.h                                 */
/*****************************************************************************/

//-----------------------------------------------------------------------------
// StormLib private defines

#define SFILE_TYPE_DATA  0              // Process the file as data file
#define SFILE_TYPE_WAVE  1              // Process the file as WAVe file

#define LISTFILE_ENTRY_DELETED   (DWORD_PTR)(-2)
#define LISTFILE_ENTRY_FREE      (DWORD_PTR)(-1)

// Prevent problems with CRT "min" and "max" functions,
// as they are not defined on all platforms
#define STORMLIB_MIN(a, b) ((a < b) ? a : b)
#define STORMLIB_MAX(a, b) ((a > b) ? a : b)

//-----------------------------------------------------------------------------
// External variables

extern TMPQArchive * pFirstOpen;
extern LCID          lcLocale;

//-----------------------------------------------------------------------------
// Encryption and decryption functions

int   PrepareStormBuffer();

void  EncryptHashTable(uint * pdwTable, BYTE * pbKey, uint dwLength);
void  DecryptHashTable(uint * pdwTable, BYTE * pbKey, uint dwLength);
TMPQHash * FindFreeHashEntry(TMPQArchive * ha,  char * szFileName);

void  EncryptBlockTable(uint * pdwTable, BYTE * pbKey, uint dwLength);
void  DecryptBlockTable(uint * pdwTable, BYTE * pbKey, uint dwLength);

uint DetectFileSeed(uint * block, uint decrypted);
uint DetectFileSeed2(uint * block, UINT nDwords, ...);
void  EncryptMPQBlock(uint * pdwBlock, uint dwLength, uint dwSeed1);
void  DecryptMPQBlock(uint * pdwBlock, uint dwLength, uint dwSeed1);

uint DecryptHashIndex(TMPQArchive * ha,  char * szFileName);
uint DecryptName1    ( char * szFileName);
uint DecryptName2    ( char * szFileName);
uint DecryptFileSeed ( char * szFileName);

TMPQHash * GetHashEntry  (TMPQArchive * ha,  char * szFileName);
TMPQHash * GetHashEntryEx(TMPQArchive * ha,  char * szFileName, LCID lcLocale);

//-----------------------------------------------------------------------------
// Compression and decompression functions

int Compress_pklib  (char * pbOutBuffer, int * pdwOutLength, char * pbInBuffer, int dwInLength, int * pCmpType, int nCmpLevel);
int Decompress_pklib(char * pbOutBuffer, int * pdwOutLength, char * pbInBuffer, int dwInLength);

//-----------------------------------------------------------------------------
// Checking functions

int IsValidMpqHandle(TMPQArchive * ha);
int IsValidFileHandle(TMPQFile * hf);

//-----------------------------------------------------------------------------
// Other functions

int SFileOpenArchiveEx( char * szMpqName, uint dwPriority, uint dwFlags, HANDLE * phMPQ, uint dwAccessMode = GENERIC_READ);
int  AddInternalFile(TMPQArchive * ha,  char * szFileName);
int  AddFileToArchive(TMPQArchive * ha, HANDLE hFile,  char * szArchivedName, uint dwFlags, uint dwQuality, int nFileType, int * pbReplaced);
int  SetDataCompression(int nDataCompression);
int  SaveMPQTables(TMPQArchive * ha);
void FreeMPQArchive(TMPQArchive *& ha);
void FreeMPQFile(TMPQFile *& hf);

int CheckWildCard( char * szString,  char * szWildCard);

//-----------------------------------------------------------------------------
// Attributes support

int  SAttrFileCreate(TMPQArchive * ha);
int  SAttrFileLoad(TMPQArchive * ha);
int  SAttrFileSaveToMpq(TMPQArchive * ha);
void FreeMPQAttributes(TMPQAttr * pAttr);

//-----------------------------------------------------------------------------
// Listfile functions

int  SListFileCreateListFile(TMPQArchive * ha);
int  SListFileCreateNode(TMPQArchive * ha,  char * szFileName, LCID lcLocale);
int  SListFileRemoveNode(TMPQArchive * ha,  char * szFileName, LCID lcLocale);
void SListFileFreeListFile(TMPQArchive * ha);

int  SListFileSaveToMpq(TMPQArchive * ha);
