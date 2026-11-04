#include "stdafx.h"

//----- (0046B770) --------------------------------------------------------
void __fastcall DupSoundUpdate( bool stopAll )
{
    for( int i = MaxDupSounds - 1; i >= 0; --i ){
        auto& buffer = SoundDups[i].buffer;
		if( ! buffer ) continue;
        if( ! stopAll ){
			if( DWORD st; buffer->GetStatus(&st) == DS_OK && st == DSBSTATUS_PLAYING ) return;
		}
		buffer->Stop();
		ReleaseZero(buffer);
    }
}

int LastDupSoundIndex, MaxDupSoundIndex;
//----- (0046B8D4) --------------------------------------------------------
IDirectSoundBuffer* __fastcall SoundDuplicate(Sound& sound) // snd_dup_snd
{
	// DuplicateSoundBuffer may causes memory leaks, so it can be disabled (see DuplicateSounds option)
	// https://groups.google.com/g/microsoft.public.win32.programmer.directx.audio/c/hXb7_btMhik/m/oIKzWCQp7A4J
	// trying to use sorted DuplicateSoundBuf access to avoid leaks
	// also for safe sounds duplicating may will be better to use new APIs (XAudio/OpenAL)
	if( ! DuplicateSounds ) return nullptr;

	for( int i = 0; i < MaxDupSounds; ++i ){
		SoundDup& dup = SoundDups[i];
		if( dup.buffer ){
			if( dup.id == sound.id ){
				if( DWORD st; dup.buffer->GetStatus(&st) == DS_OK && st != DSBSTATUS_PLAYING ){
					LastDupSoundIndex = i;
					if( LastDupSoundIndex > MaxDupSoundIndex ) MaxDupSoundIndex = LastDupSoundIndex;
					return dup.buffer;
				}
			}
			continue;
		}
		HRESULT res = DirectSoundPtr->DuplicateSoundBuffer(sound.buffer, &dup.buffer);
		if( res != DS_OK ){
			#if 0 // skiping error to wait for free sound memory
			DirectSoundError(res, __LINE__, __FILE__);
			#endif
			return nullptr;
		}
		dup.id = sound.id;
		LastDupSoundIndex = i;
		if( LastDupSoundIndex > MaxDupSoundIndex ) MaxDupSoundIndex = LastDupSoundIndex;
		return dup.buffer;
	}
	return nullptr;
}

//----- (0046B7CF) --------------------------------------------------------
int __fastcall ThisSoundIsPlaying(Sound& sound) // snd_playing
{
    DWORD status;
    if( sound.buffer && ! sound.buffer->GetStatus(&status) ){
        return status == DSBSTATUS_PLAYING;
    }else{
        return 0;
    }
}

//----- (0046B7FB) --------------------------------------------------------
void __fastcall PlaySound(Sound& sound, int range, int angle) // snd_play_snd
{
	IDirectSoundBuffer* buffer = sound.buffer;
	if( ! IsSoundEnable || ! buffer ) return;

	i64 curTickCount = GetTickCountTh(); // (rt) this timer doesn't affect gameplay, not intercepted
    if( curTickCount - sound.lastPlayTime < MIN_REPLAY_THRESHOLD ) return;

	if( ThisSoundIsPlaying(sound) ){
        buffer = SoundDuplicate(sound);
        if( ! buffer ) return;
    }
    LONG volume = SoundVolume + range;
    LimitToRange(volume, VOL_MIN, 0l);
    buffer->SetVolume(volume);
    buffer->SetPan(angle);
	DWORD playResult = buffer->Play(0, 0, 0);
    if( playResult == DSERR_BUFFERLOST ){// Buffer lost
        if( RestoreSoundBuffer(sound, buffer) ){
            buffer->Play(0, 0, 0);
        }
    }else if( playResult != DS_OK ){
        DirectSoundError(playResult, __LINE__, __FILE__);
    }
    sound.lastPlayTime = curTickCount;
}

//----- (0046B916) --------------------------------------------------------
bool __fastcall RestoreSoundBuffer( Sound& sound, IDirectSoundBuffer* buffer )
{
    if( buffer->Restore() != DS_OK ){
        return false;
    }
    bool isSuccess = false;

    // (th4) sound.offset==0 marks an OGG sound (see LoadSound_Ogg): its backing
    // store is decoded PCM, not the raw file, so we must re-decode on restore
    // rather than raw-reading compressed bytes into the buffer. WAV keeps the
    // fast raw-reread path (sound.offset points at the RIFF data chunk).
    int extension = ToLowerExtension( *(int*)( sound.name + strlen( sound.name ) - 4 ) );
    bool isOgg = ( extension == 'ggo.' );

    LPVOID aBufPtr;
    void* ppvAydioPtr2;
    DWORD ppvAydioBytes2, nNumberOfBytesToRead;

    if( isOgg ){
        HANDLE fileHandle;
        if( !File_Open( sound.name, &fileHandle, FIVE_TRY ) ){
            return false;
        }
        WAVEFORMATEX fmt;
        uint pcmSize = 0;
        char* pcm = CreateFilePlayCTX_Ogg( fileHandle, &fmt, &pcmSize );
        File_Close( fileHandle );
        if( !pcm ){
            return false;
        }
        if( !buffer->Lock( 0, sound.size, &aBufPtr, &nNumberOfBytesToRead, &ppvAydioPtr2, &ppvAydioBytes2, 0 ) ){
            uint copy = nNumberOfBytesToRead;
            if( copy > pcmSize ) copy = pcmSize;
            memcpy( aBufPtr, pcm, copy );
            if( !buffer->Unlock( aBufPtr, nNumberOfBytesToRead, ppvAydioPtr2, ppvAydioBytes2 ) ){
                isSuccess = true;
            }
        }
        FreeMem( pcm );
        return isSuccess;
    }

    HANDLE fileHandle;
    File_Open(sound.name, &fileHandle, FIVE_TRY);
    File_Seek(fileHandle, sound.offset, 0, 0);
    if( !buffer->Lock(0, sound.size, &aBufPtr, &nNumberOfBytesToRead, &ppvAydioPtr2, &ppvAydioBytes2, 0) ){
        File_Read(fileHandle, aBufPtr, nNumberOfBytesToRead);
        if( !buffer->Unlock(aBufPtr, nNumberOfBytesToRead, ppvAydioPtr2, ppvAydioBytes2) ){
            isSuccess = true;
        }
    }
    File_Close(fileHandle);
    return isSuccess;
}

