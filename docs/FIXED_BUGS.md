# Fixed, not released yet

What the `th4-v0.774-base` branch fixed since v0.2.0. This file is temporary: when the branch ships, move it into `CHANGELOG.md` and the release notes, then delete it. What is still left to do is in [OPEN_BUGS.md](OPEN_BUGS.md); the numbers below are the ones it uses.

`scripts\autotest.ps1` plays the game unattended and checks these fixes (see `docs\BUILD.md`).

| Fix | Commit | Was |
|---|---|---|
| Pixel light works: the shipped `config.ini` lacked the options that turn it on | 7ff7213 | GitHub [issue #5](https://github.com/roboserg/diablo-1-the-heaven/issues/5) |
| Closing Cain's transmute window while holding an item returns the slot items to the inventory | a5b1365 | Part of bug 2 (issue #3) |
| F9/F12 quick save and load, and loading a save from the menu, close every item window first and return their items; quick save refuses while an item is on the cursor. The autotest checks loading from the menu with an item in Adria's window | 1c8fd75 | Part of bug 2 (issue #3) |
| Griswold's personal stash no longer loses or duplicates items around Gillian's shared stash: it stays in memory while the shared stash is open, instead of being reloaded from the last save | 8193865 | [Bug 1](#bug-1-and-kishak95s-stash-fix) (Rageweaver, kishak95, issue #3). kishak95 found the cause |
| The shared stash file is written only together with a character save, when the shared stash closes. Loading a save drops unsaved shared stash moves, so an item held on the cursor during a load is back in the stash, once | 8193865 | Part of bug 2 (Marcototech, 21 Jun) |
| The once-a-minute autosave (Auto Save game changer, multiplayer) waits while an item is held or an item window holds items. Quitting, and changing level in multiplayer, first return the window items to the inventory | 8193865 | Part of bug 2: the gem lost in Griswold's crafting window (issue #3) |
| Shared stash items that would morph for the current character are locked: drawn red, can't be taken out, saved back unchanged | af5cf77 | Part of bug 3 |
| Magic items no longer flip between two versions every 5 seconds, with affixes and "Not Identified" coming and going. A rebuild that would turn a magic item plain rebuilds it as it dropped. About 1 magic drop in 200 flipped, mostly spell staves | 8193865 | Part of bug 3 (kishak95, 4 Apr) |
| Harmful penalties are gone from unique and set items, in the hero's stats and in all three tooltips: +damage from enemies, -stun threshold, -melee and -ranged absorption, -elemental, spell and weapon %, and negative armour. Trade-offs stay (see bug 4 in OPEN_BUGS.md) | 114e778 | Bug 4 (Rageweaver, Gorvex, 5 Apr; Marcototech's screenshot) |
| The doubled stat cap works: elixirs and shrines raise stats up to it and they stay there, and stats at the cap turn gold. Before, the game cut stats back to the old cap a moment later | 6668e25 | [Bug 5](#bug-5-what-was-fixed) (Rageweaver, kishak95, 4-5 Apr) |

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

kishak95, 4 Apr: affixes on an item kept disappearing and coming back, even on an unidentified item that was never in the shared stash ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490072463659503820), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490074289679106128)). The cause was The Heaven's own rare roll, skipped when a magic item is rebuilt. Morphing between classes is base TH4 and stays open in OPEN_BUGS.md.

## Bug 4: what was fixed

Rageweaver and Gorvex, 5 Apr: -stun threshold and +damage from enemies still showed on uniques ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490109613838958715), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490109862837879036); Marcototech's screenshot, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490369857357615296)). The filter `IsHarmfulWhenNegative()` only hid *negative* values of the effects on its list: +damage from enemies is harmful when positive, and stun threshold, absorption and a few others weren't on the list. `IsHiddenUniquePenalty()` in `src/Item.cpp` now decides for the hero's stats and the three tooltips together. Before the fix the autotest found 696 of 4878 uniques and set items that made the hero worse, and 842 penalty lines in their tooltips.

## Bug 5: what was fixed

Rageweaver, 4 Apr: a Spectral Elixir drunk at the cap raised the stat for about a second, then it dropped back, and capped stats never turned gold ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490066809288921090)). kishak95 guessed the cap is defined in more than one place ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490375963605860502)). It was in three: v0.0.4 (cccf003) doubled it where leveling up, elixirs and gold stats check it, but not in `LimitPlayerParams()`, which trims stats above the cap every tick, so the game cut stats back to the old cap. All three now use `StatSumCap()` in `src/theheaven.cpp`. At level 1 the autotest drank elixirs up to 200 and saw the stats drop to 100 two seconds later; now they stay at 200, drawn gold.

Rageweaver's "max stat 1500 is not doubled" ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490363103660216492)) is each stat's own maximum for the class, which v0.0.4 never meant to double: it doubles the per-level sum only.
