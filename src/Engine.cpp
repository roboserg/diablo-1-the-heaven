#include "stdafx.h"
#include "ItemOverlay.h"

CRITICAL_SECTION MemoryAllocCritical;// 0057B640
int AllocCount = 0;// (rt) not in 1.146
int FreeCount = 0;// (rt) not in 1.146

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
		for( int soundType = 0; soundType < maxSound; soundType++ ){ // (rt) HasSpecialSound was not set for the demon (00301B04)
			for( int soundIndex = 0; soundIndex < 2; soundIndex++ ){
				Sound& sound = Sounds[S_COUNT + spriteIndex * Monster_Sound_Max_8 + soundType * 2 + soundIndex];
				sprintf(sound.name, soundNameFormat, MonsterSoundSuffix[soundType], soundIndex + 1);
				LoadSound(sound); // (th4) dispatch by extension: .wav or .ogg
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
				LoadSound(sound); // (th4) dispatch by extension: .wav or .ogg
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

	for( int soundFileIndex = 0; soundFileIndex < S_COUNT; soundFileIndex++ ){ // (rt) subtract EndMarker; TH1 only checks 1024 sounds at all
		Sound& sound = Sounds[soundFileIndex];
		if( sound.buffer || sound.flag & SF_1_SOLO ) continue;
		uchar v1 = soundType & SF_120_PLAYERS ^ soundType;// (rt) TODO: unclear operation
		if( v1 && !(sound.flag & v1) ) continue;
		if( !(sound.flag & SF_120_PLAYERS) || sound.flag & (soundType & SF_120_PLAYERS) ){
			LoadSound(sound); // (th4) dispatch by extension: .wav or .ogg
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
	// (rt) a crash sometimes happens here when moving through the main menu
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
	// (rt) there was a bug: the server crashed allocating memory when a client joined
	// (rt) 0xd844c690 ( 3628385936 / -666581360 ) bytes were attempted to be allocated
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

// (rt) x, y - the bottom-left (!) corner of the bitmap
//----- (00418C48) -------------------------------------------------------- interface
void __fastcall Surface_DrawCEL(int x, int y, char* cel, int frame, int width)
{
	if( WorkingSurface && cel ){
		int* header = (int*) cel;
		RleDraw(&WorkingSurface[x] + YOffsetHashTable[y], cel + header[frame], header[frame + 1] - header[frame], width);
		// (th4) Stage 2 (2026-09-04): zero the alpha overlay exactly under the
		// pixels this CEL writes — the Stage-2 analog of the LightSurface clear
		// RleDraw already performs. Menu/panel art drawn over the playfield must
		// never have occluded-sprite RGB composited on top of it. Same
		// pixel-accurate pass the cursor uses; gated so the non-ultra path stays
		// byte-identical and pays no second CEL decode.
		if (g_ARGBLighting && EnrichedLightUltraAlpha != 0 && AlphaSurface && SpriteLayerSurface) {
			RleDraw<R_OVERLAY_CLEAR>(&WorkingSurface[x] + YOffsetHashTable[y], cel + header[frame], header[frame + 1] - header[frame], width);
		}
		// (th4) Stage 3 (2026-09-05): same pixel-accurate pass for the colored-light
		// tint — menu/panel CEL art (ESC menu logo, dialog boxes, speech box,
		// monster-bar CELs) must never be multiplied by the world's winning light
		// color. Transparent cells stay untouched, so the tint shows through the
		// art's holes. Idle-gated inside RleDraw (R_TINT_CLEAR).
		if (g_ARGBLighting && g_LightTintActive && LightTintSurface) {
			RleDraw<R_TINT_CLEAR>(&WorkingSurface[x] + YOffsetHashTable[y], cel + header[frame], header[frame + 1] - header[frame], width);
		}
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
	// (th4) A8L8-only ring light pass after the body: only pixels that still hold
	// the outline color (the visible ring) get their LightSurface lifted.
	if( outlineColor >= 0 && g_A8L8Lighting ) (flag & R_MIRROR ? RleDraw<R_LIGHT_OUTLINE|R_MIRROR> : RleDraw<R_LIGHT_OUTLINE>)(to, from, rleLen, drawWidth, outlineColor);
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
void __fastcall Font_DrawChar(int aXpos, int aYpos, char* aFontPtr, int aCharIndex, int aFontSize, int outlineColor /*= -1*/)
{
	// (th4) Same guard as DrawLetter: a negative index means "no glyph" and must not
	// reach FontTable[aCharIndex] below.
	if( aCharIndex < 0 ) return;
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
			// (th4) Optional 1px ring using the mechanism CEL_DrawTo and DrawItemOnMap
			// already use: RleDraw<R_OUTLINE> walks the frame and writes outlineColor to
			// the four neighbours of every opaque pixel. Costs no art and works on any
			// glyph. MUST run before the body pass or it would paint over the strokes.
			// Default -1 = off, so every existing caller behaves exactly as before.
			if( outlineColor >= 0 )
				RleDraw<R_OUTLINE>(DstPosPtr, PictPtr, PictSize, aFontSize, outlineColor);
			if( DarkLevel )
				RleDraw<R_DARK>(DstPosPtr, PictPtr, PictSize, aFontSize);
			else
				RleDraw(DstPosPtr, PictPtr, PictSize, aFontSize);
			// (th4) Stage 2 (2026-09-04): clear the alpha overlay exactly under
			// the glyph pixels — menu text ("Pause", menu items, dialogs) must
			// never alpha-blend with occluded sprites behind it. The transparent
			// CEL cells are skip-runs, so only the glyph silhouette is cleared.
			// Gated for the non-ultra path (byte-identical, no second decode).
			if (g_ARGBLighting && EnrichedLightUltraAlpha != 0 && AlphaSurface && SpriteLayerSurface) {
				RleDraw<R_OVERLAY_CLEAR>(DstPosPtr, PictPtr, PictSize, aFontSize);
			}
			// (th4) Stage 3 (2026-09-05): clear the colored-light tint under the
			// glyph pixels too — menu glyphs (ESC menu, pause text) must never
			// inherit the world's winning light color. Skip-runs leave the tint
			// visible around the glyph silhouette. Idle-gated (R_TINT_CLEAR).
			if (g_ARGBLighting && g_LightTintActive && LightTintSurface) {
				RleDraw<R_TINT_CLEAR>(DstPosPtr, PictPtr, PictSize, aFontSize);
			}
		}
	}
}

//----- (00418F99) -------------------------------------------------------- interface
// (th4) OUTLINE ONLY -- the ring, never the body.
//
// R_OUTLINE ALONE IS NOT AN OUTLINE. It writes outlineColor to all four neighbours of
// every cel pixel, and for a solid sprite most of those neighbours are INTERIOR pixels;
// what carves it back to a ring is the body pass drawing over the interior afterwards.
// Skipping the body therefore fills the whole silhouette -- objects came out solidly
// coloured instead of outlined.
//
// So the body IS drawn, and then everything that is not ring is put back:
//   1. copy the sprite's bounding box to scratch
//   2. R_OUTLINE      -> ring + interior all become outlineColor
//   3. normal body    -> interior overwritten with sprite pixels; the ring survives
//   4. restore from scratch every pixel that is NOT outlineColor
// Step 4 is the same "a pixel still holding outlineColor is the ring" test that
// RleDraw<R_LIGHT_OUTLINE> relies on, so no new assumption is introduced. It leaves the
// ring and nothing else, whatever was underneath.
//
// Known limit, shared with R_LIGHT_OUTLINE: a body pixel whose own palette index equals
// outlineColor is indistinguishable from ring and survives as a speck. Rle2DrawLightOutline
// solves that with a decoded body mask; not worth it here for a 1px highlight.
// onlyOverWalls: keep the ring ONLY where an opaque wall was drawn (WOB_WALL in
// WallOcclusionSurface). The flush that calls this runs after the whole scene, so
// without the restriction the ring also lands on top of the player, monsters and
// ground items -- which is what the deferral was for, but only walls needed beating.
// When the object was already drawn normally this frame, the inline outline in
// DrawObjectSprite has correct z-order; all this pass has to add is the part a wall
// covered. When the object was NOT drawn (pitch-dark cell) there is nothing to
// preserve, so the caller passes false and the whole ring is kept.
void __fastcall DrawItemOutlineOnMap(int offsetX, int offsetY, char* cel, int celIndex, int celWidth, int upHalfCel, int screenCellRow, int outlineColor, bool onlyOverWalls /*= false*/)
{
	int celFile = (int) cel;
	if( !WorkingSurface || !celFile || outlineColor < 0 ) return;
	int celOffset = *(uint*)(celFile + 4 * celIndex);
	int celBase = celFile + celOffset;
	int pcxOffset = *(ushort*)(celBase + upHalfCel);
	if( !pcxOffset ) return;
	int nextCelOffset = *(uint*)(celFile + 4 * (celIndex + 1));
	int celSize = nextCelOffset - celOffset;
	int wtfScreenRowSize = *(ushort*)(celBase + screenCellRow);
	if( screenCellRow == 8 ) wtfScreenRowSize = 0;
	int pcxSize = celSize - pcxOffset;
	if( wtfScreenRowSize ) pcxSize = wtfScreenRowSize - pcxOffset;
	char* src = (char*)pcxOffset + celBase;
	uchar* dst = &WorkingSurface[offsetX] + YOffsetHashTable[offsetY - 16 * upHalfCel];

	// Bounding box. RleDraw builds the sprite UPWARDS from dst, so the box runs from
	// (dst - rows*WorkingWidth) down to dst, with one pixel of margin for the ring.
	// ROW COUNT IS COUNTED, NOT ESTIMATED. The obvious pcxSize/celWidth is NOT an upper
	// bound: a fully transparent row is a single run byte covering up to 127 px, so a
	// tall sparse sprite has far more rows than that quotient -- its top rows would fall
	// outside the box, never get restored, and the solid block would come back for some
	// sprites and not others. Decoding the run lengths is exact and costs one pass over
	// the control bytes.
	const int kMaxRows = 256;
	int rows = 0;
	if( celWidth > 0 ){
		// Mirrors RleDraw exactly: widLeft resets to width per line and any overshoot is
		// DISCARDED (it does `goto next_line` the moment widLeft <= 0), so the remainder
		// must not be carried into the next row or the count drifts long.
		int widLeft = celWidth;
		for( int i = 0; i < pcxSize && rows <= kMaxRows; ){
			signed char run = (signed char)src[i++];
			int n = run > 0 ? run : -run;      // >0 opaque (n colour bytes follow), <0 transparent
			if( run > 0 ) i += n;
			widLeft -= n;
			if( widLeft <= 0 ){ ++rows; widLeft = celWidth; }
		}
		if( widLeft != celWidth ) ++rows;      // partial trailing row
	}else{
		rows = kMaxRows;
	}
	if( rows > kMaxRows ) rows = kMaxRows;
	if( rows < 1 ) rows = 1;
	const int bw = celWidth + 2;
	const int bh = rows + 2;
	uchar* boxTop = dst - (intptr_t)(rows + 1) * WorkingWidth - 1;
	// Stay inside the allocated working surface; the margins above are why this is
	// checked rather than assumed.
	// Both ends: the top margin can run off the front of the surface and the bottom
	// row off the end. A clipped sprite at a screen edge must not memcpy outside it.
	if( boxTop < SoftSrcSurfacePtr ) return;
	if( boxTop + (size_t)(bh - 1) * WorkingWidth + bw > WorkingSurfaceDrawEnd ) return;

	static uchar scratch[ 256 * (256 + 2) ];
	if( bw * bh > (int)sizeof(scratch) ) return;
	for( int y = 0; y < bh; ++y ) memcpy( scratch + (size_t)y * bw, boxTop + (size_t)y * WorkingWidth, bw );

	RleDraw<R_OUTLINE>(dst, src, pcxSize, celWidth, outlineColor);
	if( DarkLevel ) RleDraw<R_DARK>(dst, src, pcxSize, celWidth);
	else            RleDraw(dst, src, pcxSize, celWidth);

	const uchar* wob = ( onlyOverWalls && WallOcclusionSurface )
	                 ? WallOcclusionSurface + ( boxTop - SoftSrcSurfacePtr ) : nullptr;
	for( int y = 0; y < bh; ++y ){
		uchar* row = boxTop + (size_t)y * WorkingWidth;
		const uchar* sav = scratch + (size_t)y * bw;
		const uchar* wrow = wob ? wob + (size_t)y * WorkingWidth : nullptr;
		for( int x = 0; x < bw; ++x ){
			const bool isRing = row[x] == (uchar)outlineColor
			                 && ( !wrow || wrow[x] == WOB_WALL );
			if( !isRing ) row[x] = sav[x];
		}
	}

	// A8L8: lift the ring so ambient darkness does not swallow it. Run last -- every
	// surviving outlineColor pixel is now genuinely ring.
	if( g_A8L8Lighting ) RleDraw<R_LIGHT_OUTLINE>(dst, src, pcxSize, celWidth, outlineColor);
}

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
			// (th4) A8L8-only ring light pass after the body (see CEL_DrawTo).
			if( outlineColor >= 0 && g_A8L8Lighting ) RleDraw<R_LIGHT_OUTLINE>(dst, src, pcxSize, celWidth, outlineColor);
		}
	}
}

//----- (0041902B) -------------------------------------------------------- interface
void __fastcall CEL_DrawShadow(uchar* dst, char* celBuf, int frameNum, int drawWidth, int offsetStart, int offsetEnd, int flag /*= 0*/)
{
	if (TransparentMode) {
		flag = flag & R_MIRROR ? R_DARK | R_TRANS | R_MIRROR : R_DARK | R_TRANS;
	}
	else if (DarkLevel) {
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
	// (rt) bug: crash when a client joins
	void* memoryPtr = (void*)SMemAlloc(memorySize, __FILE__, __LINE__, 0);
	AllocCount++;// (rt) added in th2
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
		FreeCount++;// (rt) added in th2
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

// (th4) Monster silhouette brightness tuning constants.
// Silhouette brightness = clamp(lightFill * kSilhouetteFactor / 256,
//                               kSilhouetteMinBright, kSilhouetteMaxBright)
// Lower LightSurface value = brighter pixel (0 = full bright, 255 = full dark).
const int   kSilhouetteFactor     = 128;   // 50% brighter than cell ambient, larger value means weaker brightness enhancement
const uchar kSilhouetteMinBright  = 0;     // never brighter than full bright is value is 0
const uchar kSilhouetteMaxBright  = 128;   // never darker than 50% brightness, larger value means weaker brightness enhancement
// (th4) Thief + infravision (A8L8): her dungeon is pitch black (ambient ~255), so the
// red-tinted sprite would be invisible. Open pixels are pinned to this LightSurface
// value (0 = full bright; any value < ambient stays visible). Wall-occluded pixels are
// left untouched (keep ambient), so no silhouette shows through walls.
const uchar kThiefInfravisionBright = 0;   // tunable: 0 = full bright, higher = dimmer

// TODO: change to Rle2Draw
//----- (0041A1EC) -------------------------------------------------------- interface
void __fastcall Rle2Draw(uchar* dst, char* src, int size, int width, uchar lightFill /*= 0*/)
{
	int const shiftup = WorkingWidth + width;
	int leftovers = width;
	uchar blockSize;
	// Approach B (A8L8): constant offset from WorkingSurface to LightSurface.
	// Skip per-pixel clearing when A8L8 is disabled (LightSurface is never sampled).
	const bool hasLight = g_A8L8Lighting && LightSurface && SoftSrcSurfacePtr &&
		(uintptr_t)dst >= (uintptr_t)SoftSrcSurfacePtr;
	const intptr_t lightOfs = hasLight ? (intptr_t)LightSurface - (intptr_t)SoftSrcSurfacePtr : 0;
//	const intptr_t baseLightOfs = hasLight ? (intptr_t)LightSurfaceBase - (intptr_t)SoftSrcSurfacePtr : 0;

	// (th4) Per-call snapshots of globals that do not change during a draw call.
	// g_CurrentPostRenderType is assigned once per post-render draw before Rle2Draw
	// runs; WallOcclusionSurface is only ever written THROUGH, never reassigned.
	// Locals let the compiler keep these loop-invariant branch operands in registers
	// across the per-pixel loops (MSVC's aliasing rules are conservative and may
	// otherwise reload the globals every pixel).
	const SpriteType spriteType = g_CurrentPostRenderType;
	// NOTE: this must be "uchar* const" (const pointer, mutable buffer). Writing
	// "const uchar*" would make the pointee const and break the wobSurface[...] = ...
	// stores below — the occlusion buffer IS written during the main pass.
	uchar* const wobSurface = hasLight ? WallOcclusionSurface : nullptr;
	// (th4) The two occlusion flags are also per-call invariant; hoist them so the
	// per-pixel occlusion stores below do not reload the globals every pixel.
	const bool isDrawingOpaqueWall = g_IsDrawingOpaqueWall;
	const bool isDrawingOpaqueObject = g_IsDrawingOpaqueObject;

	// (th4) Compute per-sprite silhouette brightness for wall-occluded pixels.
	// Uses g_CurrentPostRenderType (set before each post-render draw call) to
	// determine silhouette behavior without fragile lightFill==0 heuristics.
	// Only needed during the A8L8 post-render pass (g_LightSurfaceOnlyPass).
	// silhouetteBright: written to wall-occluded pixels (enhanced silhouette).
	// openBright: written to non-occluded pixels (defaults to lightFill, preserving
	//             the pre-Thief uniform sprite brightness for every existing type).
	// occludedSkip: InfravisionMonsterThief leaves wall-occluded pixels untouched
	//               so they keep the pre-filled ambient value (no silhouette).
	uchar silhouetteBright = 0;
	uchar openBright = lightFill;
	bool occludedSkip = false;
	if (g_LightSurfaceOnlyPass) {
		switch (spriteType) {
		case SpriteType::Player:
			silhouetteBright = 0;   // always full bright through walls
			break;
		case SpriteType::PlayerShaded:
			// (th4) THIEF: lightFill is her cell shade (DarkLevel*17). Same value behind
			// walls, so she neither glows in the dark nor gets a bright silhouette.
			silhouetteBright = lightFill;
			break;
		case SpriteType::InfravisionMonster:
			silhouetteBright = lightFill;  // (th4) no silhouette: same brightness occluded or not
			break;
//		case SpriteType::InfravisionMonsterThief:
//			// (th4) THIEF + infravision: ambient is pitch black. Pin non-occluded
//			// pixels to kThiefInfravisionBright and leave wall-occluded pixels at
//			// ambient, so the red tint shows without a half-silhouette.
//			occludedSkip = true;
//			openBright = kThiefInfravisionBright;
//			break;
		case SpriteType::InfravisionMonsterThief:
			// (th4) THIEF + infravision: ambient is pitch black. Pin non-occluded
			// pixels to kThiefInfravisionBright. Wall-occluded pixels get the normal
			// monster silhouette (kSilhouetteMaxBright = 50%) instead of being left at
			// ambient -- the merged branch skipped them, which made infravision
			// monsters vanish behind walls. Objects are not occluders here (same as
			// every other sprite type): a monster behind a chest shows fully.
			occludedSkip = false;
			openBright = kThiefInfravisionBright;
			silhouetteBright = kSilhouetteMaxBright;
			break;
		case SpriteType::Monster:
		case SpriteType::Missile: {
			int s = ((int)lightFill * kSilhouetteFactor) >> 8;
			if (s < kSilhouetteMinBright) s = kSilhouetteMinBright;
			if (s > kSilhouetteMaxBright) s = kSilhouetteMaxBright;
			silhouetteBright = (uchar)s;
			break;
		}
		default:
			silhouetteBright = lightFill;  // legacy: uniform brightness
			break;
		}
	}

	// (th4) Per-call occlusion mask for the post-render silhouette decision.
	// InfravisionMonsterThief also treats dungeon-object pixels (WOB_OBJECT) as
	// occluders → skip write → ambient → no silhouette behind doors/chests/etc.
	// Every other type keeps the pre-existing wall-only silhouette behavior.
	const uchar occlMask = WOB_WALL | (occludedSkip ? WOB_OBJECT : 0);

	// (th4) Stage 2 alpha-overlay (ultra): per-call snapshot of the feature gate.
	// rgbLUT is the shared per-frame palette->RGB table (built in MainDrawFunc
	// before DrawIsoLand). Only wall-occluded sprite pixels write the overlay;
	// non-occluded pixels keep the existing silhouette/ambient behavior.
	const bool alphaOverlay = g_ARGBLighting && EnrichedLightUltraAlpha != 0
		&& AlphaSurface != nullptr && SpriteLayerSurface != nullptr;
	const uchar overlayAlpha = (uchar)EnrichedLightUltraAlpha;
	// (th4) Stage 3 owner-pixel tint override: when the current sprite owns a
	// colored light, the replay writes white tint at its sprite pixels — the
	// tint field covers the owner's own pixels too, and without the override
	// the owner sprite would be recolored by its own light. Marker is set per
	// sprite by the DrawIsoLand replay loops (PostRenderSpriteCmd::tintExclude).
	// The write is limited to pixels where the owner is the visible topmost
	// surface: behind a wall/object the occluder keeps the winning tint, and
	// covered by a later-drawn sprite (e.g. the player in front of a portal)
	// that sprite keeps the winning tint (per-pixel guards below, *dst == sv).
	// Idle gate: with no colored light active the tint field is never
	// uploaded, so the owner-pixel override writes are skipped too.
	const bool tintExclude = g_LightTintActive && g_TintExcludeCurrentSprite && LightTintSurface != nullptr;

	// RLE decoder (CL2)
	for (int32_t i = 0; i < size;) {
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ return; }
		blockSize = src[i++];

		if (blockSize > 190) { // regular
			blockSize = 65 - (blockSize - 191);
			while (blockSize--) {
				if (leftovers-- < 1) {
					dst -= shiftup;
//					leftovers = width;// Original logic with off by 1 error
					// FIXED: pixel that triggered wrap already consumed 1 slot;
					// width-1 prevents 1-pixel drift per scanline boundary.
					leftovers = width - 1;
				}
//				*dst = src[i++];
//				if (hasLight && !g_SkipSpriteLightClear) *(uchar*)((intptr_t)dst + lightOfs) = lightFill;// Player sprite darkened by ambient light, player appears darker in Crypt and Abyss levels
//				if (hasLight) *(uchar*)((intptr_t)dst + lightOfs) = g_SkipSpriteLightClear ? 0 : lightFill;// Enforce full brightness for player sprite, but player is seen through opaque wall.  Door may lit up as player walks through it.
				// (th4) Stage 2: the overlay layer needs the sprite color at
				// occluded px, so the light-only pass reads the RLE value too
				// -- but only when the overlay is actually active (ultra +
				// alpha). Otherwise the read is skipped so the light-only
				// pass keeps its pre-Stage-2 cost in every other mode.
				uchar sv = (!g_LightSurfaceOnlyPass || alphaOverlay || tintExclude) ? src[i] : 0;
				if (g_LightSurfaceOnlyPass) {
					// Re-draw pass: skip WorkingSurface write, only clear LightSurface
					++i;
				} else {
					*dst = sv;
					++i;
				}
				// (th4) track wall occlusion: SET for arches, WOB_OBJECT for dungeon
				// objects, CLEAR for normal sprites (monsters/players in main pass).
				if (!g_LightSurfaceOnlyPass && wobSurface) {
					wobSurface[(uchar*)dst - SoftSrcSurfacePtr] = isDrawingOpaqueWall ? WOB_WALL : (isDrawingOpaqueObject ? WOB_OBJECT : 0);
				}
				if (hasLight) {
					if (g_LightSurfaceOnlyPass) {
						// (th4) Stage 3: owner-pixel tint override — the replay
						// already knows every sprite pixel; write white so the
						// light owner keeps its original colors (owner-pixel
						// exclusion, §5 Stage 3 item 4). The override applies
						// only where the owner sprite is the visible topmost
						// surface at the pixel:
						//  * behind a wall (WOB_WALL) or a dungeon object
						//    (WOB_OBJECT, e.g. a coffin) the visible surface is
						//    the occluder, which must keep the winning light's
						//    tint so the translucent sprite composites over the
						//    same tinted scene as its surroundings (overriding
						//    to white at occluded pixels punched a bright
						//    sprite-shaped hole into a colored light pool — the
						//    "white-ish glow" behind a wall/object);
						//  * covered by a later-drawn sprite (e.g. the player
						//    standing in front of a portal) the visible surface
						//    is that sprite, which must also keep the winning
						//    light's tint. The replay runs after the whole main
						//    pass, so *dst already holds the final topmost
						//    pixel: *dst == sv exactly where the owner's own
						//    pixel survived on top (a palette-index collision
						//    between two sprites can only re-enable the override
						//    at a few stray pixels).
						// Objects are deliberately NOT in occlMask (pre-existing
						// "sprites show through objects" behaviour), so the
						// tint guard checks both flags explicitly.
						if (tintExclude) {
							const ptrdiff_t tintPix = (uchar*)dst - SoftSrcSurfacePtr;
							const bool occludedByScene = wobSurface != nullptr
								&& (wobSurface[tintPix] & (WOB_WALL | WOB_OBJECT)) != 0;
							if (!occludedByScene && *dst == sv)
								LightTintSurface[tintPix] = TINT_WHITE;
						}
//						// Post-render: use caller's lightFill for per-sprite brightness.
//						// Player: lightFill=0 → full bright.  Monster: lightFill=DarkLevel*17 → 15-level blend.
//						*(uchar*)((intptr_t)dst + lightOfs) = lightFill;
						// (th4) Per-pixel decision: silhouette behind walls, 256-level ambient in open areas.
						// (th4) InfravisionMonster: skip ALL LightSurface writes.
						// Pre-fill handles brightness — no silhouette through walls, objects, or shadows.
						if (spriteType != SpriteType::InfravisionMonster) {
							uchar* lp = (uchar*)((intptr_t)dst + lightOfs);
							if (wobSurface) {
								ptrdiff_t pixOfs = (uchar*)dst - SoftSrcSurfacePtr;
								// (th4) Stage 2: in ultra + alpha, dungeon objects
								// occlude sprites exactly like walls — a sprite behind
								// a coffin gets the translucent composite (sprite art
								// + weight) instead of the classic max-bright
								// show-through silhouette. Non-ultra keeps the
								// pre-existing "sprites show through objects" look.
								if ((wobSurface[pixOfs] & occlMask)
									|| (alphaOverlay && (wobSurface[pixOfs] & WOB_OBJECT) != 0)) {
									if (alphaOverlay) {
										// (th4) Stage 2: translucent sprite over the wall.
										// Capture the sprite RGB + overlay weight and skip
										// the silhouetteBright write, so the wall keeps its
										// ambient light under the translucent sprite.
										// SpriteLayerSurface is uint* indexed by PIXEL offset
										// (same space as WorkingSurface), 4 bytes per pixel.
										SpriteLayerSurface[pixOfs] = rgbLUT[sv];
										AlphaSurface[pixOfs] = overlayAlpha;
									} else if (!occludedSkip) {
										// (th4) InfravisionMonsterThief: leave ambient → no silhouette
										*lp = silhouetteBright;  // occluded by wall → enhanced silhouette
									}
								} else {
//									*lp = *(uchar*)((intptr_t)dst + baseLightOfs);  // open area → 256-level per-pixel ambient. This introduces boss monster uneven brightness when hugging a wall, and Water Wyrm neck cut off without silhouette issue.
									*lp = openBright;  // open/overwritten → uniform sprite brightness
								}
							} else {
								*lp = occludedSkip ? openBright : silhouetteBright;  // fallback: no occlusion buffer → uniform silhouette
							}
						}
					} else if (g_A8L8Lighting && PixelLight && g_InDungeonRender) {
						// Screen-space PPL: skip LightSurface write, pre-fill handles it
					} else if (g_SkipSpriteLightClear) {
						*(uchar*)((intptr_t)dst + lightOfs) = 0;
					} else {
						*(uchar*)((intptr_t)dst + lightOfs) = lightFill;
					}
				}
				++dst;
			}
		}
		else if( blockSize > 127) { // solid
			int n = 63 - (blockSize - 128);
//			while (blockSize--) {
//				if (leftovers-- < 1) {
//					dst -= shiftup;
//					leftovers = width;
//				}
//				*dst = src[i];
//				if (hasLight && !g_SkipSpriteLightClear) *(uchar*)((intptr_t)dst + lightOfs) = lightFill;// Player sprite darkened by ambient light, player appears darker in Crypt and Abyss levels
//				if (hasLight) *(uchar*)((intptr_t)dst + lightOfs) = g_SkipSpriteLightClear ? 0 : lightFill;// Enforce full brightness for player sprite, but player is seen through opaque wall.  Door may lit up as player walks through it.
//				++dst;
//			if (g_LightSurfaceOnlyPass && hasLight && !wobSurface && spriteType != SpriteType::InfravisionMonsterThief && spriteType != SpriteType::InfravisionMonster) {
				// FAST PATH: bulk memset for LightSurface-only solid runs.
				// (th4) HISTORY: pre-02200c8 this ran under `g_LightSurfaceOnlyPass &&
				// hasLight` and was a pure perf win -- the post-render wrote uniform
				// lightFill everywhere back then. 02200c8 added the per-pixel silhouette /
				// ambient decisions and gated it with `!WallOcclusionSurface`, intending
				// "fall through to the per-pixel loop whenever the occlusion buffer exists".
				// That guard is unsatisfiable: WallOcclusionSurface is always allocated in
				// A8L8 mode (the only mode this pass runs), so the block has been dead since
				// 02200c8. Do NOT restore the old gate: solid runs would get uniform
				// lightFill and lose silhouettes behind walls + per-pixel ambient in open
				// areas (that regresses 02200c8's Fix 2).
				// Only used when WallOcclusionSurface is unavailable (no per-pixel
				// wall-occlusion decisions needed).  Falls through to per-pixel
				// loop when WallOcclusionSurface exists.
				// The two InfravisionMonster types are excluded: their per-pixel
				// semantics (skip ALL writes / write openBright) differ from the
				// uniform lightFill memset, so they must take the per-pixel loop.
				// Solid runs (N pixels sharing one palette index) are common
				// in sprites (30-50% of pixels).  A single memset replaces N
				// per-pixel LightSurface stores.
				// IMPORTANT: the original per-pixel loop wraps when leftovers
				// goes 1→0 (post-decrement), so at most (leftovers-1) pixels
				// can be written on the current scanline before wrapping.
				// The pixel at position leftovers-1 triggers the wrap.
//				uchar* lp = (uchar*)((intptr_t)dst + lightOfs);
//				while (n > 0) {
//					if (leftovers <= 0) { lp -= shiftup; dst -= shiftup; leftovers = width; }
//					int bulk = leftovers - 1;  // pixels safe to memset before boundary
//					if (bulk < 0) bulk = 0;
//					int seg = (n < bulk) ? n : bulk;
//					if (seg > 0) {
//						memset(lp, lightFill, seg);
//						lp += seg; dst += seg; n -= seg; leftovers -= seg;
//					}
//					if (n > 0 && leftovers > 0) {
						// Boundary pixel: last pixel of current scanline (leftovers==1).
						// Decrement leftovers to 0 so the next iteration's leftovers<=0
						// check handles the wrap.
//						*lp = lightFill;
//						++lp; ++dst; --n;
//						--leftovers;  // 1→0: scanline fully consumed
//					}
//				}
//			} else {
			{
				while (n--) {
					if (leftovers-- < 1) {
						dst -= shiftup;
//						leftovers = width;// Original logic with off by 1 error
						// FIXED: pixel that triggered wrap already consumed 1 slot
						leftovers = width - 1;
					}
					// ORIGINAL (skipped WorkingSurface write during post-render):
					// if (!g_LightSurfaceOnlyPass) *dst = src[i];
					// Water Wyrm fix REVERTED: full re-blit breaks wall occlusion.
					// Post-render must only write LightSurface, not WorkingSurface.
					// (th4) Stage 2: the solid run shares one palette index for N
					// pixels; read it when the overlay (ultra + alpha) needs it
					// in the light-only pass -- skipped otherwise so other modes
					// keep their pre-Stage-2 cost.
					const uchar sv = (!g_LightSurfaceOnlyPass || alphaOverlay || tintExclude) ? src[i] : 0;
					if (!g_LightSurfaceOnlyPass) *dst = sv;
					// (th4) track wall occlusion: SET for arches, WOB_OBJECT for dungeon
					// objects, CLEAR for normal sprites
					if (!g_LightSurfaceOnlyPass && wobSurface) {
						wobSurface[(uchar*)dst - SoftSrcSurfacePtr] = isDrawingOpaqueWall ? WOB_WALL : (isDrawingOpaqueObject ? WOB_OBJECT : 0);
					}
					if (hasLight) {
						if (g_LightSurfaceOnlyPass) {
//							*(uchar*)((intptr_t)dst + lightOfs) = lightFill;
							// (th4) Stage 3: owner-pixel tint override to white,
							// only where the owner sprite is the visible topmost
							// surface — behind a wall OR a dungeon object the
							// visible surface is the occluder, which must keep
							// the winning light's tint; covered by a later-drawn
							// sprite (e.g. the player in front of a portal) the
							// visible surface is that sprite and *dst != sv, so it
							// keeps the winning light's tint too (see the
							// regular-run branch above for the full explanation).
							if (tintExclude) {
								const ptrdiff_t tintPix = (uchar*)dst - SoftSrcSurfacePtr;
								const bool occludedByScene = wobSurface != nullptr
									&& (wobSurface[tintPix] & (WOB_WALL | WOB_OBJECT)) != 0;
								if (!occludedByScene && *dst == sv)
									LightTintSurface[tintPix] = TINT_WHITE;
							}
							// (th4) Per-pixel decision: silhouette behind walls, 256-level ambient in open areas.
							// (th4) InfravisionMonster: skip ALL LightSurface writes — pre-fill handles brightness
							if (spriteType != SpriteType::InfravisionMonster) {
								uchar* lp = (uchar*)((intptr_t)dst + lightOfs);
								if (wobSurface) {
									ptrdiff_t pixOfs = (uchar*)dst - SoftSrcSurfacePtr;
									// (th4) Stage 2: in ultra + alpha, dungeon objects
									// occlude sprites exactly like walls (see the
									// regular-run branch above) — translucent composite,
									// not the classic show-through silhouette.
									if ((wobSurface[pixOfs] & occlMask)
										|| (alphaOverlay && (wobSurface[pixOfs] & WOB_OBJECT) != 0)) {
										if (alphaOverlay) {
											// (th4) Stage 2: translucent sprite over the
											// wall. Capture sprite RGB + overlay weight;
											// skip the silhouetteBright write so the wall
											// keeps ambient light under the sprite.
											// SpriteLayerSurface is uint* indexed by PIXEL
											// offset (same space as WorkingSurface).
											SpriteLayerSurface[pixOfs] = rgbLUT[sv];
											AlphaSurface[pixOfs] = overlayAlpha;
										} else if (!occludedSkip) {
											// (th4) InfravisionMonsterThief: leave ambient → no silhouette
											*lp = silhouetteBright;  // occluded by wall → enhanced silhouette
										}
									} else {
//										*lp = *(uchar*)((intptr_t)dst + baseLightOfs);  // open area → 256-level per-pixel ambient. This introduces boss monster uneven brightness when hugging a wall, and Water Wyrm neck cut off without silhouette issue.
										*lp = openBright;  // open/overwritten → uniform sprite brightness
									}
								} else {
									*lp = occludedSkip ? openBright : silhouetteBright;  // fallback: no occlusion buffer → uniform silhouette
								}
							}
						} else if (g_A8L8Lighting && PixelLight && g_InDungeonRender) {
							// Screen-space PPL: skip LightSurface write, pre-fill handles it
						} else if (g_SkipSpriteLightClear) {
							*(uchar*)((intptr_t)dst + lightOfs) = 0;
						} else {
							*(uchar*)((intptr_t)dst + lightOfs) = lightFill;
						}
					}
					++dst;
				}
			}
			++i;
		}
		else { // transparent — no pixel writes, no LightSurface clearing needed
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

// (th4) CL2 RLE walk shared by the outline pass (Rle2DrawOutline) and the A8L8
// ring-light pass (Rle2DrawLightOutline). Decodes a CL2 frame with the same
// wrap arithmetic and byte consumption as the body decoder Rle2Draw, including
// the cb20d1cd3 off-by-one wrap fix (leftovers = width - 1), so body/ring
// positions stay bit-for-bit identical. onWrap(d, row) is called when
// the cursor wraps to the next decoded row (d = the new row's first pixel);
// onPixel(d, color, row) is called for every opaque pixel of every run
// (transparent pixels are skipped). Change RLE decoding here and both passes
// follow it.
template<typename TOnWrap, typename TOnPixel>
static bool Cl2WalkOutline(uchar* dst, char* src, int size, int width, TOnWrap&& onWrap, TOnPixel&& onPixel)
{
	int const shiftup = WorkingWidth + width;
	uchar* d = dst;
	int row = 0;
	int leftovers = width;
	for (int i = 0; i < size;) {
		if ((size_t)d < (size_t)WorkingSurfaceDrawBegin) return false; // same runaway guard as before
		uchar blockSize = src[i++];
		if (blockSize > 190) {                 // regular: N pixels, one color byte each
			int n = 65 - (blockSize - 191);
			while (n--) {
				if (leftovers-- < 1) {
					d -= shiftup;
//					leftovers = width;// Original logic with off by 1 error (drifts +1 px per row)
					// FIXED (same as Rle2Draw): the pixel that triggered the wrap already
					// consumed 1 slot, so width-1 keeps every scanline at exactly `width` px.
					leftovers = width - 1;
					++row; onWrap(d, row);
				}
				uchar color = src[i++];
				onPixel(d, color, row);
				++d;
			}
		} else if (blockSize > 127) {          // solid: N pixels sharing one color byte
			int n = 63 - (blockSize - 128);
			uchar color = src[i++];
			while (n--) {
				if (leftovers-- < 1) {
					d -= shiftup;
//					leftovers = width;// Original logic with off by 1 error (drifts +1 px per row)
					// FIXED (same as Rle2Draw): the pixel that triggered the wrap already
					// consumed 1 slot, so width-1 keeps every scanline at exactly `width` px.
					leftovers = width - 1;
					++row; onWrap(d, row);
				}
				onPixel(d, color, row);
				++d;
			}
		} else {                               // transparent: skip N pixels
			while (blockSize) {
				if (blockSize >= leftovers) {
					blockSize -= leftovers;
					d -= shiftup - leftovers;
					// Transparent wraps consume no "fire pixel": `width` is correct
					// here (matches Rle2Draw's transparent branch).
					leftovers = width;
					++row; onWrap(d, row);
				} else {
					leftovers -= blockSize;
					d += blockSize;
					break;
				}
			}
		}
	}
	return true;
}

//----- (th4) -------------------------------------------------------------
void __fastcall Rle2DrawOutline(uchar* dst, char* src, int size, int width, uchar outlineColor)
{
	// Approach B (A8L8): clear LightSurface for outline neighbor pixels
	// so the outline renders at full brightness (same as RleDraw<R_OUTLINE>).
	const bool hasLight = g_A8L8Lighting && LightSurface && SoftSrcSurfacePtr &&
		(uintptr_t)dst >= (uintptr_t)SoftSrcSurfacePtr;
	const intptr_t lightOfs = hasLight ? (intptr_t)LightSurface - (intptr_t)SoftSrcSurfacePtr : 0;
	// Legacy A8L8 / non-A8L8 only: the screen-space PPL case is lifted by the
	// ring-light pass (Rle2DrawLightOutline) run after the body draw.
	const bool clearRingLight = hasLight && !g_SkipSpriteLightClear && !(g_A8L8Lighting && PixelLight && g_InDungeonRender);

	// Decode via the shared walk (single source of truth for CL2 layout).
	Cl2WalkOutline(dst, src, size, width,
		[](uchar*, int) {},
		[&](uchar* d, uchar color, int) {
			if (!color) return;
			d[1] = d[-1] = d[WorkingWidth] = d[-WorkingWidth] = outlineColor;
			if (clearRingLight) {
				*(uchar*)((intptr_t)(d + 1) + lightOfs) = 0;
				*(uchar*)((intptr_t)(d - 1) + lightOfs) = 0;
				*(uchar*)((intptr_t)(d + WorkingWidth) + lightOfs) = 0;
				*(uchar*)((intptr_t)(d - WorkingWidth) + lightOfs) = 0;
			}
		});
}

// (th4) A8L8 ring-light lift for CL2 sprites (monster hover outline) -- the CL2
// counterpart of RleDraw<R_LIGHT_OUTLINE> (CEL path, commit c5a66475). Runs AFTER
// the body draw: the outline COLOR was written first and the body has overwritten
// every interior outline-colored pixel, so a neighbor pixel that still equals
// outlineColor is the visible ring. A body mask decoded from the same RLE also
// excludes body pixels whose own palette index equals outlineColor, so the monster
// body brightness is never touched -- only the ring's LightSurface bytes are lifted
// (to kAltHighlightBright = full bright). Self-gates to the screen-space PPL case
// (g_A8L8Lighting && PixelLight && g_InDungeonRender): the legacy paths already
// clear the ring light inside Rle2DrawOutline.
void __fastcall Rle2DrawLightOutline(uchar* dst, char* src, int size, int width, uchar outlineColor)
{
	if (!(g_A8L8Lighting && PixelLight && g_InDungeonRender)) return;
	if (!LightSurface || !SoftSrcSurfacePtr) return;
	if ((uintptr_t)dst < (uintptr_t)SoftSrcSurfacePtr) return;

	const intptr_t lightOfs = (intptr_t)LightSurface - (intptr_t)SoftSrcSurfacePtr;
	// (th4) Stage 3: ring pixels must never be tinted (see the
	// RleDraw<R_LIGHT_OUTLINE> counterpart in engine.h). The lift below only
	// handles the LightSurface byte; the tint word is a separate channel that
	// still holds the world's winning light color.
	const bool hasRingTint = g_ARGBLighting && g_LightTintActive && LightTintSurface;

	// Body mask: one byte per working-surface pixel, indexed DIRECTLY by pixel
	// offset (no row/col bookkeeping, no margins).  Marked with a generation
	// stamp, so it never needs clearing between calls (cleared once per 255
	// calls when the stamp wraps).  Sized like the other per-pixel surfaces.
	static uchar* s_BodyMask = nullptr;
	static size_t s_BodyMaskBytes = 0;
	static uchar  s_BodyMaskGen = 0;
	const size_t need = (size_t)(MaxWorkingHeight + 256) * (size_t)MaxWorkingWidth;
	if (need > s_BodyMaskBytes) {
		free(s_BodyMask);
		s_BodyMask = (uchar*)calloc(need, 1);
		if (!s_BodyMask) { s_BodyMaskBytes = 0; s_BodyMaskGen = 0; return; }
		s_BodyMaskBytes = need;
		s_BodyMaskGen = 1;
	} else {
		s_BodyMaskGen = (uchar)(s_BodyMaskGen + 1);
		if (s_BodyMaskGen == 0) { memset(s_BodyMask, 0, need); s_BodyMaskGen = 1; }
	}
	const uchar gen = s_BodyMaskGen;
	uchar* const mask = s_BodyMask;

	// Pass 1: stamp body pixels (nonzero sprite pixels only -- the outline pass
	// writes the ring around exactly those).
	Cl2WalkOutline(dst, src, size, width,
		[](uchar*, int) {},
		[&](uchar* d, uchar color, int) {
			if (color) mask[d - SoftSrcSurfacePtr] = gen;
		});

	// Pass 2: for every opaque pixel, lift the light of the 4 neighbors that are
	// NOT body pixels of this sprite and still hold the outline color (the
	// visible ring).  Both walks stop at the same cursor, so a top-border clip
	// simply means fewer rows processed -- never a wrong one.
	auto liftRingPixel = [&](uchar* pix) {
		if (*pix == outlineColor) {
			uchar* lp = (uchar*)((intptr_t)pix + lightOfs);
			if (*lp > kAltHighlightBright) *lp = kAltHighlightBright;
			if (hasRingTint) LightTintSurface[pix - SoftSrcSurfacePtr] = TINT_WHITE;
		}
	};
	Cl2WalkOutline(dst, src, size, width,
		[](uchar*, int) {},
		[&](uchar* d, uchar color, int) {
			if (!color) return;
			if (mask[(d - 1) - SoftSrcSurfacePtr] != gen)              liftRingPixel(d - 1);            // left
			if (mask[(d + 1) - SoftSrcSurfacePtr] != gen)              liftRingPixel(d + 1);            // right
			if (mask[(d - WorkingWidth) - SoftSrcSurfacePtr] != gen)   liftRingPixel(d - WorkingWidth); // above
			if (mask[(d + WorkingWidth) - SoftSrcSurfacePtr] != gen)   liftRingPixel(d + WorkingWidth); // below
		});
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

		int frameBegin = *(ushort*)(from + offsetStart); // (rt) this sometimes crashes when drawing a boss, possibly because of an old save
		if( ! frameBegin ) return;
		int rleLen = header[frameNum + 1] - frameOfs; // next frame - this frame
		int frameEnd = offsetEnd == 8 ? 0 : *(ushort*)(from + offsetEnd); // 8 - item inv pic ?
		rleLen = frameEnd ? frameEnd - frameBegin : rleLen - frameBegin;
		from += frameBegin;
		uchar* to = WorkingSurface + YOffsetHashTable[y - 16 * offsetStart] + x;
		Rle2DrawOutline(to, from, rleLen, celWidth, color);
	}
}

// (th4) A8L8 ring-light pass for CL2 sprites (see Rle2DrawLightOutline). Same
// header/offset handling as CL2_DrawOutline so both decode the same RLE range.
// Call AFTER the body draw (CL2_Draw / DrawMonster).
void __fastcall CL2_DrawLightOutline(uchar color, int x, int y, char* cl2Buf, uint frameNum, int celWidth, int offsetStart, int offsetEnd)
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
		int frameEnd = offsetEnd == 8 ? 0 : *(ushort*)(from + offsetEnd); // 8 - item inv pic ?
		rleLen = frameEnd ? frameEnd - frameBegin : rleLen - frameBegin;
		from += frameBegin;
		uchar* to = WorkingSurface + YOffsetHashTable[y - 16 * offsetStart] + x;
		Rle2DrawLightOutline(to, from, rleLen, celWidth, color);
	}
}

// (rt) summon drawing may be tied to this
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
	int colsLeft; // ebx@1
	int bytesLeft; // ecx@1
	int spriteData; // eax@1
	int stepPixels; // edx@1
	uchar fillPixel; // dl@4
	//char vars0[ 4 ]; // [sp+Ch] [bp+0h]@4
	src = sprite;
	dst = surf;
	colsLeft = pitch;
	bytesLeft = size;
	CurPitch = pitch;
	if( size <= 0 ) return;  // BUG-1 guard: do-while would underflow bytesLeft to -1 on size==0
	if( pitch <= 0 ) return; // zero-pitch guard: colsLeft += 0 would never terminate in row-advance loops
	spriteData = 0;
	stepPixels = 0;
	// Approach B (A8L8): constant offset from WorkingSurface to LightSurface.
	// Skip per-pixel clearing when A8L8 is disabled (LightSurface is never sampled).
	const bool hasLight = g_A8L8Lighting && LightSurface && SoftSrcSurfacePtr &&
		(uintptr_t)dst >= (uintptr_t)SoftSrcSurfacePtr;
	const intptr_t lightOfs = hasLight ? (intptr_t)LightSurface - (intptr_t)SoftSrcSurfacePtr : 0;
	// (th4) Non-wall sprite pixels must clear WallOcclusionSurface so the A8L8 post-render
	// pass doesn't mistake them for "behind a wall". Hoisted out of the pixel loops below:
	// the test is loop-invariant, but both operands are globals the loops store through, so
	// the compiler cannot be relied on to lift it -- it would cost a reload plus a branch on
	// EVERY sprite pixel, including for players with EnrichedLight off.
	// Gated on g_A8L8Lighting explicitly rather than leaning on WallOcclusionSurface being
	// null outside A8L8 (dx.cpp only allocates it under g_A8L8Lighting): that is true today,
	// but it is an implicit coupling to another file, and every other A8L8 site here states
	// the dependency outright. Same lower-bound guard as hasLight, so the index cannot go
	// negative.
	uchar* const wobClear = ( g_A8L8Lighting && !g_LightSurfaceOnlyPass
	                       && WallOcclusionSurface && SoftSrcSurfacePtr
	                       && (uintptr_t)dst >= (uintptr_t)SoftSrcSurfacePtr )
	                       ? WallOcclusionSurface : nullptr;
	do{
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ return; }
		spriteData = *src++;
		--bytesLeft;
		if( (char) spriteData >= 0 ){
			do{
				if( spriteData <= colsLeft ){
					stepPixels = spriteData;
					dst += spriteData;
					spriteData = 0;
				}else{
					stepPixels = colsLeft;
					dst += colsLeft;
					spriteData -= colsLeft;
				}
				colsLeft -= stepPixels;
				if( colsLeft <= 0 ){
					colsLeft = CurPitch;
					dst = &dst[ -CurPitch - WorkingWidth ];
				}
			}while( spriteData > 0 );
		}else{
			spriteData = -spriteData;
			if( spriteData <= 65 ){
				bytesLeft -= spriteData;
				colsLeft -= spriteData;
				if( hasLight && !g_SkipSpriteLightClear && !(g_A8L8Lighting && PixelLight && g_InDungeonRender)){
					do{
						*dst = palette[ *(uchar*)src++ ]; // (rt) the palette index is unsigned
						*(uchar*)((intptr_t)dst + lightOfs) = 0;
						if (wobClear) wobClear[dst - SoftSrcSurfacePtr] = 0;	// (th4) see wobClear above
						--spriteData;
						++dst;
					}while( spriteData > 0 );
				}else{
					do{
						*dst = palette[ *(uchar*)src++ ]; // (rt) the palette index is unsigned
						if (wobClear) wobClear[dst - SoftSrcSurfacePtr] = 0;	// (th4) see wobClear above
						--spriteData;
						++dst;
					}while( spriteData > 0 );
				}
			}else{
				spriteData = spriteData - 65;
				--bytesLeft;
				colsLeft -= spriteData;
				fillPixel = palette[ *(uchar*)src++ ]; // (rt) the palette index is unsigned
				if( hasLight && !g_SkipSpriteLightClear && !(g_A8L8Lighting && PixelLight && g_InDungeonRender)){
					do{
						*dst = fillPixel;
						*(uchar*)((intptr_t)dst + lightOfs) = 0;
						if (wobClear) wobClear[dst - SoftSrcSurfacePtr] = 0;	// (th4) see wobClear above
						--spriteData;
						++dst;
					}while( spriteData > 0 );
				}else{
					do{
						*dst = fillPixel;
						if (wobClear) wobClear[dst - SoftSrcSurfacePtr] = 0;	// (th4) see wobClear above
						--spriteData;
						++dst;
					}while( spriteData > 0 );
				}
			}
			// BUG-2 fix: while loop handles opaque runs crossing multiple row boundaries
			while( colsLeft <= 0 ){
				colsLeft += CurPitch;
				dst = &dst[ -CurPitch - WorkingWidth ];
			}
		}
	}while( bytesLeft );
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
			// A8L8 + PixelLight: use linear LightSurface scaling instead of gamma-2
			// LightTable darkening, so monster brightness matches the floor's
			// per-pixel lighting. DarkLevel*17 maps 0..15 to 0..255, matching
			// CalculatePerPixelLight's uniform-cell scaling.
			if (g_A8L8Lighting && PixelLight) {
				Rle2Draw(to, from, rleLen, pitch, (uchar)(DarkLevel * 17 > 255 ? 255 : DarkLevel * 17));
			} else {
				Rle2DrawPal(to, from, rleLen, pitch, &LightTable[256 * DarkLevel]);
			}
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
				return;// (rt) the message is already queued, exit
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
		int currentLetter;  // bl@16  (th4) widened: CJK letterIDs reach 8434, a char caps at 127
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
		// (th4) Stage 2: the message box pixels are written via CEL/dither, not
		// through DrawBox/DrawLetter, so clear the overlay over the box region
		// explicitly — occluded-sprite RGB must not composite over the message.
		for( int j = 0; j < height + 6; ++j ){
			AlphaOverlayClearRow(&WorkingSurface[xPos - 3 + YOffsetHashTable[yPos - height - 3 + j]], width + 6);
		}
		// (th4) Stage 3 (2026-09-05): same for the colored-light tint. The corner/
		// edge CELs above clear their own pixels (Surface_DrawCEL's R_TINT_CLEAR
		// pass); this row loop covers the dither interior — the same region the
		// alpha clear above uses — so the message box never inherits the world's
		// winning light color.
		for( int j = 0; j < height + 6; ++j ){
			TintClearRow(&WorkingSurface[xPos - 3 + YOffsetHashTable[yPos - height - 3 + j]], width + 6);
		}
		// (th4) Same as the speech panel. xPos/yPos here are working-surface absolute, so the
		// borders come off to reach the screen space the overlay works in.
		ItemOverlay_QueueDimRect( xPos - (int)Screen_LeftBorder,
		                          yPos - height - (int)Screen_TopBorder,
		                          width + 6, height + 6, 0.50f );
		ItemOverlay_SnapshotDimRect();
		strcpy(InfoPanelBuffer, LocalizedOnScreenMessage(messageIndex, OnScreenMessages[messageIndex]));
		offset = YOffsetHashTable[yPos - (height >> 1)] + xPos;
		length = strlen(InfoPanelBuffer);
		for( size_t i = 0; i < length; ){ // (th4) CHI: step by character, not byte
			int lid, w;
			i += ChiNextSmall( (const uchar*)InfoPanelBuffer + i, &lid, &w );
			stringWidth += w + 1;
		}
		if( stringWidth < (width + 16) ){
			offset += ((width + 16) - stringWidth) >> 1;
		}
		for( size_t i = 0; i < length; ){ // (th4) CHI: step by character, not byte
			int w;
			i += ChiNextSmall( (const uchar*)InfoPanelBuffer + i, &currentLetter, &w );
			if( currentLetter ){
				DrawLetter(offset, currentLetter, 3);
			}
			offset += w + 1;
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