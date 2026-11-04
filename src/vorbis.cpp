#include "stdafx.h"
#include "codec.h"
#include "vorbisfile.h"

char pcmout[4096];

//(rt) read the whole ogg into the pcm buffer; this requires the full size of the pcm obtained from the ogg
//(rt) there is a function ov_pcm_total(OggVorbis_File*, -1) == total length in pcm samples OF content;
//(rt) look at the wav loading function to see how the buffer memory is allocated
	
// th2 (th4: rewritten - decode OGG from an in-memory buffer into a PCM buffer)
//
// The old version was broken: it fed SoundCache.ptr (a WAV/RIFF cache, empty at
// that point) to ov_open_callbacks with OV_CALLBACKS_DEFAULT (which fread()s a
// FILE*), hence "Input does not appear to be an Ogg bitstream". It also threw
// away every ov_read() block and returned the compressed bytes with no size.
//
// Now: slurp the whole file (File_* already yields the decompressed MPQ member
// as a flat stream, same as WAV), decode it fully via memory callbacks, and
// return a freshly-allocated 16-bit PCM buffer. *bufSize = decoded PCM bytes.

// in-memory read cursor for ov_callbacks
struct OggMemFile
{
	const char* data;
	size_t      size;
	size_t      pos;
};

static size_t OggMem_read( void* ptr, size_t size, size_t nmemb, void* src )
{
	OggMemFile* f = (OggMemFile*) src;
	size_t want = size * nmemb;
	size_t left = f->size - f->pos;
	if( want > left ) want = left;
	if( want ){
		memcpy( ptr, f->data + f->pos, want );
		f->pos += want;
	}
	return size ? ( want / size ) : 0;
}

static int OggMem_seek( void* src, ogg_int64_t offset, int whence )
{
	OggMemFile* f = (OggMemFile*) src;
	ogg_int64_t base;
	if( whence == SEEK_SET )      base = 0;
	else if( whence == SEEK_CUR ) base = (ogg_int64_t) f->pos;
	else if( whence == SEEK_END ) base = (ogg_int64_t) f->size;
	else                          return -1;
	ogg_int64_t np = base + offset;
	if( np < 0 || np > (ogg_int64_t) f->size ) return -1;
	f->pos = (size_t) np;
	return 0;
}

static int OggMem_close( void* )
{
	return 0;
}

static long OggMem_tell( void* src )
{
	return (long) ( (OggMemFile*) src )->pos;
}

char* __fastcall CreateFilePlayCTX_Ogg( HANDLE handle, WAVEFORMATEX* waveFormat, uint* bufSize )
{
	// 1) slurp the whole compressed OGG into memory
	uint fileSize = File_GetSize( handle, nullptr );
	if( !fileSize ) return 0;

	char* oggBytes = (char*) AllocMem( fileSize );
	if( !oggBytes ) return 0;

	File_Seek( handle, 0, 0, 0 );
	if( !File_Read( handle, oggBytes, fileSize ) ){ // File_Read returns a success flag, not a byte count
		FreeMem( oggBytes );
		return 0;
	}

	// 2) open the vorbis stream over the memory buffer
	OggMemFile mem = { oggBytes, fileSize, 0 };
	ov_callbacks cbs = { OggMem_read, OggMem_seek, OggMem_close, OggMem_tell };

	OggVorbis_File vorbisFile;
	if( ov_open_callbacks( &mem, &vorbisFile, NULL, 0, cbs ) < 0 ){
		FreeMem( oggBytes );
		return 0;
	}

	// 3) fill WAVEFORMATEX (always decode to signed 16-bit PCM)
	vorbis_info* vorbisInfo = ov_info( &vorbisFile, -1 );
	if( !vorbisInfo ){
		ov_clear( &vorbisFile );
		FreeMem( oggBytes );
		return 0;
	}
	int channels = vorbisInfo->channels;			// 1 = mono, 2 = stereo
	memset( waveFormat, 0, sizeof( WAVEFORMATEX ) );
	waveFormat->wFormatTag      = 1;				// WAVE_FORMAT_PCM
	waveFormat->nChannels       = (WORD) channels;
	waveFormat->wBitsPerSample  = 16;
	waveFormat->nSamplesPerSec  = vorbisInfo->rate;
	waveFormat->nBlockAlign     = (WORD)( 2 * channels );
	waveFormat->nAvgBytesPerSec = waveFormat->nSamplesPerSec * waveFormat->nBlockAlign;
	waveFormat->cbSize          = 0;

	// 4) exact-size PCM output buffer: totalSamples * channels * 2 bytes
	ogg_int64_t totalSamples = ov_pcm_total( &vorbisFile, -1 ); // per-channel sample frames
	if( totalSamples <= 0 ){
		ov_clear( &vorbisFile );
		FreeMem( oggBytes );
		return 0;
	}
	uint pcmSize = (uint)( totalSamples * channels * 2 );
	char* pcm = (char*) AllocMem( pcmSize );
	if( !pcm ){
		ov_clear( &vorbisFile );
		FreeMem( oggBytes );
		return 0;
	}

	// 5) decode the whole thing. ov_read interleaves channels for us, so mono
	//    and stereo both "just work". word=2 (16-bit), sgned=1, little-endian=0.
	uint written = 0;
	int  section = 0;
	while( written < pcmSize ){
		long ret = ov_read( &vorbisFile, pcm + written, (int)( pcmSize - written ), 0, 2, 1, &section );
		if( ret == 0 )  break;			// EOF
		if( ret < 0 )   continue;		// recoverable stream glitch: skip
		written += (uint) ret;
	}

	ov_clear( &vorbisFile );
	FreeMem( oggBytes );

	if( bufSize ) *bufSize = written;	// decoded PCM byte count
	return pcm;							// caller FreeMem()s this
}

//----- (th2) -------------------------------------------------------------
void LoadSound_Ogg( Sound& sound )
{
	if( ! DirectSoundPtr || ! sound.name ) return;

	HANDLE soundFileHandle;
	if( !File_Open(sound.name, &soundFileHandle, FIVE_TRY) ){
		TerminateWithError("Unable to open sound file %s", sound.name);
	}
	// if( sound.buffer ){} // TODO: add clear prev sound ?
	sound.lastPlayTime = GetTickCountTh() - MIN_REPLAY_THRESHOLD - 1; // (rt) this timer doesn't affect gameplay, not intercepted

	char* cachePtr = CreateFilePlayCTX_Ogg( soundFileHandle, &sound.WAVEFORMAT, &sound.size );
	if( !cachePtr ){
		TerminateWithError( "Invalid sound format on file %s", sound.name );
	}
	sound.offset = 0; // (th4) the OGG path returns a pure PCM buffer (no RIFF header),
	                  // so unlike WAV there is nothing to skip past. sound.size was just
	                  // set to the decoded PCM byte count by CreateFilePlayCTX_Ogg.
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
