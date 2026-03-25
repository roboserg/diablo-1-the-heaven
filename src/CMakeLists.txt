cmake_minimum_required(VERSION 2.8)

project(thehell2_lib)

set (th_sources
    Affixes.cpp
    BaseItems.cpp
    BaseMonster.cpp
    BaseObjectsOnMap.cpp
    BaseQuests.cpp
    CastSprites.cpp
    CharSlots.cpp
    CodeSave.cpp
    Draw.cpp
    Engine.cpp
    Entry.cpp
    Error.cpp
    Exception.cpp
    File.cpp
    GenerateMap.cpp
    GenerateQuestFloor.cpp
    Halls.cpp
    Hell.cpp
    #HellDataNew.cpp
    Help.cpp
    IniFile.cpp
    Item.cpp
    Lights.cpp
    LoadingScreen.cpp
    Main.cpp
    MainMenu.cpp
    MenuEngine.cpp
    Missile.cpp
    Monster.cpp
    MpqSave.cpp
    Net.cpp
    NetSync.cpp
    Object.cpp
    Panel.cpp
    Path.cpp
    Player.cpp
    PlayerTrade.cpp
    PlayersSync.cpp
    PopupScreens.cpp
    Quests.cpp
    Room.cpp
    SHA.cpp
    SaveInfo.cpp
    Saving.cpp
    ScreenShotMaker.cpp
    SomeSpell.cpp
    SoundBuffer.cpp
    SoundCache.cpp
    Speeches.cpp
    SpellCasts.cpp
    Spells.cpp
    Storm.cpp
    Tests.cpp
    TownMap.cpp
    UberDiabloQuest.cpp
    UniqueItems.cpp
    UniqueMonsters.cpp
    UsePotionOrScroll.cpp
    Warp.cpp
    Windows.cpp
    automap.cpp
    dataseg.cpp
    dx.cpp
    dx_utility.cpp
    menu.cpp
    palette.cpp
    serialize.cpp
    storm_sblt.cpp
    storm_sbmp.cpp
    storm_scmd.cpp
    storm_scode.cpp
    storm_sdlg.cpp
    storm_sdraw.cpp
    storm_serr.cpp
    storm_sevt.cpp
    storm_sfile.cpp
    storm_sgdi.cpp
    storm_smem.cpp
    storm_smsg.cpp
    storm_snet.cpp
    storm_sreg.cpp
    storm_strans.cpp
    storm_svid.cpp
    structs_sounds.cpp
    structs_speech.cpp
    thehell2.cpp
    thread.cpp
    vorbis.cpp
)

if (MSVC)
    enable_language(ASM_MASM)
elseif(APPLE)
    enable_language(ASM-ATT)
endif()
set(th_asms
	HellData.asm
	StormData.asm
)

set(th_includes
	codec.h
	common.h
	enums.h
	ExeMap.h
	ScreenShotMaker.h
	structs.h
	thehell2.h
	vars.h
	vorbisenc.h
	vorbisfile.h
	funcs.h
	IniFile.h
	rad.h
	reflect.h
	stdafx.h
	resource.h
	hell.h
	smack.h
	storm.h
)

if (MSVC)
	ADD_MSVC_PRECOMPILED_HEADER(stdafx.h stdafx.cpp th_sources)
endif(MSVC)

add_library(thehell2_lib STATIC ${th_sources} ${th_asms} ${th_includes})
target_compile_definitions(thehell2_lib PUBLIC NEW_EXE)
