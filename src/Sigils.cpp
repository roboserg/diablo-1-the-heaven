#include "stdafx.h"

// Base item indexes for sigils (you said 2504-2508)
const int SIGIL_BASE_MIN = 2504;
const int SIGIL_BASE_MAX = 2508;

void TryToDropSigil(int row, int col)
{
    // 1:800 sigil drop chance default
    int sdc = 800;
    if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE)) {
        sdc = 533;
    }
    else if (is(GameMode, GM_HARDCORE, GM_SURVIVAL, GM_EASY)) {
        sdc = 666;
    }
    else if (GameMode == GM_OUTBREAK) {
        sdc = 333; 
    }
    else if (GameMode == GM_DARK_CAMPAIGN) {
        sdc = 200;
    }

    if (RNG(sdc) == 11)
    {
        int sigilIndex = BI_2504_SIGIL_FIRST + (GetRndSeed() % 5);
        int qlvl = ItemLevelByDungeonLevel();

        // Use the same pattern as CreatePepinRing
        int itemIndex = -1;
        if (GetItemSpace(row, col, itemIndex))
        {
            // Parameters: itemIndex, baseItemIndex, seed, qlvl*2, chance, isGoodMagic, allowDup, delta, identified, playerMask, magicFind, rare, genVersion, uniqIndex, difficulty
            SetupAllItems(itemIndex, sigilIndex, GetRndSeed(), qlvl * 2, 1, 1, 0, 0, false, 0, 0, 0, -1, 0, Difficulty);

            if (NetItem) NetSendCmdDItem(FALSE, itemIndex);
        }
    }
}