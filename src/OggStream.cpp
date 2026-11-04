// OggStream.cpp  (th4)
// -----------------------------------------------------------------------------
// A streaming Ogg Vorbis player that runs PARALLEL to Storm's DDA streamer
// (SFileDdaBeginEx & co. in Storm.cpp). Storm DDA only understands RIFF/WAVE
// PCM, so it can't play .ogg. Rather than surgically inject a Vorbis stage into
// the fragile decompiled Storm streamer, this module provides an independent,
// self-contained looping streamer used only for .ogg music/ambient tracks.
//
// It mirrors the small slice of the DDA contract that PlayMusic/PlayAmbient use:
//   OggStream_Play(name, loop, volume)   ~ SFileOpenFile + SFileDdaBeginEx
//   OggStream_Stop()                     ~ SFileDdaEnd + SFileCloseFile
//   OggStream_SetVolume(volume)          ~ SFileDdaSetVolume
// Each logical channel (music, ambient) owns one OggStream instance.
//
// Design:
//  - Reads the compressed .ogg from the MPQ via the game's File_* wrappers
//    (File_Open/File_GetSize/File_Read -- the same path the SFX ogg loader uses),
//    slurped whole into memory on the game thread, then fed to libvorbisfile
//    through in-memory ov_callbacks. The worker thread decodes from that private
//    buffer and never touches Storm/MPQ (which isn't safe to hit concurrently
//    with the game thread's own file streaming during level load).
//  - Decodes into a small looping ("streaming") secondary DirectSound buffer.
//    A worker thread watches the play cursor and refills the half of the ring
//    the hardware has already passed. This keeps memory flat regardless of track
//    length (unlike the SFX path, which decodes whole files).
//  - Volume uses the game's native VOL_MIN..VOL_MAX range (-4800..0), which is
//    already DirectSound's hundredth-dB SetVolume scale -- pass straight through.
//
// XP-safe: plain Win32 threads + a polling refill loop (no WASAPI, no event
// notifications required). DirectSound only.
//
// (th4) MUSIC CROSSFADES. Music has OGG_MUSIC_SLOTS (4) slots (g_musicSlot is
// the one OGGCH_MUSIC means right now). OggStream_FadeOut hands the playing
// track over to a fade-out on its own slot and moves the music channel to a
// free slot; the next OggStream_Play with fadeInMs starts the new track silent
// there and fades it in -- the two overlap, which is the crossfade. Each worker
// runs its own fade (oggFadeStep), so a fade keeps going while the game thread
// is blocked on a loading screen. A finished fade-out stops its buffer and ends
// its worker; oggReap frees it on the next call. Quick level changes (down the
// stairs and straight back up) stack fading tracks; only when all slots are
// busy is one dropped at once -- the QUIETEST, the one furthest along its
// fade-out, so the cut is not heard. (Two slots are not enough: going down and
// straight back up cut the older track at about half volume, clearly heard.)
// The fades follow an equal-power curve (sin / cos of the elapsed share), so
// the sum of a crossfading pair stays as loud as one track all the way through.
// Who fades and who stops dead: SoundBuffer.cpp StopMusic / PlayMusic (the
// `fade` flag) and their callers -- only the music, never ambience or sounds.

#include "stdafx.h"
#include "codec.h"
#include "vorbisfile.h"
#include <math.h>

// ---- tuning ----------------------------------------------------------------
static const int   OGG_RING_SECONDS = 2;      // ring buffer length in seconds
static const int   OGG_RING_SLICES  = 2;      // refill granularity (halves)
static const DWORD OGG_POLL_MS      = 30;     // worker poll interval

// ---- one streaming channel -------------------------------------------------
struct OggStream
{
	// source: the ENTIRE compressed .ogg is slurped into memory on the game
	// thread, then the MPQ handle is closed immediately. The worker thread only
	// ever touches this private buffer -- never Storm/MPQ -- so there is no
	// cross-thread MPQ reentrancy (the bug that crashed level load: the worker's
	// ov_read hit SFileReadFile on the same handle the game thread was using to
	// stream the level).
	char*                data;         // compressed ogg bytes (owned)
	DWORD                dataSize;
	DWORD                dataPos;      // read cursor for ov_callbacks