//----- (0046B9A9) --------------------------------------------------------
void __fastcall LoadSound_Wav(Sound& sound)
{
    if( ! DirectSoundPtr || ! sound.name ) return;

	HANDLE soundFileHandle;
    if( !File_Open(sound.name, &soundFileHandle, FIVE_TRY) ){
        TerminateWithError("Unable to open sound file %s", sound.name);
	}
	//if( sound.buffer ); // TODO: add clear prev sound ?
	sound.lastPlayTime = GetTickCountTh() - MIN_REPLAY_THRESHOLD - 1; // (rt) this timer doesn't affect gameplay, not intercepted

    char* cachePtr = CreateFilePlayCTX_Wav(soundFileHandle, &sound.WAVEFORMAT, &sound.size);
    if( !cachePtr ){
        TerminateWithError("Invalid sound format on file %s", sound.name);
    }
    if( CreateDirectSoundBuffer(sound) ){
		void *lpvAudioPtr1;
		void *lpvAudioPtr2;
		DWORD dwAudioBytes1;
		DWORD dwAudioBytes2;
		DWORD errorCode = sound.buffer->Lock(0, sound.size, &lpvAudioPtr1, &dwAudioBytes1, &lpvAudioPtr2, &dwAudioBytes2, 0);
		if( DS_OK != errorCode ){
			DirectSoundError(errorCode, __LINE__, __FILE__);
		}
		memcpy(lpvAudioPtr1, &cachePtr[sound.offset], dwAudioBytes1);
		errorCode = sound.buffer->Unlock(lpvAudioPtr1, dwAudioBytes1, lpvAudioPtr2, dwAudioBytes2);
		if( DS_OK != errorCode ){
			DirectSoundError(errorCode, __LINE__, __FILE__);
		}
	}
    FreeMem(cachePtr);
    File_Close(soundFileHandle);
}

//----- (0046BAA5) --------------------------------------------------------
bool __fastcall CreateDirectSoundBuffer(Sound& sound)
{
    DSBUFFERDESC dsBufferDesc;
	memset(&dsBufferDesc, 0, sizeof( DSBUFFERDESC ));// The size of the old version of DSBUFFERDESC 20
	dsBufferDesc.dwSize = sizeof( DSBUFFERDESC );
    dsBufferDesc.dwBufferBytes = sound.size;
    dsBufferDesc.lpwfxFormat = &sound.WAVEFORMAT;
    dsBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_STATIC;
	if( BackgroundSound ){
		dsBufferDesc.dwFlags |= DSBCAPS_GLOBALFOCUS;
	}
    DWORD errorCode = DirectSoundPtr->CreateSoundBuffer(&dsBufferDesc, &sound.buffer, nullptr);
	if( errorCode != DS_OK || sound.buffer == 0 ){
        DirectSoundError(errorCode, __LINE__, __FILE__);
		return false;
    }
	return true;
}

//----- (0046BB02) --------------------------------------------------------
void __fastcall FreeSoundData( Sound& sound)
{
    if( sound.buffer ){
		sound.buffer->Stop();
		ReleaseZero( sound.buffer );
	}
}

//----- (0046BB2C) --------------------------------------------------------
bool __fastcall AllocateDirectSound(HWND aHWND)
{
    if( ! AllSound ){
        SoundVolume   = VOL_MIN; IsSoundEnable    = 0;
		MusicVolume   = VOL_MIN; IsMusicEnabled   = 0;
		AmbientVolume = VOL_MIN; IsAmbientEnabled = 0;
		return false;
    }else{
        GetVolumes("Sound Volume",   &SoundVolume);   IsSoundEnable    = SoundVolume   > VOL_MIN;
        GetVolumes("Music Volume",   &MusicVolume);   IsMusicEnabled   = MusicVolume   > VOL_MIN;
		GetVolumes("Ambient Volume", &AmbientVolume); IsAmbientEnabled = AmbientVolume > VOL_MIN;
		if( CallDirectSoundCreate(0, &DirectSoundPtr, 0) ){
			DirectSoundPtr = 0;
		}
		if( DirectSoundPtr && !DirectSoundPtr->SetCooperativeLevel(aHWND, DSSCL_NORMAL) ){
			SetDirectSoundBufferFormat(0);
		}
		SVidInitialize( (int)DirectSoundPtr);
		SFileDdaInitialize((int)DirectSoundPtr);
		IsDirectSoundAllocated = DirectSoundPtr != 0;
		return IsDirectSoundAllocated;
	}
}

//----- (0046BBD2) --------------------------------------------------------
void __fastcall GetVolumes( char* key, int* value )
{
    int data = *value;
    if( !SRegLoadValue("Hellfire", key, 0, &data) ){
        data = 0;
    }
    LimitToRange(data, VOL_MIN, 0);
    *value = data;
    *value -= *value % VOL_STEP;
}

