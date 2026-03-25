#include "stdafx.h"

//----- (004452F2) --------------------------------------------------------
bool __fastcall MpqSetAttributes(char* archiveName, bool hide)
{
	uint attr = GetFileAttributes(archiveName);
	if( attr == INVALID_FILE_ATTRIBUTES ){
		return GetLastError() == ERROR_FILE_NOT_FOUND;
	}
	uint newAttr = hide ? MPQ_HIDE_ATTR : 0;
	if( attr == newAttr ) return true;
	return SetFileAttributes(archiveName, newAttr);
}

//----- (0044532E) --------------------------------------------------------
void __fastcall MpqUpdateCreationTimeStamp(char* archiveName, int heroSlot)
{
	// only update registry stamps for multiplayer mode
	if( MaxCountOfPlayersInGame == 1 ) return;

	// get archive timestamps from registry
	FILETIME ft[MaxSaveSlots][NUM_TIMES];
	RegGetStamps(&ft[0][0],sizeof ft);

	// get archive file timestamps
	WIN32_FIND_DATA findData;
	HANDLE findhandle = FindFirstFile(archiveName, &findData);
	if( findhandle == INVALID_HANDLE_VALUE ) return;
	FindClose(findhandle);

	// update timestamp
	ft[heroSlot][CREATION_TIME] = findData.ftCreationTime;

	RegSetStamps(&ft[0][0],sizeof ft);
}

//----- (004453A1) --------------------------------------------------------
bool __fastcall RegGetStamps(FILETIME* ftime, uint size)
{
	memset(ftime, 0, size);
	uint read;
	if( ! SRegLoadData("Hellfire", "Video Player ", 0, ftime, size, &read) || read != size ) return false;
	while( size >= sizeof(FILETIME) ){
		XorTimeStamp(ftime++);
		size -= sizeof(FILETIME);
	}
	return true;
}

//----- (004453F7) --------------------------------------------------------
void __fastcall XorTimeStamp(FILETIME* ftime)
{
	DWORD dwKey = 0xf0761ab;
	LPBYTE lpStamp = (LPBYTE) ftime;
	DWORD dwBytes = sizeof(FILETIME);
	while (dwBytes--) {
		*lpStamp++ ^= (BYTE) dwKey;
		dwKey = _rotl(dwKey, 1);
	}
}

//----- (00445416) --------------------------------------------------------
void __fastcall RegSetStamps(FILETIME* ftime, uint size)
{
	// only update registry stamps for multiplayer mode
	FILETIME* ft = ftime;
	for( uint d = size; d >= sizeof(FILETIME); d -= sizeof(FILETIME)){
		XorTimeStamp(ft++);
	}
	SRegSaveData("Hellfire", "Video Player ", 0, ftime, size);
}

//----- (00445454) --------------------------------------------------------
void __fastcall MpqDeleteFile(char* fileName)
{
	uint dwEntry = SearchHashName(fileName);
	if( dwEntry == HASH_ENTRY_UNUSED ) return;

	HashEntry*  hash  = HashTbl + dwEntry;
	BlockEntry* block = BlockTbl + hash->block;

	// we can't set the hash block to HASH_BLOCK_UNUSED, because
	// we use closed hashing.  Another file may have hashed
	// to the same spot, and so was added to the hash table
	// after this hash entry.  Therefore, just mark the hash
	// entry as freed, but not unused
	hash->block = HASH_BLOCK_FREED;

	// free the block entry and give back the memory
	uint dwOffset = block->offset;
	uint dwSize = block->sizealloc;
	memset(block, 0, sizeof(*block));
	AddFreeBlock(dwOffset, dwSize);

	// modified the file
	SaveChanged = true;
}

//----- (004454A6) --------------------------------------------------------
void __fastcall AddFreeBlock(uint offset, uint size)
{
	// see if we can merge this block with an existing free block
	BlockEntry* block = BlockTbl;
	for( uint i = BLOCK_ENTRIES; i--; block++ ){
		// is this block unused?
		if( block->offset == 0 || block->flags != 0 || block->sizefile != 0) continue;

		if( block->offset + block->sizealloc == offset ){
			offset = block->offset;
			size += block->sizealloc;
		}else if( offset + size == block->offset ){
			size += block->sizealloc;
		}else continue;

		// try adding the new larger block.
		// NOTE: in the worst case, we free a block which is between
		// two free blocks, which causes only one deep recursion
		memset(block, 0, sizeof(*block));
		AddFreeBlock(offset, size);
		return;
	}

	if( offset + size > NextFileStart)
		TerminateWithError("MPQ free list error");

	// is this block at the end of the file?
	if (offset + size == NextFileStart) {
		NextFileStart = offset;
		return;
	}

	// create a new block entry
	block = GetFreeBlock(0);
	block->offset = offset;
	block->sizealloc = size;
	block->sizefile = 0;
	block->flags = 0;
}