	// IMPORTANT: the prebuilt vorbis libs (VS2012/vc110) compute a LARGER
	// sizeof(OggVorbis_File) than our v141 headers do -- the lib stores its
	// ov_callbacks at OggVorbis_File+0x2C0, past where our header thinks the
	// struct ends. If we embed OggVorbis_File inline, the lib's writes to
	// vf.callbacks collide with the following fields (buffer/ringBytes/...),
	// corrupting the read callback pointer -> ov_read jumps through garbage
	// (EIP=0x22000000). We therefore give vf its own heavily over-sized backing
	// and only ever access it through the OggVorbis_File* view. 0x400 bytes is
	// far larger than either layout's real size.
	union {
		OggVorbis_File vf;
		unsigned char  vfPad[ 0x400 ];
	};
	bool                 vfOpen;
	int                  channels;
	int                  rate;
	bool                 loop;

	// output
	IDirectSoundBuffer*  buffer;       // looping secondary buffer
	DWORD                ringBytes;    // total ring size
	DWORD                sliceBytes;   // ringBytes / OGG_RING_SLICES
	int                  lastSlice;    // last slice index we refilled

	// worker
	HANDLE               thread;
	volatile bool        stop;

	bool                 ended;        // decoder hit EOF and loop==false

	// (th4) volume fade, run by the worker (MUSIC CROSSFADES above); guarded by g_fadeLock
	int                  fadeVol;      // full volume of the track (hundredths of dB): the fade's top
	DWORD                fadeStart;    // GetTickCount when the fade (virtually) began
	DWORD                fadeMs;       // fade length; 0 = no fade running
	bool                 fadeOut;      // true = towards silence, then stop the buffer and end the worker
	volatile bool        done;         // a fade-out finished: buffer stopped, worker ended (oggReap frees it)
};

// Locking model: the worker thread is the ONLY thing that touches the decoder
// and the DirectSound buffer while it is running. Control calls (Play/Stop)
// FIRST join the worker (so it is no longer running), THEN mutate state. That
// makes a mutex unnecessary and, crucially, avoids the classic "hold lock while
// joining a thread that wants the same lock" deadlock. SetVolume is the only
// concurrent touch and goes straight to IDirectSoundBuffer::SetVolume, which is
// safe to call from another thread.

// The game uses two streamed channels that need .ogg (OGGCH_MUSIC, OGGCH_AMBIENT
// in funcs.h). Music owns OGG_MUSIC_SLOTS slots (MUSIC CROSSFADES above), ambient
// one. File-scope singletons, addressed through oggSlot(). Each music track keeps
// its whole compressed .ogg in memory (a few MB) while it plays or fades.
// The fade fields are the second concurrent touch besides SetVolume: the game
// thread writes them, the worker reads them, both under g_fadeLock -- which is
// never held while joining a worker, so the no-deadlock rule above still holds.
enum { OGG_SLOT_AMBIENT = 0, OGG_SLOT_MUSIC0 = 1, OGG_MUSIC_SLOTS = 4, OGG_SLOT_COUNT = OGG_SLOT_MUSIC0 + OGG_MUSIC_SLOTS };
static OggStream g_ogg[ OGG_SLOT_COUNT ];
static int       g_musicSlot = OGG_SLOT_MUSIC0;	// the slot OGGCH_MUSIC means; the other music slots may be fading out
static bool      g_oggInit = false;
static CRITICAL_SECTION g_fadeLock;

static OggStream* oggSlot( int ch )
{
	if( ch == OGGCH_MUSIC ) return &g_ogg[ g_musicSlot ];
	if( ch == OGGCH_AMBIENT ) return &g_ogg[ OGG_SLOT_AMBIENT ];
	return 0;
}

// File I/O: the compressed .ogg is loaded via the game's File_* API (File_Open /
// File_GetSize / File_Read / File_Close / File_Seek), declared in funcs.h and
// already available through stdafx.h. These are the same wrappers the working
// SFX ogg path uses; the raw SFile* API resolved the ambient file wrongly.

