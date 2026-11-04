#include "stdafx.h"

char CryptKey[9] = "xrgyrkj1";	// (rt) static key for save encryption, moved from dataseg.asm
								// (rt) used only for single-player saves; multiplayer uses the computer name
CharName CharNameSlots[MaxSaveSlots];                     // (rt) 10 for the slots and 1 for creating a new player
int gbValidSaveFile;
int gbSaveFileExists;

const int SaveSignature         = 'HEL4'; // HELF - th1, HEL2 - th2, HEL3 - th3, HEL4 - th4
const int SaveSignatureReverse  = '4LEH';

char* SaveFolders[6] = { "th4_save_sp\\", "th4_save_mp\\", "th4_save_col\\", "th4_save_cls\\", "th4_save_ob\\", "th4_save_drk\\" }; // (th4) [5] = Dark Campaign
char SaveFile_Hero[]       = "hero";
char SaveFile_Stash[]      = "stash";
char SaveFile_DunTemp[]    = "templ%03d";
char SaveFile_DunPerm[]    = "perml%03d";
char SaveFile_Game[]       = "game";
char SaveFile_Swap[]       = "wpnswp";
char SaveFile_Spellhot[]   = "spellhotkeys";
char SaveFile_Additional[] = "additionalherodata";
char SaveFile_Transmute[]  = "trans";
char SaveFile_HashTable[]  = "(hash table)";
char SaveFile_BlockTable[] = "(block table)";

HashEntry  HashTbl [HASH_ENTRIES];
BlockEntry BlockTbl[BLOCK_ENTRIES];
uchar SaveBuffer_Transmute     [CalcEncodeDstBytes(sizeof(TransmuteInfo))];
uchar SaveBuffer_Stash         [CalcEncodeDstBytes(sizeof(StashInfo))];
uchar SaveBuffer_PlayerInfo    [CalcEncodeDstBytes(sizeof(LastPlayerInfo))];
uchar SaveBuffer_SpellHotKeys  [CalcEncodeDstBytes(sizeof(SpellbookHotkeysInfo))];
uchar SaveBuffer_AdditionalInfo[CalcEncodeDstBytes(sizeof(AdditionalSaveInfo))];
uchar SaveBuffer_WeaponSwap    [CalcEncodeDstBytes(sizeof(WeaponSwapItems))];
uchar SaveBuffer_Dungeon       [CalcEncodeDstBytes(sizeof(DungeonInfo))];

//----- (th4) -------------------------------------------------------------
char* SaveFolderName()
{
	char* folder = SaveFolders[MaxCountOfPlayersInGame > 1];
	if( ColiseumSaveFolder && GameMode == GM_COLISEUM ) folder = SaveFolders[2];
	if( ClassicSaveFolder  && GameMode == GM_CLASSIC  ) folder = SaveFolders[3];
	if (OutbreakSaveFolder && GameMode == GM_OUTBREAK ) folder = SaveFolders[4]; 
	if( DarkCampaignSaveFolder && GameMode == GM_DARK_CAMPAIGN ) folder = SaveFolders[5]; // (th4)
	return folder;
}

//----- (th4) -------------------------------------------------------------
char* SaveArchiveName() // TODO: change to scan and parse by file extentions (not name mask) in new savefile engine (naming/parsing/safe saving)
{
	char* name = MaxCountOfPlayersInGame == 1 ? "%s%sth4sp_%d.ssv" : "%s%sth4mp_%d.msv";
	if( GameMode == GM_COLISEUM ) name = "%s%sth4col_%d.csv";
	if( GameMode == GM_CLASSIC  ) name = "%s%sth4cls_%d.ksv";
	if (GameMode == GM_OUTBREAK ) name = "%s%sth4ob_%d.osv";
	if( GameMode == GM_DARK_CAMPAIGN ) name = "%s%sth4drk_%d.dsv"; // (th4)
	return name;
}

//----- (th4) -------------------------------------------------------------
char* SaveFileHero()
{
	char* hero = SaveFile_Hero; // TODO: convert to unifed block name in new savefile engine (naming/parsing/safe saving)
    hero = MaxCountOfPlayersInGame == 1 ? "sphero" : "mphero";
	if( GameMode == GM_COLISEUM ) hero = "clhero";
	if( GameMode == GM_CLASSIC  ) hero = "cphero";
	if (GameMode == GM_OUTBREAK ) hero = "obhero";
	if( GameMode == GM_DARK_CAMPAIGN ) hero = "drkhero"; // (th4)
	return hero;
}

