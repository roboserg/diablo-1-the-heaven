#include "stdafx.h"

CRITICAL_SECTION MemoryAllocCritical;// 0057B640
int AllocCount = 0;// в 1.146 нет
int FreeCount = 0;// в 1.146 нет

int LastSeed;// 0057B630
u64 LastSeed64;
uint RandUsingCounter;// 0057B658
uint Rand64UsingCounter;
int RandomSeed;// 0057B638
u64 RandomSeed64;

uint NonGameplaySeed;

//----- (0041824B) --------------------------------------------------------
int __fastcall SoundIsPlaying(int soundIndex)
{
	Sound& sound = Sounds[soundIndex];
	if( sound.buffer ){
		return ThisSoundIsPlaying(sound);
	}else if( sound.flag & SF_1_SOLO ){
		return &sound == CurSpeechSound
			;
	}
	return 0;
}

//----- (00418278) --------------------------------------------------------
void StopSpeech()
{
	if( CurSpeechHandle ){
		SFileDdaEnd(CurSpeechHandle);
		SFileCloseFile(CurSpeechHandle);
		CurSpeechHandle = 0;
		CurSpeechSound = 0;
	}
}

uchar MonsterSoundSuffix[MST_COUNT] = { 'a'/*attack*/, 'h'/*hit*/, 'd'/*die*/, 's'/*special*/ };
//----- (004182A1) --------------------------------------------------------
void __fastcall LoadMonsterSounds(int spriteIndex)
{
	MonsterSprite& sprite = MonsterSprites[spriteIndex];
	const char* soundNameFormat = BaseMonsters[sprite.baseMonsterIndex].SoundFileName;
	if( IsDirectSoundAllocated ){
		int maxSound = BaseMonsters[sprite.baseMonsterIndex].HasSpecialSounds ?	MST_COUNT :	MST_COUNT - 1;
		for( int soundType = 0; soundType < maxSound; soundType++ ){ // не был установлен HasSpecialSound у демона (00301B04)
			for( int soundIndex = 0; soundIndex < 2; soundIndex++ ){
				Sound& sound = Sounds[S_COUNT + spriteIndex * Monster_Sound_Max_8 + soundType * 2 + soundIndex];
				sprintf(sound.name, soundNameFormat, MonsterSoundSuffix[soundType], soundIndex + 1);
				// false ? LoadSound(soundName) :
				LoadSound_Wav(sound);
			}
		}	
	}
}

//----- (00418386) --------------------------------------------------------
void FreeMonsterSounds()
{
	for( int i = S_COUNT; i < countof(Sounds); ++i ) FreeSoundData( Sounds[i] );
}

//----- (004183F0) --------------------------------------------------------
void __fastcall PlayMonsterSound(int monsterIndex, MONSTER_SOUND_TYPE soundType)
{
	int range, angle;
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& sprite = MonsterSprites[monster.SpriteIndex];
	Player& player = Players[CurrentPlayerIndex];
	if( ! player.SomeSoundMuteDelay ){
		if( IsDirectSoundAllocated && IsSoundEnable && gbBufferMsgs == BUFFER_OFF ){
			Sound& sound = Sounds[S_COUNT + monster.SpriteIndex * Monster_Sound_Max_8 + soundType * 2 + rand() % 2];
			if( sound.buffer && ! ThisSoundIsPlaying(sound) && SoundPos(monster.Row, monster.Col, range, angle) ){
				PlaySound(sound, range, angle);
			}
		}
	}
}

//----- (00418493) -------------------------------------------------------- player
bool __fastcall SoundPos(int row, int col, int& maxDelta, int& angle)
{
	Player& player = Players[CurrentPlayerIndex];
	int dr = row - player.Row;
	int dc = col - player.Col;
	angle = (dr - dc) * 256;
	if( abs(angle) <= 6400 ){
		maxDelta = std::max(abs(dr), abs(dc)) * 64;
		if( maxDelta < 6400 ) maxDelta = -maxDelta;
		return true;
	}
	return false;
}

//----- (0041850D) --------------------------------------------------------
void __fastcall PlayGlobalSound(int soundIndex)
{
	soundIndex = RandomizeSound(soundIndex);
	LoadAndPlaySoundFile(Sounds[soundIndex], 0, 0, 0);
}

//----- (00418525) --------------------------------------------------------
void __fastcall LoadAndPlaySoundFile( Sound& sound, int isLocal, int row, int col )
{
	Player& player = Players[CurrentPlayerIndex];
	if( player.SomeSoundMuteDelay && MaxCountOfPlayersInGame != 1 ){
		return;
	}
	if( !IsDirectSoundAllocated || !IsSoundEnable || gbBufferMsgs != BUFFER_OFF ){
		return;
	}
	if( !(sound.flag & (SF_1_SOLO|SF_2_MIX)) && sound.buffer && ThisSoundIsPlaying( sound ) ){
		return;
	}
	int angle = 0;
	int range = 0;
	if( !isLocal || SoundPos(row, col, range, angle) ){
		if( sound.flag & SF_1_SOLO ){
			PlaySpeech(sound, range, angle);
		}else{
			if( ! sound.buffer ){
				// false ? LoadSound(soundFile.FileNamePtr) :*/
				LoadSound_Wav(sound);
			}
			if( sound.buffer ){
				PlaySound(sound, range, angle);
			}
		}
	}
}

//----- (004185E5) -------------------------------------------------------- FileIO
void __fastcall PlaySpeech(Sound& sound, int range, int angle)
{
	StopSpeech();  // check and stop previous speech
	int soundVolume = GetOrSetSoundVolume(1) + range;
	if( soundVolume < VOL_MIN ){
		return;
	}
	LimitToMax(soundVolume, 0);
	if( SFileOpenFile(sound.name, (HANDLE*)&CurSpeechHandle) ){
		if( SFileDdaBeginEx(CurSpeechHandle, DDA_BUF_SIZE, 0, 0, soundVolume, angle, 0) ){
			CurSpeechSound = &sound;
		}else{
			StopSpeech();
		}
	}else{
		CurSpeechHandle = 0;
	}
}