//----- (00445536) --------------------------------------------------------
BlockEntry* __fastcall GetFreeBlock(uint* pdwBlockIndex)
{
	BlockEntry* pBlk = BlockTbl;
	for (DWORD i = 0; i < BLOCK_ENTRIES; i++, pBlk++) {
		if( pBlk->offset
		 || pBlk->sizealloc
		 || pBlk->flags
		 || pBlk->sizefile)
			continue;
		if( pdwBlockIndex ) *pdwBlockIndex = i;
		return pBlk;
	}

	TerminateWithError("Out of free block entries");
	return nullptr;
}

//----- (00445575) --------------------------------------------------------
uint __fastcall SearchHashName(char* key)
{
	uint hashIndex = Hash( key, 2 );
	uint hash1 = Hash( key, 1 );
	uint hash2 = Hash( key, 0 );
	return SearchHashEntry( hash2, hash1, hashIndex, 0 );
}

//----- (004455A5) --------------------------------------------------------
uint __fastcall SearchHashEntry( uint hashindex, uint hashcheck0, uint hashcheck1, uint lcid )
{
	DWORD dwCount = HASH_ENTRIES;
	for( uint entry = hashindex & (HASH_ENTRIES - 1); HashTbl[entry].block != HASH_BLOCK_UNUSED;
		entry = (entry + 1) & (HASH_ENTRIES - 1)
		) {
		if (! dwCount--) break;

		if( HashTbl[entry].hashcheck[0] != hashcheck0
		 || HashTbl[entry].hashcheck[1] != hashcheck1
		 || HashTbl[entry].lcid != lcid
		 || HashTbl[entry].block == HASH_BLOCK_FREED)
			continue;
		return entry;
	}
	return HASH_ENTRY_UNUSED;
}
//----- (00445613) --------------------------------------------------------
void __fastcall MpqDeleteFiles(int (__stdcall *getFileNames)(int, char(&)[MAX_PATH]))
{
	char fileName[MAX_PATH];
	int fileIndex = 0;
	while( getFileNames(fileIndex++, fileName) ){
		MpqDeleteFile(fileName);
	}
}

//----- (0044564E) --------------------------------------------------------
bool __fastcall MpqAddFile(char* fileName, uchar* data, uint len)
{
	SaveChanged = 1;
	MpqDeleteFile(fileName); // delete any existing file with the same name
	BlockEntry* block = InsertIntoHash(fileName, 0, 0);
	if( WriteFileData(fileName, data, len, block) ){ // insert the new file data
		return true;
	}else{
		MpqDeleteFile(fileName);
		return false;
	}
}

//----- (00445693) --------------------------------------------------------
BlockEntry* __fastcall InsertIntoHash(char* pszName, BlockEntry* pBlk, uint dwBlock)
{
	uint hashindex  = Hash(pszName,HASH_INDEX);
	uint hashcheck0 = Hash(pszName,HASH_CHECK0);
	uint hashcheck1 = Hash(pszName,HASH_CHECK1);

	if( HASH_ENTRY_UNUSED != SearchHashEntry( hashindex, hashcheck0, hashcheck1, MAKELCID(MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL),SORT_DEFAULT) ) ){
		TerminateWithError("Hash collision between \"%s\" and existing file\n", pszName);
	}
	// find free slot in hash table
	long lCount = HASH_ENTRIES;
	DWORD entry = hashindex & (HASH_ENTRIES - 1);
	while (lCount--) {
		if( HashTbl[entry].block == HASH_BLOCK_UNUSED ) break;
		if( HashTbl[entry].block == HASH_BLOCK_FREED  ) break;
		entry = (entry + 1) & (HASH_ENTRIES - 1);
	}
	if (lCount < 0) TerminateWithError("Out of hash space");

	if( ! pBlk ) pBlk = GetFreeBlock(&dwBlock);
	HashTbl[entry].hashcheck[0]	= hashcheck0;
	HashTbl[entry].hashcheck[1]	= hashcheck1;
	HashTbl[entry].lcid			= MAKELCID(MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL),SORT_DEFAULT); // 0
	HashTbl[entry].block		= dwBlock;
	return pBlk;
}