//----- (th4) -------------------------------------------------------------
char* SaveFileStash()
{
	char* stash = SaveFile_Stash; // TODO: convert to unifed block name in new savefile engine (naming/parsing/safe saving)
	stash = MaxCountOfPlayersInGame == 1 ? "spstash" : "mpstash";
	if( GameMode == GM_COLISEUM ) stash = "clstash";
	if( GameMode == GM_CLASSIC  ) stash = "cpstash";
	if (GameMode == GM_OUTBREAK ) stash = "obstash";
	if( GameMode == GM_DARK_CAMPAIGN ) stash = "drkstash"; // (th4)
	return stash;
}

//----- (00456284) --------------------------------------------------------
void __fastcall GetSaveArchivePath( char (&saveArchivePath)[MAX_PATH], int stringSize, int slotNum, int isHellfire )
{
	char buffer[MAX_PATH];
	if( !GetModuleFileNameA(HInstance, saveArchivePath, MAX_PATH) ){
		TerminateWithError("Unable to get save directory");
	}
	char* pathEnd = strrchr(saveArchivePath, '\\'); // (rt) search from the right
	if( pathEnd ){
		*pathEnd = 0;// (rt) cut off "\\" and the exe name
	}
	sprintf(buffer, SaveArchiveName(), "\\", SaveFolder ? SaveFolderName() : "", LoadOnStart && AutoLoadSlot != slotNum ? 'X' : slotNum);
	strcat(saveArchivePath, buffer);
	_strlwr(saveArchivePath);// (rt) non-original CharLower(saveFullName);
}

//----- (00456034) --------------------------------------------------------
void InitSaveDirectory()
{
	char buffer[MAX_PATH];

	// (rt) the game requires 10 MB on the system drive
	if( !GetWindowsDirectoryA(buffer, MAX_PATH) ){
		TerminateWithError("Unable to initialize save directory");
	}
	CheckDiskFreeSpace(buffer);

	// (rt) and 10 MB on the game drive
	if( !GetModuleFileNameA(HInstance, buffer, MAX_PATH) ){
		TerminateWithError("Unable to initialize save directory");
	}
	CheckDiskFreeSpace(buffer);
}

//----- (00456093) --------------------------------------------------------
void __fastcall CheckDiskFreeSpace( char* driveString )
{
	// (rt) keep only the drive letter from the path, in the form "C:\"
	for( int curCharIndex = 0; driveString[curCharIndex]; curCharIndex++ ){
		if( driveString[curCharIndex] == '\\' ){
			driveString[curCharIndex + 1] = '\0';
			break;
		}
	}
	// (rt) check the disk space. If less than 10 MB, exit with an error
	DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;
	if( !GetDiskFreeSpaceA(driveString, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters) ){
		DiskFreeDlg((LPARAM)driveString);
	}
	if( (signed __int64)(bytesPerSector * (unsigned __int64)numberOfFreeClusters * sectorsPerCluster) < 10i64 * 1024 * 1024 ){
		DiskFreeDlg((LPARAM)driveString);
	}
}

//----- (th2) --------------------------------------------------------
void BackupSaveIfNeed( int slotIndex )
{
	if( AutoBackup ){ // (rt) backup depth could be added here
		char saveArchivePath[MAX_PATH];
		GetSaveArchivePath(saveArchivePath, MAX_PATH, slotIndex, true);
		string path = saveArchivePath;
		if( exists( path ) ){
			copy_options op = copy_options::overwrite_existing;
			copy_file( path, path + ".bak", op );
		}
	}
}

//----- (0045610A) --------------------------------------------------------
void UpdatePlayerFile()
{
	if( EnforceNoSave ){ return; }
	int slotIndex = GetSlotByHeroName(Players[CurrentPlayerIndex].playerName);
	BackupSaveIfNeed(slotIndex);
	LastPlayerInfo heroData;
	if( OpenArchiveWrite(true, slotIndex) ){
		SavePlayerInfo(&heroData, CurrentPlayerIndex);
		SaveCharacter(&heroData);
		WriteWeaponSwapInfoToSave( WeaponSwapItems );
		WriteStashInfoToSave();
		WriteHotkeysDataToSave();
		WriteAdditionalHeroDataToSave();
		Transmute_WriteToSave();
		CloseArchiveWrite(MaxCountOfPlayersInGame == 1, slotIndex);
	}
}

//----- (00456173) --------------------------------------------------------
int __fastcall GetSlotByHeroName(char* heroName)
{
	int slotNumber;
	for( slotNumber = 0; slotNumber < MaxSaveSlots - 1; slotNumber++ ){
		if( !_strcmpi(CharNameSlots[slotNumber], heroName) ){
			break;
		}
	}
	return slotNumber;
}