//----- (00418652) --------------------------------------------------------
int __fastcall RandomizeSound(int sound)
{
	switch( sound ){
	case S_798_WAR_69:
	case S_586_SOR_69:
	case S_689_ROG_69:
	case S_906_MONK_69:
	//case S_9_SWING:
	case S_92_ACIDS1:
	case S_36_I_FLIPMAG:
	case S_69_I_MAGIC:
	case S_22_I_BHIT:
		sound += rand() % 2;
		break;
	//case S_725_WAR_02:
	case S_737_WAR_14:
	case S_740_WAR_15:
	case S_743_WAR_16:
		sound += rand() % 3;
		break;
	}
	return sound;
}

//----- (004186C4) --------------------------------------------------------
void __fastcall PlayLocalSound(int soundIndex, int row, int col)
{
	soundIndex = RandomizeSound(soundIndex);
	Sound& sound = Sounds[soundIndex];
	if( soundIndex >= S_0_WALK1 && soundIndex <= S_3_WALK4 && sound.buffer ){
		sound.lastPlayTime = 0;
	}
	LoadAndPlaySoundFile(sound, 1, row, col);
}

//----- (004186FC) --------------------------------------------------------
void StopAllSounds()
{
	DupSoundUpdate(1);
	StopSpeech();
	for(Sound& sound: Sounds) if(sound.buffer) sound.buffer->Stop();
}

//----- (0041877D) --------------------------------------------------------
void UpdateSound()
{
	DirectSoundErrorCheck();
	uint pos = 0, maxPos = 0;
	if( IsDirectSoundAllocated ){
		DupSoundUpdate(0);
		if( CurSpeechHandle && SFileDdaGetPos( CurSpeechHandle, &pos, &maxPos ) && pos >= maxPos ){
			StopSpeech();
		}
	}
}

//----- (004187C2) --------------------------------------------------------
void FreeAllSound()
{
	DupSoundUpdate(1);
	for( int i = 0; i < S_COUNT; i++ ) if( Sounds[i].buffer ) FreeSoundData(Sounds[i]);
	FreeMonsterSounds();
	StopSpeech();
	StopMusic();
	StopAmbient();
	ReleaseDirectSound();
}

//----- (004187E8) --------------------------------------------------------
void LoadClassSoundEffect()
{
	uchar soundType = SF_120_PLAYERS;
	if( MaxCountOfPlayersInGame <= 1 ){
		switch( Players[CurrentPlayerIndex].ClassID ){
		case PC_3_MONK:		soundType = SF_8_MONK; break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:	soundType = SF_16_SCOUT_ASSASIN; break;
		case PC_0_WARRIOR:
		case PC_5_SAVAGE:soundType = SF_32_PAL_GLAD; break;
		case PC_2_MAGE:		soundType = SF_64_MAGE; break;
		default: TerminateWithError("effects:1");
		}
	}
	LoadSoundType(soundType);
}

//----- (0041884D) --------------------------------------------------------
void __fastcall LoadSoundType(uchar soundType) // TODO: check HF source
{
	if( !IsDirectSoundAllocated ) return;

	for( int soundFileIndex = 0; soundFileIndex < S_COUNT; soundFileIndex++ ){ // отнимаем EndMarker, в TH1 вообще только 1024 звука проверяется
		Sound& sound = Sounds[soundFileIndex];
		if( sound.buffer || sound.flag & SF_1_SOLO ) continue;
		uchar v1 = soundType & SF_120_PLAYERS ^ soundType;// TODO: непонятная операция
		if( v1 && !(sound.flag & v1) ) continue;
		if( !(sound.flag & SF_120_PLAYERS) || sound.flag & (soundType & SF_120_PLAYERS) ){
			//false ? LoadSound(sound) :
			LoadSound_Wav(sound);
		}
	}
}

//----- (004188A4) --------------------------------------------------------
void MenuSoundInit()
{
	LoadSoundType(SF_4_MENU);
}

//----- (004188AB) --------------------------------------------------------
void __stdcall PlaySoundFile(int soundIndex)
{
	// тут бывает падеж при перемещении по главному меню
	if( ! IsDirectSoundAllocated || ! IsSoundEnable || (uint)soundIndex >= S_COUNT ) return;
	
	Sound& sound = Sounds[soundIndex];
	if( sound.buffer && ! ThisSoundIsPlaying(sound) ) PlaySound( sound, 0, 0 );
}	

uint HashSource[5][256];

//----- (00418910) --------------------------------------------------------
void __fastcall Decrypt(uint* data, uint bytes, uint key)
{
	uint adjust = 0xEEEEEEEE;
	uint iter = bytes >> 2;
	while (iter--) {
		adjust += HashSource[HASH_ENCRYPTDATA][key & 0xFF];
		adjust += (*data++ ^= adjust+key)+(adjust << 5)+3;
		key     = (key >> 11) | ((key << 21) ^ 0xFFE00000)+0x11111111;
	}
}

//----- (00418966) --------------------------------------------------------
void __fastcall Encrypt(uint* data, uint bytes, uint key)
{
	DWORD adjust = 0xEEEEEEEE;
	DWORD iter	= bytes >> 2;
	while (iter--) {
		DWORD origdata = *data;
		adjust += HashSource[HASH_ENCRYPTDATA][key & 0xFF];
		*data++ = origdata ^ (adjust+key);
		adjust += origdata + (adjust << 5)+3;
		key		= (key >> 11) | ((key << 21) ^ 0xFFE00000)+0x11111111;
	}
}

//----- (004189C0) --------------------------------------------------------
uint __fastcall Hash(char* filename, int hashType)
{
	DWORD result = 0x7FED7FED;
	DWORD adjust = 0xEEEEEEEE;
	while (filename && *filename) {
		char origchar = toupper(*filename++);
		result	= (result+adjust) ^ HashSource[hashType][origchar];
		adjust += origchar+result+(adjust << 5)+3;
	}

	return result;
}

//----- (00418A0F) --------------------------------------------------------
void InitializeHashSource()
{
	uint seed = 0x100001;
	for (int loop1 = 0; loop1 < 256; ++loop1) {
		for (int loop2 = 0; loop2 < 5; ++loop2) {
			seed = (seed * 0x7D + 3) % 0x2AAAAB;
			uint rand1 = seed & 0xFFFF;
			seed = (seed * 0x7D + 3) % 0x2AAAAB;
			uint rand2 = seed & 0xFFFF;
			HashSource[loop2][loop1] = (rand1 << 16) | rand2;
		}
	}
}

