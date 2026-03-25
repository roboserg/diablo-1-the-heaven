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

	i64 curTickCount = GetTickCountTh(); // это таймер не влияет на гемплей, не перехватываем
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
    HANDLE fileHandle;
    File_Open(sound.name, &fileHandle, FIVE_TRY);
    File_Seek(fileHandle, sound.offset, 0, 0);
    LPVOID aBufPtr;
    void* ppvAydioPtr2;
    DWORD ppvAydioBytes2, nNumberOfBytesToRead;
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
	sound.lastPlayTime = GetTickCountTh() - MIN_REPLAY_THRESHOLD - 1; // это таймер не влияет на гемплей, не перехватываем

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
	dsCaps.dwSize = sizeof( _DSCAPS ); // 96 в TH1
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
    SVidDestroy();
    SFileDdaDestroy();
    if( DirectSoundPtr ){
        DirectSoundPtr->Release();
        DirectSoundPtr = 0; // one of the places of death when leaving the game in 640 to 480
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

//----- (0046BE0E) --------------------------------------------------------
void StopMusic()
{
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
    "Music\\or\\dtowne.wav",
    "Music\\or\\dlvla.wav",
    "Music\\or\\dlvlb.wav",
    "Music\\or\\dlvlc.wav",
    "Music\\or\\dlvld.wav",
    "Music\\or\\dlvle.wav",
    "Music\\or\\dlvlf.wav",
};
char* ambient_default[DT_COUNT] = {
    "sfx\\ambience\\scene\\amb_town_vol29.wav", //"sfx\\ambience\\scene\\town3day.wav", 
    "sfx\\ambience\\scene\\crypt.wav", 
    "sfx\\ambience\\scene\\catacombs.wav", 
    "sfx\\ambience\\scene\\lava.wav", 
    "sfx\\ambience\\scene\\cathedral.wav", 
    "sfx\\ambience\\scene\\creepywind.wav", 
    "sfx\\ambience\\scene\\sewer.wav",
};
char* ice_age[DT_COUNT] = {
    "music\\th2_tris_v2_04.wav",
    "music\\dungeondeep.wav",
    "music\\ghostly.wav",
    "music\\caverns_of_ice.wav",
    "Music\\ambience_generic.wav",
    "Music\\002_Abyss2.wav",
    "Music\\ambience_nex7.wav",
};
char* ice_age2[DT_COUNT] = {
    "music\\th2_tris_v4_00_ext.wav", //"music\\th2_tris_v2_03.wav",
    "music\\dungeondeep.wav",
    "music\\ghostly.wav",
    "music\\caverns_of_ice.wav",
    "Music\\ambience_gru.wav",
    "Music\\002_Abyss2.wav",
    "Music\\ambience_nex7.wav",
};
char* ice_age3[DT_COUNT] = {
    "music\\th2_tris_v3_00.wav",
    "music\\dungeondeep.wav",
    "music\\ghostly.wav",
    "music\\caverns_of_ice.wav",
    "Music\\planar.wav",
    "Music\\002_Abyss2.wav",
    "Music\\ambience_nex7.wav",
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
void __fastcall PlayMusic(char* musicName /* = 0 */ )
{
    if( ! DirectSoundPtr || ! IsMusicEnabled ) return;
	bool iceage = Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE);
	if( ! musicName ){
		NonGameplaySeed = grand();
		Players[CurrentPlayerIndex].seed2 = NonGameplaySeed;
		// TOWN = 0, CHURCH = 1, CATACOMB = 2, CAVE = 3, HELL = 4, CRYPT = 5, ABYSS = 6
        int dungeonType = Dungeon->graphType;
        if (GameMode == GM_CLASSIC) {
            goto ORMUZ;
        }
        if (Doom_Music) {
            if (Dungeon->isQuest) {
                switch ((DUNGEON)Dungeon) {
                case DUN_52_MOSSY_GROTTO:
                case DUN_109_HALL_OF_HEROES:
                case DUN_26_BONE_CHAMBER:
                case DUN_53_VAULTKEEP:                      musicName = (char*)RandFrom("Music\\doom\\quest_BarrelsOFun.wav", "Music\\doom\\quest_BarrelsOFun2.wav");                     break;
                case DUN_34_THE_PIT:
                case DUN_51_FETID_CAVERN:                   musicName = "Music\\doom\\quest_Waste.wav";                                                         break;
                case DUN_50_TEMPLE_OF_SUNSET:
                case DUN_49_SICK_VILLAGERS_BASEMENT:
                case DUN_28_POISONED_WATER_SUPPLY:          musicName = "Music\\doom\\quest_DonnatotheRescue.wav";                                              break;
                case DUN_29_ARCHBISHOP_LAZARUS_LAIR:        
                case DUN_44_RAVENHOLM:
                case DUN_25_KING_LEORICS_TOMB:
                case DUN_37_DISUSED_RELIQUARY:
                case DUN_46_COLISEUM:                       musicName = (char*)RandFrom("Music\\doom\\quest_SignofEvil.wav", "Music\\doom\\quest_SignDARv2.wav");  break;
                case DUN_43_HEPHASTO:
                case DUN_54_ABANDONED_FANE:                 musicName = "Music\\doom\\quest_AtDoomsGate.wav";                                                   break;
                case DUN_33_SUMMONING_ROOMS:
                case DUN_41_FLESHDOOM:
                case DUN_42_WIELDER:                        musicName = (char*)RandFrom("Music\\doom\\quest_DemonsonthePrey.wav", "Music\\w4r2pv2.wav");        break;
                case DUN_35_PASSAGE_OF_FIRE:
                case DUN_45_THE_DEN:                        musicName = "Music\\POFv2.wav";                                                                     break;
                case DUN_111_CHURCH_SACRISTY:
                case DUN_30_HALLS_OF_ANGUISH: 
                case DUN_31_ANDARIELS_THRONE:               musicName = "Music\\doom\\quest_Sini.wav";                                                          break;
                case DUN_55_DEEP_HOLLOW:
                case DUN_59_HALL_OF_FIRE:
                case DUN_58_GATEWAY_OF_BLOOD:
                case DUN_32_HORAZONS_HAVEN:
                case DUN_47_DARK_NEST:                      musicName = (char*)RandFrom("Music\\doom\\quest_Crusher.wav", "Music\\doom\\quest_Crusher_2.wav");  break;
                case DUN_48_BLOOD_HIVE:                     musicName = (char*)RandFrom("Music\\doom\\quest_DS2.wav", "Music\\doom\\quest_DeadSimple.wav");     break;
                case DUN_27_THE_INFESTED_CELLAR:
                case DUN_56_WEB_TUNNELS:
                case DUN_57_ARACHNID_LAIR:
                case DUN_36_VALLEY_OF_DESPAIR:              musicName = "Music\\doom\\quest_SignAH.wav";                                                        break;
                case DUN_110_BLACK_CHAPEL:
                case DUN_40_ISLAND:                         musicName = "Music\\doom\\quest_Id.wav";                                                            break; 
                case DUN_38_GLACIAL_CAVERNS:
                case DUN_39_FROZEN_LOCH:
                default:                                    musicName = (char*)RandFrom("Music\\doom\\quest_SignofEvil.wav", "Music\\doom\\quest_DeepIntoTheCode_v2_75vol.wav"); break;
                }
                if (Dungeon >= DUN_60_HAUNTED_LABYRINTH && Dungeon <= DUN_108_ARCANE_PRISON) {
                    musicName = (char*)RandFrom("Music\\doom\\quest_SignofEvil.wav", "Music\\doom\\quest_DeepIntoTheCode_v2_75vol.wav", "Music\\doom\\quest_Id.wav", "Music\\doom\\quest_SignAH.wav",
                        "Music\\doom\\quest_DS2.wav", "Music\\doom\\quest_DeadSimple.wav", "Music\\doom\\quest_Crusher.wav", "Music\\doom\\quest_Crusher_2.wav",
                        "Music\\doom\\quest_Sini.wav", "Music\\POFv2.wav", "Music\\doom\\quest_DemonsonthePrey.wav", "Music\\w4r2pv2.wav", "Music\\doom\\quest_AtDoomsGate.wav",
                        "Music\\doom\\quest_SignofEvil.wav", "Music\\doom\\quest_SignDARv2.wav", "Music\\doom\\quest_DonnatotheRescue.wav", "Music\\doom\\quest_Waste.wav",
                        "Music\\doom\\quest_BarrelsOFun.wav", "Music\\doom\\quest_BarrelsOFun2.wav");
                }
            }
            else {
                switch (dungeonType) {
                case DT_0_TOWN:         musicName = "Music\\doom\\0.wav";                                                                                                           break; 
                case DT_1_CHURCH:       musicName = (char*)RandFrom("Music\\doom\\01-1.wav", "Music\\doom\\01-2.wav", "Music\\doom\\01-3.wav");                                     break; 
                case DT_2_CATACOMB:     musicName = (char*)RandFrom("Music\\doom\\02-1.wav", "Music\\doom\\02-2.wav", "Music\\doom\\02-3.wav", "Music\\doom\\quest_Traps_v2.wav");  break;
                case DT_3_CAVE:         musicName = (char*)RandFrom("Music\\doom\\03-1.wav", "Music\\doom\\03-2.wav", "Music\\doom\\03-3.wav", "Music\\doom\\03-4v2.wav");          break;
                case DT_4_HELL:         musicName = (char*)RandFrom("Music\\doom\\04-1.wav", "Music\\doom\\04-2.wav");                                                              break; 
                case DT_5_CRYPT:        musicName = (char*)RandFrom("Music\\doom\\06-1.wav", "Music\\doom\\06-2.wav");                                                              break; 
                case DT_6_ABYSS:        musicName = (char*)RandFrom("Music\\doom\\05-1.wav", "Music\\doom\\05-2.wav", "Music\\doom\\05-3.wav");                                     break;
                }
            }
        }
        else if( OriginalMusic ){
            ORMUZ:
			musicName = origi[dungeonType];
        }else if( Dungeon->isQuest ){ // default music selections start from here >>>
            switch( (DUNGEON)Dungeon ){
            case DUN_27_THE_INFESTED_CELLAR    : musicName = "music\\Abs02.wav";            break;
            case DUN_30_HALLS_OF_ANGUISH       : musicName = "music\\Dintr1.wav";           break;
            case DUN_31_ANDARIELS_THRONE       : musicName = "music\\Dintro.wav";           break;
            case DUN_36_VALLEY_OF_DESPAIR      : musicName = "music\\valley.wav";           break;
            case DUN_45_THE_DEN                : musicName = "music\\POFv2.wav";            break;
            case DUN_59_HALL_OF_FIRE:
            case DUN_58_GATEWAY_OF_BLOOD       : musicName = "music\\dtheda.wav";           break;
            case DUN_73_LAVA_PITS:
            case DUN_38_GLACIAL_CAVERNS:
            case DUN_39_FROZEN_LOCH            : musicName = "music\\Caverns_of_Ice.wav";   break;
            case DUN_37_DISUSED_RELIQUARY      : musicName = "music\\DlvlO.wav";            break;
            case DUN_49_SICK_VILLAGERS_BASEMENT:
            case DUN_51_FETID_CAVERN           : musicName = "music\\sgsg.wav";             break;
            case DUN_28_POISONED_WATER_SUPPLY  : musicName = "music\\DungeonDeep.wav";      break;
            case DUN_113_THE_MAZE:
            case DUN_44_RAVENHOLM              : musicName = "music\\Dintr4.wav";           break;
            case DUN_25_KING_LEORICS_TOMB      : musicName = "music\\king1.wav";            break;
            case DUN_40_ISLAND                 : musicName = "music\\ambience_generic.wav"; break;
            case DUN_41_FLESHDOOM              :
            case DUN_42_WIELDER                :
            case DUN_43_HEPHASTO               : musicName = "music\\w4r2pv2.wav";          break;
            case DUN_35_PASSAGE_OF_FIRE        : musicName = "music\\const_time1.wav";      break;
            case DUN_29_ARCHBISHOP_LAZARUS_LAIR: musicName = "music\\ALv4.wav";             break;
            case DUN_110_BLACK_CHAPEL:
            case DUN_53_VAULTKEEP:
            case DUN_32_HORAZONS_HAVEN         : musicName = "music\\planar.wav";           break;
            case DUN_111_CHURCH_SACRISTY       : musicName = "music\\dlvlN.wav";            break;
            case DUN_33_SUMMONING_ROOMS        : musicName = "music\\BloodCrypt_v2.wav";    break;
            case DUN_34_THE_PIT                : musicName = "music\\ambience_nex7.wav";    break;
            case DUN_109_HALL_OF_HEROES:
            case DUN_26_BONE_CHAMBER           : musicName = "music\\Ghostly.wav";          break;
            case DUN_46_COLISEUM               : musicName = "music\\DlvlQ.wav";            break;
            case DUN_47_DARK_NEST:
            case DUN_55_DEEP_HOLLOW:
            case DUN_48_BLOOD_HIVE             : musicName = "music\\hive.wav";             break;
            case DUN_50_TEMPLE_OF_SUNSET       : musicName = "music\\th2_cata_doom.wav";    break;
            case DUN_56_WEB_TUNNELS:
            case DUN_57_ARACHNID_LAIR          : musicName = "music\\spiders.wav";          break;
            case DUN_52_MOSSY_GROTTO           : musicName = "music\\Mossy_Grotto.wav";     break;
            case DUN_54_ABANDONED_FANE         : musicName = "music\\hglmuseum.wav";        break;
            case DUN_112_STILL_SPRINGS         : musicName = "music\\Springs7.wav";         break;
            case DUN_114_LOWER_SCRIPTORIUM     : musicName = "music\\DlvlO.wav";            break; // adjust later
			}
            if ((Dungeon >= DUN_60_HAUNTED_LABYRINTH && Dungeon <= DUN_108_ARCANE_PRISON) && Dungeon != DUN_73_LAVA_PITS) {
                goto NORMAL_MUSIC;
            }
		}
        else if( iceage ){
            musicName = RandFrom(ice_age, ice_age2, ice_age3)[dungeonType];
		}
        else{
            NORMAL_MUSIC:
            switch (dungeonType) {
            case DT_0_TOWN:         musicName = (char*)RandFrom("Music\\RazTE.wav", "Music\\DTown3.wav", "Music\\DTown4.wav", "Music\\DTown5.wav", "Music\\th3_twn_var_5.wav"); break;
            case DT_1_CHURCH:       musicName = (char*)RandFrom("Music\\RazA.wav", "Music\\RazA.wav", "Music\\DLvlH.wav", "Music\\DlvlO.wav", "Music\\th3_chu_var_5.wav", "Music\\DLvlJ.wav");     break;
            case DT_2_CATACOMB:     musicName = (char*)RandFrom("Music\\RazB.wav", "Music\\RazB.wav", "Music\\DLvlK.wav", "Music\\DlvlP.wav", "Music\\th3_cat_var_4.wav", "Music\\001_Tomb2.wav"); break;
            case DT_3_CAVE:         musicName = (char*)RandFrom("Music\\RazC.wav", "Music\\DlvlC.wav", "Music\\th3_hel_var_4.wav", "Music\\DLvlL.wav", "Music\\Alex_Trz_Caverns_v5.wav"); break;
            case DT_4_HELL:         musicName = (char*)RandFrom("Music\\RazD.wav", "Music\\DlvlS.wav", "Music\\DlvlD.wav", "Music\\th3_cav_var_4.wav", "Music\\Dtown8.wav");    break;
            case DT_5_CRYPT:        musicName = (char*)RandFrom("Music\\DlvlE.wav", "Music\\DLvlT.wav", "Music\\RazE.wav", "Music\\002_Abyss2.wav");                            break;
            case DT_6_ABYSS:        musicName = (char*)RandFrom("Music\\DlvlG.wav", "Music\\DLvlR.wav", "Music\\RazF.wav", "Music\\DLvlF.wav");                                 break;
            }
		}
	}
	//musicName = "Music\\DLvlL.wav"; // для проверки замены трека в THmusic.mor
	if( musicName && MusicName && strcmp(musicName, MusicName) || ! musicName || ! MusicName ){
		StopMusic();
		if( musicName ){
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

//----- (0046BE3A) --------------------------------------------------------
void __fastcall PlayAmbient(char* ambientName /* = 0 */ )
{
	if( ! DirectSoundPtr || ! IsAmbientEnabled ) return;
	bool iceAge = Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE);
    if (!ambientName) {
        // TOWN = 0, CHURCH = 1, CATACOMB = 2, CAVE = 3, HELL = 4, CRYPT = 5, ABYSS = 6
        int dungeonType = Dungeon->graphType;
        if (is(Dungeon, DUN_38_GLACIAL_CAVERNS, DUN_39_FROZEN_LOCH)) {
            ambientName = ambient_ice_age[dungeonType];
        }
        else if (is(Dungeon, DUN_28_POISONED_WATER_SUPPLY, DUN_51_FETID_CAVERN, DUN_52_MOSSY_GROTTO, DUN_49_SICK_VILLAGERS_BASEMENT, DUN_27_THE_INFESTED_CELLAR, DUN_112_STILL_SPRINGS)) {
            ambientName = "sfx\\ambience\\scene\\cave.wav";
        }
        else if (Dungeon == DUN_35_PASSAGE_OF_FIRE) {
            ambientName = "sfx\\ambience\\scene\\lava.wav";
        }
        else if (Dungeon == DUN_40_ISLAND) {
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
    }
         
	if( ambientName && AmbientName && strcmp(ambientName, AmbientName) || ! ambientName || ! AmbientName ){
		StopAmbient();
		if( ambientName ){
			int opened = SFileOpenFile( ambientName, &AmbientFile );
			SetDirectSoundBufferFormat( AmbientFile );
			if( opened ){
				SFileDdaBeginEx( AmbientFile, DDA_BUF_SIZE, SFILE_DDA_LOOP, 0, AmbientVolume, 0, 0 );
				AmbientName = ambientName;
			}else{
				AmbientFile = 0;
			}
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
        if( MusicFile ){
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
		if( AmbientFile ){
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