//----- (0045619E) --------------------------------------------------------
void __fastcall SaveCharacter( LastPlayerInfo* heroData )
{
	int blockSize = sizeof(SaveBuffer_PlayerInfo);
	uchar* heroBuffer = SaveBuffer_PlayerInfo;
	memcpy(heroBuffer, heroData, sizeof(LastPlayerInfo));
	EncodeFile(heroBuffer, sizeof(LastPlayerInfo), blockSize, CryptKey);
	MpqAddFile(SaveFileHero(), heroBuffer, blockSize);
}

//----- (th2) -------------------------------------------------------------
void __fastcall WriteHotkeysDataToSave( void )
{
	int blockSize = sizeof(SaveBuffer_SpellHotKeys);
	uchar* hotkeysBuffer = SaveBuffer_SpellHotKeys;
	memcpy(hotkeysBuffer, &SpellHotkeys, sizeof(SpellbookHotkeysInfo));
	EncodeFile(hotkeysBuffer, sizeof(SpellbookHotkeysInfo), blockSize, CryptKey);
	MpqAddFile(SaveFile_Spellhot, hotkeysBuffer, blockSize);
}

//----- (th2) -------------------------------------------------------------
bool __fastcall ReadHotkeysInfoFromSave(HANDLE save)
{
	for(int i = 0; i < 64; ++i) { SpellHotkeys.hotkey[i] = 0; SpellHotkeys.type[i] = 0; }

	HANDLE hotkeysHandle;
	if (!SFileOpenFileEx(save, SaveFile_Spellhot, 0, &hotkeysHandle)) {
		return false;
	}

	int isSuccess = false;
	uchar* hotkeysData = 0;
	DWORD fileSize = SFileGetFileSize(hotkeysHandle, 0);
	if (fileSize) {
		hotkeysData = (uchar*)AllocMem(fileSize);
		uint bytesReaded;
		if (SFileReadFile(hotkeysHandle, hotkeysData, fileSize, (DWORD*)&bytesReaded, 0)) {
			int readSize = DecodeFile(hotkeysData, fileSize, CryptKey);
			if (readSize == sizeof(SpellbookHotkeysInfo)) {
				memcpy(&SpellHotkeys, hotkeysData, readSize);
			}
		}
		FreeMem(hotkeysData);
	}
	SFileCloseFile(hotkeysHandle);
	return isSuccess;
}

//----- (th2) -------------------------------------------------------------
void __fastcall WriteAdditionalHeroDataToSave(void)
{
	memcpy(&AdditionalStateData.lastsave_version, &THE_HELL_VERSION_STRING, 8);
	int blockSize = sizeof(SaveBuffer_AdditionalInfo);
	uchar* heroDataBuffer = SaveBuffer_AdditionalInfo;
	memcpy(heroDataBuffer, &AdditionalStateData, sizeof(AdditionalSaveInfo));
	EncodeFile(heroDataBuffer, sizeof(AdditionalSaveInfo), blockSize, CryptKey);
	MpqAddFile(SaveFile_Additional, heroDataBuffer, blockSize);
}

//----- (th2) -------------------------------------------------------------
bool __fastcall ReadAdditionalHeroDataFromSave(HANDLE save)
{
	memset(&AdditionalStateData, 0, sizeof(AdditionalSaveInfo));

	HANDLE heroDataHandle;
	if( ! SFileOpenFileEx(save, SaveFile_Additional, 0, &heroDataHandle)) {
		AdditionalStateData.StashTabsPurchased = 100;
		AdditionalStateData.StashCurrentTab = 0;
		strcpy(AdditionalStateData.creation_version, "unknow");
		return false;
	}

	int isSuccess = false;
	uchar* heroData = 0;
	DWORD fileSize = SFileGetFileSize(heroDataHandle, 0);
	if (fileSize) {
		heroData = (uchar*)AllocMem(fileSize);
		uint bytesReaded;
		if (SFileReadFile(heroDataHandle, heroData, fileSize, (DWORD*)&bytesReaded, 0)) {
			int readSize = DecodeFile(heroData, fileSize, CryptKey);
			if (readSize == sizeof(AdditionalSaveInfo)) {
				memcpy(&AdditionalStateData, heroData, readSize);
			}
		}
		FreeMem(heroData);
	}
	SFileCloseFile(heroDataHandle);
	return isSuccess;
}

int OpenFileError;
//----- (00456227) --------------------------------------------------------
bool __fastcall OpenArchiveWrite( bool isAutoSave, int heroSlot )
{
	char buffer[MAX_PATH];
	GetSaveArchivePath(buffer, MAX_PATH, heroSlot, true);
	return MpqOpenArchive(buffer, false, heroSlot);
}