// ---- ov_callbacks over the in-memory compressed buffer ---------------------
// datasource is the OggStream* itself; we read from s->data with s->dataPos.
// This runs the decoder entirely off a private buffer, so the worker thread
// never calls into Storm/MPQ (which is not safe to hit concurrently with the
// game thread's own file streaming during level load).
static size_t ovMem_read( void* ptr, size_t size, size_t nmemb, void* src )
{
	OggStream* s = (OggStream*) src;
	size_t want = size * nmemb;
	size_t left = ( s->dataPos <= s->dataSize ) ? ( s->dataSize - s->dataPos ) : 0;
	if( want > left ) want = left;
	if( want ){
		memcpy( ptr, s->data + s->dataPos, want );
		s->dataPos += (DWORD) want;
	}
	return size ? ( want / size ) : 0;
}

static int ovMem_seek( void* src, ogg_int64_t offset, int whence )
{
	OggStream* s = (OggStream*) src;
	ogg_int64_t base;
	if( whence == SEEK_SET )      base = 0;
	else if( whence == SEEK_CUR ) base = (ogg_int64_t) s->dataPos;
	else if( whence == SEEK_END ) base = (ogg_int64_t) s->dataSize;
	else                          return -1;
	ogg_int64_t np = base + offset;
	if( np < 0 || np > (ogg_int64_t) s->dataSize ) return -1;
	s->dataPos = (DWORD) np;
	return 0;
}

static int ovMem_close( void* )
{
	return 0; // buffer freed in oggTeardown, not by ov_clear
}

static long ovMem_tell( void* src )
{
	return (long) ( (OggStream*) src )->dataPos;
}

// ---- decode helper: fill `bytes` of PCM into dst, looping/zero-padding ------
// Returns bytes actually produced (== bytes unless the track ended and !loop).
static DWORD oggDecodeInto( OggStream* s, char* dst, DWORD bytes )
{
	DWORD done = 0;
	int   section = 0;
	while( done < bytes ){
		long ret = ov_read( &s->vf, dst + done, (int)( bytes - done ), 0, 2, 1, &section );
		if( ret > 0 ){
			done += (DWORD) ret;
		}else if( ret == 0 ){
			// EOF
			if( s->loop ){
				ov_pcm_seek( &s->vf, 0 );   // seamless restart
				continue;
			}
			s->ended = true;
			break;
		}else{
			// ret < 0: recoverable stream glitch -> skip
			continue;
		}
	}
	// pad the remainder with silence so the ring never plays stale audio
	if( done < bytes ){
		memset( dst + done, 0, bytes - done );
	}
	return done;
}

// ---- write one specific slice `idx` of the ring ----------------------------
static void oggFillSliceAt( OggStream* s, int idx )
{
	if( !s->buffer ) return;
	DWORD ofs = (DWORD) idx * s->sliceBytes;
	void*  p1 = 0; void* p2 = 0;
	DWORD  b1 = 0;  DWORD b2 = 0;
	if( s->buffer->Lock( ofs, s->sliceBytes, &p1, &b1, &p2, &b2, 0 ) != DS_OK ){
		return;
	}
	oggDecodeInto( s, (char*) p1, b1 );
	if( p2 && b2 ){
		// slice never wraps (ring is an exact multiple of sliceBytes), but honor
		// the split defensively.
		oggDecodeInto( s, (char*) p2, b2 );
	}
	s->buffer->Unlock( p1, b1, p2, b2 );
}