volatile size_t max_compress_size = 0;
//----- (00418A7E) --------------------------------------------------------
size_t __fastcall Compress(uchar* dst, size_t size) // dx: PkwareCompress
{
	if( size > max_compress_size ) max_compress_size = size;
	
	// ALLOCATE COMPRESSION BUFFERS
	static TCmpStruct CompressBuffer;
	size_t destBufSize = max(SECTORSIZE * 2, size * 2);
	void* destbuffer = new uchar[ destBufSize ];
	// был bug! падение сервера при выделении памяти при присоединении клиента
	// 0xd844c690 ( 3628385936 / -666581360 ) байт пыталось выделится
	memset(destbuffer, 0, destBufSize);

	// CREATE AN INFORMATION RECORD
	CompressionInfo info;
	info.sourcebuffer = dst;
	info.sourceoffset = 0;
	info.destbuffer	= destbuffer;
	info.destoffset	= 0;
	info.bytes = size;

	// PERFORM THE COMPRESSION
	uint comptype = CMP_BINARY;
	uint dictsize = max( 512u, min(4096u, (uint)SECTORSIZE) );
	implode(CompBufferRead, CompBufferWrite, &CompressBuffer, &info, &comptype, &dictsize);

	// IF THE DATA WAS NOT COMPRESSABLE, RETURN THE SOURCE DATA
	// OTHERWISE, RETURN THE COMPRESSED DATA
	if( info.destoffset < size ){
		memcpy(dst, destbuffer, info.destoffset);
		size = info.destoffset;
	}

	delete[] destbuffer;
	return size;
}

//----- (00418B14) --------------------------------------------------------
uint __cdecl CompBufferRead(char* buffer, uint* size, void* param)
{
	CompressionInfo* infoptr = (CompressionInfo*)param;
	uint bytes = min(*size,infoptr->bytes-infoptr->sourceoffset);
	memcpy(buffer,(LPSTR)infoptr->sourcebuffer+infoptr->sourceoffset,bytes);
	infoptr->sourceoffset += bytes;
	return bytes;
}

//----- (00418B48) --------------------------------------------------------
void __cdecl CompBufferWrite(char* buffer, uint* size, void* param)
{
	CompressionInfo* infoptr = (CompressionInfo*)param;
	memcpy((LPSTR)infoptr->destbuffer + infoptr->destoffset, buffer, *size);
	infoptr->destoffset += *size;
}

volatile size_t max_decompress_size = 0;
//----- (00418B6F) --------------------------------------------------------
void __fastcall Expand(uchar* data, uint bytes, uint size)
{
	if( size > max_decompress_size ) max_decompress_size = size;

	// ALLOCATE COMPRESSION BUFFERS
	static TDcmpStruct CompressBuffer;
	void* destbuffer = new uchar[size];
	memset(destbuffer, 0, size);

	// CREATE AN INFORMATION RECORD
	CompressionInfo info;
	info.sourcebuffer = data;
	info.sourceoffset = 0;
	info.destbuffer	= destbuffer;
	info.destoffset	= 0;
	info.bytes = bytes;

	// PERFORM THE DECOMPRESSION
	explode(CompBufferRead, CompBufferWrite, &CompressBuffer, &info);
	//app_assert(info.destoffset <= dwMaxBytes);

	// copy back into the original buffer
	memcpy(data, destbuffer, info.destoffset);

	delete[] destbuffer;
}

// x, y - левый нижний (!) угол битмапа
//----- (00418C48) -------------------------------------------------------- interface
void __fastcall Surface_DrawCEL(int x, int y, char* cel, int frame, int width)
{
	if( WorkingSurface && cel ){
		int* header = (int*) cel;
		RleDraw(&WorkingSurface[x] + YOffsetHashTable[y], cel + header[frame], header[frame + 1] - header[frame], width);
	}
}

