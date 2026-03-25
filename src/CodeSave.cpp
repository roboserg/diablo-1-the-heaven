#include "stdafx.h"

//----- (00403722) --------------------------------------------------------
uint __fastcall DecodeFile( uchar* data, uint size, char* password )
{
	// initialize encryption keys
	InitializeKeys(false, password);

	// make sure the length is correct
	if( size <= sizeof(AppendRec) ) return 0;
	size -= sizeof(AppendRec);
	if( size & (SHA1_BLOCK_SIZE - 1) ) return 0; // не кратно SHA1_BLOCK_SIZE

	// DECRYPT THE FILE BLOCK BY BLOCK
	uchar buffer[2][SHA1_BLOCK_SIZE];
	for( uchar* end = data + size; data < end; data += SHA1_BLOCK_SIZE ){
		
		// get the next chunk
		memcpy(buffer[0], data, SHA1_BLOCK_SIZE);
		
		// DECRYPT THE BLOCK
		uchar hash[SHA1_DIGEST_SIZE];
		ShaGetLastHash(0, hash);
		
		for( int i = 0; i < SHA1_BLOCK_SIZE; i++ ){
			buffer[0][i] ^= hash[i % SHA1_DIGEST_SIZE];
		}

		ShaHash(0, buffer[0], 0);
		memset(hash, 0, SHA1_DIGEST_SIZE);

		// WRITE THE BLOCK
		memcpy(data, buffer[0], SHA1_BLOCK_SIZE);
	}
	
	// hide buffer contents
	memzero(buffer);

	// CHECK THE FILE TERMINATION RECORD
	AppendRec& append = *(AppendRec*) data;
	
	// CHECK THE ENCRYPTION VERSION
	if( append.version > 0 ){  // VERSION = 0
		ShaDestroy();
		return 0;
	}

	// CONFIRM THAT THE KEY AND PASSWORD WERE VALID
	uchar hash[SHA1_DIGEST_SIZE];
	ShaGetLastHash(0, hash);
	if( append.checkValue != *(uint*) hash ){
		memzero(hash);
		ShaDestroy();
		return 0;
	}

	// SET THE EXACT OUTPUT SIZE
	ShaDestroy();
	return size + append.lastBlockSize - SHA1_BLOCK_SIZE;
}

//----- (0040382D) --------------------------------------------------------
void __fastcall InitializeKeys( bool encrypt, char* password )
{
	KeyRec keyset;

	// save file "version"
	// so we don't have version number conflicts:
	// - EVEN number if RETAIL/SHAREWARE version
	// - ODD number if BETA version
	enum { SAVE_GAME_KEY = 0x7058 };

	// generate a key
	srand(SAVE_GAME_KEY);
	for( int i = 0; i < sizeof(keyset); i++ ){
		((uchar*)&keyset)[i] = (uchar) rand();
	}

	// HASH THE PASSWORD AND MIX IT WITH THE KEY
	uchar originalpassword[SHA1_BLOCK_SIZE];
	uchar hashedpassword[SHA1_DIGEST_SIZE];

	int passchar = 0;
	for( int i = 0; i < SHA1_BLOCK_SIZE; passchar++, i++ ){
		if( ! password[passchar] ){
			passchar = 0;
		}
		originalpassword[i] = password[passchar];
	}
	ShaInitialize(0);
	ShaHash(0, originalpassword, hashedpassword);
	ShaDestroy();

	for( int i = 0; i < sizeof(keyset); i++ ){
		((uchar*)&keyset)[i] ^= hashedpassword[i % SHA1_DIGEST_SIZE];
	}

	memzero(originalpassword);
	memzero(hashedpassword);

	// INITIALIZE THE ENCRYPTION ALGORITHMS
	for( int i = 0; i < 3; i++ ){
		ShaInitialize(i);
		ShaHash(i, keyset.shaInitVect, 0);
	}
	
	// WIPE OUT THE LOCAL COPY OF THE KEYS
	memset(&keyset, 0, sizeof(keyset));
	srand(GetTickCount());
}

//----- (00403913) --------------------------------------------------------
void __fastcall EncodeFile( uchar* data, uint sizeSrc, uint sizeDst, char* password )
{
	// make sure the user allocated enough bytes for the destination
	if( sizeDst != CalcEncodeDstBytes(sizeSrc) ){
		TerminateWithError("Invalid encode parameters");
	}

	// initialize encryption keys
	InitializeKeys(true, password);
	 
	// ENCRYPT THE FILE BLOCK BY BLOCK
	uint lastBlockSize = 0;

	uchar buffer[2][SHA1_BLOCK_SIZE];
	for( uchar* end = data + sizeSrc; data < end; data += SHA1_BLOCK_SIZE ){

		// get the next src data chunk
		uint blockSize = std::min(sizeSrc, (uint) SHA1_BLOCK_SIZE);
		memcpy(buffer[0], data, blockSize);

		// blank out any unused portion of the buffer
		if( blockSize < SHA1_BLOCK_SIZE ){
			memset(&buffer[0][blockSize], 0, SHA1_BLOCK_SIZE - blockSize);
		}
		
		// ENCRYPT THE BLOCK
		uchar hash[SHA1_DIGEST_SIZE];
		ShaGetLastHash(0, hash);
		ShaHash(0, buffer[0], 0);

		for( int i = 0; i < SHA1_BLOCK_SIZE; ++i ){
			buffer[0][i] ^= hash[i % SHA1_DIGEST_SIZE];
		}
		
		// hide hash info
		memzero(hash);
		
		// write encrypted chunk to destination
		memcpy(data, buffer[0], SHA1_BLOCK_SIZE);

		// next block
		sizeSrc -= blockSize;
		lastBlockSize = blockSize;
	}

	// hide buffer
	memzero(buffer);
	
	// APPEND THE TERMINATION RECORD
	uchar hash[SHA1_DIGEST_SIZE];
	AppendRec& append = *(AppendRec*) data;
	ShaGetLastHash(0, hash);
	append.checkValue = *(uint*) hash;
	append.version = VERSION;
	append.lastBlockSize = (uchar) lastBlockSize;
	append.reserved = 0;

	ShaDestroy();
}