//----- (0046BC23) --------------------------------------------------------
void __fastcall SetDirectSoundBufferFormat(HANDLE directSoundHandle)
{
	DWORD errorCode;
	if( !directSoundHandle ){
		DSBUFFERDESC dsBufferDesc;
		memset(&dsBufferDesc, 0, sizeof( DSBUFFERDESC ));//The size of the old version of DSBUFFERDESC 20
		dsBufferDesc.dwSize = sizeof( DSBUFFERDESC );
		dsBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
		//dsBufferDesc.dwBufferBytes = 0;
		//dsBufferDesc.dwReserved = 0;
		//dsBufferDesc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
		//dsBufferDesc.dwBufferBytes = 3 * waveFormat.nAvgBytesPerSec;
		//dsBufferDesc.lpwfxFormat = &waveFormat;
		errorCode = DirectSoundPtr->CreateSoundBuffer(&dsBufferDesc, &IDirectSoundBufferPtr, 0);
		if( errorCode ){
			DirectSoundError(errorCode, __LINE__, __FILE__);
		}
	}
	if( !IDirectSoundBufferPtr ){
		return;
	}

	_DSCAPS dsCaps;
	dsCaps.dwSize = sizeof( _DSCAPS ); // (rt) 96 in TH1
	errorCode = DirectSoundPtr->GetCaps(&dsCaps);// not used??
	if( errorCode ){
		DirectSoundError(errorCode, __LINE__, __FILE__);
	}

	WAVEFORMATEX waveFormat;
	if( !directSoundHandle || !CreateFilePlayCTX2(directSoundHandle, &waveFormat) ){
		memset(&waveFormat, 0, sizeof( WAVEFORMATEX ));
		waveFormat.wFormatTag = 1;
		waveFormat.nSamplesPerSec = 22050;
		waveFormat.wBitsPerSample = 16;
		waveFormat.cbSize = 0;
	}
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = 2 * waveFormat.wBitsPerSample / 8;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	IDirectSoundBufferPtr->SetFormat(&waveFormat);
}

//----- (0046BD25) --------------------------------------------------------
HRESULT __fastcall CallDirectSoundCreate(LPGUID guid, IDirectSound** ppDS, IUnknown* pUnkOuter)
{
    if( !DirectSoundModule ){
        DirectSoundModule = LoadLibraryA("dsound.dll");
    }
    if( !DirectSoundModule ){
		DirectSoundError(GetLastError(), __LINE__, __FILE__); //ErrorIn_CPP_File(ID_DIALOG5, GetLastError(), __FILE__, __LINE__);
    }
    HRESULT(__stdcall * DirectSoundCreate)(LPGUID, IDirectSound**, IUnknown*) = (HRESULT (__stdcall*)(LPGUID, IDirectSound **, IUnknown*))GetProcAddress(	DirectSoundModule, "DirectSoundCreate");
    if( !DirectSoundCreate ){
		DirectSoundError(GetLastError(), __LINE__, __FILE__); //ErrorIn_CPP_File(ID_DIALOG5, GetLastError(), __FILE__, __LINE__);
    }
    return DirectSoundCreate(guid, ppDS, pUnkOuter);
}

//----- (0046BDA6) --------------------------------------------------------
void ReleaseDirectSound()
{
    OggStream_Stop( OGGCH_MUSIC );   // (th4) join ogg worker threads before
    OggStream_Stop( OGGCH_AMBIENT ); // the device is released
    SVidDestroy();
    SFileDdaDestroy();
    if( DirectSoundPtr ){
        DirectSoundPtr->Release();
        DirectSoundPtr = 0; // one of the places of gamecrash when leaving the game in 640 to 480
    }
    if( IsDirectSoundAllocated ){ 
        IsDirectSoundAllocated = false;
        SaveVolume("Sound Volume", SoundVolume);
		SaveVolume("Music Volume", MusicVolume);
		SaveVolume("Ambient Volume", AmbientVolume);
    }
}

//----- (0046BDFF) --------------------------------------------------------
int __fastcall SaveVolume( char* valueName, int value )
{
    return SRegSaveValue("Hellfire", valueName, 0, value);
}

// (th4) MUSIC CROSSFADES: how long a music track takes to fade out, and the next one to fade in.
// The two overlap (OggStream.cpp keeps a fading track on its own slot), so a level change, a town
// portal or a save loaded from the in-game menu crossfades instead of cutting. Only .ogg tracks
// fade (all of TH4's music is .ogg); a .wav track still stops dead. Callers that pass fade = true:
// Main.cpp ClearPrevSessionData + LoadGameLevel's PlayMusic after the level is loaded (not when
// startNewGame). Switching between the game and the META MENU (main menu, hero select / creation:
// MainMenu.cpp DoMenu, StartMenuMusic) is instant on purpose, as are videos, music off in the
// options, shutdown and the sound device restart. CLASSIC never crossfades: StopMusic / PlayMusic
// drop the flag there.
static const int MUSIC_FADE_MS = 6000;

// (th4) the level type (Dungeon->graphType: town, church, catacomb, ...) the level music was last picked for,
// -1 = none yet. Main.cpp LoadGameLevel reads it before the new level's PlayMusic: a SAVE LOADED IN GAME
// into a different level type switches the music at once instead of crossfading (Mordor).
static int MusicGraphType = -1;
int Music_GraphType(){ return MusicGraphType; }

//----- (0046BE0E) --------------------------------------------------------
void StopMusic( bool fade /* = false */ )
{
    if( GameMode == GM_CLASSIC ) fade = false;	// (th4) Classic music never crossfades (Mordor's call)
    if( OggStream_Active( OGGCH_MUSIC ) ){ // (th4) ogg-streamed track
        if( fade ) OggStream_FadeOut( OGGCH_MUSIC, MUSIC_FADE_MS );	// (th4) fades out on its own, see MUSIC_FADE_MS
        else       OggStream_Stop( OGGCH_MUSIC );
        MusicName = 0;
        return;
    }
    if( !fade ) OggStream_Stop( OGGCH_MUSIC );	// (th4) a hard stop also silences a track still fading out
    if( !MusicFile ){
        return;
    }
    SFileDdaEnd(MusicFile);
    SFileCloseFile(MusicFile);
    MusicFile = 0;
	MusicName = 0;
}

//----- (0046BE0E) --------------------------------------------------------
void StopAmbient()
{
	if( OggStream_Active( OGGCH_AMBIENT ) ){ // (th4) ogg-streamed track
		OggStream_Stop( OGGCH_AMBIENT );
		AmbientName = 0;
		return;
	}
	if( !AmbientFile ){
		return;
	}
	SFileDdaEnd(AmbientFile);
	SFileCloseFile(AmbientFile);
	AmbientFile = 0;
	AmbientName = 0;
}