//----- (00418C8C) -------------------------------------------------------- interface
void __fastcall Surface_DrawCelTo(uchar* dst, char* cel, int frame, int width)
{
	if( cel && dst ){
		int* header = (int*) cel;
		RleDraw(dst, cel + header[frame], header[frame + 1] - header[frame], width);
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall CEL_DrawTo(uchar* to, char* celBuf, uint frameNum, int drawWidth, int offsetStart, int offsetEnd, int flag /*= R_DIRECT*/, int outlineColor /*= -1*/)
{
	if( !WorkingSurface || !celBuf || !drawWidth ) return;

	uint* header = (uint*)celBuf;
	uint frameCount = header[0];
	if( frameNum > frameCount ) frameNum = frameCount;
	int frameOffset = header[frameNum];
	char* from = celBuf + frameOffset;
	int offVal = *(ushort*)(from + offsetStart);
	if( ! offVal )return;
	int rleLen = header[frameNum + 1] - frameOffset; // next frame - this frame
	int offVal2 = offsetEnd == 8 ? 0 : *(ushort*)(from + offsetEnd); // 8 - item inv pic ?
	rleLen = offVal2 ? offVal2 - offVal : rleLen - offVal;
	from += offVal;
	decltype(&RleDraw<>) draw;
	switch(flag){
	case  1: draw = RleDraw< 1>; break;
	case  2: draw = RleDraw< 2>; break;
	case  3: draw = RleDraw< 3>; break;
	case  4: draw = RleDraw< 4>; break;
	case  5: draw = RleDraw< 5>; break;
	case  6: draw = RleDraw< 6>; break;
	case  7: draw = RleDraw< 7>; break;
	case  8: draw = RleDraw< 8>; break;
	case  9: draw = RleDraw< 9>; break;
	case 10: draw = RleDraw<10>; break;
	case 11: draw = RleDraw<11>; break;
	case 12: draw = RleDraw<12>; break;
	case 13: draw = RleDraw<13>; break;
	case 14: draw = RleDraw<14>; break;
	case 15: draw = RleDraw<15>; break;
	case 16: draw = RleDraw<16>; break;
	case 17: draw = RleDraw<17>; break;
	case 18: draw = RleDraw<18>; break;
	case 19: draw = RleDraw<19>; break;
	case 20: draw = RleDraw<20>; break;
	case 21: draw = RleDraw<21>; break;
	case 22: draw = RleDraw<22>; break;
	case 23: draw = RleDraw<23>; break;
	case 24: draw = RleDraw<24>; break;
	case 25: draw = RleDraw<25>; break;
	case 26: draw = RleDraw<26>; break;
	case 27: draw = RleDraw<27>; break;
	case 28: draw = RleDraw<28>; break;
	case 29: draw = RleDraw<29>; break;
	case 30: draw = RleDraw<30>; break;
	case 31: draw = RleDraw<31>; break;
	default: draw = RleDraw; break;
	}
	if( outlineColor >= 0 ) (flag & R_MIRROR ? RleDraw<R_OUTLINE|R_MIRROR> : RleDraw<R_OUTLINE>)(to, from, rleLen, drawWidth, outlineColor);
	draw(to, from, rleLen, drawWidth, 0);
}

//----- (00418CB2) -------------------------------------------------------- interface
void __fastcall CEL_Draw(int x, int y, char* celBuf, uint frameNum, int drawWidth, int offsetStart, int offsetEnd, int flag /*= R_DIRECT*/, int outlineColor /*= -1*/)
{
	CEL_DrawTo(WorkingSurface + YOffsetHashTable[y - 16 * offsetStart] + x, celBuf, frameNum, drawWidth, offsetStart, offsetEnd, flag, outlineColor);
}

//----- (00418D31) -------------------------------------------------------- interface
void __fastcall WriteTileToSurface(void* dst, char* src, int frameNumber, int width, int a5, int sizeOffset)
{
	if( !src || !dst ){
		return;
	}
	uint* animationHeader = (uint*)src;
	uint frameOffset = animationHeader[frameNumber];
	char* framePtr = src + frameOffset;
	int frameDataStart = *(ushort*)(framePtr + a5);
	if( !frameDataStart ){
		return;
	}
	int frameSize = animationHeader[frameNumber+1] - frameOffset;
	int frameSize2;
	if( sizeOffset == 8 ){
		frameSize2 = 0;
	}else{
		frameSize2 = *(ushort*)(framePtr + sizeOffset);
	}
	int frameDataSize;
	if( frameSize2 ){
		frameDataSize = frameSize2 - frameDataStart;
	}else{
		frameDataSize = frameSize - frameDataStart;
	}
	RleDraw((uchar*)dst, framePtr + frameDataStart, frameDataSize, width);
}

//----- (00418F41) --------------------------------------------------------
void __fastcall Font_DrawChar(int aXpos, int aYpos, char* aFontPtr, unsigned char aCharIndex, int aFontSize)
{
	int Ypos;               // ebx@1
	char* PictPtr;			// edx@3
	uchar* DstPosPtr;       // ecx@3
	int PictSize;           // ST00_4@3
	int CharOffset;         // esi@3
	int Xpos;               // [sp+Ch] [bp-4h]@1
	unsigned* FontTable = (unsigned*)aFontPtr;
	Ypos = aYpos;
	Xpos = aXpos;
	if( WorkingSurface ){
		if( aFontPtr ){
			CharOffset = FontTable[aCharIndex];
			PictPtr = &aFontPtr[CharOffset];
			PictSize = FontTable[aCharIndex + 1] - CharOffset;
			DstPosPtr = &WorkingSurface[Xpos + YOffsetHashTable[Ypos]];
			if( DarkLevel )
				RleDraw<R_DARK>(DstPosPtr, PictPtr, PictSize, aFontSize);
			else
				RleDraw(DstPosPtr, PictPtr, PictSize, aFontSize);
		}
	}
}

//----- (00418F99) -------------------------------------------------------- interface
void __fastcall DrawItemOnMap(int offsetX, int offsetY, char* cel, int celIndex, int celWidth, int upHalfCel, int screenCellRow, int outlineColor /*= -1*/ )
{
	int celFile = (int) cel; // TODO: fix celFile type to char* below
	if( WorkingSurface && celFile ){
		int celOffset = *(uint*)(celFile + 4 * celIndex);
		int cel = celFile + celOffset;
		if( int pcxOffset = *(ushort*)(cel + upHalfCel/*0*/) ){
			int nextCelOffset = *(uint*)(celFile + 4 * (celIndex + 1));
			int celSize = nextCelOffset - celOffset;
			int wtfScreenRowSize = *(ushort*)(cel + screenCellRow);
			if( screenCellRow == 8 ){ // center row ?
				wtfScreenRowSize = 0; // first row and center row
			}
			int pcxSize = celSize - pcxOffset;
			if( wtfScreenRowSize ){
				pcxSize = wtfScreenRowSize - pcxOffset;
			}
			char* src = (char*)pcxOffset + cel;
			uchar* dst = &WorkingSurface[offsetX] + YOffsetHashTable[offsetY - 16 * upHalfCel];
			if( outlineColor >= 0 ) RleDraw<R_OUTLINE>(dst, src, pcxSize, celWidth, outlineColor);
			if( DarkLevel ){
				RleDraw<R_DARK>(dst, src, pcxSize, celWidth);
			}else{
				RleDraw(dst, src, pcxSize, celWidth);
			}
		}
	}
}

//----- (0041902B) -------------------------------------------------------- interface
void __fastcall CEL_DrawShadow(uchar* dst, char* celBuf, int frameNum, int drawWidth, int offsetStart, int offsetEnd, int flag /*= 0*/)
{
	if( TransparentMode ){
		flag = flag & R_MIRROR ? R_DARK|R_TRANS|R_MIRROR : R_DARK|R_TRANS;
	}else if( DarkLevel ){
		flag |= R_DARK;
	}
	CEL_DrawTo(dst, celBuf, frameNum, drawWidth, offsetStart, offsetEnd, flag);
}

//----- (00419734) -------------------------------------------------------- interface
void __fastcall ParseCELFile(uchar* aDestPtr, int aDstStartX, int aDstStartY, int aDstMaxX, char* aCELFilePtr, int aFrameNumber, int aSrcMaxX)
{
	int *frameStart; // ebx@3
	char *cel; // esi@3
	uchar *dest; // edi@3
	char* frameEnd; // ebx@3
	int widLeft; // edx@4
	int len; // eax@5
	int halfLen; // ecx@6
	int quartLen; // ecx@8
	int aDstMaxXa; // [sp+14h] [bp+Ch]@3

	if( aCELFilePtr && aDestPtr ){
		frameStart = (int*)&aCELFilePtr[ 4 * aFrameNumber ];
		cel = &aCELFilePtr[ *frameStart ];
		dest = &aDestPtr[ aDstStartX ] + aDstStartY * aDstMaxX;
		aDstMaxXa = aSrcMaxX + aDstMaxX;
		frameEnd = &cel[ frameStart[1] - *frameStart ];
		do{
			widLeft = aSrcMaxX;
			do{
				while( 1 ){
					len = *cel++;
					if( len >= 0 ){
						break;
					}
					len  = -len;
					dest += len;
					widLeft -= len;
					if( widLeft <= 0 ){
						goto LABEL_14;
					}
				}
				widLeft -= len;
				halfLen = len >> 1;
				if( len & 1 ){
					*dest++ = (uchar)*cel++;
					if( !halfLen ){
						continue;
					}
				}
				quartLen = len >> 2;
				if( halfLen & 1 ){
					*(ushort *) dest = *(ushort *) cel;
					cel += 2;
					dest += 2;
					if( !quartLen ){
						continue;
					}
				}
				memcpy( dest, cel, 4 * quartLen );
				cel += 4 * quartLen;
				dest += 4 * quartLen;
			}while( widLeft > 0 );
		LABEL_14:
			dest -= aDstMaxXa;
		}while( frameEnd > cel );
	}
}

//----- (00419EBA) --------------------------------------------------------
int __fastcall OrientationToTarget(int row, int col, int targetRow, int targetCol)
{
	int dx = targetRow - row;
	int dy = targetCol - col;
	int dir;
	if( dx >= 0 ){
		if( dy >= 0 ){
			dir = DIR_0_D;
			if( 2 * dx < dy ){ dir = DIR_1_DL; }
		}else{
			dy = -dy;
			dir = DIR_6_R;
			if( 2 * dx < dy ){ dir = DIR_5_UR; }
		}
		if( 2 * dy < dx ){ dir = DIR_7_DR; }
	}else{
		dx = -dx;
		if( dy >= 0 ){
			dir = DIR_2_L;
			if( 2 * dx < dy ){ dir = DIR_1_DL; }
		}else{
			dy = -dy;
			dir = DIR_4_U;
			if( 2 * dx < dy ){ dir = DIR_5_UR; }
		}
		if( 2 * dy < dx ){ dir = DIR_3_UL; }
	}
	return dir;
}

//----- (00419F27) --------------------------------------------------------
void __fastcall SetRndSeed( int seed )
{
	RandUsingCounter = 0;
	RandomSeed = seed;
	LastSeed = seed;
}

//----- (00419F3B) --------------------------------------------------------
int GetRndSeed()
{
	RandUsingCounter++;
	RandomSeed = RandomSeed * 22695477 + 1;
	return abs(RandomSeed);
}

// callerId used to debug sync multiplayer game by comparing callerId from all players
//----- (00419F5A) --------------------------------------------------------
int __fastcall RNG(int range, int callerId /*= 0*/)
{
	// TODO: check if we can use return address as callerId:
	// uintptr_t caller = (uintptr_t)__builtin_extract_return_addr(__builtint_retune_address(0)); // GCC variant
	// compiler can optimize same call in func to jmp to one call
	// then add compile time callerId generation
	if( range > 0 ){
		int randomInt = GetRndSeed();
		//if( randomInt == 1611170612 ) __debugbreak();
		if( range < 65535 ) return (randomInt >> 16) % range; // high order bits are more "random" than low order bits
		return randomInt % range;
	}else{
		return 0;
	}
}

//----- (th3) -------------------------------------------------------------
i64 Rng32to64(i64 range) // temporary func before move on Rng64
{
	if( range > 0 ){
		GetRndSeed();
		i64 random64 = (uint)RandomSeed;
		random64 |= (u64)(uint)GetRndSeed() << 32;
		return random64 % range;
	}else{
		return 0;
	}
}

//----- (th3) -------------------------------------------------------------
float __fastcall RngPercent(float rangePercent)
{
	return 0.0001f * RNG(int(rangePercent * 10000)); // 2 digit after dot precision
}

//----- (th2) -------------------------------------------------------------
u64 TrueRandom64()
{
	u64 trueRandom = 0;
	// https://github.com/cjee21/RDRAND-Tester/blob/main/RDRAND_Tester.c
	enum { eax, ebx, ecx, edx, rdrand_bit = 1 << 30, rdseed_bit = 1 << 18 };
	int cpuInfo[4];
	__cpuid(cpuInfo, 1);
	bool rdrand_supported = cpuInfo[ecx] & rdrand_bit;
	__cpuid(cpuInfo, 7);
	bool rdseed_supported = cpuInfo[ebx] & rdseed_bit;
	if( rdrand_supported && rdseed_supported ){
		if( ! _rdseed32_step( (uint*)&trueRandom     ) ) _rdrand32_step( (uint*)&trueRandom     );
		if( ! _rdseed32_step( (uint*)&trueRandom + 1 ) ) _rdrand32_step( (uint*)&trueRandom + 1 );
	}else{
		// use tick counter if no hardware random generator
		u64 h = 0, l = 0, r = 0; // TODO: try to fill seed with RDTSC lower bit
		for( int i = 0; i < 23; ++i ){
			r = __rdtsc();
			if( r & 1 )	l += r;
		}
		for( int i = 0; i < 19; ++i ){
			r = __rdtsc();
			if( r & 1 )	h += r;
		}
		trueRandom = (h << 32) | (uint)l;
		trueRandom += GetTickCountTh();
	}
	return trueRandom + time(0); // add classic time init to garantee non-zero random on some bugged CPUs
}

//----- (th2) -------------------------------------------------------------
void __fastcall InitRandomSeed64( u64 seed )
{
	Rand64UsingCounter = 0;
	RandomSeed64 = seed;
	LastSeed64 = seed;
}

//----- ( Xorshift64* ) ---------------------------------------------------
u64 GetRndSeed64()
{
	Rand64UsingCounter++;
	u64 x = RandomSeed64;
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;
	return RandomSeed64 = x;
}

//----- (th2) -------------------------------------------------------------
uint __fastcall Rng64(uint range /*= 0xFFFF'FFFFU*/ )
{
	return uint((GetRndSeed64() * 0x2545F4914F6CDD1DULL) >> 32) % range;
}

//----- (00419F83) --------------------------------------------------------
void InitLocking()
{
	InitializeCriticalSection(&MemoryAllocCritical);
	atexit([](){ DeleteCriticalSection(&MemoryAllocCritical); });
}

//----- (00419FB1) --------------------------------------------------------
void __fastcall ActNullSpell(int spellCastIndex)
{
	return;
}

//----- (00419FB2) --------------------------------------------------------
void* __fastcall AllocMem( uint memorySize )
{
	EnterCriticalSection(&MemoryAllocCritical);
	if( memorySize > 100000000 ) TerminateWithError( "Wrong memory size allocation" );
	// bug! падение при присоединении клиента
	void* memoryPtr = (void*)SMemAlloc(memorySize, __FILE__, __LINE__, 0);
	AllocCount++;// в th2 добавлено
	LeaveCriticalSection(&MemoryAllocCritical);
	if( !memoryPtr ){
		ErrorIn_CPP_File(ID_DIALOG2, GetLastError(), __FILE__, __LINE__);
	}

	return memoryPtr;
}

//----- (00419FFF) --------------------------------------------------------
void __fastcall FreeMem( void* memoryPtr )
{
	if( memoryPtr ){
		EnterCriticalSection(&MemoryAllocCritical);
		SMemFree(memoryPtr, __FILE__, __LINE__, 0);
		FreeCount++;// в th2 добавлено
		LeaveCriticalSection(&MemoryAllocCritical);
	}
}

//----- (0041A02F) --------------------------------------------------------
void* __fastcall LoadFile( const char* fileName, int* byteReaded /*= 0*/)
{
	HANDLE file;
	File_Open(fileName, &file, FIVE_TRY);
    int fileSize = File_GetSize(file, nullptr);
	if( byteReaded ) *byteReaded = fileSize;
	if( !fileSize ){
		TerminateWithError("Zero length SFILE:\n%s", fileName);
	}
	void* fileData = AllocMem(fileSize);
	File_Read(file, fileData, fileSize);
	File_Close(file);
	return fileData;
}

//----- (0041A08A) --------------------------------------------------------
int __fastcall LoadFileWithMem( char* fileName, void* dst )
{
	if( !dst ){
		TerminateWithError("LoadFileWithMem(NULL):\n%s", fileName);
	}
	HANDLE file;
	File_Open(fileName, &file, FIVE_TRY);
	int fileSize = File_GetSize(file, 0);
	if( !fileSize ){
		TerminateWithError("Zero length SFILE:\n%s", fileName);
	}
	File_Read(file, dst, fileSize);
	File_Close(file);
	return fileSize;
}

// ARM compatibility trick for multiplatform support ?
#define LOAD_LE32(b) (((DWORD)(b)[3] << 24) | ((DWORD)(b)[2] << 16) | ((DWORD)(b)[1] << 8) | (DWORD)(b)[0])
//----- (th2) -------------------------------------------------------------
__forceinline uchar *CelGetFrame(uchar *pCelBuff, int nCel, int *nDataSize)
{
	DWORD nCellStart;

	nCellStart = LOAD_LE32(&pCelBuff[nCel * 4]);
	*nDataSize = LOAD_LE32(&pCelBuff[(nCel + 1) * 4]) - nCellStart;
	return pCelBuff + nCellStart;
}

//----- (0041A0E9) --------------------------------------------------------
void __fastcall ColorSpriteAnimation( char* anim, uchar* colorTable, int frameCount )
{
	int i, nDataSize;
	int width;
	uchar *dst;
	if( ! anim || ! colorTable ) return;
	// TODO: apply algorithm for refactoring monster drawing decompilate: DrawMovingSprite...(), DrawSprite...()
	// TODO: compare with RleDraw() and try to unify
	for( i = 1; i <= frameCount; i++ ){
		dst = CelGetFrame((uchar*)anim, i, &nDataSize);
		if( is(*(short*)dst, 10, 12) ){ // optional header for pixels offset, TODO: check for use in big monster drawing offsets
			nDataSize -= *dst;
			dst += *dst;
		}
		while( nDataSize > 0 ){
			width = (char)*dst++;
			nDataSize--;
			if( width < 0 ){
				width = -width;
				if( width > 65 ){
					if( nDataSize <= 0 ) break;
					nDataSize--;
					*dst = colorTable[*dst];
					dst++;
				}else{
					LimitToMax( width, nDataSize );
					nDataSize -= width;
					for( ; width--; dst++ ) *dst = colorTable[*dst];
				}
			}
		}
	}
}

// TODO: change to Rle2Draw
//----- (0041A1EC) -------------------------------------------------------- interface
void __fastcall Rle2Draw(uchar* dst, char* src, int size, int width)
{
	int const shiftup = WorkingWidth + width;
	int leftovers = width;
	uchar blockSize;

	// RLE decoder (CL2)
	for (int32_t i = 0; i < size;) {
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ return; }
		blockSize = src[i++];

		if (blockSize > 190) { // regular
			blockSize = 65 - (blockSize - 191);
			while (blockSize--) {
				if (leftovers-- < 1) {
					dst -= shiftup;
					leftovers = width;
				}
				*dst++ = src[i++];
			}
		}
		else if( blockSize > 127) { // solid
			blockSize = 63 - (blockSize - 128);
			while (blockSize--) {
				if (leftovers-- < 1) {
					dst -= shiftup;
					leftovers = width;
				}
				*dst++ = src[i];
			}
			++i;
		}
		else { // transparent
			while (blockSize) {
				if (blockSize >= leftovers) {
					blockSize -= leftovers;
					dst -= shiftup - leftovers;
					leftovers = width;
				} else {
					leftovers -= blockSize;
					dst += blockSize;
					break;
				}
			}
		}
	}
}

//----- (th4) -------------------------------------------------------------
void __fastcall Rle2DrawOutline(uchar* dst, char* src, int size, int width, uchar outlineColor)
{
	int const shiftup = WorkingWidth + width;
	int leftovers = width;
	uchar blockSize;

	// RLE decoder (CL2)
	for (int32_t i = 0; i < size;) {
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ return; }
		blockSize = src[i++];

		if (blockSize > 190) { // regular
			blockSize = 65 - (blockSize - 191);
			while (blockSize--) {
				if (leftovers-- < 1) {
					dst -= shiftup;
					leftovers = width;
				}
				if( src[i++] ) dst[1] = dst[-1] = dst[WorkingWidth] = dst[-WorkingWidth] = outlineColor;
				dst++;
			}
		}
		else if( blockSize > 127) { // solid
			blockSize = 63 - (blockSize - 128);
			while (blockSize--) {
				if (leftovers-- < 1) {
					dst -= shiftup;
					leftovers = width;
				}
				if( src[i] ) dst[1] = dst[-1] = dst[WorkingWidth] = dst[-WorkingWidth] = outlineColor;
				dst++;
			}
			++i;
		}
		else { // transparent
			while (blockSize) {
				if (blockSize >= leftovers) {
					blockSize -= leftovers;
					dst -= shiftup - leftovers;
					leftovers = width;
				} else {
					leftovers -= blockSize;
					dst += blockSize;
					break;
				}
			}
		}
	}
}

