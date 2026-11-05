# Open Bugs

Bugs that players reported and that are still left to do, checked against the `th4-v0.774-base` branch. Feature requests and player feedback are in [COMMUNITY_FEEDBACK.md](COMMUNITY_FEEDBACK.md).

When you fix a bug, move it out of this file into [FIXED_BUGS.md](FIXED_BUGS.md), the branch's list of fixes for the next release.

Every report links to its source on **The Hell 4 mod** Discord server: *channel* means `#the-heaven`, *thread* means the archived "The Heaven mod" thread in `#modding`, and *antigrind* and *godroll* are the channels of those two sub-mods. You must be signed in and a member to open them. To check a report against the original messages, see [Sources and how to re-check them](COMMUNITY_FEEDBACK.md#sources-and-how-to-re-check-them).

## What to decide

Checked on 5 Nov 2026 after the night's fixes. Each row says whether the problem is real and what I suggest.

| Bug | Problem | Real? | Suggestion |
|---|---|---|---|
| — | **A perk reset refunds Here and Now,** so it can be taken again for another level, as often as you pay 1000 gold × level | Yes, in code: free perk points are the level's points minus the perks held | **Fix:** keep Here and Now through a reset, like the stat limit perks. Small |
| 4 | -light radius is removed from uniques, two players want it back | Yes, removed on purpose | **Your call.** Restoring it is one line in `IsHiddenUniquePenalty()` |
| 4 | Trade-offs kept on 94 uniques (life for mana, lose resistances for a big bonus) | By design | **Keep.** On Anti-Grind, players complain when these lose their downside ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1566903035379064982)) |
| 6 | Single-player items reach multiplayer through the shared stash | Yes, every character uses `th4shared.stv` | **Keep,** players asked for a shared stash in multiplayer (issue #3, Gorvex). Say "every character" in the changelog |
| 6 | Time speed can't be changed in multiplayer | Works as designed: time speed applies only when `MaxCountOfPlayersInGame == 1` (`Main.cpp`) | **Skip.** Syncing speed between PCs is a big change |
| 3 | Magic items change once at the first 5-second check after they drop | Yes, 9 in 10. Players don't see it: drops aren't identified yet | **Skip** |
| 3 | Items morph between characters (base TH4) | Yes | **Done enough:** the shared stash lock covers sharing |
| 9 | Pause menu title hard to read; the Symptoms tree looks unfinished | Not checked in the game. Symptoms is base TH4's Black Death tree | **Skip** unless someone sends a screenshot |
| — | Lock other progression perks against a reset, like Godroll | Design | **Skip,** unless the Here and Now fix should cover more |
| — | A crash between writing the shared stash file and the character file | A window of milliseconds | **Skip** |
| — | Griswold's stash reports (Marcototech, Blackheart) and Rageweaver's missing Book of Healing | Probably bug 1, fixed | **Close,** ask them to try the next build |
| — | Black Death in a game where Marcototech didn't pick it | No: nothing in the code turns it on by itself | **Close** |
| — | Yuki's unofficial 0.2.0 build ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508838222828929086)) | Bugs 4 and 5 are fixed from source; it's a third-party binary | **Skip** |

## Bugs, most important first

Bugs 1 (Griswold's stash loses and duplicates items), 2 (items lost in a window or on the cursor during a save or load), 5 (the doubled stat cap), 7 (Sisyphean Task hides Swift Learner), 8 (wrong or missing documentation) and 10 (unarmed damage) are fixed: see [FIXED_BUGS.md](FIXED_BUGS.md). The other numbers stay as they were, so links to them keep working.

### 3. Magic items morph
The game keeps only a magic item's seed and a few fields, and rebuilds the rest after every load and, in the inventory, every 5 seconds. Anything that makes a rebuild come out different morphs the item.

Items that flipped between two versions every 5 seconds are fixed (see [FIXED_BUGS.md](FIXED_BUGS.md)).

Still open:
- **Across characters (base TH4):** staff and book spells depend on the character's class, and Classic mode uses its own affix table. `th4-v0.774-base` locks shared stash items that would morph for the current character. The autotest checks that the lock holds while the stash stays open through the 5-second checks.
- **Magic items change once after they drop (The Heaven).** In the autotest, 9 in 10 magic items change at the first 5-second check after they drop. From then on they stay the same. Players don't see it, because drops aren't identified yet. `build\autotest\autotest-rebuilds.csv` lists 1798 magic items as dropped and after 4 checks.
  - **Cause:** The Heaven's independent rare roll (dd8817e, v0.0.3) runs when an item drops, but is skipped when a magic item is rebuilt (`SetupAllItems` in `src/Item.cpp`), so every random number after it comes out different.
  - **Fixing it:** only new drops can be fixed, since existing items must keep the version players have. That needs a marker on each item (the unused `charFlag` byte) and in the multiplayer item messages. Not worth it unless players notice.

Reports:
- Rageweaver, 25 May: a magic ring changed after switching characters, even though both were Dimachaerus and could use it ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508581640606322860)). Both were the same class, so it may have been the flip.
- Rageweaver, 3 Apr: blue items change while in the shared stash and change back once taken out ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489618645180219423)). May also be the flip.
- Marcototech (Discord) and Gron21 (issue #3) can't reproduce it between Paladin, Archer and Mage. Marcototech suspects it only happens between two characters of the same class.
- Mordor (TH4 author), 26 May: this is one of the reasons loot must not be shared ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508761056175063112)). Masterteo89 suggests sharing only items every class can use, like Giant Knuckle ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508834029565251675)).