//----- (0045632F) --------------------------------------------------------
void __fastcall CloseArchiveWrite( bool clearHashTables, int heroSlot )
{
	char buffer[MAX_PATH];
	GetSaveArchivePath(buffer, MAX_PATH, heroSlot, true);
	MpqCloseArchive(buffer, clearHashTables, heroSlot);
}

//----- (00456363) --------------------------------------------------------
void ReleasePlayerFile()
{
	int slot = GetSlotByHeroName(Players[CurrentPlayerIndex].playerName);
	CloseArchiveWrite(true, slot);
}

//----- (0045638A) --------------------------------------------------------
void __fastcall InitCharData( Player& player, CharSaveInfo& charInfo, int isHellfire )
{
	memset(&charInfo, 0, sizeof( CharSaveInfo ));
	strncpy(charInfo.Name, player.playerName, 15);
	charInfo.Name[15] = 0;
	charInfo.Level = player.CharLevel;
	charInfo.Class = player.ClassID;
    charInfo.SubClass = player.subclassID;
    charInfo.Specialization = player.specializationID;
    charInfo.GameMode = player.gameMode;
	charInfo.GameChanger = player.gameChanger;
	charInfo.Stats.Strength = player.CurStrength;
	charInfo.Stats.Magic = player.CurMagic;
	charInfo.Stats.Dexterity = player.CurDexterity;
	charInfo.Stats.Vitality = player.CurVitality;
	charInfo.allowedDifficulty = player.allowedDifficulty;
	charInfo.CurrentSeedRun = player.CurrentSeedRun;
	charInfo.Traits = player.traits;
}

//----- (th3) -------------------------------------------------------------
template <typename Pi> bool NeedMurder(Pi* pi, CharName& name) 
{
	strcpy(name, pi->CharName);
	IS( Pi, PlayerInfo_TH1 ){
		return false;
	}else{
		return ! pi->UberDialogKilled && ( (InstantMurderHero && _strcmpi(MurderHeroName, name) == 0)
			//|| is(pi->gameMode, GM_NIGHTMARE, GM_SPEEDRUN) 
			);
	}
}

//----- (th3) -------------------------------------------------------------
template<> bool NeedMurder<uchar>(uchar* playerInfo, CharName& name) 
{
	auto pi = (PlayerInfo*)playerInfo;
	if(pi->version >= 26) return NeedMurder((PlayerInfo_v26*)pi, name); else
	if(pi->version >= 22) return NeedMurder((PlayerInfo_v22*)pi, name); else
	if(pi->version >= 21) return NeedMurder((PlayerInfo_v21*)pi, name);	else
	if(pi->version >= 18) return NeedMurder((PlayerInfo_v18*)pi, name);	else
	if(pi->version >= 17) return NeedMurder((PlayerInfo_v17*)pi, name);	else
	if(pi->version >=  1) return NeedMurder((OldPlayerInfo5*)pi, name);	else
	                      return NeedMurder((PlayerInfo_TH1*)pi, name);
}