// TODO: move to DungeonTypes table -------------------------------
char* origi[DT_COUNT] = {
    "Music\\or\\dtowne.ogg",
    "Music\\or\\dlvla.ogg",
    "Music\\or\\dlvlb.ogg",
    "Music\\or\\dlvlc.ogg",
    "Music\\or\\dlvld.ogg",
    "Music\\or\\dlvle.ogg",
    "Music\\or\\dlvlf.ogg",
};
char* ambient_default[DT_COUNT] = {
    "sfx\\ambience\\scene\\amb_town_vol29.ogg", //"sfx\\ambience\\scene\\town3day.wav", 
    "sfx\\ambience\\scene\\crypt.wav", 
    "sfx\\ambience\\scene\\catacombs.wav", 
    "sfx\\ambience\\scene\\lava.wav", 
    "sfx\\ambience\\scene\\cathedral.wav", 
    "sfx\\ambience\\scene\\creepywind.wav", 
    "sfx\\ambience\\scene\\sewer.wav",
};
char* ice_age[DT_COUNT] = {
    "music\\th2_tris_v2_04.ogg",
    "music\\dungeondeep.ogg",
    "music\\ghostly.ogg",
    "music\\caverns_of_ice.ogg",
    "Music\\ambience_generic.ogg",
    "Music\\002_Abyss2.ogg",
    "Music\\ambience_nex7.ogg",
};
char* ice_age2[DT_COUNT] = {
    "music\\th2_tris_v4_00_ext.ogg", //"music\\th2_tris_v2_03.wav",
    "music\\dungeondeep.ogg",
    "music\\ghostly.ogg",
    "music\\caverns_of_ice.ogg",
    "Music\\ambience_gru.ogg",
    "Music\\002_Abyss2.ogg",
    "Music\\ambience_nex7.ogg",
};
char* ice_age3[DT_COUNT] = {
    "music\\th2_tris_v3_00.ogg",
    "music\\dungeondeep.ogg",
    "music\\ghostly.ogg",
    "music\\caverns_of_ice.ogg",
    "Music\\planar.ogg",
    "Music\\002_Abyss2.ogg",
    "Music\\ambience_nex7.ogg",
};
char* ambient_ice_age[DT_COUNT] = {
    "sfx\\ambience\\scene\\wildernessday2.wav",
    "sfx\\ambience\\scene\\crypt.wav",
    "sfx\\ambience\\scene\\catacombs.wav",
    "sfx\\ambience\\scene\\harem.wav",
    "sfx\\ambience\\scene\\cathedral.wav",
    "sfx\\ambience\\scene\\creepywind.wav",
    "sfx\\ambience\\scene\\sewer.wav",
};