**Gorvex's explanation** (17 Sep, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1550231476996538450)) is partly right:
- Magic items are regenerated from their seed when loaded. True.
- The affix pool differs per class/subclass, and disallowed slots get refilled. Only for spells: the staff spell list skips spells the class can't use, and books pick from a list filtered by class. Affix selection has no class filter.
- The seed is updated on the fly to fix invalid rolls. Only when an item drops; a rebuild never changes the seed.

### 4. Some unique-item penalties remain
The harmful penalties players reported are fixed (see [FIXED_BUGS.md](FIXED_BUGS.md)). Two decisions are left:
- **Trade-offs are kept on 94 uniques:** moving life to mana or mana to life, and losing resistances or regeneration for a big bonus. Grace of Death, for example, takes 40-45 less damage from enemies but loses resistances and regeneration. Removing the downside would make these items far stronger than meant, and players in the Anti-Grind channel complain that removing it breaks them: Darkbone115's item that moves 40% of life to mana becomes useless ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1566903035379064982)). Plain penalties, like Ravenous Hatred's -26% life that Zerathyl found in Anti-Grind ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1520186604373676043)), are gone in The Heaven. Keep them as they are, or remove these too.
- **-light radius was removed, but players want it kept,** because it helps archers and mages avoid pulling too many monsters (Darkbone115, 30 Mar, with screenshots, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488064296993820692), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488295169727008809); again on 15 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537953948856614943)). You replied "Will look at the bug tomorrow" ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488316119994466315)). It's removed on purpose in code, so this is a design decision as much as a bug.

### 6. Multiplayer
- **Acceleration doesn't work:** in a LAN game on two PCs, time speed (the Acceleration game changer) can't be changed (Marcototech, 21 Jun, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1518059339343069234)). In the same test other players couldn't see dropped items, which is probably base TH4's separate loot per player.
- **Single-player items reach multiplayer.** The changelog says the shared stash is for single-player characters, but Marcototech sees SP items in the MP stash (19 Jun, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1517491398708957364)), with 5 SP characters and 1 MP character all using the same stash (14 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537852169124843660)). He counts it as an advantage over Anti-Grind. Decide whether to keep or block it.

5x XP in multiplayer is a feature request; see [COMMUNITY_FEEDBACK.md](COMMUNITY_FEEDBACK.md#most-asked).

### 9. Smaller items from GitHub issue #3
From [issue #3](https://github.com/roboserg/diablo-1-the-heaven/issues/3):
- The pause menu title is hard to read in some zones.
- The "Symptoms" tree looks unfinished ("work in progress?").

## Unconfirmed

- **Griswold's stash, two reports that may be bug 1:** Marcototech, 25 May: "Stash from griswold: Caution is bugged" ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508488088580788375)). Blackheart, 18 Jul: anything put into Griswold's stash in Purgatory disappears after closing it ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1527881491592253521)). Neither was reproduced. Ask them to try again with the next build.
- **Black Death game changer:** Marcototech saw it in a game where he says he only picked Easy and the Acceleration game changers. Rageweaver says it's intended ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508841792731086868)).
- **Missing Book of Healing:** Rageweaver's went missing, probably before applying the stash fix ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489715218815778856)).

## To check: problems reported in other sub-mods' channels

These came up in `#antigrind` and `#godroll` (links marked *antigrind* and *godroll*). They aren't reports against The Heaven, but they may affect it.

- **Shared stash: a crash between two files.** StArchon ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557685749065715824)): a crash between writing the shared stash file and the character file can duplicate or lose items. The Heaven now writes the two together, when the shared stash closes or the character saves, one right after the other, so the window is a few milliseconds. Closing it fully needs both in one file or a write-ahead copy. His other shared stash cases are caught by the morph lock (see [FIXED_BUGS.md](FIXED_BUGS.md)).
- **Perk reset refunds every perk.** `VisualTrade_ResetPerks()` in `src/VisualTrade.cpp` resets all perks for 1000 gold × character level. Godroll locks perks where a reset would open an exploit: Heavy Drinker, Swift Learner, Educated, Higher Education, Pick up the Pace and others ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1558229347318366229), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1558409752655036497)). Decide whether The Heaven should too. **Here and Now is an exploit:** `SpendPerkPoint()` gives a full level when it's taken, and a reset gives its point back, so it can be taken again and again. The stat loss from resetting Raise the Bar, Thaumaturgy and Ostentation is fixed (see [FIXED_BUGS.md](FIXED_BUGS.md)).