//----- (00456440) --------------------------------------------------------
int __stdcall LoadCharSlots(void( __stdcall *allocFunc )( CharSaveInfo* ))
{
	// (th4) LoadPlayerInfo (SaveInfo.cpp) copies each listed hero's UberDialogKilled into the
	// GLOBAL UberDiabloKilled. Listing heroes must not change game state: without this restore
	// the global ended up holding the LAST listed hero's value, and a hero created next
	// (CreateCharSlot -> SavePlayerInfo, SaveInfo.cpp pi.UberDialogKilled = UberDiabloKilled)
	// was saved as "Uber killed" -- which disables Murder Hero deletion (menu.cpp
	// MurderHeroInGame, NeedMurder below) and unlocks Save & Quit / saving in Ironman,
	// Nightmare and Speedrun (menu.cpp). Every return below goes through the restore.
	const bool uberKilledBeforeScan = UberDiabloKilled;
	CharSaveInfo charInfo = {};
	memset(CharNameSlots, 0, sizeof( CharNameSlots ));
	int v10 = 1;
	for( int slotIndex = 0; slotIndex < MaxSaveSlots - 1; slotIndex++ ){
		if( HANDLE archiveHandle = OpenArchive(&v10, slotIndex) ){
			if( uchar* pi = ReadPlayerInfo(archiveHandle) ){
				CharName& heroName = CharNameSlots[slotIndex];
				// (th4) a murder deletes only the EXACT save it was armed for (menu.cpp
				// MurderHeroInGame): same slot, same single/multiplayer list, and the name check
				// in NeedMurder on top. A hero elsewhere with the same name is never touched.
				const bool murderTargetsThisSave = InstantMurderHero && slotIndex == MurderHeroSlot
				                                && ( MaxCountOfPlayersInGame != 1 ) == MurderHeroMulti;
				if( NeedMurder(pi, heroName) && murderTargetsThisSave ){
					FreeMemZero(pi);
					CloseArchiveRead(archiveHandle);
					strcpy(charInfo.Name, heroName);
					DeleteCharSlot(&charInfo);
					InstantMurderHero = false;
					continue;
				}
				if( GameModeFilter != -1 ){
					char gameMode;
					uint version = ((PlayerInfo*)pi)->version;
					if(version >= 26) gameMode = ((PlayerInfo_v26*)pi)->gameMode; else
					if(version >= 22) gameMode = ((PlayerInfo_v22*)pi)->gameMode; else
					if(version >= 21) gameMode = ((PlayerInfo_v21*)pi)->gameMode; else
					if(version >= 18) gameMode = ((PlayerInfo_v18*)pi)->gameMode; else
					if(version >= 17) gameMode = ((PlayerInfo_v17*)pi)->gameMode; else
					if(version >=  1) gameMode = ((OldPlayerInfo5*)pi)->gameMode; else
					                  gameMode = 0;						
					if( !(B(gameMode) & GameModeFilter) ){
						FreeMemZero(pi);
						CloseArchiveRead(archiveHandle);
						continue;
					}
				}
				if( LoadPlayerInfo(pi, 0, false) ){
					InitCharData(Players[0], charInfo, CheckSavefileSignature(archiveHandle));
					if( OriginalMenu && ! LoadOnStart && allocFunc ){
						allocFunc(&charInfo);
					}
				}
				FreeMemZero(pi);

			}
			CloseArchiveRead(archiveHandle);
		}
	}
	// Disarm after the full scan: if the armed murder matched no slot (e.g. the
	// named save was manually moved/renamed), don't let it stay armed into a
	// later, unrelated rescan.
	InstantMurderHero = false; 
	UberDiabloKilled = uberKilledBeforeScan;	// (th4) see the note at the top of this function
	return true;
}

//----- (004564E4) --------------------------------------------------------
uchar* __fastcall ReadPlayerInfo( HANDLE saveHandle )
{
	HANDLE heroHandle = 0;
	if( !SFileOpenFileEx(saveHandle, SaveFileHero(), 0, &heroHandle) ){
		return false;
	}
	uchar key[KEY_SIZE];
	memset(key, 0, KEY_SIZE);
	memcpy(key, CryptKey, sizeof( CryptKey ));
	DWORD nSize = KEY_SIZE;

	DWORD fileSize = SFileGetFileSize(heroHandle, 0);
	uchar* heroData = 0;
	if( fileSize ){
		heroData = (uchar*)AllocMem(fileSize);
		uint bytesReaded = 0;
		if( SFileReadFile(heroHandle, heroData, fileSize, (DWORD*)&bytesReaded, 0) ){
			int decodeSize = DecodeFile( heroData, fileSize, (char*) key );
			static const size_t AllowedPlayerInfoSizesToLoad[] = 
			{ 
				11798,                           // old size for v26 (before powerups)
				11798 + POWERUP_COUNT,           // (th4) v27 size (before totalKills)
				sizeof(PlayerInfo_v26), 
				sizeof(PlayerInfo_v22), 
				sizeof(PlayerInfo_v21), 
				sizeof(PlayerInfo_v18) 
			};
			if( ! has(AllowedPlayerInfoSizesToLoad, decodeSize) ) FreeMemZero(heroData);
			// (rt) th1 save conversion
			#ifdef ALLOW_TH1_CONVERTATION
			else if( File_Seek( heroHandle, 0, 0, 0 ),
				SFileReadFile( heroHandle, heroData, fileSize, (DWORD*) &bytesReaded, 0 );
				DecodeFile( heroData, fileSize, (char*) key ) == sizeof( PlayerInfo_TH1 ) ){
				memcpy( playerInfo, heroData, sizeof( PlayerInfo_TH1 ) ); // (rt) TH1 with the single-player key
				isSuccess = 1;
			}else if( MaxCountOfPlayersInGame != 1 ){// (rt) attempt to load old-format multiplayer saves (key == the computer's network name)
				GetComputerNameA( (LPSTR) key, &nSize );
				File_Seek( heroHandle, 0, 0, 0 );
				SFileReadFile( heroHandle, heroData, fileSize, (DWORD*) &bytesReaded, 0 );
				if( DecodeFile( heroData, fileSize, (char*) key ) == sizeof( PlayerInfo_TH1 ) ){
					memcpy( playerInfo, heroData, sizeof( PlayerInfo_TH1 ) ); // (rt) TH1 with the multiplayer key
					isSuccess = 1;
				}
			}
			#endif
		}
	}
	SFileCloseFile(heroHandle);
	return heroData;
}

