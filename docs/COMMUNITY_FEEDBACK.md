# Community Feedback

Bug reports, fixes and feature requests from players, collected from three places:
- the `#the-heaven` channel on **The Hell 4 mod** Discord server (all 144 messages, 25 May – 5 Nov 2026)
- the archived **"The Heaven mod"** thread in that server's `#modding` channel (all 198 messages, 30 Mar – 6 Apr 2026)
- [GitHub issues](https://github.com/roboserg/diablo-1-the-heaven/issues)

None of these are fixed yet: the latest release is still v0.2.0 (30 March 2026).

Links marked *channel* go to `#the-heaven` and links marked *thread* go to the `#modding` thread. They open in Discord, and you must be signed in and a member of the server.

## Fixes players already made

Check these first: they may solve several of the bugs below.

| What | Who, when | Where | Status |
|---|---|---|---|
| **Stash fix:** modified `VisualStash.cpp` (47 KB, 1,260 lines) and `CharSlots.cpp` (33 KB, 907 lines) | kishak95, 3 Apr | [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489677986855915620) | Tested by kishak95 ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489712222228844765)) and Rageweaver ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489715218815778856)). Our copies have 1,255 and 898 lines, so the change is small. |
| **Stat cap fix:** `TheHeaven-nightly.zip` (4.49 MB), doubles the per-level stat cap | kishak95, 5 Apr | [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490355431598592040) | Untested by others. Compiled build. |
| **Unofficial 0.2.0 build:** `TH4Heaven - 0.2.0 stats uniques fixed.zip` (4.62 MB), fixes by "Yuki" | Marcototech, 26 May | [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508838222828929086) | See [Later](#later-check-the-unofficial-020-patch). |

Rageweaver also plays a further-modded private build based on 0.2.0 that includes the stash fix ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490370898161569962)).

## Bugs

### 1. Griswold's personal stash loses and duplicates items
The most-reported bug.
- **Loss:** put an item into the personal stash, open the shared stash, reopen the personal stash, and the item is gone. Seen with blacksmith oil, repair oil (Rageweaver, 3 Apr, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489618475386146867)) and potions (kishak95).
- **Duplication:** put an item into the personal stash, take it out, put it into the shared stash, and it shows up in the personal stash again (Rageweaver, 3 Apr, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489621389085245480)).
- Marcototech, 25 May: "Stash from griswold: Caution is bugged" ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508488088580788375))
- Blackheart, 18 Jul: anything put into Griswold's stash in Purgatory disappears after closing it ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1527881491592253521))
- GitHub [issue #3](https://github.com/roboserg/diablo-1-the-heaven/issues/3): rings and potions disappear. The reporter suggests removing the option.
- Rageweaver suggests putting both stashes at Gillian with a choice of which to open ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489621478646354080)).
- **A fix exists:** kishak95's stash fix (see [Fixes players already made](#fixes-players-already-made)).

### 2. Magic items change (morph)
- **In the shared stash:**
  - Rageweaver, 25 May: a magic ring changed after switching characters, even though both were Dimachaerus and could use it ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508581640606322860))
  - Rageweaver, 3 Apr: blue items change while in the shared stash and change back once taken out ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489618645180219423))
  - Mordor (TH4 author), 26 May: this is one of the reasons loot must not be shared ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508761056175063112))
  - Masterteo89, 26 May, suggests sharing only items every class can use, like Giant Knuckle ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508834029565251675))
  - Marcototech (Discord) and Gron21 (GitHub issue #3) can't reproduce it moving items between Paladin, Archer and Mage. Marcototech suspects it only happens between two characters of the same class.
- **In the inventory:** kishak95, 4 Apr: affixes on an item keep disappearing and coming back, even on an unidentified item that was never in the shared stash ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490072463659503820), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490074289679106128))
- **Cause** (Gorvex, 17 Sep, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1550231476996538450)):
  - Magic items are regenerated from their seed when loaded.
  - The affix pool differs per class/subclass. Affix slots that are not allowed for one subclass get filled by valid ones, which shifts the pool.
  - The seed is also updated on the fly to fix invalid rolls, so the same item rolls differently for another class.
  - The root cause is in base TH4; the shared stash just exposes it.

### 3. Items lost when a window is closed or a save is loaded with an item still in it
- Marcototech, 21 Jun: pick an item up from the stash, keep it on the cursor with the stash open, press ESC and load a game, and the item is gone forever ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1518073293121519646))
- GitHub issue #3:
  - A gem put into Griswold's crafting window disappears if the window is closed suddenly.
  - A ring transmuted at Cain disappears if it isn't moved to the inventory before closing the window.
  - Quick save, enchant at Adria, then quick load leaves two copies of the item (enchanted and not), and up to three if repeated. Loading from the menu makes the item disappear.

### 4. Some unique-item penalties were not removed
The changelog says negative penalties on uniques and sets are removed, but some remain:
- **-light radius was removed even though some players want it.** Darkbone115, 30 Mar, with screenshots ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488064296993820692), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488295169727008809)); again on 15 Aug ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537953948856614943)). -light radius helps archers and mages avoid pulling too many monsters. You replied "Will look at the bug tomorrow" ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488316119994466315)).
- **-stun threshold and +damage from enemies (DFE) remain.** Rageweaver and Gorvex, 5 Apr ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490109613838958715), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490109862837879036)); confirmed with Marcototech's screenshot ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490369857357615296)).

