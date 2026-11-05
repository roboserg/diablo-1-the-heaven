# Fixed, not released yet

What the `th4-v0.774-base` branch fixed since v0.2.0. This file is temporary: when the branch ships, move it into `CHANGELOG.md` and the release notes, then delete it. What is still left to do is in [OPEN_BUGS.md](OPEN_BUGS.md); the numbers below are the ones it uses. Reports that turned out to be by design are at the end, under [Not bugs](#not-bugs). Bugs that are confirmed but left as they are, are in OPEN_BUGS.md; they belong in the release notes as known issues.

`scripts\autotest.ps1` plays the game unattended and checks these fixes (see `docs\BUILD.md`).

| Fix | Commit | Was |
|---|---|---|
| Pixel light works: the shipped `config.ini` lacked the options that turn it on | 7ff7213 | GitHub [issue #5](https://github.com/roboserg/diablo-1-the-heaven/issues/5) |
| Closing Cain's transmute window while holding an item returns the slot items to the inventory | a5b1365 | Part of bug 2 (issue #3) |
| F9/F12 quick save and load, and loading a save from the menu, close every item window first and return their items; quick save refuses while an item is on the cursor. The autotest checks loading from the menu with an item in Adria's window | 1c8fd75 | Part of bug 2 (issue #3) |
| Griswold's personal stash no longer loses or duplicates items around Gillian's shared stash: it stays in memory while the shared stash is open, instead of being reloaded from the last save | 8193865 | [Bug 1](#bug-1-and-kishak95s-stash-fix) (Rageweaver, kishak95, issue #3). kishak95 found the cause |
| The shared stash file is written only together with a character save, when the shared stash closes. Loading a save drops unsaved shared stash moves, so an item held on the cursor during a load is back in the stash, once | 8193865 | Part of bug 2 (Marcototech, 21 Jun) |
| The once-a-minute autosave (Auto Save game changer, multiplayer) waits while an item is held or an item window holds items. Quitting, and changing level in multiplayer, first return the window items to the inventory | 8193865 | Part of bug 2: the gem lost in Griswold's crafting window (issue #3) |
| Shared stash items that would morph for the current character are locked: drawn red, can't be taken out, saved back unchanged. This also covers StArchon's cases: socketed white items, shop items, Night Kin and Clean Hands gold | af5cf77, test ec8a89c | Part of bug 3; [StArchon's cases](#shared-stash-starchons-cases) |
| Magic items no longer flip between two versions every 5 seconds, with affixes and "Not Identified" coming and going. A rebuild that would turn a magic item plain rebuilds it as it dropped. About 1 magic drop in 200 flipped, mostly spell staves | 8193865 | Part of bug 3 (kishak95, 4 Apr) |
| Harmful penalties are gone from unique and set items, in the hero's stats and in all three tooltips: +damage from enemies, -stun threshold, -melee and -ranged absorption, -elemental, spell and weapon %, and negative armour. Trade-offs stay (see [Not bugs](#not-bugs)) | 114e778 | Bug 4 (Rageweaver, Gorvex, 5 Apr; Marcototech's screenshot) |
| -light radius is back on unique and set items: v0.1.0 removed it with the other penalties, but archers and mages use it to pull fewer monsters. The autotest wears all 205 uniques that have it | 6690d5e | Bug 4 (Darkbone115, 30 Mar, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488064296993820692), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488295169727008809); 15 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537953948856614943)) |
| The doubled stat cap works: elixirs and shrines raise stats up to it and they stay there, and stats at the cap turn gold. Before, the game cut stats back to the old cap a moment later | 6668e25 | [Bug 5](#bug-5-what-was-fixed) (Rageweaver, kishak95, 4-5 Apr) |
| A perk reset no longer takes away base stats: it keeps Raise the Bar, Thaumaturgy and Ostentation when dropping them would cut stats above the lower limits, and says so. Before, those stats were cut and never came back | b6776a7 | Found in Godroll's perk locks ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1558409752655036497)) |
| A perk reset keeps Here and Now, and its tooltip says so. Here and Now grants a full level when it's taken, and the reset gave its point back, so a level could be bought again after every reset | 62dc666 | Found while checking Godroll's perk locks ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1558229347318366229)) |
| A perk reset also keeps Educated, Higher Education, Overwhelm and Heavy Drinker, and its tooltip lists them. The stat points they add at every level-up and Heavy Drinker's elixirs stayed after a reset, so a player could take them while leveling and swap them for other perks later. The autotest resets them in Nightmare and checks they are kept, still work without their requirements, and their requirements still reset | 71fba95 | Bug 12 (Godroll's perk locks, [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1558229347318366229), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1558409752655036497)) |
| Sound and music volume no longer go up a bar on every restart: the sliders had one step more than the volume, so the upper 12 bars saved a volume between two steps, which the next start rounded up (base TH4) | b918b17 | Rageweaver, on TH4 with AG ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1567163934660755599), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1567264595834576957)) |
| Unarmed damage keeps its base with items that change base damage: a level 75 Monk in Plate Harness (-5) does 32-70 instead of -5 (base TH4) | 25576be | Bug 10 (Gorvex's list, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1487940115207684226)) |
| Sisyphean Task's penalty text lists the perks it blocks: Swift Learner, Here and Now, Educated and nine synergies | a1bb25b | Bug 7 (UbergamR, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1536867137627365416)) |
| QUESTS.md no longer says killing Diablo ends the game (the ending plays after Uber Diablo, or Diablo in Classic mode), and the README says the shared stash is at Gillian | 9ffd14a | Bug 8 (Azzi, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490548888031985787); Tharunes and Zgeg, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508581213928161373)) |
| The changelog and README say the shared stash is shared by every character, single-player and multiplayer. They said single-player only, but multiplayer characters always used it too. Players asked for that, so it stays | 59931ff | Bug 6 (Marcototech, 19 Jun, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1517491398708957364); 14 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537852169124843660)) |
| The pause menu title "DIABLO 1 THE HEAVEN" is readable in the dungeon. It was drawn with the darkness the dungeon drawing left behind, so it was missing or nearly black on every dungeon level, while the menu items below it set full brightness first. The autotest opens the menu on levels 1, 5, 9 and 13 and checks the title reaches the screen at full brightness | 0ab5542 | Part of bug 9 ([issue #3](https://github.com/roboserg/diablo-1-the-heaven/issues/3): "On some zones it can't be seen") |

## Bug 1 and kishak95's stash fix

kishak95 found the cause of bug 1 and posted a fix; `th4-v0.774-base` fixes it another way.

Rageweaver also plays a further-modded private build based on 0.2.0 that includes the stash fix ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490370898161569962)).

**What kishak95's stash fix changes,** compared with v0.2.0 (the same code is still on `th4-v0.774-base`):
- **`VisualStash.cpp`:** calls `UpdatePlayerFile()` at the start of `StashPanel_OpenShared()`, so the personal stash is saved before the shared stash replaces it in memory. This is the fix for bug 1.
- **`CharSlots.cpp`:** the shared stash file format goes from version 2 to 3 and stores its own save version, so its items are read with the format they were saved in rather than the format of the character opening them. Version 1 and 2 files still load.

**Why `th4-v0.774-base` fixes bug 1 differently.** kishak95 found the cause, and his fix stops the loss and duplication as reported. Two problems are left:
- **Saving on open:** the shared stash file is still written on every move, while the character is saved at other times. Load a save after moving an item and it's in both files or in neither (Marcototech's held-item report under bug 2). In Nightmare and Speedrun the save on open does nothing, because those modes never save the character.
- **The format change isn't needed:** the item layout is the same in v0.2.0 and on `th4-v0.774-base`, and each item already records the save version it was written with, so the shared stash reads the same for every character. A version 3 file also can't be read by v0.2.0, which players still use.

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