// TODO: change to Rle2Draw (.CL2)
//----- (0041A274) -------------------------------------------------------- interface
void __fastcall CL2_DrawOutline(uchar color, int x, int y, char* cl2Buf, uint frameNum, int celWidth, int offsetStart, int offsetEnd)
{
	if( WorkingSurface && cl2Buf && frameNum > 0 ){
		uint* header = (uint*)cl2Buf;
		uint frameCount = header[0];
		if( frameNum > frameCount ) frameNum = frameCount;
		int frameOfs = header[frameNum];
		char* from = cl2Buf + frameOfs;

		int frameBegin = *(ushort*)(from + offsetStart); // тут иногда падает на отрисовке босса, возможно из-за старой записи
		if( ! frameBegin ) return;
		int rleLen = header[frameNum + 1] - frameOfs; // next frame - this frame
		int frameEnd = offsetEnd == 8 ? 0 : *(ushort*)(from + offsetEnd); // 8 - item inv pic ?
		rleLen = frameEnd ? frameEnd - frameBegin : rleLen - frameBegin;
		from += frameBegin;
		uchar* to = WorkingSurface + YOffsetHashTable[y - 16 * offsetStart] + x;
		Rle2DrawOutline(to, from, rleLen, celWidth, color);
	}
}

// может тут отрисовка суммонов завязана
//----- (0041A3B3) -------------------------------------------------------- interface
void __fastcall CL2_DrawDark(int x, int y, char* cl2Buf, uint frameNum, int pitch, int offsetStart, int offsetEnd, char lightType)
{
	if( WorkingSurface && cl2Buf && frameNum > 0 ){
		uint* header = (uint*)cl2Buf;
		uint frameCount = header[0];
		if( frameNum > frameCount ) frameNum = frameCount;
		int frameOfs = header[frameNum];
		char* from = cl2Buf + frameOfs;

		int frameBegin = *(ushort*)(from + offsetStart);
		if( ! frameBegin ) return;
		int rleLen = header[frameNum + 1] - frameOfs; // next frame - this frame
		int frameEnd = offsetEnd == 8 ? 0 : *(ushort*)(from + offsetEnd);
		rleLen = frameEnd ? frameEnd - frameBegin : rleLen - frameBegin;
		from += frameBegin;
		uchar* to = &WorkingSurface[x] + YOffsetHashTable[y - 16 * offsetStart];
		int lightTableIndex = Light4Levels ? 4 : 16;
		if( lightType == LT_2_STONE ) lightTableIndex += 1;
		if( lightType >= LT_4_U     ) lightTableIndex += lightType - LT_4_U + 3;
		Rle2DrawPal(to, from, rleLen, pitch, &LightTable[lightTableIndex * 256]);
	}
}