//----- (0046BE3A) --------------------------------------------------------
void __fastcall PlayMusic(char* musicName /* = 0 */, bool fade /* = false */ )
{
	if( GameMode == GM_CLASSIC ) fade = false;	// (th4) Classic music never crossfades (Mordor's call)
    if( ! DirectSoundPtr || ! IsMusicEnabled ) return;
	bool iceage = Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE);
    bool terminator = Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR);
	if( ! musicName ){
		NonGameplaySeed = grand();
		Players[CurrentPlayerIndex].seed2 = NonGameplaySeed;
		// TOWN = 0, CHURCH = 1, CATACOMB = 2, CAVE = 3, HELL = 4, CRYPT = 5, ABYSS = 6
        int dungeonType = Dungeon->graphType;
        MusicGraphType = dungeonType;	// (th4) see Music_GraphType
        if (GameMode == GM_CLASSIC) {
            goto ORMUZ;
        }
        if (Doom_Music) {
            if (Dungeon->isQuest) {
                switch ((DUNGEON)Dungeon) {
                case DUN_52_MOSSY_GROTTO:
                case DUN_109_HALL_OF_HEROES:
                case DUN_26_BONE_CHAMBER:
                case DUN_53_VAULTKEEP:                      musicName = (char*)RandFrom("Music\\doom\\quest_BarrelsOFun.ogg", "Music\\doom\\quest_BarrelsOFun2.ogg"); break;
                case DUN_34_THE_PIT:
                case DUN_51_FETID_CAVERN:                   musicName = "Music\\doom\\quest_Waste.ogg";                                                         break;
                case DUN_50_TEMPLE_OF_SUNSET:
                case DUN_49_SICK_VILLAGERS_BASEMENT:
                case DUN_28_POISONED_WATER_SUPPLY:          musicName = "Music\\doom\\quest_DonnatotheRescue.ogg";                                              break;
                case DUN_29_ARCHBISHOP_LAZARUS_LAIR:        
                case DUN_44_RAVENHOLM:
                case DUN_25_KING_LEORICS_TOMB:
                case DUN_37_DISUSED_RELIQUARY:
                case DUN_46_COLISEUM:                       musicName = (char*)RandFrom("Music\\doom\\quest_SignofEvil.ogg", "Music\\doom\\quest_SignDARv2.ogg");  break;
                case DUN_43_HEPHASTO:
                case DUN_54_ABANDONED_FANE:                 musicName = "Music\\doom\\quest_AtDoomsGate.ogg";                                                   break;
                case DUN_33_SUMMONING_ROOMS:
                case DUN_41_FLESHDOOM:
                case DUN_42_WIELDER:                        musicName = (char*)RandFrom("Music\\doom\\quest_DemonsonthePrey.ogg", "Music\\w4r2pv2.ogg");        break;
                case DUN_35_PASSAGE_OF_FIRE:
                case DUN_45_THE_DEN:                        musicName = "Music\\POFv2.ogg";                                                                     break;
                case DUN_111_CHURCH_SACRISTY:
                case DUN_30_HALLS_OF_ANGUISH: 
                case DUN_31_ANDARIELS_THRONE:               musicName = "Music\\doom\\quest_Sini.ogg";                                                          break;
                case DUN_55_DEEP_HOLLOW:
                case DUN_59_HALL_OF_FIRE:
                case DUN_58_GATEWAY_OF_BLOOD:
                case DUN_32_HORAZONS_HAVEN:
                case DUN_47_DARK_NEST:                      musicName = (char*)RandFrom("Music\\doom\\quest_Crusher.ogg", "Music\\doom\\quest_Crusher_2.ogg");  break;
                case DUN_48_BLOOD_HIVE:                     musicName = (char*)RandFrom("Music\\doom\\quest_DS2.ogg", "Music\\doom\\quest_DeadSimple.ogg");     break;
                case DUN_27_THE_INFESTED_CELLAR:
                case DUN_56_WEB_TUNNELS:
                case DUN_57_ARACHNID_LAIR:
                case DUN_36_VALLEY_OF_DESPAIR:              musicName = "Music\\doom\\quest_SignAH.ogg";                                                        break;
                case DUN_110_BLACK_CHAPEL:
                case DUN_40_ISLAND:                         musicName = "Music\\doom\\quest_Id.ogg";                                                            break; 
                case DUN_117_DARKLORD_HIDEOUT:              musicName = "Music\\doom\\0.ogg";                                                                   break;
                case DUN_38_GLACIAL_CAVERNS:
                case DUN_39_FROZEN_LOCH:
                default:                                    musicName = (char*)RandFrom("Music\\doom\\quest_SignofEvil.ogg", "Music\\doom\\quest_DeepIntoTheCode_v2_75vol.ogg"); break;
                }
                if (Dungeon >= DUN_60_HAUNTED_LABYRINTH && Dungeon <= DUN_108_ARCANE_PRISON) {
                    musicName = (char*)RandFrom("Music\\doom\\quest_SignofEvil.ogg", "Music\\doom\\quest_DeepIntoTheCode_v2_75vol.ogg", "Music\\doom\\quest_Id.ogg", 
                        "Music\\doom\\quest_SignAH.ogg", "Music\\doom\\quest_DS2.ogg", "Music\\doom\\quest_DeadSimple.ogg", "Music\\doom\\quest_Crusher.ogg", 
                        "Music\\doom\\quest_Crusher_2.ogg", "Music\\doom\\quest_Sini.ogg", "Music\\POFv2.ogg", "Music\\doom\\quest_DemonsonthePrey.ogg", "Music\\w4r2pv2.ogg", 
                        "Music\\doom\\quest_AtDoomsGate.ogg", "Music\\doom\\quest_SignofEvil.ogg", "Music\\doom\\quest_SignDARv2.ogg", "Music\\doom\\quest_DonnatotheRescue.ogg", 
                        "Music\\doom\\quest_Waste.ogg", "Music\\doom\\quest_BarrelsOFun.ogg", "Music\\doom\\quest_BarrelsOFun2.ogg");
                }
            }
            else {
                switch (dungeonType) {
                case DT_0_TOWN:         musicName = "Music\\doom\\0.ogg";                                                                                                           break; 
                case DT_1_CHURCH:       musicName = (char*)RandFrom("Music\\doom\\01-1.ogg", "Music\\doom\\01-2.ogg", "Music\\doom\\01-3.ogg");                                     break; 
                case DT_2_CATACOMB:     musicName = (char*)RandFrom("Music\\doom\\02-1.ogg", "Music\\doom\\02-2.ogg", "Music\\doom\\02-3.ogg", "Music\\doom\\quest_Traps_v2.ogg");  break;
                case DT_3_CAVE:         musicName = (char*)RandFrom("Music\\doom\\03-1.ogg", "Music\\doom\\03-2.ogg", "Music\\doom\\03-3.ogg", "Music\\doom\\03-4v2.ogg");          break;
                case DT_4_HELL:         musicName = (char*)RandFrom("Music\\doom\\04-1.ogg", "Music\\doom\\04-2.ogg");                                                              break; 
                case DT_5_CRYPT:        musicName = (char*)RandFrom("Music\\doom\\06-1.ogg", "Music\\doom\\06-2.ogg");                                                              break; 
                case DT_6_ABYSS:        musicName = (char*)RandFrom("Music\\doom\\05-1.ogg", "Music\\doom\\05-2.ogg", "Music\\doom\\05-3.ogg");                                     break;
                }
            }
        }
        else if (terminator) {
            switch (dungeonType) {
            case DT_0_TOWN:             musicName = "Music\\term\\eye_surgery_v2.ogg";    break;
            case DT_1_CHURCH:           musicName = "Music\\term\\xplore.ogg";            break;
            case DT_2_CATACOMB:         musicName = "Music\\term\\infil.ogg";             break;
            case DT_3_CAVE:             musicName = "Music\\term\\gar_v2.ogg";            break;
            case DT_4_HELL:             musicName = "Music\\term\\polst_v3(-40vol).ogg";  break;
            case DT_5_CRYPT:            musicName = "Music\\term\\infil2.ogg";            break;
            case DT_6_ABYSS:            musicName = "Music\\term\\factory.ogg";           break;
            }   
            if (Dungeon->isQuest) { 
                switch ((DUNGEON)Dungeon) {
                case DUN_54_ABANDONED_FANE:             musicName = "Music\\term\\HKv2(-40vol).ogg";        break;
                case DUN_59_HALL_OF_FIRE:
                case DUN_35_PASSAGE_OF_FIRE:
                case DUN_58_GATEWAY_OF_BLOOD:
                case DUN_121_UNHOLY_ALTAR_DC:                                                               // (th4) DC mission 13
                case DUN_29_ARCHBISHOP_LAZARUS_LAIR:    musicName = "Music\\term\\T-850v2(-60vol).ogg";     break;
                default:                                musicName = (char*)RandFrom(
                                                        "Music\\term\\mackv2(-25vol).ogg", 
                                                        "Music\\term\\HKv2(-40vol).ogg"
                                                        ); break;
                }
            }
        }
        else if( OriginalMusic ){
            ORMUZ:
			musicName = origi[dungeonType];
        }
        else if( Dungeon->isQuest ){ // default music selections start from here >>>
            switch( (DUNGEON)Dungeon ){
            case DUN_27_THE_INFESTED_CELLAR    : musicName = "music\\Abs02.ogg";            break;
            case DUN_30_HALLS_OF_ANGUISH       : musicName = "music\\Dintr1.ogg";           break;
            case DUN_31_ANDARIELS_THRONE       : musicName = "music\\Dintro.ogg";           break;
            case DUN_36_VALLEY_OF_DESPAIR      : musicName = "music\\valley.ogg";           break;
            case DUN_45_THE_DEN                : musicName = "music\\POFv2.ogg";            break;
            case DUN_115_VALOR_TOMB:
            case DUN_59_HALL_OF_FIRE:
            case DUN_58_GATEWAY_OF_BLOOD       : musicName = "music\\dtheda.ogg";           break;
            case DUN_73_LAVA_PITS:
            case DUN_38_GLACIAL_CAVERNS:
            case DUN_39_FROZEN_LOCH            : musicName = "music\\Caverns_of_Ice.ogg";   break;
            case DUN_37_DISUSED_RELIQUARY:
            case DUN_114_LOWER_SCRIPTORIUM     : musicName = "music\\DlvlO.ogg";            break; // pick smth else later for scriptorium
            case DUN_49_SICK_VILLAGERS_BASEMENT:
            case DUN_116_MINING_CAVES:                                                          // pick smth else later for mining caves
            case DUN_51_FETID_CAVERN           : musicName = "music\\sgsg.ogg";             break;
            case DUN_118_POISONED_WATER_SUPPLY_DC:                                               // (th4) DC mission 10, same level
            case DUN_28_POISONED_WATER_SUPPLY  : musicName = "music\\DungeonDeep.ogg";      break;
            case DUN_113_THE_MAZE:
            case DUN_117_DARKLORD_HIDEOUT:                                                      // (th4) Dark Campaign hub
            case DUN_44_RAVENHOLM              : musicName = "music\\Dintr4.ogg";           break;
            case DUN_25_KING_LEORICS_TOMB      : musicName = "music\\king1.ogg";            break;
            case DUN_40_ISLAND                 : musicName = "music\\ambience_generic.ogg"; break;
            case DUN_41_FLESHDOOM              :
            case DUN_42_WIELDER                :
            case DUN_43_HEPHASTO               : musicName = "music\\w4r2pv2.ogg";          break;
            case DUN_35_PASSAGE_OF_FIRE        : musicName = "music\\const_time1.ogg";      break;
            case DUN_121_UNHOLY_ALTAR_DC       :                                           // (th4) DC mission 13
            case DUN_29_ARCHBISHOP_LAZARUS_LAIR: musicName = "music\\ALv4.ogg";             break;
            case DUN_110_BLACK_CHAPEL:
            case DUN_53_VAULTKEEP:
            case DUN_32_HORAZONS_HAVEN         : musicName = "music\\planar.ogg";           break;
            case DUN_111_CHURCH_SACRISTY       : musicName = "music\\dlvlN.ogg";            break;
            case DUN_33_SUMMONING_ROOMS        : musicName = "music\\BloodCrypt_v2.ogg";    break;
            case DUN_34_THE_PIT                : musicName = "music\\ambience_nex7.ogg";    break;
            case DUN_109_HALL_OF_HEROES:
            case DUN_26_BONE_CHAMBER           : musicName = "music\\Ghostly.ogg";          break;
            case DUN_46_COLISEUM               : musicName = "music\\DlvlQ.ogg";            break;
            case DUN_47_DARK_NEST:
            case DUN_55_DEEP_HOLLOW:
            case DUN_48_BLOOD_HIVE             : musicName = "music\\hive.ogg";             break;
            case DUN_50_TEMPLE_OF_SUNSET       : musicName = "music\\th2_cata_doom.ogg";    break;
            case DUN_56_WEB_TUNNELS:
            case DUN_57_ARACHNID_LAIR          : musicName = "music\\spiders.ogg";          break;
            case DUN_52_MOSSY_GROTTO           : musicName = "music\\Mossy_Grotto.ogg";     break;
            case DUN_54_ABANDONED_FANE         : musicName = "music\\hglmuseum.ogg";        break;
            case DUN_112_STILL_SPRINGS         : musicName = "music\\Springs7.ogg";         break;
            case DUN_119_DEEPER_CAVES          : musicName = "music\\DeeperCaves50.ogg";    break;
			}
            if ((Dungeon >= DUN_60_HAUNTED_LABYRINTH && Dungeon <= DUN_108_ARCANE_PRISON) && Dungeon != DUN_73_LAVA_PITS) {
                goto NORMAL_MUSIC;
            }
            if (Dungeon == DUN_120_PANDEMONIUM_DC){ 
                goto NORMAL_MUSIC; 
            }
		}
        else if( iceage ){
            musicName = RandFrom(ice_age, ice_age2, ice_age3)[dungeonType];
		}
        else{
            NORMAL_MUSIC:
            switch (dungeonType) {
            case DT_0_TOWN:         musicName = (char*)RandFrom("Music\\RazTE.ogg", "Music\\DTown3.ogg", "Music\\DTown4.ogg", "Music\\DTown5.ogg", "Music\\th3_twn_var_5.ogg"/*, "music\\Hurdy_v4_vol_25.wav"*/); break;
            case DT_1_CHURCH:       musicName = (char*)RandFrom("Music\\RazA.ogg", "Music\\RazA.ogg", "Music\\DLvlH.ogg", "Music\\DlvlO.ogg", "Music\\th3_chu_var_5.ogg", "Music\\DLvlJ.ogg");     break;
            case DT_2_CATACOMB:     musicName = (char*)RandFrom("Music\\RazB.ogg", "Music\\RazB.ogg", "Music\\DLvlK.ogg", "Music\\DlvlP.ogg", "Music\\th3_cat_var_4.ogg", "Music\\001_Tomb2.ogg"); break;
            case DT_3_CAVE:         musicName = (char*)RandFrom("Music\\RazC.ogg", "Music\\DlvlC.ogg", "Music\\th3_hel_var_4.ogg", "Music\\DLvlL.ogg", "Music\\Alex_Trz_Caverns_v5.ogg"); break;
            case DT_4_HELL:         musicName = (char*)RandFrom("Music\\RazD.ogg", "Music\\DlvlS.ogg", "Music\\DlvlD.ogg", "Music\\th3_cav_var_4.ogg", "Music\\Dtown8.ogg");    break;
            case DT_5_CRYPT:        musicName = (char*)RandFrom("Music\\DlvlE.ogg", "Music\\DLvlT.ogg", "Music\\RazE.ogg", "Music\\002_Abyss2.ogg");                            break;
            case DT_6_ABYSS:        musicName = (char*)RandFrom("Music\\DlvlG.ogg", "Music\\DLvlR.ogg", "Music\\RazF.ogg", "Music\\DLvlF.ogg");                                 break;
            }
		}
        if (GameMode == GM_DARK_CAMPAIGN) {
            switch ((DUNGEON)Dungeon) {
            case DUN_66_SEWERS:         musicName = "music\\dregatar_v4.ogg";             break;
            case DUN_72_NECROPOLIS:     musicName = "music\\Dintr1.ogg";             break;
            case DUN_44_RAVENHOLM:      musicName = "Music\\DTown3.ogg";             break;	// (th4) DC mission 26
            }
        }
       
	}
	//(rt) musicName = "Music\\DLvlL.wav"; // for testing a track replacement in THmusic.mor
	if( musicName && MusicName && strcmp(musicName, MusicName) || ! musicName || ! MusicName ){
		StopMusic( fade );	// (th4) fade: the old track fades out while the new one fades in (MUSIC_FADE_MS)
		if( musicName ){
			if( OggStream_IsOgg( musicName ) ){ // (th4) stream .ogg via our player
				if( OggStream_Play( OGGCH_MUSIC, musicName, true, MusicVolume, fade ? MUSIC_FADE_MS : 0 ) ){
					MusicName = musicName;
				}
			}else{
				int opened = SFileOpenFile( musicName, &MusicFile );
				SetDirectSoundBufferFormat( MusicFile );
				if( opened ){
					SFileDdaBeginEx( MusicFile, DDA_BUF_SIZE, SFILE_DDA_LOOP, 0, MusicVolume, 0, 0 );
					MusicName = musicName;
				}else{
					MusicFile = 0;
				}
			}
		}
	}
}