//----- (00445769) --------------------------------------------------------
bool __fastcall WriteFileData(char* fileName, uchar* data, uint len, BlockEntry* block) // TODO: BLOCKENTRY* blockAddr
{
	// DETERMINE THE FILE NAME PORTION OF THE PATH NAME
	char * pszTemp;
	while (NULL != (pszTemp = strchr(fileName,':')))
		fileName = pszTemp + 1;
	while (NULL != (pszTemp = strchr(fileName,'\\')))
		fileName = pszTemp + 1;

	// CREATE AN ENCRYPTION KEY BASED ON THE FILE NAME
	DWORD key = Hash(fileName,HASH_ENCRYPTKEY);
	DWORD sectors = (len + SECTORSIZE - 1) / SECTORSIZE;
	DWORD sectoroffsetsize = (sectors + 1) * sizeof(DWORD);

	// find free space for file
	DWORD dwSizeAllocGuess = len + sectoroffsetsize;
	block->offset = MpqGetFreeSpace(dwSizeAllocGuess, &block->sizealloc);
	block->sizefile = len;
	block->flags = MPQ_ADD_COMPRESSED | MPQ_ADD_ALLOCATED;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(CurrentSaveArchiveHandle, block->offset, NULL, FILE_BEGIN))
		return FALSE;

	DWORD sector = 0;
	DWORD destsize = 0;
	static BYTE buffer[SECTORSIZE];
	LPDWORD sectoroffsettable = NULL;
	while (len) {
		for (int loop = 0; loop < SECTORSIZE; ++loop)
			buffer[loop] += 0xAA;

		// PERFORM COMPRESSION
		DWORD bytes = min(len, (uint)SECTORSIZE);
		memcpy(buffer, data, bytes);
		data += bytes;
		bytes = Compress(buffer,bytes);

		// LEAVE SPACE FOR THE SECTOR OFFSET TABLE.
		if (! sector) {
			sectoroffsetsize = (sectors+1)*sizeof(DWORD);
			sectoroffsettable = (LPDWORD) malloc(sectoroffsetsize);
			memset(sectoroffsettable, 0, sectoroffsetsize);
			if (! WriteFile(CurrentSaveArchiveHandle, sectoroffsettable, sectoroffsetsize, &sectoroffsetsize, NULL))
				goto error;
			destsize += sectoroffsetsize;
		}

		// SAVE THE SECTOR OFFSET
		//app_assert(sectoroffsettable);
		*(sectoroffsettable+sector) = destsize;

		// PERFORM ENCRYPTION -- not encrypted for save files
		// Encrypt((LPDWORD)buffer,bytes & 0xFFFFFFFC,key+sector);

		// WRITE THE SECTOR
		if (! WriteFile(CurrentSaveArchiveHandle,buffer,bytes,&bytes,NULL))
			goto error;

		++sector;
		if (len > SECTORSIZE)
			len -= SECTORSIZE;
		else
			len = 0;
		destsize += bytes;
	}

	//app_assert(sectoroffsettable);
	*(sectoroffsettable+sector) = destsize;
	// bug in mopaq 1.91 -- fixed in 1.92
	// sector table is not supposed to be encrypted for unencrypted files
	// Encrypt(sectoroffsettable,sectoroffsetsize,key+0xFFFFFFFF);
	if (INVALID_SET_FILE_POINTER == SetFilePointer(CurrentSaveArchiveHandle,-(LONG)destsize,NULL,FILE_CURRENT))
		goto error;
	if (! WriteFile(CurrentSaveArchiveHandle,sectoroffsettable,sectoroffsetsize,&sectoroffsetsize,NULL))
		goto error;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(CurrentSaveArchiveHandle,destsize-sectoroffsetsize,NULL,FILE_CURRENT))
		goto error;
	free(sectoroffsettable); sectoroffsettable = 0;

	// make sure the file fit into the hole we provided
	//app_assert(destsize <= block->sizealloc);

	// give back any extra space we might have allocated
	if (destsize < block->sizealloc) {
		DWORD dwLeftover = block->sizealloc - destsize;
		if (dwLeftover >= MIN_FREE_SIZE) {
			block->sizealloc = destsize;
			AddFreeBlock(block->offset + destsize, dwLeftover);
		}
	}

	return true;
