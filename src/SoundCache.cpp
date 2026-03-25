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
	uint cacheSize = maxCache; // 0xFFFFFFFF или 0
	LimitToRange(cacheSize, 4096u, fileSize);
	soundCache->cacheSize = cacheSize;
	soundCache->handle = file;
	soundCache->ptr = (char*)AllocMem(cacheSize); // выделение памяти под wav pcm data размером с файл, но не меньше 4096 байт 
}

//----- (0047971B) --------------------------------------------------------
void __fastcall SoundCache_Free(SoundCache* soundCache)
{
	FreeMemZero(soundCache->ptr);
}

//----- (00479729) --------------------------------------------------------
bool __fastcall SoundCache_FillWaveFormat( SoundCache* soundCache, WAVEFORMATEX* waveFormat, uint* bufferSize )
{
	// читаем с начала файла riff заголовок
	RiffChunkDiscriptor riffChunkDiscriptor;
	if( !SoundCache_Read(soundCache, (char*)&riffChunkDiscriptor, sizeof( riffChunkDiscriptor )) ){
		return 0;
	}

	// сверяем формат файла
	if( riffChunkDiscriptor.ChunkId != 'FFIR' 
		|| riffChunkDiscriptor.Format != 'EVAW' ){
			return 0;
	}

	// читаем параметры fmt sub-chunk а
	WaveTagInfo tagInfo;
	if( !SoundCache_GetWaveTagInfo(soundCache, ' tmf', &tagInfo) ){
		return 0;
	}	

	// нас интересуют первые 16 байт этого чанка, потому размер должен быть не меньше 16 байт
	if( tagInfo.Size < 16 ){
		return 0;
	}

	// читаем первые 16 байт чанка fmt
	WAVEFORMATEX waveFormat2;
	if( !SoundCache_Read(soundCache, (char*)&waveFormat2, sizeof( WAVEFORMATEX ) - 2) ){
		return 0;
	}

	// возможно проверка на наличие чанка data в файле. Странная операция
	if( SoundCache_Skip(soundCache, tagInfo.Size - (sizeof( WAVEFORMATEX ) - 2)) == -1 ){
		return 0;
	}

	*waveFormat = waveFormat2;
	waveFormat->cbSize = 0;
	if( !bufferSize ){
		return 1;
	}

	// если передали указатель, то возвращаем по нему параметры чанка data
	return SoundCache_GetWaveTagInfo(soundCache, 'atad', (WaveTagInfo*)bufferSize);
}

//----- (004797F3) --------------------------------------------------------
bool __fastcall SoundCache_Read( SoundCache* soundCache, char* aBufPtr, int size )
{
	if( !size ){
		return true;
	}
	// не понимаю зачем тут цикл
	while( 1 ){
		if( !soundCache->remain ){ // если wav файл не загружен, грузим его в буфер 
			SoundCache_LoadFile(soundCache); // загрузка собственно pcm sample data 
		}
		int readedSize = soundCache->remain;
		LimitToMax(readedSize, size);
		if( !readedSize ){
			return false;
		}
		// при повторном проходе цикла вроде будет ошибка так как недостающие байты будут по тому же адресу записаны
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
		// дошли до конца кешированного размера
		soundCache->remain = 0;
	}else{
		// сдвигаем позицию кеша дальше
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
		// читаем заголовок
		if( !SoundCache_Read(soundCache, (char*)&curTag, sizeof( WaveTag )) ){
			return 0;
		}
		// если имя заголовка совпадает, то возвращаем размер и позицию
		if( curTag.Tag == tag ){
			tagInfo->Size = curTag.Size;
			tagInfo->Pos = SoundCache_Skip(soundCache, 0);
			return tagInfo->Pos != INVALID_SET_FILE_POINTER;
		}
		// переходим к следующиему
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
