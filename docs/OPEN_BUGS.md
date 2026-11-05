# Open Bugs

Bugs that players reported and that are still left to fix. Checked against the `th4-v0.774-base` branch (commit 6907b52) on 5 Nov 2026. Feature requests and player feedback are in [COMMUNITY_FEEDBACK.md](COMMUNITY_FEEDBACK.md).

When you fix a bug, move it to [Fixed, not released yet](#fixed-not-released-yet) at the bottom, and add it to `CHANGELOG.md` when it ships.

Every report links to its source on **The Hell 4 mod** Discord server: *channel* means `#the-heaven`, *thread* means the archived "The Heaven mod" thread in `#modding`, and *antigrind* and *godroll* are the channels of those two sub-mods. You must be signed in and a member to open them. To check a report against the original messages, see [Sources and how to re-check them](COMMUNITY_FEEDBACK.md#sources-and-how-to-re-check-them).

## Ready-made fixes from players

| Fix | For | Who, when | Status |
|---|---|---|---|
| **Stash fix:** modified `VisualStash.cpp` and `CharSlots.cpp` | Bug 1, [fixed](#fixed-not-released-yet) another way | kishak95, 3 Apr ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489677986855915620)) | Downloaded to `F:\git\kishak-stash-fix\` (outside the repo, this PC only). Tested by kishak95 ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489712222228844765)) and Rageweaver ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489715218815778856)). Not applied: see below why. |
| **Stat cap fix:** `TheHeaven-nightly.zip` (4.49 MB) | [Bug 5](#5-the-doubled-stat-cap-doesnt-fully-work) | kishak95, 5 Apr ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490355431598592040)) | Compiled build only, no source. Untested by others. |
| **Unofficial 0.2.0 build:** `TH4Heaven - 0.2.0 stats uniques fixed.zip` (4.62 MB), fixes by "Yuki" | Possibly [bug 4](#4-some-unique-item-penalties-remain) | Marcototech, 26 May ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508838222828929086)) | Not checked yet. See [To check](#to-check-yukis-unofficial-020-build). |

Rageweaver also plays a further-modded private build based on 0.2.0 that includes the stash fix ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490370898161569962)).

**What kishak95's stash fix changes,** compared with v0.2.0 (the same code is still on `th4-v0.774-base`):
- **`VisualStash.cpp`:** calls `UpdatePlayerFile()` at the start of `StashPanel_OpenShared()`, so the personal stash is saved before the shared stash replaces it in memory. This is the fix for bug 1.
- **`CharSlots.cpp`:** the shared stash file format goes from version 2 to 3 and stores its own save version, so its items are read with the format they were saved in rather than the format of the character opening them. Version 1 and 2 files still load.

**Why `th4-v0.774-base` fixes bug 1 differently.** kishak95 found the cause, and his fix stops the loss and duplication as reported. Two problems are left:
- **Saving on open:** the shared stash file is still written on every move, while the character is saved at other times. Load a save after moving an item and it's in both files or in neither (Marcototech's held-item report under bug 2). In Nightmare and Speedrun the save on open does nothing, because those modes never save the character.
- **The format change isn't needed:** the item layout is the same in v0.2.0 and on `th4-v0.774-base`, and each item already records the save version it was written with, so the shared stash reads the same for every character. A version 3 file also can't be read by v0.2.0, which players still use.

## Bugs, most important first

Bugs 1 (Griswold's stash loses and duplicates items) and 2 (items lost in a window or on the cursor during a save or load) are fixed on `th4-v0.774-base`: see [Fixed, not released yet](#fixed-not-released-yet). The other numbers stay as they were, so links to them keep working.

### 3. Magic items morph
The game keeps only a magic item's seed and a few fields, and rebuilds the rest after every load and, in the inventory, every 5 seconds. Anything that makes a rebuild come out different morphs the item.

Fixed on `th4-v0.774-base` (see [Fixed, not released yet](#fixed-not-released-yet)): items flipping between two versions every 5 seconds, kishak95's report below.

Still open:
- **Across characters (base TH4):** staff and book spells depend on the character's class, and Classic mode uses its own affix table. `th4-v0.774-base` locks shared stash items that would morph for the current character. The autotest checks that the lock holds while the stash stays open through the 5-second checks.
- **Magic items change once after they drop (The Heaven).** In the autotest, 9 in 10 magic items change at the first 5-second check after they drop. From then on they stay the same. Players don't see it, because drops aren't identified yet. `build\autotest\autotest-rebuilds.csv` lists 1798 magic items as dropped and after 4 checks.
  - **Cause:** The Heaven's independent rare roll (dd8817e, v0.0.3) runs when an item drops, but is skipped when a magic item is rebuilt (`SetupAllItems` in `src/Item.cpp`), so every random number after it comes out different.
  - **Fixing it:** only new drops can be fixed, since existing items must keep the version players have. That needs a marker on each item (the unused `charFlag` byte) and in the multiplayer item messages. Not worth it unless players notice.

Reports:
- kishak95, 4 Apr: affixes on an item keep disappearing and coming back, even on an unidentified item that was never in the shared stash ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490072463659503820), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490074289679106128)). Fixed: the flip.
- Rageweaver, 25 May: a magic ring changed after switching characters, even though both were Dimachaerus and could use it ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508581640606322860)). Both were the same class, so it may have been the flip.
- Rageweaver, 3 Apr: blue items change while in the shared stash and change back once taken out ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489618645180219423)). May also be the flip.
- Marcototech (Discord) and Gron21 (issue #3) can't reproduce it between Paladin, Archer and Mage. Marcototech suspects it only happens between two characters of the same class.
- Mordor (TH4 author), 26 May: this is one of the reasons loot must not be shared ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508761056175063112)). Masterteo89 suggests sharing only items every class can use, like Giant Knuckle ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508834029565251675)).

