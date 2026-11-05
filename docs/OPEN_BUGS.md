# Open Bugs

What is left to fix: bugs confirmed in the code or by the autotest, then player reports that nobody could confirm yet. Feature requests are in [COMMUNITY_FEEDBACK.md](COMMUNITY_FEEDBACK.md).

When you fix a bug, move it into [FIXED_BUGS.md](FIXED_BUGS.md), the branch's list of fixes for the next release. Reports that turned out to be by design are there too, under [Not bugs](FIXED_BUGS.md#not-bugs). Give a new bug the next number, 15, so links to the old numbers keep working.

Reports from the comments on the [ModDB page](https://www.moddb.com/mods/diablo-1-the-heaven) say so; sign in as RobosergTV to see the comments that wait for approval. Every other report links to its source on **The Hell 4 mod** Discord server: *channel* means `#the-heaven`, *thread* means the archived "The Heaven mod" thread in `#modding`, and *antigrind* and *godroll* are the channels of those two sub-mods. You must be signed in and a member to open them. To check a report against the original messages, see [Sources and how to re-check them](COMMUNITY_FEEDBACK.md#sources-and-how-to-re-check-them).

## Confirmed bugs

Each was reproduced by the autotest (`scripts\autotest.ps1`, see `docs\BUILD.md`) or found in the code. Checked on 5 Nov 2026.

| # | Bug | From | Reported by | Confirmed by | Decision |
|---|---|---|---|---|---|
| 3 | [Magic items change once after they drop](#3-magic-items-change-once-after-they-drop) | The Heaven | Found by the autotest | Autotest: 9 in 10 magic drops change at the first 5-second item check | Skip: players don't see it |
| 3 | [Magic items morph between characters](#3-magic-items-morph-between-characters) | Base TH4 | Rageweaver, kishak95 and others | Autotest: items a Warrior stashed come out different for a Classic Mage | Skip: the shared stash locks them |
| 11 | [A crash between the two shared stash writes can duplicate or lose items](#11-crash-between-the-two-shared-stash-writes) | The Heaven | StArchon (godroll) | Code: the shared stash and the character are two files, written one after the other | Skip: a window of milliseconds |

## Player reports, not confirmed

Nobody reproduced these. Each needs an answer from the player.

| Report | Who | What was checked | Next |
|---|---|---|---|
| Items put into Griswold's stash disappear after closing it, since reaching Purgatory | Blackheart, 18 Jul ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1527881491592253521)) | Not reproduced. No stash code depends on the difficulty. In v0.2.0, the build he played, closing Gillian's shared stash reloaded Griswold's stash from the last save, losing what was put in since: bug 1, fixed in v0.3.0 | Ask him to try v0.3.0 |
| "Stash from griswold: Caution is bugged" | Marcototech, 25 May ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508488088580788375)) | One line in his list of the mod's features, so most likely a warning about bug 1, fixed | Close |
| His Book of Healing went missing | Rageweaver, 3 Apr ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489715218815778856)) | He thinks it went missing before kishak95's stash fix, so bug 1 | Close |
| Black Death and its Symptoms tree in a game where he says he only picked Easy, That's My Secret and Acceleration (the rest of bug 9; its pause menu title is fixed) | Marcototech, 26 May ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508856317425483958)); issue #3: "Symptoms tree? For what are they?" | Not reproduced. The Symptoms tree shows only with Black Death on. Easy never lists Black Death, a click toggles only a game changer the list shows, and going back to the mode screen clears all game changers | Ask for the save file or a screenshot of the game changers |
| Rare items with requirements above the hero's level drop on very low levels and are useless | A guest (ModDB, 15 Aug) | Not checked. Rare drops have their own chance since v0.0.3 (The Heaven) | Check which item level rare drops roll with |

## Details

### 3. Magic items change once after they drop
In the autotest, 9 in 10 magic items change at the first 5-second check after they drop. From then on they stay the same. Players don't see it, because drops aren't identified yet. `build\autotest\autotest-rebuilds.csv` lists 1798 magic items as dropped and after 4 checks.
- **Cause:** The Heaven's independent rare roll (dd8817e, v0.0.3) runs when an item drops, but is skipped when a magic item is rebuilt (`SetupAllItems` in `src/Item.cpp`), so every random number after it comes out different.
- **Fixing it:** only new drops can be fixed, since existing items must keep the version players have. That needs a marker on each item (the unused `charFlag` byte) and in the multiplayer item messages. Not worth it unless players notice.

### 3. Magic items morph between characters
The game keeps only a magic item's seed and a few fields, and rebuilds the rest after every load. Staff and book spells depend on the character's class, and Classic mode uses its own affix table, so an item can come out different for another character. The shared stash locks items that would morph for the character who opens it (af5cf77), which covers sharing. The autotest's "other" phase finds the items a Warrior stashed that come out different for a Classic Mage, and checks they are locked.

Reports:
- Rageweaver, 25 May: a magic ring changed after switching characters, even though both were Dimachaerus and could use it ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508581640606322860)). Both were the same class, so it may have been the flip.
- Rageweaver, 3 Apr: blue items change while in the shared stash and change back once taken out ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489618645180219423)). May also be the flip.
- Marcototech (Discord) and Gron21 (issue #3) can't reproduce it between Paladin, Archer and Mage. Marcototech suspects it only happens between two characters of the same class.
- Mordor (TH4 author), 26 May: this is one of the reasons loot must not be shared ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508761056175063112)). Masterteo89 suggests sharing only items every class can use, like Giant Knuckle ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508834029565251675)).

**Gorvex's explanation** (17 Sep, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1550231476996538450)) is partly right:
- Magic items are regenerated from their seed when loaded. True.
- The affix pool differs per class/subclass, and disallowed slots get refilled. Only for spells: the staff spell list skips spells the class can't use, and books pick from a list filtered by class. Affix selection has no class filter.
- The seed is updated on the fly to fix invalid rolls. Only when an item drops; a rebuild never changes the seed.

### 11. Crash between the two shared stash writes
StArchon ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557685749065715824)): a crash between writing the shared stash file and the character file can duplicate or lose items. The Heaven now writes the two together, when the shared stash closes or the character saves, one right after the other, so the window is a few milliseconds. Closing it fully needs both in one file or a write-ahead copy. His other shared stash cases are caught by the morph lock (see [StArchon's cases](FIXED_BUGS.md#shared-stash-starchons-cases)).