//----- (004565B5) --------------------------------------------------------
HANDLE __fastcall OpenArchive( int* ecx_unused, int slotIndex )
{
	char saveFullName[MAX_PATH];
	GetSaveArchivePath(saveFullName, MAX_PATH, slotIndex, true);
	HANDLE handle = NULL;
	if( SFileOpenArchive(saveFullName, 0x7000u, 0, (MPQ**)&handle) != 0 ){
		return handle;
	}else{
		return NULL;
	}
}

//----- (004565F1) --------------------------------------------------------
void __fastcall CloseArchiveRead( HANDLE handle )
{
	SFileCloseArchive(handle);
}

//----- (004565F8) --------------------------------------------------------
int __fastcall CheckSavefileSignature(HANDLE archiveHandle)
{
	gbSaveFileExists = false;
	HANDLE fileHandle;
	if( MaxCountOfPlayersInGame != 1 || !SFileOpenFileEx(archiveHandle, SaveFile_Game, 0, &fileHandle) ){
		return false;
	}
	int fileSize = SFileGetFileSize(fileHandle, 0);
	if( !fileSize ){
		TerminateWithError("Invalid file size on check file");
	}
	void* data = AllocMem(fileSize + 8);
	uchar* data2 = &((uchar*)data)[4];
	bool result = false;
	uint bytesReaded;
	if( SFileReadFile(fileHandle, data2, fileSize, (DWORD*)&bytesReaded, 0) && bytesReaded == fileSize ){
		gbSaveFileExists = true;
		if( DecodeFile(data2, fileSize, CryptKey) )	{
			int saveTypeCode = *(uint*)data2;
			// (rt) disable th1 save conversion
			if( saveTypeCode == SaveSignatureReverse
				#ifdef ALLOW_TH1_CONVERTATION
			 || saveTypeCode == 'FLEH'
				#endif
				){
				result = true;
			}
		}
	}
	FreeMem(data);
	SFileCloseFile(fileHandle);
	return result;
}

//----- (00456704) --------------------------------------------------------
int __stdcall GetStartingStatsByClass( uint classId, ushort* stats )
{
	PlayerClass& playerClass = PlayerClasses[GetClassByClass(classId)];
	
	stats[0] = playerClass.StartStat.Strength;
	stats[1] = playerClass.StartStat.Magic;
	stats[2] = playerClass.StartStat.Dexterity;
	stats[3] = playerClass.StartStat.Vitality;

	return true;
}

//----- (00456748) --------------------------------------------------------
int __fastcall GetClassByClass( int charClass )
{
	return charClass;
}

//----- (00456772) --------------------------------------------------------
int __stdcall UiCreateHero( CharSaveInfo* newSaveInfo )
{
    LastPlayerInfo charInfo;// [sp+Ch] [bp-4F4h]@8
	int firstFreeSlot = GetSlotByHeroName(newSaveInfo->Name);
	if( firstFreeSlot >= MaxSaveSlots - 1 ){ // (rt) if the name isn't taken, find a free slot, otherwise overwrite
		for( firstFreeSlot = 0; firstFreeSlot < MaxSaveSlots; firstFreeSlot++ ){
			if( !CharNameSlots[firstFreeSlot][0] ){
				break;
			}
		}
		if( firstFreeSlot >= MaxSaveSlots - 1 ){ // (rt) no free slots
			return false;
		}
	}
	if( !OpenArchiveWrite(false, firstFreeSlot) ){
		return false;
	}
	MpqDeleteFiles(GetPlayerFileNames);
	strncpy(CharNameSlots[firstFreeSlot], newSaveInfo->Name, sizeof( CharName ));
	CharNameSlots[firstFreeSlot][sizeof( CharName ) - 1] = 0;
	Player& player = Players[0];

	CreatePlayer(0, newSaveInfo->Class, newSaveInfo->SubClass, newSaveInfo->Specialization, newSaveInfo->GameMode, newSaveInfo->GameChanger, newSaveInfo->Traits );
	strncpy(player.playerName, newSaveInfo->Name, sizeof( CharName ));
	player.playerName[sizeof( CharName ) - 1] = 0;
	// (th4) A NEW hero has never killed Uber Diablo. SavePlayerInfo stores the global
	// UberDiabloKilled into the save (SaveInfo.cpp), so clear it first -- whatever an earlier
	// game or hero-list scan left in it must not be inherited (see LoadCharSlots).
	UberDiabloKilled = false;
	SavePlayerInfo( &charInfo, 0);
	SaveCharacter(&charInfo);

	AdditionalStateData.StashTabsPurchased = 1;
	AdditionalStateData.StashCurrentTab = 0;
	memcpy(&AdditionalStateData.creation_version, &THE_HELL_VERSION_STRING, 8);
	WriteAdditionalHeroDataToSave();

	WriteHotkeysDataToSave();

	InitCharData(player, *newSaveInfo, false);
	CloseArchiveWrite(true, firstFreeSlot);
	return true;
}