// ---- (th4) one step of a running fade (MUSIC CROSSFADES above) --------------
// Called by the worker every poll. Equal-power curve: gain sin(t * pi/2) in,
// cos(t * pi/2) out, t = elapsed / length; the gain becomes hundredths of dB
// below the track's full volume (2000 * log10). Returns true when a fade-out
// has finished: the buffer is stopped and the worker must end.
static bool oggFadeStep( OggStream* s )
{
	if( !s->buffer ) return false;
	EnterCriticalSection( &g_fadeLock );
	const DWORD ms = s->fadeMs, t0 = s->fadeStart;
	const int   full = s->fadeVol;
	const bool  out = s->fadeOut;
	LeaveCriticalSection( &g_fadeLock );
	if( !ms ) return false;
	const DWORD elapsed = GetTickCount() - t0;
	const float t = elapsed >= ms ? 1.0f : (float) elapsed / (float) ms;
	const float g = out ? cosf( t * 1.5707963f ) : sinf( t * 1.5707963f );
	int vol = g <= 0.0001f ? (int) DSBVOLUME_MIN : full + (int)( 2000.0f * log10f( g ) );
	LimitToRange( vol, (int) DSBVOLUME_MIN, (int) DSBVOLUME_MAX );
	s->buffer->SetVolume( vol );
	if( t < 1.0f ) return false;
	EnterCriticalSection( &g_fadeLock );
	if( s->fadeStart == t0 ) s->fadeMs = 0;	// unless the game thread just started another fade
	LeaveCriticalSection( &g_fadeLock );
	if( !out ) return false;
	s->buffer->Stop();
	s->done = true;
	return true;
}

// ---- worker thread: refill each slice the play cursor has vacated ----------
// Invariant: the slice CONTAINING the play cursor is "live"; every other slice
// should already hold fresh audio. When the cursor crosses into a new slice, the
// slice it just left is now safe to overwrite -- decode the next chunk into it.
static unsigned __stdcall oggWorker( void* arg )
{
	OggStream* s = (OggStream*) arg;
	while( !s->stop ){
		if( oggFadeStep( s ) ) break;	// (th4) a finished fade-out ends the worker (MUSIC CROSSFADES)
		if( s->buffer ){
			DWORD play = 0, write = 0;
			if( s->buffer->GetCurrentPosition( &play, &write ) == DS_OK ){
				int curSlice = (int)( play / s->sliceBytes );
				// advance one slice at a time until we've caught up to the cursor,
				// filling the slice the cursor just vacated each step.
				int guard = OGG_RING_SLICES + 1; // never loop forever
				while( s->lastSlice != curSlice && guard-- > 0 ){
					oggFillSliceAt( s, s->lastSlice );          // vacated slice
					s->lastSlice = ( s->lastSlice + 1 ) % OGG_RING_SLICES;
				}
			}
		}
		Sleep( OGG_POLL_MS );
	}
	return 0;
}

// ---- lifecycle -------------------------------------------------------------
static void oggInitOnce()
{
	if( g_oggInit ) return;
	for( int i = 0; i < OGG_SLOT_COUNT; i++ ){
		memset( &g_ogg[i], 0, sizeof( OggStream ) );
	}
	InitializeCriticalSection( &g_fadeLock );
	g_oggInit = true;
}

static void oggTeardown( OggStream* s )
{
	// Join the worker to FULL COMPLETION before touching any vorbis/DirectSound
	// state. This is the crash fix: previously the wait had a 2000 ms timeout, so
	// if the worker was still inside ov_read(), teardown fell through and called
	// ov_clear() concurrently -- freeing the decoder's function pointers under
	// the running ov_read and jumping through garbage (EIP=0x22000000). The join
	// MUST be unconditional. The worker checks s->stop frequently and cannot
	// block (it only decodes from an in-memory/MPQ source), so this returns
	// quickly in practice.
	if( s->thread ){
		s->stop = true;
		WaitForSingleObject( s->thread, INFINITE );
		CloseHandle( s->thread );
		s->thread = 0;
	}
	// From here the worker is guaranteed dead; safe to tear down shared state.
	if( s->buffer ){
		s->buffer->Stop();
		s->buffer->Release();
		s->buffer = 0;
	}
	if( s->vfOpen ){
		ov_clear( &s->vf );   // frees decoder; ovMem_close is a no-op (buffer freed below)
		s->vfOpen = false;
	}
	if( s->data ){
		FreeMem( s->data );
		s->data = 0;
		s->dataSize = 0;
		s->dataPos = 0;
	}
	s->stop = false;
	s->ended = false;
	s->lastSlice = 0;
	EnterCriticalSection( &g_fadeLock );
	s->fadeMs = 0; s->fadeOut = false;
	LeaveCriticalSection( &g_fadeLock );
	s->done = false;
}