### 5. The doubled stat cap doesn't fully work
The changelog (v0.0.4) says the per-level stat cap is doubled.
- Rageweaver, 4 Apr: drinking a Spectral Elixir at the cap raises the stat for about a second, then it drops back. Capped stats don't turn gold either ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490066809288921090)).
- kishak95: only the 1500 maximum was edited, not the per-level cap, and the cap is defined in more than one place ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490353817768693770), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490375963605860502)).
- Rageweaver tested: the 1500 maximum isn't doubled either ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490363103660216492)).

### 6. Multiplayer gaps
- Marcototech, 21 Jun, two PCs on the same LAN: time speed (Acceleration game changer) can't be changed, and other players can't see dropped items ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1518059339343069234)). Separate loot per player is probably base TH4 behavior.
- Itadori, 14 Aug: the 5x XP option doesn't work in multiplayer and they'd like it to ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537844932948263002)). Their group plays TH4 with the Anti-Grind submod in multiplayer and The Heaven in single player.
- **Single-player items reach multiplayer.** The changelog says the shared stash is for single-player characters, but Marcototech sees SP items in the MP stash (19 Jun, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1517491398708957364)), with 5 SP characters and 1 MP character all using the same stash (14 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537852169124843660)). He counts it as an advantage over Anti-Grind. Decide whether to keep or block it.

### 7. Sisyphean Task silently hides Swift Learner
UbergamR, 12 Aug: a character with the Sisyphean Task trait can't pick the Swift Learner perk, and nothing in the game says why. Worth noting in the trait or perk description ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1536870226858082446)).

### 8. Documentation
- **The shared stash is hard to find.** Tharunes and Zgeg couldn't find it (25–26 May, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508581213928161373)). It's at Gillian; worth saying in the README.
- `docs/QUESTS.md` says killing Diablo on level 15 ends the game. In TH4 the ending only plays after Uber Diablo. Azzi pointed it out as an AI-written mistake ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490548888031985787)); Blackheart confirmed the Uber Diablo ending in `#the-heaven` on 15 Jul.

### 9. Other open GitHub issues
- [Issue #3](https://github.com/roboserg/diablo-1-the-heaven/issues/3): the pause menu title is hard to read in some zones, and the "Symptoms" tree looks unfinished ("work in progress?").
- [Issue #5](https://github.com/roboserg/diablo-1-the-heaven/issues/5): pixel light doesn't work.

## Feature requests

### Most asked
- **Rebase on the latest TH4.** Mordor has released the latest TH4 source in the downloads section of [patreon.com/thmod](https://www.patreon.com/thmod/about) (Slack Daddy, 14 Oct, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1559691391121104967); Nikodemsky, 4 Nov, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1567663591039959160)). It would bring:
  - pixel light (Rageweaver, 2 Apr, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489260002702262423)). Gorvex advised waiting for the newer source instead of porting it by hand.
  - the Wirt power-up and sigils (Ponder, 2 Jun, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1511302320934228008); Tharunes missed the sigils, 25 May, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508582352429777048))
- **Remove VISE in single player** (VISE is what players call not being able to save or town-portal during combat). Rageweaver calls it the most-asked feature after the shared stash ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489271217545412700)).
- **Multiplayer trading and more than 4 players** (Danycron, 30 Mar). Zgeg (26 May, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508654276639981649)) and UbergamR (12 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1536859083250999357)) also asked about trading. You released an experimental 8-player build on 31 Mar ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488550931464458383)); no one reported back on it.
- **5x XP in multiplayer** (Itadori, see bug 6).
- **Keep -light radius** on uniques (see bug 4).

### Other requests
- **Hardcore:** a scaling XP bonus like Survival or Ironman, and the shared stash as continuity between hardcore deaths (Danycron, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488091359964434503)).
- **More gear crafting** (Danycron, 6 Apr, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490550986094280834), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551174745559123)):
  - a gamble that adds affixes to blue items
  - "slamming" gear with gems bought in town: a chance at a good affix, a large chance to ruin the item (like worldstone shards in Project Diablo 2)
- **A gold cheat or more lenient cheat options.** Itadori says The Heaven is the best version they found, but their group plays another one because it allows a gold cheat ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1535406229961179347), [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1535406339411673179)).
- **Work together with the Anti-Grind team** (Itadori, 14 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537844824483434607)).
- **The Ironman speed-up button in every game mode**, and **bring back Infernal Bargain** (Azzi, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551299492806738), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551359446192238)).
- From GitHub issue #3:
  - the original town NPC greetings (Cain, Griswold, Adria)
  - a special drop sound for unique and set items
  - a shared stash in multiplayer
  - more players per multiplayer game
- **Unanswered question:** is the drop split 70% smart drop / 30% parent class, or 100%? (Rageweaver, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489280883126173896))