//----- (0045685D) --------------------------------------------------------
int __stdcall GetPlayerFileNames(int fileIndex, char (&fileName)[MAX_PATH])
{
	if( MaxCountOfPlayersInGame != 1 ){
        const char* mp_keys[] = {
            SaveFileHero(),
			SaveFileStash(),
			SaveFile_Swap,
        };
        if( fileIndex < countof(mp_keys) ){
            sprintf(fileName, "%s", mp_keys[fileIndex]);
            return true;
        }
        return false;
    }else{
		if( fileIndex < DungeonCount ){
            sprintf(fileName, SaveFile_DunPerm, fileIndex);
        }else if( fileIndex == DungeonCount ){
            sprintf(fileName, SaveFile_Game);
        }else if( fileIndex == DungeonCount + 1 ){
            sprintf(fileName, SaveFileHero());
        }else if( fileIndex == DungeonCount + 2 ){
            sprintf(fileName, SaveFileStash());
        }else if( fileIndex == DungeonCount + 3 ){
            sprintf(fileName, SaveFile_Swap);
        }else{
            return false;
        }
        return true;
    }
}

//----- (004568B4) --------------------------------------------------------
int __stdcall DeleteCharSlot( CharSaveInfo* saveInfo )
{
	char archivePath[MAX_PATH];
	uint slotIndex = GetSlotByHeroName(saveInfo->Name);
	if( slotIndex < MaxSaveSlots - 1 ){
		CharNameSlots[slotIndex][0] = '\0';
		GetSaveArchivePath(archivePath, MAX_PATH, slotIndex, true);
		DeleteFileA(archivePath);
	}
	return true;
}

//----- (00456900) --------------------------------------------------------
void LoadCurrentPlayerInfo()
{
	uint slotIndex = GetSlotByHeroName(HeroName);
	HANDLE archiveHandle = OpenArchive(false, slotIndex);
	if( !archiveHandle ){
		TerminateWithError("Unable to open archive");
	}
	uchar* heroData = ReadPlayerInfo(archiveHandle);
	if( !heroData ){
		TerminateWithError("Unable to load character");
	}
	ClearAltWeapons();
	ReadWeaponSwapInfoFromSave( archiveHandle, WeaponSwapItems );
	
	ReadStashInfoFromSave( archiveHandle );
	ReadHotkeysInfoFromSave( archiveHandle );
	ReadAdditionalHeroDataFromSave( archiveHandle );
	Transmute_ReadFromSave( archiveHandle );
	
	LoadPlayerInfo(heroData, CurrentPlayerIndex, false);
	FreeMemZero(heroData);
	gbValidSaveFile = CheckSavefileSignature(archiveHandle);
	CloseArchiveRead(archiveHandle);
}

//----- (00456979) --------------------------------------------------------
void __fastcall CreateSaveLevelName( char (&key)[MAX_PATH] )
{
	sprintf(key, SaveFile_DunTemp, (DUNGEON)Dungeon);
}

//----- (004569C9) --------------------------------------------------------
void __fastcall CreateLoadLevelName( char (&key)[MAX_PATH] )
{
	Player& player = Players[CurrentPlayerIndex];
	uint slotIndex = GetSlotByHeroName(player.playerName);
	CreateSaveLevelName(key);
	if( !OpenArchiveWrite(false, slotIndex) ){
		TerminateWithError("Unable to read to save file archive, error num: %i", OpenFileError);
	}
	// (rt) these two lines must not be swapped, since CloseArchiveWrite resets all hashes
	bool hashCollision = MpqFileExists(key);
	CloseArchiveWrite(true, slotIndex); 
	if( hashCollision ){
		return;
	}
	sprintf(key, SaveFile_DunPerm, (DUNGEON)Dungeon);
}

