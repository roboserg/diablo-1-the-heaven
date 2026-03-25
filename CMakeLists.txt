# clion project. Not to build
cmake_minimum_required(VERSION 3.10)
project(th2)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(lib)
include_directories(lib/bzip2)
include_directories(lib/huffman)
include_directories(lib/misc)
include_directories(lib/pklib)
include_directories(lib/wave)
include_directories(lib/zfs/include)
include_directories(lib/zlib)
include_directories(src)
include_directories(tools)

add_executable(th2
        src/automap.cpp
        src/dataseg.cpp
        src/dx_utility.cpp
        src/IniFile.cpp
        src/menu.cpp
        src/palette.cpp
        src/serialize.cpp
        src/BaseItems.cpp
        src/BaseMonster.cpp
        src/UniqueItems.cpp
        src/Affixes.cpp
        src/UniqueMonsters.cpp
        src/Spells.cpp
        src/SpellCasts.cpp
        src/BaseObjectsOnMap.cpp
        src/thehell2.cpp
        src/structs_sounds.cpp
        src/structs_speech.cpp
        src/CastSprites.cpp
        src/Lights.cpp
        src/BaseQuests.cpp
        src/PlayerTrade.cpp
        src/PopupScreens.cpp
        src/Engine.cpp
        src/CodeSave.cpp
        src/SHA.cpp
        src/Speeches.cpp
        src/UberDiabloQuest.cpp
        src/GenerateQuestFloor.cpp
        src/Quests.cpp
        src/CharSlots.cpp
        src/Room.cpp
        src/Warp.cpp
        src/Saving.cpp
        src/Draw.cpp
        src/TownMap.cpp
        src/Panel.cpp
        src/Missile.cpp
        src/Error.cpp
        src/Main.cpp
        src/GenerateMap.cpp
        src/thread.cpp
        src/dx.cpp
        src/Halls.cpp
        src/Help.cpp
        src/Windows.cpp
        src/LoadingScreen.cpp
        src/Item.cpp
        src/MainMenu.cpp
        src/Monster.cpp
        src/MpqSave.cpp
        src/Net.cpp
        src/Object.cpp
        src/SaveInfo.cpp
        src/Path.cpp
        src/Player.cpp
        src/Entry.cpp
        src/SoundBuffer.cpp
        src/SomeSpell.cpp
        src/SoundCache.cpp
        src/File.cpp
        src/MenuEngine.cpp
        src/stdafx.cpp
        src/th2_proposals.cpp
        src/Config.cpp
        src/Hell.cpp
        src/th2data.cpp
        src/TH1dll.cpp
        src/AffixEffects.cpp
        src/AffixesRare.cpp
        src/Perks.cpp
        src/FlasksInfo.cpp
        src/TrapsInfo.cpp
        src/VisualStash.cpp
        src/VisualTrade.cpp
        src/Tooltip.cpp
        src/CharPanel.cpp
        src/InventoryPanel.cpp
        src/SpellPanel.cpp
        src/UniqueSets.cpp
        src/track.cpp
        src/msg.cpp
        src/sync.cpp
        src/multi.cpp
        src/tmsg.cpp
        src/PerksData.cpp
        src/Traits.cpp
        src/GameModes.cpp
        src/vorbis.cpp
        src/ScreenShotMaker.cpp
        src/Crafting.cpp
        )

# old content
# project(TheHell2)
# cmake_minimum_required(VERSION 2.8)

# set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# include("utils")

# include_directories(${CMAKE_SOURCE_DIR})

# RESET_DEPS()

# add_subdirectory("External_Deps")
# add_subdirectory("lib")
# add_subdirectory("src")
# add_subdirectory("Tests")


# BUILD_DEPS()