error:
	free(sectoroffsettable);
	return false;

}

//----- (00445971) --------------------------------------------------------
uint __fastcall MpqGetFreeSpace(uint size, uint* sizeAlloc)
{
	uint offset;

	// see if there is a space large enough in an existing block
	BlockEntry* block = BlockTbl;
	for( uint i = BLOCK_ENTRIES; i--; block++ ){
		// is this block unused?
		if( block->offset == 0 || block->flags != 0 || block->sizefile != 0 || block->sizealloc < size ) continue;

		// use a portion of this block
		offset = block->offset;
		*sizeAlloc = size;

		// fixup this block
		block->offset += size;
		block->sizealloc -= size;

		// did we use the entire block?
		if (! block->sizealloc) memset(block, 0, sizeof(BlockEntry));

		return offset;
	}

	// use free space at end of .MPQ file
	*sizeAlloc = size;
	offset = NextFileStart;
	NextFileStart += size;
	return offset;
}

//----- (004459C9) --------------------------------------------------------
void __fastcall MpqRenameFile(char* pszOld, char* pszNew)
{
	uint dwEntry = SearchHashName(pszOld);
	if( dwEntry == HASH_ENTRY_UNUSED ) return;

	HashEntry* pHash = HashTbl + dwEntry;
	BlockEntry* pBlk = BlockTbl + pHash->block;
	uint dwBlock = pHash->block;

	// we can't set the hash block to HASH_BLOCK_UNUSED, because
	// we use closed hashing.  Another file may have hashed
	// to the same spot, and so was added to the hash table
	// after this hash entry.  Therefore, just mark the hash
	// entry as freed, but not unused
	pHash->block = HASH_BLOCK_FREED;

	// create a new hash entry which references the existing block
	InsertIntoHash(pszNew, pBlk, dwBlock);

	// modified the file
	SaveChanged = true;
}

//----- (00445A08) --------------------------------------------------------
bool __fastcall MpqFileExists(char* pszName)
{
	return SearchHashName(pszName) != HASH_ENTRY_UNUSED;
}

//----- (00445A18) --------------------------------------------------------
bool __fastcall MpqOpenArchive(char * pszArchive, bool bHide, uint dwChar)
{
	DWORD dwTemp;
	InitializeHashSource();

	if( ! MpqSetAttributes(pszArchive, bHide) ) return false;

	uint dwFlags = 0; // MaxCountOfPlayersInGame > 1 ? FILE_FLAG_WRITE_THROUGH : 0;
	uint dwAttr = 0; // bHide ? MPQ_HIDE_ATTR : 0;

	SaveCreationKey = false;
	int tryCount = 5;	

	extern int OpenFileError; 
	retry:
	CurrentSaveArchiveHandle = CreateFile(pszArchive, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, dwFlags, NULL );
	if( CurrentSaveArchiveHandle == INVALID_HANDLE_VALUE ){
		OpenFileError = GetLastError();
		CurrentSaveArchiveHandle = CreateFile(pszArchive, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, dwAttr|dwFlags, NULL);
		if( CurrentSaveArchiveHandle == INVALID_HANDLE_VALUE ){
			OpenFileError = GetLastError();
			if( OpenFileError == ERROR_SHARING_VIOLATION && tryCount-- > 0 ){
				Sleep(500);
				goto retry;
			}
			return false;
		}
		SaveCreationKey = true;
		SaveChanged = true;
	}

	// if the file is still loaded in memory, skip reading file
	//#ifdef OPTIMIZE_SAVE_LOAD
	if( BlockLoaded && HashLoaded ) return true;
	//#endif

	FullHeader fhdr;
	memset(&fhdr, 0, sizeof(fhdr));
	if( ! ReadMpqFileHdr(&fhdr, &NextFileStart) ){
		OpenFileError = GetLastError();
		goto error;
	}
	// init blocks we might need
	memset(BlockTbl, 0, BLOCK_TBL_SIZE);
	BlockLoaded = true;

	// read in block table
	if( fhdr.hdr.blockcount ){
		if( INVALID_SET_FILE_POINTER == SetFilePointer(CurrentSaveArchiveHandle, BLOCK_TBL_OFFSET, NULL, FILE_BEGIN)
		 || ! ReadFile(CurrentSaveArchiveHandle, BlockTbl, BLOCK_TBL_SIZE, &dwTemp, NULL) ){
			OpenFileError = GetLastError();
			goto error;
		}
		Decrypt( (uint*) BlockTbl, BLOCK_TBL_SIZE, Hash(SaveFile_BlockTable, HASH_ENCRYPTKEY) );
	}

	// init hash table
	memset(HashTbl, 0xff, HASH_TBL_SIZE);
	HashLoaded = true;

	// read in hash table
	if (fhdr.hdr.hashcount) {
		if( INVALID_SET_FILE_POINTER == SetFilePointer(CurrentSaveArchiveHandle, HASH_TBL_OFFSET, NULL, FILE_BEGIN)
		 || ! ReadFile(CurrentSaveArchiveHandle, HashTbl, HASH_TBL_SIZE, &dwTemp, NULL) ){
			OpenFileError = GetLastError();
			goto error;
		}
		Decrypt( (uint*) HashTbl, HASH_TBL_SIZE, Hash(SaveFile_HashTable, HASH_ENCRYPTKEY) );
	}
	return true;
error:
	CloseSaveArchive(pszArchive, true, dwChar);
	return false;
}