//----- (00456A83) --------------------------------------------------------
void DestroyTempSaves()
{
	if (EnforceNoSave == true) { return; }
	Player& player = Players[CurrentPlayerIndex];
	if( MaxCountOfPlayersInGame > 1 ){
		return;
	}
	uint slotIndex = GetSlotByHeroName(player.playerName);
	if( !OpenArchiveWrite(false, slotIndex) ){
		TerminateWithError("Unable to write to save file archive, error num %i", OpenFileError);
	}
	MpqDeleteFiles(GetTempSaveNames);
	CloseArchiveWrite(true, slotIndex);
}

//----- (00456ADA) --------------------------------------------------------
int __stdcall GetTempSaveNames(int dungeonKeyIndex, char (&path)[MAX_PATH])
{
	if( dungeonKeyIndex < DungeonCount ){
		sprintf(path, SaveFile_DunTemp, dungeonKeyIndex);
	}else{
		return false;
	}
	return true;
}

//----- (00456B0F) --------------------------------------------------------
void MoveTempSavesToPermanent()
{
	Player& player = Players[CurrentPlayerIndex];
	uint slotIndex = GetSlotByHeroName(player.playerName);
	if( !OpenArchiveWrite(false, slotIndex) ){
		TerminateWithError("Unable to write to save file archive, error num %i", OpenFileError);
	}
	int dungeon = 0;
	char temp[MAX_PATH];
	char perm[MAX_PATH];
	while( GetTempSaveNames(dungeon, temp) ){
		GetPermSaveNames(dungeon, perm);
		if( MpqFileExists(temp) ){
			if( MpqFileExists(perm) ){
				MpqDeleteFile(perm);
			}
			MpqRenameFile(temp, perm);
		}
		dungeon++;
	}
	GetPermSaveNames(dungeon, perm);
	CloseArchiveWrite(true, slotIndex);
}

//----- (00456BC1) --------------------------------------------------------
int __stdcall GetPermSaveNames( int dungeon, char (&path)[MAX_PATH] )
{
	if( dungeon < DungeonCount ){
		sprintf(path, SaveFile_DunPerm, dungeon);
	}else{
		return false;
	}
	return true;
}

//----- (00456BF6) --------------------------------------------------------
void __fastcall WriteSaveFile( char* fileName, uchar* data, uint realSaveSize, int saveSizeRounded )
{
	Player& player = Players[ CurrentPlayerIndex ];
	int slotIndex = GetSlotByHeroName( player.playerName );
	EncodeFile((uchar*)data, realSaveSize, saveSizeRounded, CryptKey);
	BackupSaveIfNeed(slotIndex);
	if( ! OpenArchiveWrite(false, slotIndex) ){
		TerminateWithError("Unable to write to save file archive, error num %i", OpenFileError);
	}
	MpqAddFile(fileName, data, saveSizeRounded);
	CloseArchiveWrite(true, slotIndex);
}

//----- (00456C9F) --------------------------------------------------------
void* __fastcall ReadSaveFile( char *fileName, uint *loadedSize, int loadAdditionalSaveData )
{
	Player& player = Players[CurrentPlayerIndex];
	uint slotIndex = GetSlotByHeroName(player.playerName);
	HANDLE archiveHandle = OpenArchive(0, slotIndex);
	if( !archiveHandle ){
		TerminateWithError("Unable to open save file archive");
	}
	if( loadAdditionalSaveData ){
		ClearAltWeapons();
		ReadWeaponSwapInfoFromSave( archiveHandle, WeaponSwapItems );
		ReadStashInfoFromSave( archiveHandle );
		ReadHotkeysInfoFromSave( archiveHandle );
		ReadAdditionalHeroDataFromSave( archiveHandle );
		Transmute_ReadFromSave( archiveHandle );
	}
	HANDLE fileHandle;
	if( !SFileOpenFileEx(archiveHandle, fileName, 0, &fileHandle) ){
		TerminateWithError("Unable to open save file");
	}
	DWORD fileSize = SFileGetFileSize(fileHandle, 0);
	*loadedSize = fileSize;
	if( !fileSize ){
		TerminateWithError("Invalid file size");
	}
	void* data = AllocMem(*loadedSize);
	DWORD bytesReaded = 0;
	
	int tryCount = 5;
	retry:
	File_Seek(fileHandle, 0, 0, 0);
	if( ! SFileReadFile(fileHandle, data, *loadedSize, &bytesReaded, 0) ){
		TerminateWithError("Unable to read save file");
	}
	*loadedSize = DecodeFile((uchar*)data, *loadedSize, CryptKey);
	if( ! *loadedSize ){
		if( tryCount-- > 0 ){
			Sleep(500);
			goto retry;
		}
		TerminateWithError("Invalid save file");
	}
	SFileCloseFile(fileHandle);
	CloseArchiveRead(archiveHandle);;

	return data;
}