// (th4) Free the slots whose fade-out has finished (their workers have already ended).
static void oggReap()
{
	for( int i = 0; i < OGG_SLOT_COUNT; i++ ){
		if( g_ogg[i].done ) oggTeardown( &g_ogg[i] );
	}
}

// Returns true on success. `volume` is VOL_MIN..VOL_MAX (-4800..0).
// (th4) fadeInMs > 0: start silent and fade in to `volume` (MUSIC CROSSFADES).
static bool oggStart( OggStream* s, char* name, bool loop, int volume, int fadeInMs )
{
	if( !DirectSoundPtr ) return false;

	oggTeardown( s ); // ensure clean slate

	// Slurp the ENTIRE compressed .ogg into memory on THIS (game) thread via the
	// File_* API -- the SAME wrappers the working SFX ogg path uses. (The raw
	// SFile* API searched a different scope and reported a wrong 176400-byte size
	// for the ambient file, so ov_open read a garbage buffer and the decoder
	// jumped through an uninitialized pointer -> EIP=0x22000000. File_* resolves
	// the MPQ member correctly.)
	{
		HANDLE h = 0;
		if( !File_Open( name, &h, FIVE_TRY ) || !h ){
			return false;
		}
		DWORD sz = File_GetSize( h, nullptr );
		if( sz == 0 || sz == INVALID_FILE_SIZE ){
			File_Close( h );
			return false;
		}
		s->data = (char*) AllocMem( sz );
		if( !s->data ){
			File_Close( h );
			return false;
		}
		File_Seek( h, 0, 0, 0 );
		if( !File_Read( h, s->data, sz ) ){ // returns a success flag, not a count
			File_Close( h );
			oggTeardown( s );
			return false;
		}
		File_Close( h );
		s->dataSize = sz;
		s->dataPos  = 0;
	}

	// open vorbis stream over the in-memory buffer (datasource = the stream)
	memset( &s->vf, 0, sizeof( s->vfPad ) ); // clear the whole over-sized backing
	ov_callbacks cbs = { ovMem_read, ovMem_seek, ovMem_close, ovMem_tell };
	if( ov_open_callbacks( s, &s->vf, NULL, 0, cbs ) < 0 ){
		oggTeardown( s );
		return false;
	}
	s->vfOpen = true;

	vorbis_info* vi = ov_info( &s->vf, -1 );
	if( !vi ){ oggTeardown( s ); return false; }
	s->channels = vi->channels;
	s->rate     = vi->rate;
	s->loop     = loop;
	s->ended    = false;

	// build WAVEFORMATEX for a 16-bit PCM looping buffer
	WAVEFORMATEX wf;
	memset( &wf, 0, sizeof( wf ) );
	wf.wFormatTag      = WAVE_FORMAT_PCM;
	wf.nChannels       = (WORD) s->channels;
	wf.wBitsPerSample  = 16;
	wf.nSamplesPerSec  = s->rate;
	wf.nBlockAlign     = (WORD)( s->channels * 2 );
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
	wf.cbSize          = 0;

	// ring buffer size (round to block align and to OGG_RING_SLICES)
	DWORD ring = (DWORD)( wf.nAvgBytesPerSec * OGG_RING_SECONDS );
	ring -= ring % wf.nBlockAlign;
	ring -= ring % OGG_RING_SLICES;
	DWORD minRing = (DWORD) wf.nBlockAlign * OGG_RING_SLICES;
	if( ring < minRing ) ring = minRing;
	s->ringBytes  = ring;
	s->sliceBytes = ring / OGG_RING_SLICES;

	DSBUFFERDESC desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize        = sizeof( DSBUFFERDESC );
	desc.dwBufferBytes = s->ringBytes;
	desc.lpwfxFormat   = &wf;
	// (th4) GLOBALFOCUS keeps a buffer audible while the app is minimised or behind
	// another window, so it must follow the BackgroundSound setting -- it was set
	// unconditionally here, which is why turning BackgroundSound off stopped
	// silencing the music. The other two audio paths already gate it: SFX in
	// CreateDirectSoundBuffer (SoundBuffer.cpp) and Storm's DDA streamer via the
	// byte patch in PatchStormWin (TH1dll.cpp). This streamer was added later for
	// .ogg and never picked the rule up -- and since every music track in the mod
	// is .ogg, including the original Diablo set under Music\\or\\, it took over
	// the whole music path and the setting looked broken for music specifically.
	desc.dwFlags       = DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2;
	if( BackgroundSound ){
		desc.dwFlags |= DSBCAPS_GLOBALFOCUS;
	}

	if( DirectSoundPtr->CreateSoundBuffer( &desc, &s->buffer, nullptr ) != DS_OK || !s->buffer ){
		s->buffer = 0;
		oggTeardown( s );
		return false;
	}

	// prime every slice with decoded audio before playing
	for( int i = 0; i < OGG_RING_SLICES; i++ ){
		oggFillSliceAt( s, i );
	}
	// play cursor will start in slice 0; the worker fills slices as the cursor
	// vacates them, beginning with slice 0 once the cursor enters slice 1.
	s->lastSlice = 0;

	// volume (pass-through: VOL range == DSBVOLUME range)
	LimitToRange( volume, (int) DSBVOLUME_MIN, (int) DSBVOLUME_MAX );
	// (th4) a fade-in starts silent; the worker raises it (oggFadeStep)
	EnterCriticalSection( &g_fadeLock );
	s->fadeVol = volume; s->fadeOut = false;
	s->fadeStart = GetTickCount(); s->fadeMs = fadeInMs > 0 ? (DWORD) fadeInMs : 0;
	LeaveCriticalSection( &g_fadeLock );
	s->buffer->SetVolume( fadeInMs > 0 ? (int) DSBVOLUME_MIN : volume );

	// start playback, looping the ring; the worker keeps it fed
	if( s->buffer->Play( 0, 0, DSBPLAY_LOOPING ) != DS_OK ){
		oggTeardown( s );
		return false;
	}

	// spawn worker
	s->stop   = false;
	s->thread = (HANDLE) _beginthreadex( NULL, 0, oggWorker, s, 0, NULL );
	if( !s->thread ){
		oggTeardown( s );
		return false;
	}
	return true;
}

