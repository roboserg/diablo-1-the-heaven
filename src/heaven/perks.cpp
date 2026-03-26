#include "stdafx.h"

// Resets all perks for a player, refunding all spent perk points.
// Args:
//   playerIndex - The index of the player in the Players array.
// Returns:
//   The number of perk points refunded.
int ResetPerks(int playerIndex) {
    Player& player = Players[playerIndex];
    int refundedPoints = 0;
    for (int i = 0; i < PERKS_MAX_800; ++i) {
        refundedPoints += player.perk[i];
        player.perk[i] = 0;
    }
    RecalcPlayer(playerIndex, 1);
    return refundedPoints;
}