//----- (0046BE3A) --------------------------------------------------------
// (th4) ambient fades, see PlayAmbient: how long, and how far down before the swap
static const int AMB_FADE_OUT_MS = 6000;
static const int AMB_FADE_IN_MS  = 8000;
static const int AMB_FADE_DEPTH  = 3500;	// 1/100 dB below the ambient volume = practically silent

// (th4) Start an ambient loop at a given volume on whichever player fits the file (.ogg streams
// through OggStream, anything else through Storm DDA). Sets AmbientName on success.
static void AmbientStartTrack( char* name, int volume )
{
	if( OggStream_IsOgg( name ) ){ // (th4) stream .ogg via our player
		if( OggStream_Play( OGGCH_AMBIENT, name, true, volume ) ){
			AmbientName = name;
		}
	}else{
		int opened = SFileOpenFile( name, &AmbientFile );
		SetDirectSoundBufferFormat( AmbientFile );
		if( opened ){
			SFileDdaBeginEx( AmbientFile, DDA_BUF_SIZE, SFILE_DDA_LOOP, 0, volume, 0, 0 );
			AmbientName = name;
		}else{
			AmbientFile = 0;
		}
	}
}

// (th4) Volume of the playing ambient loop only (AmbientVolume, the player's setting, untouched).
static void AmbientSetChannelVolume( int volume )
{
	if( OggStream_Active( OGGCH_AMBIENT ) ){
		OggStream_SetVolume( OGGCH_AMBIENT, volume );
	}else if( AmbientFile ){
		SFileDdaSetVolume( (int)AmbientFile, volume, 0 );
	}
}