// =============================================================================
// Public API (declared in funcs.h)
// =============================================================================

// True if `name` is an .ogg path (case-insensitive). Callers use this to decide
// between the OGG streamer and the Storm DDA (wav) path.
bool OggStream_IsOgg( const char* name )
{
	if( !name ) return false;
	size_t n = strlen( name );
	if( n < 4 ) return false;
	const char* e = name + n - 4;
	return ( e[0] == '.' )
	    && ( e[1] == 'o' || e[1] == 'O' )
	    && ( e[2] == 'g' || e[2] == 'G' )
	    && ( e[3] == 'g' || e[3] == 'G' );
}

// Start streaming `name` on channel `ch` (OGGCH_MUSIC/OGGCH_AMBIENT).
// loop = SFILE_DDA_LOOP semantics; volume in VOL_MIN..VOL_MAX.
// (th4) fadeInMs > 0: the track starts silent and fades in (MUSIC CROSSFADES); a
// track handed to OggStream_FadeOut just before keeps fading out meanwhile.
bool OggStream_Play( int ch, char* name, bool loop, int volume, int fadeInMs /* = 0 */ )
{
	OggStream* s = oggSlot( ch );
	if( !s ) return false;
	oggInitOnce();
	oggReap();
	// oggStart() calls oggTeardown() first, which joins any running worker before
	// touching shared state -- no lock needed (see the locking-model note above).
	return oggStart( s, name, loop, volume, fadeInMs );
}

// Stop at once. For music that includes every track still fading out.
void OggStream_Stop( int ch )
{
	if( !oggSlot( ch ) ) return;
	if( !g_oggInit ) return;
	if( ch == OGGCH_MUSIC ){
		for( int i = OGG_SLOT_MUSIC0; i < OGG_SLOT_COUNT; i++ ) oggTeardown( &g_ogg[i] );
		return;
	}
	oggTeardown( oggSlot( ch ) ); // joins worker, then releases buffer/decoder/handle
}

