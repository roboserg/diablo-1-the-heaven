# Fixed, not released yet

What was fixed since v0.3.0. When the next version ships, move the table into `CHANGELOG.md` and the release notes, then empty it. What is still left to do is in [OPEN_BUGS.md](OPEN_BUGS.md); the numbers below are the ones it uses. Bugs that are confirmed but left as they are, are in OPEN_BUGS.md; they belong in the release notes as known issues.

`scripts\autotest.ps1` plays the game unattended and checks the fixes (see `docs\BUILD.md`).

| Fix | Commit | Was |
|---|---|---|
| Without `TH4data.mor` the game stops at the start and names it, and a file missing from the archives gives an error that names that file and both data files. Both used to blame DIABDAT.MPQ. The autotest runs without `TH4data.mor` and asks for a file no archive has. Blueplane and two guests reported it on ModDB. | dda655e | Bug 13 |
| Infernal Bargain gives its perk points with Skilled, Lord of the Rings and Grim Deal. They got its lower regeneration and stats but none of its points. The autotest takes each trait at rank 1 and the top rank. A guest on ModDB reported it with an Archer. | 5182bbb | Bug 14 |

## Details of the v0.3.0 fixes

What v0.3.0 fixed is in [CHANGELOG.md](CHANGELOG.md). The sections below explain the bigger fixes. Reports that turned out to be by design are at the end, under [Not bugs](#not-bugs).

## Bug 1 and kishak95's stash fix

kishak95 found the cause of bug 1 and posted a fix; v0.3.0 fixes it another way.

Rageweaver also plays a further-modded private build based on 0.2.0 that includes the stash fix ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490370898161569962)).

**What kishak95's stash fix changes,** compared with v0.2.0 (the same code is still in v0.3.0):
- **`VisualStash.cpp`:** calls `UpdatePlayerFile()` at the start of `StashPanel_OpenShared()`, so the personal stash is saved before the shared stash replaces it in memory. This is the fix for bug 1.
- **`CharSlots.cpp`:** the shared stash file format goes from version 2 to 3 and stores its own save version, so its items are read with the format they were saved in rather than the format of the character opening them. Version 1 and 2 files still load.

**Why v0.3.0 fixes bug 1 differently.** kishak95 found the cause, and his fix stops the loss and duplication as reported. Two problems are left:
- **Saving on open:** the shared stash file is still written on every move, while the character is saved at other times. Load a save after moving an item and it's in both files or in neither (Marcototech's held-item report under bug 2). In Nightmare and Speedrun the save on open does nothing, because those modes never save the character.
- **The format change isn't needed:** the item layout is the same in v0.2.0 and v0.3.0, and each item already records the save version it was written with, so the shared stash reads the same for every character. A version 3 file also can't be read by v0.2.0, which players still use.

UbergamR says he "could dupe with the shared stash" in The Heaven ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538143668421136476)), without saying how. Bug 1's duplication is the likely way.

## Bug 3: what was fixed

kishak95, 4 Apr: affixes on an item kept disappearing and coming back, even on an unidentified item that was never in the shared stash ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490072463659503820), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490074289679106128)). The cause was The Heaven's own rare roll, skipped when a magic item is rebuilt. Morphing between classes is base TH4 and stays: see [OPEN_BUGS.md](OPEN_BUGS.md#3-magic-items-morph-between-characters).

## Bug 4: what was fixed

Rageweaver and Gorvex, 5 Apr: -stun threshold and +damage from enemies still showed on uniques ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490109613838958715), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490109862837879036); Marcototech's screenshot, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490369857357615296)). The filter `IsHarmfulWhenNegative()` only hid *negative* values of the effects on its list: +damage from enemies is harmful when positive, and stun threshold, absorption and a few others weren't on the list. `IsHiddenUniquePenalty()` in `src/Item.cpp` now decides for the hero's stats and the three tooltips together. Before the fix the autotest found 696 of 4878 uniques and set items that made the hero worse, and 842 penalty lines in their tooltips.

## Bug 5: what was fixed

Rageweaver, 4 Apr: a Spectral Elixir drunk at the cap raised the stat for about a second, then it dropped back, and capped stats never turned gold ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490066809288921090)). kishak95 guessed the cap is defined in more than one place ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490375963605860502)). It was in three: v0.0.4 (cccf003) doubled it where leveling up, elixirs and gold stats check it, but not in `LimitPlayerParams()`, which trims stats above the cap every tick, so the game cut stats back to the old cap. All three now use `StatSumCap()` in `src/theheaven.cpp`. At level 1 the autotest drank elixirs up to 200 and saw the stats drop to 100 two seconds later; now they stay at 200, drawn gold.

Rageweaver's "max stat 1500 is not doubled" ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490363103660216492)) is each stat's own maximum for the class, which v0.0.4 never meant to double: it doubles the per-level sum only.

## Shared stash: StArchon's cases

StArchon (godroll) listed how TH4 rebuilds an item for the character who loads it ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557685739481866274), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557685749065715824)). The morph lock rebuilds every shared stash item for the character who opens the stash and locks it if anything differs, so it catches all of them. The autotest checks it ("other" phase, then "owner"):
- **Sockets:** white monster drops roll sockets only for a character who can grind. 3 of 3 socketed white items are locked for a Classic Mage, so a character who can't socket never writes them back with other sockets.
- **Shop items** (Griswold's basic and premium, Wirt, Pepin) are rebuilt with the loading character's shop rules: 6 of 9 change for the Mage, all 6 locked.
- **Night Kin and Clean Hands** turn equipment into gold: opened by a Night Kin, 3 of 3 such items are locked, and the Warrior gets them back unchanged.
- **Books** whose spell depends on the class were already in the test. Terminator and Survival use the same spell rules.

The crash case stays: see [OPEN_BUGS.md](OPEN_BUGS.md#11-crash-between-the-two-shared-stash-writes).

## Not bugs

Player reports that turned out to be by design or base TH4 (checked 5 Nov 2026). Bugs that are confirmed but left as they are, are in [OPEN_BUGS.md](OPEN_BUGS.md#confirmed-bugs).

### Trade-offs on 94 uniques (bug 4)
These uniques move life to mana or mana to life, or lose resistances or regeneration for a big bonus. Grace of Death, for example, takes 40-45 less damage from enemies but loses resistances and regeneration. Removing the downside would make these items far stronger than meant, and players in the Anti-Grind channel complain that removing it breaks them: Darkbone115's item that moves 40% of life to mana becomes useless ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1566903035379064982)). Plain penalties, like Ravenous Hatred's -26% life that Zerathyl found in Anti-Grind ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1520186604373676043)), are gone in The Heaven.

### Multiplayer (bug 6)
- **Time speed:** in a LAN game on two PCs, time speed (the Acceleration game changer) can't be changed (Marcototech, 21 Jun, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1518059339343069234)). It applies only in single player (`MaxCountOfPlayersInGame == 1` in `Main.cpp`); syncing the speed between PCs would be a big change.
- **Dropped items aren't shared:** in the same test other players couldn't see dropped items. Base TH4 turns item sharing off in multiplayer (`NetItem` is `false` in `src/msg.cpp` and nothing sets it), so each player sees only their own drops.

### Others
- **Yuki's unofficial 0.2.0 build** ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508838222828929086)): a third-party binary, not tried. Bugs 4 and 5, which it fixes, are fixed from source here.