**Gorvex's explanation** (17 Sep, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1550231476996538450)) is partly right:
- Magic items are regenerated from their seed when loaded. True.
- The affix pool differs per class/subclass, and disallowed slots get refilled. Only for spells: the staff spell list skips spells the class can't use, and books pick from a list filtered by class. Affix selection has no class filter.
- The seed is updated on the fly to fix invalid rolls. Only when an item drops; a rebuild never changes the seed.

### 4. Some unique-item penalties remain
The changelog says harmful penalties on uniques and sets are removed, but some still show up:
- **-stun threshold and +damage from enemies (DFE)** (Rageweaver and Gorvex, 5 Apr, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490109613838958715), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490109862837879036); confirmed with Marcototech's screenshot, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490369857357615296)).
  - **Cause:** the filter `IsHarmfulWhenNegative()` (`src/Item.cpp:3406`, used in `src/Item.cpp` and `src/Tooltip.cpp`) only hides *negative* values of the effects on its list. The stun threshold effect isn't on the list. +DFE is harmful when *positive*, so a negative-only check can't catch it.
- **-light radius was removed, but players want it kept,** because it helps archers and mages avoid pulling too many monsters (Darkbone115, 30 Mar, with screenshots, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488064296993820692), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488295169727008809); again on 15 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537953948856614943)). You replied "Will look at the bug tomorrow" ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488316119994466315)). It's removed on purpose in code, so this is a design decision as much as a bug.

### 5. The doubled stat cap doesn't fully work
The changelog (v0.0.4) says the per-level stat cap is doubled. Not re-checked in code yet.
- Rageweaver, 4 Apr: drinking a Spectral Elixir at the cap raises the stat for about a second, then it drops back. Capped stats don't turn gold either ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490066809288921090)).
- kishak95: only the 1500 maximum was edited, not the per-level cap, and the cap is defined in more than one place ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490353817768693770), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490375963605860502)).
- Rageweaver tested: the 1500 maximum isn't doubled either ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490363103660216492)).

