#include "stdafx.h"

//----- (004796A4) --------------------------------------------------------
bool __fastcall CreateFilePlayCTX2( HANDLE handle, WAVEFORMATEX* waveFormat )
{
	SoundCache soundCache;
	SoundCache_Init(handle, &soundCache, MIN_CACHE);
	bool result = SoundCache_FillWaveFormat(&soundCache, waveFormat, 0);
	SoundCache_Free(&soundCache);
	return result;
}

//----- (004796D2) --------------------------------------------------------
void __fastcall SoundCache_Init( HANDLE file, SoundCache* soundCache, uint maxCache )
{
	memset(soundCache, 0, sizeof( SoundCache ));
	uint fileSize = File_GetSize(file, nullptr);
	soundCache->fileSize = fileSize;
	uint cacheSize = maxCache; // (rt) 0xFFFFFFFF or 0
	LimitToRange(cacheSize, 4096u, fileSize);
	soundCache->cacheSize = cacheSize;
	soundCache->handle = file;
	soundCache->ptr = (char*)AllocMem(cacheSize); // (rt) allocate memory for the wav pcm data the size of the file, but no less than 4096 bytes 
}

//----- (0047971B) --------------------------------------------------------
void __fastcall SoundCache_Free(SoundCache* soundCache)
{
	FreeMemZero(soundCache->ptr);
}

//----- (00479729) --------------------------------------------------------
bool __fastcall SoundCache_FillWaveFormat( SoundCache* soundCache, WAVEFORMATEX* waveFormat, uint* bufferSize )
{
	// (rt) read the riff header from the start of the file
	RiffChunkDiscriptor riffChunkDiscriptor;
	if( !SoundCache_Read(soundCache, (char*)&riffChunkDiscriptor, sizeof( riffChunkDiscriptor )) ){
		return 0;
	}

	// (rt) check the file format
	if( riffChunkDiscriptor.ChunkId != 'FFIR' 
		|| riffChunkDiscriptor.Format != 'EVAW' ){
			return 0;
	}

	// (rt) read the fmt sub-chunk parameters
	WaveTagInfo tagInfo;
	if( !SoundCache_GetWaveTagInfo(soundCache, ' tmf', &tagInfo) ){
		return 0;
	}	

	// (rt) we need the first 16 bytes of this chunk, so its size must be at least 16 bytes
	if( tagInfo.Size < 16 ){
		return 0;
	}

	// (rt) read the first 16 bytes of the fmt chunk
	WAVEFORMATEX waveFormat2;
	if( !SoundCache_Read(soundCache, (char*)&waveFormat2, sizeof( WAVEFORMATEX ) - 2) ){
		return 0;
	}

	// (rt) possibly a check that the data chunk exists in the file. Unusual operation
	if( SoundCache_Skip(soundCache, tagInfo.Size - (sizeof( WAVEFORMATEX ) - 2)) == -1 ){
		return 0;
	}

	*waveFormat = waveFormat2;
	waveFormat->cbSize = 0;
	if( !bufferSize ){
		return 1;
	}

	// (rt) if a pointer was passed, return the data chunk parameters through it
	return SoundCache_GetWaveTagInfo(soundCache, 'atad', (WaveTagInfo*)bufferSize);
}

//----- (004797F3) --------------------------------------------------------
bool __fastcall SoundCache_Read( SoundCache* soundCache, char* aBufPtr, int size )
{
	if( !size ){
		return true;
	}
	// (rt) the purpose of the loop here is unclear
	while( 1 ){
		if( !soundCache->remain ){ // (rt) if the wav file is not loaded, load it into the buffer 
			SoundCache_LoadFile(soundCache); // (rt) loading the pcm sample data itself 
		}
		int readedSize = soundCache->remain;
		LimitToMax(readedSize, size);
		if( !readedSize ){
			return false;
		}
		// (rt) on a repeat pass of the loop there will probably be an error, since the missing bytes will be written to the same address
		memcpy(aBufPtr, &soundCache->ptr[soundCache->readed], readedSize);
		soundCache->seekPos += readedSize;
		soundCache->readed += readedSize;
		soundCache->remain -= readedSize;
		size -= readedSize;
		if( !size ){
			return true;
		}
	}
}

//----- (00479849) --------------------------------------------------------
void __fastcall SoundCache_LoadFile( SoundCache* soundCache )
{
	File_Seek(soundCache->handle, soundCache->seekPos, 0, 0);
	uint bytesToRead = soundCache->fileSize - soundCache->seekPos;
	LimitToMax(bytesToRead, soundCache->cacheSize);
	if( bytesToRead ){
		File_Read(soundCache->handle, soundCache->ptr, bytesToRead);
	}
	soundCache->readed = 0;
	soundCache->remain = bytesToRead;
}

//----- (00479884) --------------------------------------------------------
int __fastcall SoundCache_Skip(SoundCache* soundCache, uint size)
{
	uint remain = soundCache->remain;
	if( size >= remain ){
		// (rt) reached the end of the cached size
		soundCache->remain = 0;
	}else{
		// (rt) move the cache position further
		soundCache->readed += size;
		soundCache->remain = remain - size;
	}
	soundCache->seekPos += size;
	return soundCache->seekPos;
}

//----- (004798A2) --------------------------------------------------------
bool __fastcall SoundCache_GetWaveTagInfo( SoundCache* soundCache, unsigned int tag, WaveTagInfo* tagInfo )
{
	while( 1 ){
		WaveTag curTag;
		// (rt) read the header
		if( !SoundCache_Read(soundCache, (char*)&curTag, sizeof( WaveTag )) ){
			return 0;
		}
		// (rt) if the header name matches, return the size and position
		if( curTag.Tag == tag ){
			tagInfo->Size = curTag.Size;
			tagInfo->Pos = SoundCache_Skip(soundCache, 0);
			return tagInfo->Pos != INVALID_SET_FILE_POINTER;
		}
		// (rt) go to the next one
		if( SoundCache_Skip(soundCache, curTag.Size) == -1 ){
			return 0;
		}
	}
}

//----- (004798FD) --------------------------------------------------------
char* __fastcall CreateFilePlayCTX_Wav( HANDLE handle, WAVEFORMATEX* waveFormat, uint* bufferSize )
{
	SoundCache soundCache;
	SoundCache_Init(handle, &soundCache, MAX_CACHE);
	if( SoundCache_FillWaveFormat(&soundCache, waveFormat, bufferSize) ){
		return soundCache.ptr;
	}else{
		SoundCache_Free(&soundCache);
		return 0;
	}
}