void __fastcall PlayAmbient(char* ambientName /* = 0 */ )
{
	if( ! DirectSoundPtr || ! IsAmbientEnabled ) return;
	const bool passedName = ambientName != 0;	// (th4) a caller-chosen name switches at once (see AMBIENT FADES)
	bool iceAge = Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE);
    if (!ambientName) {
        // TOWN = 0, CHURCH = 1, CATACOMB = 2, CAVE = 3, HELL = 4, CRYPT = 5, ABYSS = 6
        int dungeonType = Dungeon->graphType;
        if (is(Dungeon, DUN_38_GLACIAL_CAVERNS, DUN_39_FROZEN_LOCH)) {
            ambientName = ambient_ice_age[dungeonType];
        }
        else if (is(Dungeon, DUN_28_POISONED_WATER_SUPPLY, DUN_51_FETID_CAVERN, DUN_52_MOSSY_GROTTO, DUN_49_SICK_VILLAGERS_BASEMENT, DUN_27_THE_INFESTED_CELLAR, DUN_112_STILL_SPRINGS, DUN_118_POISONED_WATER_SUPPLY_DC)) {
            ambientName = "sfx\\ambience\\scene\\cave.wav";
        }
        else if (Dungeon == DUN_35_PASSAGE_OF_FIRE) {
            ambientName = "sfx\\ambience\\scene\\lava.wav";
        }
        else if (is(Dungeon, DUN_40_ISLAND, DUN_116_MINING_CAVES)) {
            ambientName = "sfx\\ambience\\scene\\creepywind.wav";
        }
        else if (is(Dungeon, DUN_43_HEPHASTO, DUN_42_WIELDER, DUN_41_FLESHDOOM, DUN_36_VALLEY_OF_DESPAIR)) {
            ambientName = "sfx\\ambience\\scene\\hell1.wav";
        }
        else if (Dungeon >= DUN_60_HAUNTED_LABYRINTH && Dungeon <= DUN_108_ARCANE_PRISON) { // alternative dungeons are QUEST type and thus should play standard ambient
            ambientName = ambient_default[dungeonType];
        }
        else if (iceAge) {
            ambientName = ambient_ice_age[dungeonType];
        }
        else {
            ambientName = ambient_default[dungeonType];
        }

        // (th4) Night town ambience (night_01), for the non-winter setting only
        // (Ice Age keeps its own ambience -- no crickets on snow):
        //   - Real Tristram town (DUN_0_TOWN): at night, past the dusk midpoint
        //     of the palette day/night cycle (re-evaluated each palette frame).
        //   - Ravenholm (DUN_44, town tileset): ALWAYS night ambience -- it has
        //     no day/night cycle and reads as a perpetual dead night town.
        // Other levels on the town tileset keep their own ambience.
        if( !iceAge && ( ( Dungeon == DUN_0_TOWN && DayNight_IsNight() )
                      ||   Dungeon == DUN_44_RAVENHOLM ) ){
            ambientName = "sfx\\ambience\\scene\\night_01.ogg";
        }
        // (th4) Storm nights (Weather.cpp: normal town only, never Classic / Ice Age / Ravenholm):
        // the rain loop REPLACES the crickets. It follows the rain the player sees -- on from
        // early dusk to late dawn (DayNight_NightQ8, with a little hysteresis so it cannot flap
        // at the threshold), not the dusk-midpoint latch the crickets use.
        static bool rainSound = false;
        const int nightQ8 = DayNight_NightQ8();
        if( Weather_Tonight() != WEATHER_STORM ) rainSound = false;
        else if( nightQ8 > 24 ) rainSound = true;
        else if( nightQ8 < 8 ) rainSound = false;	// 8: dx_utility.cpp RAIN_SOUND_OFF_NIGHT -- the visible rain ends here too
        if( rainSound ){
            // .ogg on purpose: it streams through OggStream like the other ambience loops. The same
            // loop as a .wav went through Storm's DDA streamer (SFileDdaBeginEx), which restarts a
            // stereo loop off by a sample -- left and right swap on every loop, so the rain seemed
            // to move behind the player and back.
            ambientName = "sfx\\ambience\\01_Rain_Ambience.ogg";
        }
    }

	// (th4) AMBIENT FADES. The town swaps its ambience while the player stands in it (day ->
	// crickets -> day, or day -> rain -> day on storm nights); PlayAmbient() is re-run every palette
	// frame for that (Main.cpp, the town branch of the palette animation). Those swaps now fade:
	// the old loop fades out over AMB_FADE_OUT_MS, then the new one starts silent and fades in over
	// AMB_FADE_IN_MS. The fade is a straight line in decibels (the volume unit here is DirectSound's
	// 1/100 dB), which the ear hears as an even fade, down to AMB_FADE_DEPTH below the ambient
	// volume. Only for a swap inside the same level (ambientName chosen here, not passed in, and
	// the same Dungeon as when the old loop started) -- entering a level, a passed-in name
	// (PauseAmbient) or turning ambience off still switch at once, as before. Works for both
	// players: OggStream (.ogg) and Storm DDA (.wav), through AmbientSetChannelVolume.
	static int ambDungeon = -1;			// Dungeon the playing loop was started on
	static bool ambFading = false;
	static int ambCurVol = 0;
	static i64 ambLastTick = 0;
	const bool changed = ( ambientName && AmbientName && strcmp( ambientName, AmbientName ) ) || !ambientName || !AmbientName;
	const bool autoPick = !passedName;
	const bool fadeOk = autoPick && ambientName && AmbientName && ambDungeon == (int)(DUNGEON)Dungeon;
	if( ( changed || ambFading ) && fadeOk ){
		const i64 now = GetTickCountTh();
		int dtMs = ambFading ? (int)( ( now - ambLastTick ) * 1000 / MAX_FPS ) : 0;
		if( dtMs < 0 ) dtMs = 0; else if( dtMs > 250 ) dtMs = 250;	// a hitch must not skip the fade
		ambLastTick = now;
		if( !ambFading ){ ambFading = true; ambCurVol = AmbientVolume; }
		int floorVol = AmbientVolume - AMB_FADE_DEPTH;
		if( floorVol < DSBVOLUME_MIN ) floorVol = DSBVOLUME_MIN;
		const int span = AmbientVolume - floorVol;
		if( changed ){		// the wrong loop is playing: fade it out, then start the right one silent
			ambCurVol -= span * dtMs / AMB_FADE_OUT_MS;
			if( ambCurVol <= floorVol ){
				StopAmbient();
				AmbientStartTrack( ambientName, floorVol );
				ambCurVol = floorVol;
			}
		}else{				// the right loop: fade it in
			ambCurVol += span * dtMs / AMB_FADE_IN_MS;
			if( ambCurVol >= AmbientVolume ){ ambCurVol = AmbientVolume; ambFading = false; }
		}
		AmbientSetChannelVolume( ambCurVol );
		return;
	}
	if( changed ){
		ambFading = false;
		StopAmbient();
		if( ambientName ){
			AmbientStartTrack( ambientName, AmbientVolume );
			ambDungeon = (int)(DUNGEON)Dungeon;
		}
	}
}