### 6. Multiplayer
- **Acceleration doesn't work:** in a LAN game on two PCs, time speed (the Acceleration game changer) can't be changed (Marcototech, 21 Jun, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1518059339343069234)). In the same test other players couldn't see dropped items, which is probably base TH4's separate loot per player.
- **Single-player items reach multiplayer.** The changelog says the shared stash is for single-player characters, but Marcototech sees SP items in the MP stash (19 Jun, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1517491398708957364)), with 5 SP characters and 1 MP character all using the same stash (14 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537852169124843660)). He counts it as an advantage over Anti-Grind. Decide whether to keep or block it.

5x XP in multiplayer is a feature request; see [COMMUNITY_FEEDBACK.md](COMMUNITY_FEEDBACK.md#most-asked).

### 7. Sisyphean Task silently hides Swift Learner
A character with the Sisyphean Task trait can't pick the Swift Learner perk, and nothing in the game says why (UbergamR, 12 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1536867137627365416); DarkAidan95 explained it, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1536870226858082446)). Say so in the trait or perk description.

### 8. Wrong or missing documentation
- **Wrong Diablo ending:** `docs/QUESTS.md:34` says killing Diablo on level 15 ends the game. In TH4 the ending only plays after Uber Diablo. Azzi pointed it out as an AI-written mistake ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490548888031985787)); Blackheart confirmed the Uber Diablo ending ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1526809479365263511)).
- **Shared stash hard to find:** Tharunes and Zgeg couldn't find it ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508581213928161373)). It's at Gillian, and the README doesn't say so.

### 9. Smaller items from GitHub issue #3
From [issue #3](https://github.com/roboserg/diablo-1-the-heaven/issues/3):
- The pause menu title is hard to read in some zones.
- The "Symptoms" tree looks unfinished ("work in progress?").

### 10. Unarmed damage drops with items that change base damage (base TH4)
From Gorvex's suggestion list ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1487940115207684226)): unarmed base damage loses its flat bonus when wearing items with innate +/- base damage, such as Plate Harness. A Shugoki loses about 37 damage on average. Reported against TH4, not specifically The Heaven.

## Unconfirmed

- **Griswold's stash, two reports that may be bug 1:** Marcototech, 25 May: "Stash from griswold: Caution is bugged" ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508488088580788375)). Blackheart, 18 Jul: anything put into Griswold's stash in Purgatory disappears after closing it ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1527881491592253521)). Neither was reproduced. Ask them to try again with the next build.
- **Black Death game changer:** Marcototech saw it in a game where he says he only picked Easy and the Acceleration game changers. Rageweaver says it's intended ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508841792731086868)).
- **Missing Book of Healing:** Rageweaver's went missing, probably before applying the stash fix ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489715218815778856)).

## To check: problems reported in other sub-mods' channels

These came up in `#antigrind` and `#godroll` (links marked *antigrind* and *godroll*). They aren't reports against The Heaven, but they may affect it.

- **Shared stash edge cases.** StArchon tested how TH4 rebuilds items for the character that loads them ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557685739481866274), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557685749065715824)). Check each case against the morph lock (af5cf77):
  - Night Kin and Clean Hands turn dropped gear into gold when they load it, and it stays gold once they save.
  - White items roll their sockets again on every load. A character that can't socket writes only the gem-holding sockets into the item, so 3 sockets with 1 gem comes back to its owner with 4.
  - Griswold's premium items and Wirt's items roll weaker affixes for a lower-level character.
  - Items bought from Pepin are re-picked from the loading character's shop pool.
  - Books switch spells for some subclasses (Terminator, Survival), and you learn the spell you see.
  - A crash between writing the shared stash file and the character file can duplicate or lose items.
  - UbergamR says he "could dupe with the shared stash" in The Heaven ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538143668421136476)), without saying how.