// (th4) Hand the playing music track over to a fade-out of `fadeMs` and move the
// music channel to a free slot for the next track (MUSIC CROSSFADES above). When
// every slot is busy, the quietest fading track (furthest along its fade-out) is
// dropped at once to make room. A track caught in the middle of its own fade-in
// fades out from the level it has reached, without a jump.
// Only the music has spare slots; any other channel just stops.
void OggStream_FadeOut( int ch, int fadeMs )
{
	if( ch != OGGCH_MUSIC || fadeMs <= 0 ){ OggStream_Stop( ch ); return; }
	if( !g_oggInit ) return;
	oggReap();
	OggStream* cur = oggSlot( ch );
	if( !cur->buffer ) return;		// nothing playing (older tracks may still be fading out: leave them)
	const DWORD now = GetTickCount();
	// the slot for the next track: a free one, else the quietest fading track, dropped now (so the music
	// channel never points at a fading track -- OggStream_Active / StopMusic rely on that)
	int next = -1;
	float furthest = -1.0f;
	for( int i = OGG_SLOT_MUSIC0; i < OGG_SLOT_COUNT; i++ ){
		if( i == g_musicSlot ) continue;
		OggStream* s = &g_ogg[i];
		if( !s->buffer ){ next = i; break; }
		EnterCriticalSection( &g_fadeLock );
		const DWORD el = now - s->fadeStart, ms = s->fadeMs ? s->fadeMs : 1;
		LeaveCriticalSection( &g_fadeLock );
		const float share = el >= ms ? 1.0f : (float) el / (float) ms;
		if( share > furthest ){ furthest = share; next = i; }
	}
	if( g_ogg[ next ].buffer ) oggTeardown( &g_ogg[ next ] );
	EnterCriticalSection( &g_fadeLock );
	DWORD start = now;
	if( cur->fadeMs && !cur->fadeOut ){
		// still fading in at gain sin(t * pi/2): start the fade-out where cos(t' * pi/2) is the same gain
		const DWORD el = now - cur->fadeStart;
		const float t = el >= cur->fadeMs ? 1.0f : (float) el / (float) cur->fadeMs;
		const float tOut = 1.0f - t;	// cos((1 - t) * pi/2) == sin(t * pi/2)
		start = now - (DWORD)( tOut * fadeMs );
	}
	cur->fadeOut = true; cur->fadeStart = start; cur->fadeMs = (DWORD) fadeMs;
	LeaveCriticalSection( &g_fadeLock );
	g_musicSlot = next;
}

void OggStream_SetVolume( int ch, int volume )
{
	if( !oggSlot( ch ) ) return;
	if( !g_oggInit ) return;
	LimitToRange( volume, (int) DSBVOLUME_MIN, (int) DSBVOLUME_MAX );
	// (th4) music: the playing track AND every one still fading out follow the volume setting
	const int first = ch == OGGCH_MUSIC ? OGG_SLOT_MUSIC0 : OGG_SLOT_AMBIENT;
	const int last  = ch == OGGCH_MUSIC ? OGG_SLOT_COUNT - 1 : OGG_SLOT_AMBIENT;
	for( int i = first; i <= last; i++ ){
		OggStream* s = &g_ogg[i];
		if( !s->buffer || s->done ) continue;
		EnterCriticalSection( &g_fadeLock );
		s->fadeVol = volume;					// a running fade now heads for / starts from this
		const bool fading = s->fadeMs != 0;
		LeaveCriticalSection( &g_fadeLock );
		if( !fading ) s->buffer->SetVolume( volume ); // SetVolume is safe to call while the worker runs
	}
}

// True if channel currently has an active OGG stream (used so Stop/Volume on the
// wav path can tell whether an ogg stream owns the channel instead). A music track
// that is only fading out does not count: the channel is free for the next one.
bool OggStream_Active( int ch )
{
	if( !oggSlot( ch ) ) return false;
	if( !g_oggInit ) return false;
	return oggSlot( ch )->buffer != 0;
}