// возможны расхождения в размере файлов если разные данные так как запись пишется при переходе между уровнями
//----- (00445BEA) --------------------------------------------------------
bool __fastcall ReadMpqFileHdr(FullHeader* hdr, uint* nextFileStart)
{
	// read file hdr
	uint fileSize = GetFileSize(CurrentSaveArchiveHandle, 0);
	*nextFileStart = fileSize;
	DWORD bytes;
	bool error = fileSize == INVALID_FILE_SIZE
		|| sizeof(*hdr) > fileSize
		|| ! ReadFile(CurrentSaveArchiveHandle, hdr, sizeof(*hdr), &bytes, 0)
		|| sizeof(*hdr) != bytes
		|| hdr->hdr.signature != ID_MPQ
		|| hdr->hdr.headersize != sizeof(FileHeader)
		|| hdr->hdr.version > VERSION
		|| hdr->hdr.sectorsizeid != SECTORSIZEID
		|| hdr->hdr.filesize != fileSize
		|| hdr->hdr.hashoffset != HASH_TBL_OFFSET
		|| hdr->hdr.blockoffset != BLOCK_TBL_OFFSET
		|| hdr->hdr.hashcount != HASH_ENTRIES
		|| hdr->hdr.blockcount != BLOCK_ENTRIES;

	if( error ){
		// kill off any existing file hdr information
		if( INVALID_SET_FILE_POINTER == SetFilePointer(CurrentSaveArchiveHandle, 0 ,NULL, FILE_BEGIN)
		 || ! SetEndOfFile(CurrentSaveArchiveHandle) )
			return false;

		// initialize file header
		memset(hdr, 0, sizeof(*hdr));

		// fill in .mpq header
		hdr->hdr.signature = ID_MPQ;
		hdr->hdr.headersize = sizeof(FileHeader);
		hdr->hdr.sectorsizeid = SECTORSIZEID;
		hdr->hdr.version = VERSION;

		// set location of start of next file
		*nextFileStart = FIRST_FILE_START;

		// modified the file
		SaveChanged = true;
		SaveCreationKey = true;
	}
	return true;
}

//----- (00445CDB) --------------------------------------------------------
void __fastcall CloseSaveArchive(char* archiveName, bool clearHashBuffer, int heroSlot)
{
	if( clearHashBuffer ){
		memzero(BlockTbl);
		memzero(HashTbl);
		BlockLoaded = false;
		HashLoaded = false;
	}
	if( CurrentSaveArchiveHandle != INVALID_HANDLE_VALUE ){
		CloseHandle(CurrentSaveArchiveHandle);// какая то ошибка при уходе с данжеона
		CurrentSaveArchiveHandle = INVALID_HANDLE_VALUE;
	}
	if( SaveChanged ){
		SaveChanged = 0;
		MPQUpdateLastWriteTimeStamp(archiveName, heroSlot);
	}
	if( SaveCreationKey ){
		SaveCreationKey = 0;
		MpqUpdateCreationTimeStamp(archiveName, heroSlot);
	}
}