- **Perk reset refunds every perk.** `VisualTrade_ResetPerks()` in `src/VisualTrade.cpp` zeroes all perks for 1000 gold × character level. Godroll locks two kinds of perks ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1558229347318366229), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1558409752655036497)):
  - perks where a reset would open an exploit: Heavy Drinker, Swift Learner, Educated, Higher Education, Pick up the Pace and others
  - Raise the Bar, Thaumaturgy and Ostentation, because lowering an attribute limit cuts base stats above it without refunding them

  Check whether The Heaven's reset loses stats that way, and whether progression perks should be locked.
- **No-penalty uniques break items whose penalty is the point.** Both reports are about AG's No Negative game changer; check both in The Heaven together with [bug 4](#4-some-unique-item-penalties-remain).
  - Darkbone115 has an item meant to move 40% of life to mana, and it becomes useless ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1566903035379064982)).
  - Zerathyl found Ravenous Hatred keeps −26% HP even with penalties off ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1520186604373676043)).
- **Volume creeps up:** sound and music volume go up one bar on every restart (Rageweaver, on TH4 v0.774 with AG, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1567163934660755599), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1567264595834576957)). It's probably in The Heaven too.

## To check: Yuki's unofficial 0.2.0 build

Marcototech posted an unofficial build with fixes by someone named **Yuki**. Check what it changes and whether we need any of the fixes. (Yuki also runs the Antigrind submod thread in `#modding`.)

- **File:** `TH4Heaven - 0.2.0 stats uniques fixed.zip` (4.62 MB), attached to Marcototech's message in `#the-heaven`, 26 May 2026: https://discord.com/channels/1261720897975357460/1507669410330972260/1508838222828929086
- **Announced here** ("something fixed by Yuki"): [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508760621473071125). Right after the file, Marcototech posted a screenshot captioned "important for play it" ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508838742654189739)).
- **How to download:** open the message link while signed in to Discord and click the file. Link to the message, not the file: Discord's direct file links expire.
- The name suggests fixes to unique-item stats (possibly bug 4). It's probably a compiled build, not source, so the check is a diff against our v0.2.0 release.
- It's a third-party binary. Unzip it into its own empty folder outside the repo, and don't run the exe before checking it.

## Fixed, not released yet

On `th4-v0.774-base`. `scripts\autotest.ps1` plays the game unattended to check these (see `docs\BUILD.md`).

| Fix | Commit | Was |
|---|---|---|
| Pixel light works: the shipped `config.ini` lacked the options that turn it on | 7ff7213 | GitHub [issue #5](https://github.com/roboserg/diablo-1-the-heaven/issues/5) |
| Closing Cain's transmute window while holding an item returns the slot items to the inventory | a5b1365 | Part of bug 2 (issue #3) |
| F9/F12 quick save and load, and loading a save from the menu, close every item window first and return their items; quick save refuses while an item is on the cursor. The autotest checks loading from the menu with an item in Adria's window | 1c8fd75 | Part of bug 2 (issue #3) |
| Griswold's personal stash no longer loses or duplicates items around Gillian's shared stash: it stays in memory while the shared stash is open, instead of being reloaded from the last save | 8193865 | [Bug 1](#ready-made-fixes-from-players) (Rageweaver, kishak95, issue #3). kishak95 found the cause |
| The shared stash file is written only together with a character save, when the shared stash closes. Loading a save drops unsaved shared stash moves, so an item held on the cursor during a load is back in the stash, once | 8193865 | Part of bug 2 (Marcototech, 21 Jun) |
| The once-a-minute autosave (Auto Save game changer, multiplayer) waits while an item is held or an item window holds items. Quitting, and changing level in multiplayer, first return the window items to the inventory | 8193865 | Part of bug 2: the gem lost in Griswold's crafting window (issue #3) |
| Shared stash items that would morph for the current character are locked: drawn red, can't be taken out, saved back unchanged | af5cf77 | Part of bug 3 |
| Magic items no longer flip between two versions every 5 seconds, with affixes and "Not Identified" coming and going. A rebuild that would turn a magic item plain rebuilds it as it dropped. About 1 magic drop in 200 flipped, mostly spell staves | 8193865 | Part of bug 3 (kishak95, 4 Apr) |