### Gorvex's suggestion list
Gorvex attached `Suggestion compilation.txt` (36 KB, 333 lines) to the thread ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1487940115207684226)). Most of it is TH4 balance; the parts closest to what The Heaven does:
- **Mechanics:**
  - shared stash in Easy and Normal, single player and multiplayer
  - one-way item trading in multiplayer for gold, paid by whoever starts the trade
  - costly, limited trait/perk/synergy resets
  - attribute points over the cap drop as elixirs instead of being wasted
  - solo multiplayer: half monster life, or double drops and XP
  - single-player tank mercenaries that do little damage
- **Monsters:** new champion modifiers (debuffs, elemental "enchanted", extra speed, "greedy" extra drops); a new Elemental monster family.
- **Items:**
  - pick the output gem type when transmuting; convert gems between types for gold
  - Relicts of Identify (and identify charges) only consumed on unidentified items
  - buffs for weak uniques (Gloom, Crazefire's Focusing Prism, Soulbinder Cord)
- **Traits, perks, synergies:**
  - about 45 traits he considers too weak or too punishing, with suggested numbers
  - perk rewards at clvl 25/50/75/100/125/150
  - Infernal Bargain back, to turn synergy points into perk points
- **Game changers:** Acceleration and That's My Secret in single-player Normal and multiplayer Easy/Normal; Double Damage, Effortless and Vaccinated in multiplayer Easy.
- **UI:**
  - a monster info box showing damage reductions and attack stats
  - more spell details in the spellbook
  - a much bigger character sheet
- **Shrines:** attribute shrines drop elixirs; the gold shrine drops one pile; shrine effects shared in multiplayer.
- **Misc:** shoot barrels and urns from range in Normal mode; restore Cain's and Griswold's original greetings.
- **Bug he lists:** unarmed base damage loses its flat bonus when wearing items with +/- base damage (e.g. Plate Harness).

## Later: check the unofficial 0.2.0 patch

Marcototech posted an unofficial build with fixes by someone named **Yuki**. Check what it changes and whether we need any of the fixes. (Yuki also runs the Antigrind submod thread in `#modding`.)

- **File:** `TH4Heaven - 0.2.0 stats uniques fixed.zip` (4.62 MB)
- **Where:** attached to Marcototech's message in `#the-heaven`, 26 May 2026, 16:24:
  https://discord.com/channels/1261720897975357460/1507669410330972260/1508838222828929086
- **Announced here** ("something fixed by Yuki"): [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508760621473071125)
- Right after it (16:26) Marcototech posted something captioned "important for play it", probably install notes or a screenshot: [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508838742654189739)
- **How to download:** open the message link while signed in to Discord and click the file to download it. Link to the message, not the file: Discord's direct file links expire.
- The name suggests fixes to unique-item stats (possibly bug 4). It is probably a compiled build (exe and data files), not source, so the check is a diff against our v0.2.0 release.
- It is a third-party binary. Unzip it into its own empty folder outside the repo, and don't run the exe before checking it.

## Feedback and perception

- **What players like:** less grind, the shared stash, the QoL features.
  - Rageweaver argues for keeping no-penalty uniques and the MF buff: they make finding a unique exciting, and players who want balance can play TH4 ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489304899408760882)).
  - Shelk, 11 Jul: "much more my speed" than the huge grind of classic TH4 ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1525290493322203216)). UbergamR, 11 Aug: just what they wanted ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1536852346460577903)).
  - Ponder, 3 Jun: "so many improvements", and wishes TH4 itself had these options ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1511775633129340928)).
- **Pacing:** with 5x XP, Itadori killed Diablo on dungeon level 16 at clvl 30 ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1535849448129953882)).
- **Misconceptions:**
  - gazoo, 16 Aug: the mod only exists to make Psion overpowered ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1538326216572936305)). You replied on 17 Sep that it has many other additions.
  - Ketsuen, 14 Aug: Anti-Grind already offers everything people ask for here, and the creator has never posted in the channel ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537900354551677019)).
- **What some don't:** Danycron liked some changes but finds there are too many sweeping ones.
- **Abandonment and AI:** from 6 Apr players noticed you had gone quiet. Azzi and Danycron guessed you were "bullied out" or ran out of steam, and that much of the code and docs were AI-written ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490548356609609818)). Gorvex said the same in `#the-heaven` on 18 Jul ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1527931229406625884)). Others (Ponder, Marcototech) are still waiting for updates.
- **Sigils** (relevant once rebased on newer TH4): players find them too rare with too-small bonuses, and Lithe halving the sigil inventory makes that trait too punishing ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490553886073950368)).

## Other notes

- **Sub-mods listing:** the mod isn't in the server's TH4 sub-mods section because no one has asked for a subforum yet. Ask the server staff for one (Nikodemsky, 5 Nov, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1567674951891157114)).
- **Compatibility question:** a Russian-speaking player asked which TH4 version the mod works with and never got a clear answer ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508931328601755790)). The README now names TH4 v0.208.
- **Black Death game changer:** Marcototech saw it in a game where he says he only picked Easy and the Acceleration game changers. Rageweaver says it's intended ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508841792731086868)). Unclear whether it's a bug.