//----- (0046BEA5) --------------------------------------------------------
void __fastcall PauseMusic(bool pause)
{
	static char* pausedMusicName = 0;
	if( pause ){
        pausedMusicName = MusicName;
        StopMusic();
    }else if( pausedMusicName ){
        PlayMusic(pausedMusicName);
    }
}

//----- (0046BEA5) --------------------------------------------------------
void __fastcall PauseAmbient(bool pause)
{
	static char* pausedAmbientName = 0;
	if( pause ){
		pausedAmbientName = AmbientName;
		StopAmbient();
	}else if( pausedAmbientName ){
		PlayAmbient(pausedAmbientName);
	}
}

//----- (0046BEBF) --------------------------------------------------------
int __fastcall GetOrSetMusicVolume(int volume)
{
    if( volume != 1 ){
        MusicVolume = volume;
        OggStream_SetVolume( OGGCH_MUSIC, volume );	// (th4) the playing track and one still fading out (OggStream.cpp)
        if( !OggStream_Active( OGGCH_MUSIC ) && MusicFile ){
            SFileDdaSetVolume((int)MusicFile, volume, 0);
        }
    }
	return MusicVolume;
}

//----- (0046BEBF) --------------------------------------------------------
int __fastcall GetOrSetAmbientVolume(int volume)
{
	if( volume != 1 ){
		AmbientVolume = volume;
		if( OggStream_Active( OGGCH_AMBIENT ) ){ // (th4)
			OggStream_SetVolume( OGGCH_AMBIENT, volume );
		}else if( AmbientFile ){
			SFileDdaSetVolume((int)AmbientFile, volume, 0);
		}
	}
	return AmbientVolume;
}

//----- (0046BEE2) --------------------------------------------------------
int __fastcall GetOrSetSoundVolume(int volume)
{
    if( volume != 1 ){
		SoundVolume = volume;
    }
	return SoundVolume;
}