// TODO: change to template Rle2Draw
//----- (0041A47A) -------------------------------------------------------- interface
void __fastcall Rle2DrawPal(uchar* surf, char* sprite, int size, int pitch, uchar* palette)
{
	char *src; // esi@1
	uchar *dst; // edi@1
	int v7; // ebx@1
	int v8; // ecx@1
	int spriteData; // eax@1
	int v10; // edx@1
	uchar v11; // dl@4
	//char vars0[ 4 ]; // [sp+Ch] [bp+0h]@4
	src = sprite;
	dst = surf;
	v7 = pitch;
	v8 = size;
	CurPitch = pitch;
	spriteData = 0;
	v10 = 0;
	do{
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ return; }
		spriteData = *src++;
		--v8;
		if( (char) spriteData >= 0 ){
			do{
				if( spriteData <= v7 ){
					v10 = spriteData;
					dst += spriteData;
					spriteData = 0;
				}else{
					v10 = v7;
					dst += v7;
					spriteData -= v7;
				}
				v7 -= v10;
				if( v7 <= 0 ){
					v7 = CurPitch;
					dst = &dst[ -CurPitch - WorkingWidth ];
				}
			}while( spriteData > 0 );
		}else{
			spriteData = -spriteData;
			if( spriteData <= 65 ){
				v8 -= spriteData;
				v7 -= spriteData;
				do{
					*dst = palette[ *(uchar*)src++ ]; // индекс палитры беззнаковый
					--spriteData;
					++dst;
				}while( spriteData > 0 );
			}else{
				spriteData = spriteData - 65;
				--v8;
				v7 -= spriteData;
				v11 = palette[ *(uchar*)src++ ]; // индекс палитры беззнаковый
				do{
					*dst = v11;
					--spriteData;
					++dst;
				}while( spriteData > 0 );
			}
			if( v7 <= 0 ){
				v7 = CurPitch;
				dst = &dst[ -CurPitch - WorkingWidth ];
			}
		}
	}while( v8 );
}

