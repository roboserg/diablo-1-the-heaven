#include "stdafx.h"
#include "codec.h"
#include "vorbisfile.h"

char pcmout[4096];

//читаем весь ogg в pcm buffer, для этого нужно узнать полный размер pcm получаемого из ogg
//есть функция ov_pcm_total(OggVorbis_File*, -1) == total length in pcm samples OF content;
//смотрим функцию загрузки wav чтобы определить как выделяется память под буфер
	
// th2
char* __fastcall CreateFilePlayCTX_Ogg( HANDLE handle, WAVEFORMATEX* waveFormat, uint* bufSize )
{
	OggVorbis_File vorbisFile;
	
	SoundCache soundCache;
	SoundCache_Init(handle, &soundCache, MAX_CACHE);
	
	// TODO: тут пока ошибка. Надо разобраться как читать из байт массива а не из файла
	// Input does not appear to be an Ogg bitstream.
	if( ov_open_callbacks(soundCache.ptr, &vorbisFile, NULL, 0, OV_CALLBACKS_DEFAULT) < 0 ){
		return 0;
	}

	{
		vorbis_info *vorbisInfo=ov_info(&vorbisFile,-1);
		memset(&waveFormat, 0, sizeof(WAVEFORMATEX));
		waveFormat->nChannels = vorbisInfo->channels;
		waveFormat->wBitsPerSample = 16;
		waveFormat->nSamplesPerSec = vorbisInfo->rate;
		waveFormat->nAvgBytesPerSec = waveFormat->nSamplesPerSec * waveFormat->nChannels * 2;
		waveFormat->nBlockAlign = 2 * waveFormat->nChannels;
		waveFormat->wFormatTag = 1;
		waveFormat->cbSize = 0;
	}

	int eof = 0;
	int current_section;
	while(!eof){
		long ret = ov_read( &vorbisFile, pcmout, sizeof(pcmout), 0, 2, 1, &current_section);
		if( ret == 0 ){
			/* EOF */
			eof=1;
		}else if( ret < 0 ){
			/* error in the stream.  Not a problem, just reporting it in
			case we (the app) cares.  In this case, we don't. */
		}else{
			/* we don't bother dealing with sample rate changes, etc, but
			you'll have to */
		}
	}
	ov_clear(&vorbisFile);

	return soundCache.ptr;
}

//----- (th2) -------------------------------------------------------------
void LoadSound_Ogg( Sound& sound )
{
	if( ! DirectSoundPtr || ! sound.name ) return;

	HANDLE soundFileHandle;
	if( !File_Open(sound.name, &soundFileHandle, FIVE_TRY) ){
		TerminateWithError("Unable to open sound file %s", sound.name);
	}
	if( sound.buffer ); // TODO: add clear prev sound ?
	sound.lastPlayTime = GetTickCountTh() - MIN_REPLAY_THRESHOLD - 1; // это таймер не влияет на гемплей, не перехватываем

	char* cachePtr = CreateFilePlayCTX_Ogg( soundFileHandle, &sound.WAVEFORMAT, &sound.size );
	if( !cachePtr ){
		TerminateWithError( "Invalid sound format on file %s", sound.name );
	}
	if( CreateDirectSoundBuffer( sound ) ){
		void *lpvAudioPtr1, *lpvAudioPtr2;
		DWORD dwAudioBytes1, dwAudioBytes2;
		DWORD errorCode = sound.buffer->Lock( 0, sound.size, &lpvAudioPtr1, &dwAudioBytes1, &lpvAudioPtr2, &dwAudioBytes2, 0 );
		if( errorCode ){
			DirectSoundError( errorCode, __LINE__, __FILE__ );
		}
		memcpy( lpvAudioPtr1, &cachePtr[ sound.offset ], dwAudioBytes1 );
		errorCode = sound.buffer->Unlock( lpvAudioPtr1, dwAudioBytes1, lpvAudioPtr2, dwAudioBytes2 );
		if( errorCode ){
			DirectSoundError( errorCode, __LINE__, __FILE__ );
		}
	}
	FreeMem( cachePtr );
	File_Close( soundFileHandle );
}

//----- (th2) -------------------------------------------------------------
int ToLowerExtension( int extension )
{
	char* extByte = (char*) &extension;
	for( int i = 0; i < 4; i++ ){
		extByte[ i ] = (char) tolower( extByte[ i ] );
	}
	return extension;
}

//----- (th2) -------------------------------------------------------------
void LoadSound( Sound& sound )
{
	if( ! sound.name ) return;
	int extension = ToLowerExtension( *(int*) (sound.name + strlen( sound.name ) - 4) );
	if( extension == 'vaw.' ){ // wav
		LoadSound_Wav( sound );
	}else if( extension == 'ggo.' ){ // ogg vorbis
		LoadSound_Ogg( sound );
	}else{
		TerminateWithError( "LoadSound: unknown file extension %s", (sound.name + strlen( sound.name ) - 4) );
	}
}