//----- (00445D56) --------------------------------------------------------
void __fastcall MPQUpdateLastWriteTimeStamp(char* archiveName, int heroSlot)
{
	// only update registry stamps for multiplayer mode
	if( MaxCountOfPlayersInGame == 1 ) return;

	// get archive timestamps from registry
	FILETIME ft[MaxSaveSlots][NUM_TIMES];
	RegGetStamps(&ft[0][0], sizeof(ft));

	// get archive file timestamps
	WIN32_FIND_DATA findData;
	HANDLE findhandle = FindFirstFile(archiveName, &findData);
	if (findhandle == INVALID_HANDLE_VALUE) return;
	FindClose(findhandle);

	// update timestamp
	ft[heroSlot][LASTWRITE_TIME] = findData.ftLastWriteTime;

	// save back to registry
	RegSetStamps(&ft[0][0], sizeof(ft));
}

//----- (00445DC9) --------------------------------------------------------
bool __fastcall MpqCloseArchive(char* saveArchivePath, bool clearHashTables, int heroSlot)
{
	bool result = true;
	if( CurrentSaveArchiveHandle != INVALID_HANDLE_VALUE && SaveChanged ){
		result = MpqSetEOF() && WriteMpqFileHeader() && WriteBlockTable() && WriteHashTable();
	}
	CloseSaveArchive(saveArchivePath, clearHashTables, heroSlot);
	return result;
}

//----- (00445E25) --------------------------------------------------------
bool WriteMpqFileHeader()
{
	FullHeader fhdr;
	memset(&fhdr, 0, sizeof(fhdr));

	fhdr.hdr.signature    = ID_MPQ;
	fhdr.hdr.headersize   = sizeof(FileHeader);
	fhdr.hdr.filesize     = GetFileSize(CurrentSaveArchiveHandle, 0);
	fhdr.hdr.version      = VERSION;
	fhdr.hdr.sectorsizeid = SECTORSIZEID;
	fhdr.hdr.hashoffset   = HASH_TBL_OFFSET;
	fhdr.hdr.blockoffset  = BLOCK_TBL_OFFSET;
	fhdr.hdr.hashcount    = HASH_ENTRIES;
	fhdr.hdr.blockcount   = BLOCK_ENTRIES;

	DWORD written;
	if( INVALID_SET_FILE_POINTER == SetFilePointer(CurrentSaveArchiveHandle, 0, 0, FILE_BEGIN)
	 || ! WriteFile(CurrentSaveArchiveHandle, &fhdr.hdr, sizeof(fhdr), &written, 0) )
		return false;
	return sizeof(fhdr) == written;
}

//----- (00445EBB) --------------------------------------------------------
bool WriteBlockTable()
{
	if( INVALID_SET_FILE_POINTER == SetFilePointer(CurrentSaveArchiveHandle, BLOCK_TBL_OFFSET, 0, FILE_BEGIN) ) return false;
	DWORD written;
	Encrypt( (uint*) BlockTbl, BLOCK_TBL_SIZE, Hash(SaveFile_BlockTable, HASH_ENCRYPTKEY) );
	bool result = WriteFile(CurrentSaveArchiveHandle, BlockTbl, BLOCK_TBL_SIZE, &written, 0);
	Decrypt( (uint*) BlockTbl, BLOCK_TBL_SIZE, Hash(SaveFile_BlockTable, HASH_ENCRYPTKEY) );
	return result && BLOCK_TBL_SIZE == written;
}

//----- (00445F46) --------------------------------------------------------
bool WriteHashTable()
{
	if( INVALID_SET_FILE_POINTER == SetFilePointer(CurrentSaveArchiveHandle, HASH_TBL_OFFSET, 0, FILE_BEGIN) ) return false;
	DWORD written;
	Encrypt( (uint*) HashTbl, HASH_TBL_SIZE, Hash(SaveFile_HashTable, HASH_ENCRYPTKEY) );
	bool result = WriteFile(CurrentSaveArchiveHandle, HashTbl, HASH_TBL_SIZE, &written, 0);
	Decrypt( (uint*) HashTbl, HASH_TBL_SIZE, Hash(SaveFile_HashTable, HASH_ENCRYPTKEY) );
	return result && HASH_TBL_SIZE == written;
}

//----- (00445FD4) --------------------------------------------------------
bool MpqSetEOF()
{
	if( INVALID_SET_FILE_POINTER == SetFilePointer(CurrentSaveArchiveHandle, NextFileStart, 0, FILE_BEGIN) ) return false;
	return SetEndOfFile(CurrentSaveArchiveHandle);
}