//----- (0041A51F) -------------------------------------------------------- interface
void __fastcall CL2_Draw(int x, int y, char* cl2Buf, uint frameNum, int pitch, int offsetStart, int offsetEnd, bool darken /*= false*/ )
{
	if( WorkingSurface && cl2Buf && frameNum > 0 ){
		uint* header = (uint*)cl2Buf;
		uint frameCount = header[0];
		if( frameNum > frameCount ) frameNum = frameCount;
		int frameOfs = header[frameNum];
		char* from = cl2Buf + frameOfs;

		int frameBegin = *(ushort*)(from + offsetStart);
		if( ! frameBegin ) return;
		int rleLen = header[frameNum + 1] - frameOfs; // next frame - this frame
		int frameEnd = offsetEnd == 8 ? 0 : *(ushort*)(from + offsetEnd);
		rleLen = frameEnd ? frameEnd - frameBegin : rleLen - frameBegin;
		from += frameBegin;
		uchar* to = &WorkingSurface[x] + YOffsetHashTable[y - 16 * offsetStart];
		if( darken && DarkLevel ){
			Rle2DrawPal(to, from, rleLen, pitch, &LightTable[256 * DarkLevel]);
		}else{
			Rle2Draw(to, from, rleLen, pitch);
		}
	}
}

//----- (0041AA65) -------------------------------------------------------- game
void __fastcall PlayVideo2(char* name)
{
	PaletteFadeOut(8);
	PlayVideo(name, 1);
	ClearScreen();
	DrawNotGameplayScreen(1);
	PaletteFadeIn(8);
}

//----- (0041AAA1) -------------------------------------------------------- interface
void __fastcall AddOnScreenMessage(char messageIndex, int messagetime /*= 70*/ )
{
	if( OnScreenMessage::Tail + 1 != OnScreenMessage::Head ){
		for( uchar i = OnScreenMessage::Head; i != OnScreenMessage::Tail; ++i ){
			OnScreenMessage& m = OnScreenMessage::Queue[i];
			if( m.index == messageIndex ){
				return;// сообщение уже есть в очереди, выходим
			}
		}
		OnScreenMessage& newMessage = OnScreenMessage::Queue[OnScreenMessage::Tail++];
		newMessage.index = messageIndex;
		newMessage.time = messagetime;
	}
}

//----- (0041AAE2) -------------------------------------------------------- interface
void ClearOnScreenMessages()
{
	OnScreenMessage::Head = 0;
	OnScreenMessage::Tail = 0;
	OnScreenMessage::NotMainDrawing = 0;
}

// Draws the shrineEffect/save/load window + text
// next to fix :P
// Size of this window can be changed dinamically (its generated from 4 "corner" images and repeating
// horizontal and vertical tile. This way, we can also generate other windows based on the current
// resolution (Speech/barter etc...)
//----- (0041AAFD) --------------------------------------------------------
void DrawOnScreenMessage()
{
	uchar messageIndex = OnScreenMessage::Head != OnScreenMessage::Tail ? OnScreenMessage::Queue[OnScreenMessage::Head].index : OnScreenMessage::NotMainDrawing;
	if( messageIndex && messageIndex < countof(OnScreenMessages) && OnScreenMessages[messageIndex] && *OnScreenMessages[messageIndex] ){
		uchar* currentPixel;
		int rollback = 0;
		int width = 426;
		int height = 48;
		int xPos = ((WorkingWidth - width) >> 1);
		int yPos = Screen_TopBorder + height + ((ScreenHeight - height - 80) >> 1);
		int stringWidth = 0; // eax@11
		size_t length = 0;   // edx@11
		int offset;          // edi@11
		char currentLetter;  // bl@16
		// 	corners go like this
		// 	1   2
		// 	3   4
		Surface_DrawCEL(xPos, yPos - height, TextSlidCELPtr, 1, 12);
		Surface_DrawCEL(xPos + width, yPos - height, TextSlidCELPtr, 4, 12);
		Surface_DrawCEL(xPos, yPos, TextSlidCELPtr, 2, 12);
		Surface_DrawCEL(xPos + width, yPos, TextSlidCELPtr, 3, 12);
		for( int i = xPos + 8; i < xPos + width - 8; i += 12 ){
			Surface_DrawCEL(i, yPos - height, TextSlidCELPtr, 5, 12);
			Surface_DrawCEL(i, yPos, TextSlidCELPtr, 7, 12);
		}
		for( int i = yPos - height + 12; i < yPos; i += 12 ){
			Surface_DrawCEL(xPos, i, TextSlidCELPtr, 6, 12);
			Surface_DrawCEL(xPos + width, i, TextSlidCELPtr, 8, 12);
		}
		currentPixel = &WorkingSurface[xPos] + YOffsetHashTable[yPos - 3] + 3;
		// i add +6 to width and height because 1 of the "corners" is not included in width/height
		// (which is in fact "distance of corners") (--) 
		for( int j = 0; j < (height + 6); j++ ){
			rollback = 0;
			if( (j % 2) == 0 ){
				currentPixel++;
				rollback++;
			}
			for( int i = 0; i < ((width + 6) >> 1); i++ ){
				if( (i % 2) == 1 ){
					*currentPixel = 0;
					currentPixel += 2;
					rollback += 2;
				}else if( (i % 2) == 0 ){
					*currentPixel = 0;
					currentPixel += 2;
					rollback += 2;
				}
			}
			currentPixel = currentPixel - rollback - YOffsetHashTable[1];
		}
		strcpy(InfoPanelBuffer, OnScreenMessages[messageIndex]);
		offset = YOffsetHashTable[yPos - (height >> 1)] + xPos;
		length = strlen(InfoPanelBuffer);
		for( size_t i = 0; i < length; i++ ){
			stringWidth += FontWidthSmall[FontIndexSmall[Codepage[InfoPanelBuffer[i]]]] + 1;
		}
		if( stringWidth < (width + 16) ){
			offset += ((width + 16) - stringWidth) >> 1;
		}
		for( size_t i = 0; i < length; i++ ){
			currentLetter = FontIndexSmall[Codepage[InfoPanelBuffer[i]]];
			if( currentLetter ){
				DrawLetter(offset, currentLetter, 3);
			}
			offset += FontWidthSmall[currentLetter] + 1;
		}
	}
	// message ttl queuing
	if( IsMainDraw ){
		OnScreenMessage::NotMainDrawing = 0;
		if( OnScreenMessage::Head != OnScreenMessage::Tail ){
			OnScreenMessage& m = OnScreenMessage::Queue[OnScreenMessage::Head];
			if( m.time ){ --m.time; }
			if( ! m.time ){
				++OnScreenMessage::Head;
				OnScreenMessage::NotMainDrawing = m.index;
			}
		}
	}
}

// MSVC CRT internal structure hack
// TODO: change to second seed in built-in Rand engine with new Rng64 for better portability
struct crt_ptd
{
	uint pxcptacttab   ;
	uint tpxcptinfoptrs;
	uint tfpecode      ;
	uint terminate     ;
	uint terrno        ;
	uint tdoserrno     ;
	uint rand_state    ;
};

extern "C" crt_ptd* _cdecl __acrt_getptd();

uint grand()
{
	return __acrt_getptd()->rand_state;
}