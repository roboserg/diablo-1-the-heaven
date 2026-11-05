# Community Feedback

Feature requests and feedback from players, collected from three places:
- the `#the-heaven` channel on **The Hell 4 mod** Discord server (all 144 messages, 25 May – 5 Nov 2026)
- the archived **"The Heaven mod"** thread in that server's `#modding` channel (all 218 messages, 30 Mar – 6 Apr 2026)
- [GitHub issues](https://github.com/roboserg/diablo-1-the-heaven/issues)

**Bugs that are still open are in [OPEN_BUGS.md](OPEN_BUGS.md),** together with the fixes players made and what `th4-v0.774-base` already fixes.

Features that players praise or ask for in the other TH4 sub-mods' channels (`#antigrind` and `#godroll`) are in [Ideas from other sub-mods](#ideas-from-other-sub-mods).

The latest release is still v0.2.0 (30 March 2026). The `th4-v0.774-base` branch, not released yet, rebases on TH4 v0.774, which brings sigils and pixel light back.

Links marked *channel* go to `#the-heaven`, links marked *thread* go to the `#modding` thread, and links marked *antigrind* or *godroll* go to those channels. They open in Discord, and you must be signed in and a member of the server.

## Sources and how to re-check them

**Local snapshot (fastest).** Every message is saved with author, UTC time and message ID:
- `the-heaven-channel.md` (144 messages)
- `modding-thread-the-heaven-mod.md` (218 messages)
- `antigrind-channel.md`: the `#antigrind` channel of the Anti-Grind sub-mod (2,115 messages, 23 May – 5 Nov 2026)
- `godroll-channel.md`: the `#godroll` channel of the Godroll sub-mod (762 messages, 3 Oct – 5 Nov 2026)

The folder is `F:\git\diablo-1-the-heaven\.claude\worktrees\discord-bot-channel-access-6724f2\.claude\community-sources\`, captured on 5 Nov 2026. Search it for the message ID in any link here or in [OPEN_BUGS.md](OPEN_BUGS.md).
- **Not committed:** the repo is public and these are other people's posts from a members-only server.
- **Where it lives:** only on this PC, inside that worktree. It disappears if the worktree is deleted, so copy it elsewhere before cleaning up worktrees.

**Live from Discord** (for newer messages or attachments):
1. In the Claude desktop app's built-in browser, open discord.com and sign in. The QR code from the Discord phone app is fastest. Agents must not type passwords or use a Discord token.
2. Open the sources:
   - `#the-heaven`: https://discord.com/channels/1261720897975357460/1507669410330972260
   - the thread: in `#modding`, click the Threads icon in the channel header, then "The Heaven mod" under older threads (https://discord.com/channels/1261720897975357460/1487937850824593611)
3. Discord loads about 50 messages at a time. Scroll up until the channel's first message, then down to the end. Stopping early misses messages: the first read of the thread missed its last 20.
   - Read the text, not screenshots.
   - When scrolling by script, setting `scrollTop = 0` once the list is already at the top doesn't load older messages. Scroll down a little and back to 0 each time.
   - An unread channel opens at the first unread message, not at the newest one. Scroll to the bottom first.
   - Messages are `li[id^="chat-messages-<channel id>-"]` elements; the author is in `#message-username-<id>` and the text in `#message-content-<id>`.
4. Switch channels with the sidebar inside the signed-in tab. A new browser tab doesn't share the login, and opening one signed the pane out once.
5. Attachments (patches, code, Gorvex's suggestion list) exist only in Discord: open the message link and click the file.
6. To save a big channel to a file, build the text in the page and download it as a file: create a `Blob`, then click an `<a download>` link to it. The browser saves it to `C:\Users\shebo\Downloads`. The page can't write to the clipboard, and returning hundreds of KB through tool output is slow and costly.

A message ID encodes its UTC time: `(id >> 22) + 1420070400000` is milliseconds since 1970.

## Danycron's feedback (priority)

Danycron is worth special attention:
- **Streamer:** he streams TH4, and viewers often want to join his multiplayer games.
- **Group leader:** he plays in a multiplayer group ("the MP pod") of about a dozen players.
- **Expert:** you called him "the expert". He says he only knows one class well and points to Gorvex as the most knowledgeable player on the server.

All 69 of his messages are in the `#modding` thread (30–31 Mar and 5–6 Apr); he never posted in `#the-heaven`.

### What would get his group to play The Heaven
- **Shared stash:** one of the group's biggest wants. Done in v0.2.0.
- **Multiplayer trading and more than 4 players:** "the biggest desires in the MP pod now" ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488153254918357064)).
  - Viewers often want to play with him but can't because of the 4-player limit ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488177978834682018)).
  - You released an experimental 8-player build the next day; he never reported back on it.
- **His caveats:**
  - **Missing features:** the open-source TH4 lacked features of the then-current TH4, such as sigils, and the group might not want to lose them ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488177978834682018)). The `th4-v0.774-base` branch rebases on TH4 v0.774, and sigils are back.
  - **Morphing:** item morphing may be too big a problem to work around in multiplayer ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488153395058442420)). The `th4-v0.774-base` branch locks shared stash items that would morph; the morphing itself is base TH4 behavior and remains (see [bug 3](OPEN_BUGS.md#3-magic-items-morph)).
  - **Switching versions:** getting a dozen people onto a new version is a hassle. He wanted to ask the group first, so you wouldn't do the work for nothing ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488153780250742938), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488154037793718373)).
  - **Timing:** not soon ("another day far away"). He'll keep playing TH4 "for the rest of my life" and wants to try mods like this once TH4 reaches 1.0 ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488163367750209616)).

### Hardcore
- A scaling XP bonus for hardcore, like Survival and Ironman have ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488091359964434503)).
- The shared stash as continuity between hardcore deaths: starting self-found every time is rough (same message).
- When you pointed to 5x XP from Acceleration and +20% per rank of Swift Learner, he said that's "prolly good" but he'd have to test it ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488149517927973047)).

### Gear and crafting
He liked the socketing buff for uniques and rares, and wants more ways to improve gear ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490550986094280834)):
- a gamble that adds affixes to blue items
- "slamming" gear with gems bought in town: pricier gems allow better affixes, with a chance of a good affix and a large chance of ruining the item, like worldstone shards in Project Diablo 2 ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551174745559123))

### Balance and scope
- **Balance:** a 44-class mod can't be truly balanced; even Project Diablo 2 has weak builds every league ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490549326391414971)). He agrees with Azzi that making the game more forgiving and fleshing things out is the better goal ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490550855080874117)).
- **Scope (his main verdict on The Heaven):** "There's def good stuff in there. But too many large scale, sweeping changes overall for my taste" ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551392845172898)). Just before, he said he liked some of your changes (the socketing buff), and Azzi praised Acceleration in every mode and Infernal Bargain, so "in there" means The Heaven.
- **End game:** monsters on Crucible and higher scale too hard, and he doubts sigils will let players keep up ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551857020538940)).
  - He calls the scaling "honestly off the charts overpowered". Even his Hydramancer, which he thinks is the strongest class, struggled. Players need a big power boost, not tiny upgrades ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490555563736563802)).
  - He finished Inferno with that Hydramancer, and the whole run is in his live VODs ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490555894331605133)).
- **Advisor:** he wants to advise a developer on the "real" mod after TH4 1.0, and thinks he has sensible ideas for systems that are fun and still challenging ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551461631885452), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551530779050170)). Worth inviting him to advise The Heaven.

### Sigils (relevant after the v0.774 rebase)
- **He likes the system:** the sigil bag and the mechanic are fine; the problem is only how rare they are ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490555122986516590)). It feels bad to wait an hour or two for one and get +1 gold or +1 minion damage ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490555196307279882)).
- **Drop rate:** sigils need to drop far more often, almost as often as gems ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490553185763594270)). He also agrees with Azzi that two-affix sigils should be more common ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490555396685824150)).
- **Why:** they're too rare, their bonuses are too small, and a full inventory takes dozens of hours of play. Many are useless anyway ("+1 gold") ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490552807663603764)). With more drops they'd work like D2 charms, which you keep replacing and upgrading ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490553026921103360)).
- **Lithe:** the Lithe trait halving the sigil inventory makes Lithe a trait no one would take. It loses 35 charm slots, which turns a former inconvenience into a large power loss. Even 35 charms of +3 Dexterity would outweigh what Lithe gives ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490553886073950368), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490554145248252015), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490554467354017985)).

### On you and the mod
- 31 Mar: he reacted with a "noooo" emoji to the screenshots showing that -light radius was removed ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488301098061135873)).
- 5 Apr: "Lotta bugs poppin up I notice" ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490197992941031464)).
- 6 Apr: he guessed you were "bullied out" or ran out of steam once you saw the knock-on effects of big sweeping changes ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490547511793029183), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490547662465011853)). He agreed with Mordor that modders start out zealous and lose steam when they see how big the mod is ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490548240704077956)).

## Feature requests

### Most asked
- **Rebase on the latest TH4.** Mordor has released the latest TH4 source in the downloads section of [patreon.com/thmod](https://www.patreon.com/thmod/about) (Slack Daddy, 14 Oct, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1559691391121104967); Nikodemsky, 4 Nov, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1567663591039959160)). It would bring:
  - pixel light (Rageweaver, 2 Apr, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489260002702262423)). Gorvex advised waiting for the newer source instead of porting it by hand.
  - the Wirt power-up and sigils (Ponder, 2 Jun, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1511302320934228008); Tharunes missed the sigils, 25 May, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508582352429777048))
  - **Done on `th4-v0.774-base`** (not released yet): it rebases on TH4 v0.774 (13 Oct 2026). Sigils are back and pixel light works. The Wirt power-up isn't checked yet.
- **Remove VISE in single player** (VISE is what players call not being able to save or town-portal during combat). Rageweaver calls it the most-asked feature after the shared stash ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489271217545412700)).
- **Multiplayer trading and more than 4 players** (Danycron, 30 Mar).
  - Zgeg (26 May, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508654276639981649)) and UbergamR (12 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1536859083250999357)) also asked about trading. Itadori's group can't pass items between players (9 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1535848055574761583)).
  - You released an experimental 8-player build on 31 Mar ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488550931464458383)); no one reported back on it.
- **5x XP in multiplayer.** Itadori, 14 Aug: the 5x XP option doesn't work in multiplayer and they'd like it to ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537844932948263002)). Their group plays TH4 with the Anti-Grind submod in multiplayer and The Heaven in single player.
- **Keep -light radius** on uniques (see [bug 4](OPEN_BUGS.md#4-some-unique-item-penalties-remain)).

### Other requests
- **Hardcore:** a scaling XP bonus like Survival or Ironman, and the shared stash as continuity between hardcore deaths (Danycron, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1488091359964434503)).
- **More gear crafting** (Danycron, 6 Apr, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490550986094280834), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551174745559123)):
  - a gamble that adds affixes to blue items
  - "slamming" gear with gems bought in town: a chance at a good affix, a large chance to ruin the item (like worldstone shards in Project Diablo 2)
- **Easier multiplayer connection:** Itadori's group needed third-party software to connect their PCs (9 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1535847734437871718)).
- **A gold cheat or more lenient cheat options.** Itadori says The Heaven is the best version they found, but their group plays another one because it allows a gold cheat ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1535406229961179347), [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1535406339411673179)).
- **Work together with the Anti-Grind team** (Itadori, 14 Aug, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537844824483434607)).
- From GitHub [issue #3](https://github.com/roboserg/diablo-1-the-heaven/issues/3):
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
  - Infernal Bargain back, to turn synergy points into perk points (The Heaven already re-enabled it)
- **Game changers:** Acceleration and That's My Secret in single-player Normal and multiplayer Easy/Normal; Double Damage, Effortless and Vaccinated in multiplayer Easy.
- **UI:**
  - a monster info box showing damage reductions and attack stats
  - more spell details in the spellbook
  - a much bigger character sheet
- **Shrines:** attribute shrines drop elixirs; the gold shrine drops one pile; shrine effects shared in multiplayer.
- **Misc:** shoot barrels and urns from range in Normal mode; restore Cain's and Griswold's original greetings.

The one bug in his list (unarmed damage with +/- base damage items) is [bug 10](OPEN_BUGS.md#10-unarmed-damage-drops-with-items-that-change-base-damage-base-th4) in the bugs file.

## Ideas from other sub-mods

Two other TH4 sub-mods have channels on the same server:
- **Anti-Grind** (AG) by Yuki: `#antigrind`, 2,115 messages, 23 May – 5 Nov 2026
- **Godroll** by StArchon, a patched TH4.exe: `#godroll`, 762 messages, 3 Oct – 5 Nov 2026

These are features their players praise or ask for that The Heaven doesn't have and that aren't listed above. Each was checked against the README, the changelog and the code on `th4-v0.774-base`. Since 17 Oct, AG 7.4 runs on TH4 v0.774, the same base as The Heaven, and Yuki says it loads vanilla TH4 saves ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1561047153697099878)).

### Most praised: every change is optional (AG)
AG's main selling point is that every change is either a game changer picked when creating a character or a config option. Yuki calls it a sandbox, from trivially easy to harder than vanilla, and Masterteo89 calls this versatility the best thing about AG ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1561092952564957245), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508073977820614797), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538144330160799744)). In The Heaven, no-penalty uniques, the rare item buffs and the doubled stat cap are always on. Mordor, Masterteo89 and Danycron criticize exactly those, so making them optional would answer that criticism.

### Loot, selling and inventory
- **Loot filter and auto-sell:**
  - AG has a loot filter in its config, and Rageweaver says it's needed with +100% loot ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508155691284566028)).
  - UbergamR wants to hide items his class can't use ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538164656135340052)).
  - Godroll is adding `AutoSellFilter.txt`: junk sells for the shop price the moment you pick it up ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1567702919107837964)). Darkbone115 asked for a loot filter there ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1567622772425883739)).
- **Sell from anywhere** (Godroll): Ctrl+Shift-click sells an item from the inventory, belt, sigil bag, stash or the ground ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1563376614077632633), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1565927978158784552)).
- **Auto attack and auto loot** (Godroll, 2–4 Nov): click a monster once and the hero keeps fighting and moves on to the next monster. When nothing is left, it picks up drops and opens chests and shrines. Both can be turned off in the config. Matt called it one of the best QoL features ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1566897157301207190), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1566666441317556229)).
- **Backpack sort** (Godroll): Backspace packs the inventory and groups items ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1566356389494915172)). The Heaven only sorts the stash, by quality, level, price or picture; ZemoG asked for alphabetical sorting ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1560094942032953345)).
- **More visible loot:**
  - socket counts shown on items on the ground (AG, praised by bigguyforyou, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1507815571901907114))
  - item outlines always on, without holding Alt (Godroll; Slack Daddy plays on a 65" TV, [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1560172334277464104))
  - inventory cells colored by item quality (Godroll; made optional after Rageweaver and Magnus found it messy, [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1566001049955860556))
- **More loot, chosen per character** (AG):
  - +50–100% loot per difficulty, or a flat bonus
  - magic find and gold find bonuses
  - an All Drop game changer that mostly removes class requirements, so other classes' gear is usable

  Masterteo89 calls All Drop one of AG's winning points ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508076361837969439), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1539195315067035749), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1560143047692386366)).
- **Sigils:**
  - AG added a config setting for the sigil drop rate, from 0% to 1000% ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1561043669161738361)).
  - Players also asked to reroll useless sigil bonuses ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1507918058671312980)) and to recycle 2–3 sigils into one at Cain ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1559984115443564585)).
  - Godroll puts picked-up sigils straight into the sigil bag ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1558226796514902170)).

### Crafting and town services
- **Reroll enchantments at Adria without save-scumming,** so it also works in multiplayer (AG; asked by bigguyforyou, Rageweaver, Styx and Ketsuen). Yuki also plans a "Dispel magic" button that turns a magic item back into a white one ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1507815571901907114), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1526360402173104258), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1564670827981701163)). The README lists "Re-roll affixes" as planned.
- **Pick exactly what you get** (Godroll):
  - a custom item at Griswold: you choose the base and the affixes within level caps, and it always comes out max-rolled
  - a custom enchant and a custom book at Adria

  ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1556734425994240194), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557305817437437962), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1563005103420608536))
- **Pepin sells elixirs** (Godroll), as a gold sink at 48k–60k gold each. The attribute elixirs unlock after the Clear Water quest, and the Spectral Elixir after Black Mushroom. It was Slack Daddy's idea ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1558228901119918081)).
- **Batch transmute at Cain** (Godroll): any number of potions, holy water or repair oils merge in one click. urza praised it ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1563888787707404420)). In AG's channel, STIMbear and Slack Daddy asked for a bigger transmute window ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1564458493187194953)).
- **Less gem grind:** the gem upgrade window has to be emptied after every combine, and grade +4 takes 81 gems (Ketsuen; Rageweaver hates the gem grind, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1507893386810753034)). In Godroll, gem grades follow the difficulty ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557044520678330459)).
- **Belt refill for every belt item** (Godroll), including elixirs, relicts, runes and oils ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557102619359576165)). The Heaven refills potions only.
- **Sockets on white items:** Godroll drops white items with 3–6 sockets ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1556734425994240194)). In AG's channel, Ketsuen asked for an optional 6-socket game changer for enchanted gear ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1550599867242381313)).

### Perks, stats and classes
- **Perk points for clearing a difficulty:** coming to AG as a config option, with about 8 players for it ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1528686649251463179), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1528946409926295592)). AG also has a double-perks game changer ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1528114451226689676)).
- **+5 stats per level and a perk at level 1** (AG). Ketsuen says the stat game changer is the main reason he won't play without a sub-mod ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1529229407498141948), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1507815598430883861)).
- **Synergy rework** (AG): Ketsuen calls it a major point ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1556740173692141701), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508987112840757248)).
- **Looser trait limits** (AG), such as Lord of the Rings for every class (only Monk gets it in TH4) and Giant for Rogue. Zerathyl names it as the reason to install AG ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1522938413865500762), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508071412970164354)). Slack Daddy wants a config option to turn off trait penalties ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1560041334738264207)).
- **Class reworks** (AG):
  - Iron Maiden buffs, and a Rogue that blocks with two claws. Both are on Masterteo89's list of AG's winning points ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1560143047692386366)).
  - a Trapper rework ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1559988408942600223))
  - auras whose "standing still" bonus also counts attacking and casting ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538372085137801297))

  In Godroll's channel, Marcototech asked for melee cleave, and Gorvex suspects melee progresses slower than ranged ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557399944275427391), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1563845602117156897)).

### Combat flow and movement (Godroll)
- **Pause while a panel is open,** only in battle. It paused everywhere until Marcototech objected ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557063418731569223), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557180033414467658)).
- **Cooldowns reset out of battle, and buffs last.** Buffs carry across stairs, and Fury, Seeing, Etherealize, Reflect and Mana Shield can be cast in town and stay on ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1565808660582764555), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1556440664609263636), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1563769457631821987)). The Heaven only allows Teleport and Mana Shield in town.
- **2x speed and faster jogging:** the speed button goes up to 2x, and the game starts at the speed you left it at ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1564886495280496672)). The Heaven stops at 1.5x.
- **Long walks and WASD:** one click walks any reachable path, and Shift+WASD walks ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1556921339087028344)).
- **Shift-click on a stat's + spends 5 points.** It was Slack Daddy's idea ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1562576488547614720)).

### Map and information (Godroll)
- **Map overhaul:**
  - markers for shrines, bookcases, levers and fountains
  - arrows on every exit
  - the full map once a level is cleared
  - markers on monsters still fighting

  urza got lost in Horazon's areas without it ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1560942135040016384), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1562028804933816341)).
- **Loot and uniques left on the level,** shown while holding Alt and unlocked for a difficulty after killing Uber Diablo there. Gorvex first called it "on the edge of cheating", and Masterteo89 called it a spoiler ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1561201576075591761), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1561299210148642940)).
- **Game time per hero:** Marcototech asked for it, and Gorvex wants it to compare classes ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1563841690437951532)).
- **Small touches:**
  - a sound when a level is cleared, and HP bars marked in quarters (Godroll, [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1564174780658221176), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1567678973779910778))
  - the player's pain cries when hit by spells and traps, restored (AG, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1559892585064038401))
  - renaming a character (AG, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1527867845998940170))

### Difficulty and content
- **Monster HP and damage sliders from 0 to 1000%** (AG), with stun threshold scaling planned ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508807297659437226), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538370564832362506)).
- **Hard-to-reach content opened up** (AG): the alternative route in every mode, AR levels and Uber bosses ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508078322070847508), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1527860107558518875)).
- **New game changers** (Godroll): No Reinforcements (monsters don't summon or raise others) and Paladin's Bane ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1564063611159056384)). Ketsuen quit Dark Campaign because it has no game changers ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1559985152384565280)).

## Feedback and perception

- **What players like:** less grind, the shared stash, the QoL features.
  - Rageweaver argues for keeping no-penalty uniques and the MF buff: they make finding a unique exciting, and players who want balance can play TH4 ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489304899408760882)).
  - Shelk, 11 Jul: "much more my speed" than the huge grind of classic TH4 ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1525290493322203216)). UbergamR, 11 Aug: just what they wanted ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1536852346460577903)).
  - Azzi, 6 Apr: likes that the speed-up (Acceleration) works in every game mode and that Infernal Bargain is back ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551299492806738), [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490551359446192238)).
  - Ponder, 3 Jun: "so many improvements", and wishes TH4 itself had these options ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1511775633129340928)).
- **Hard mode:** on Hard, melee characters struggle because ranged monsters retreat more aggressively, so no-penalty uniques may mostly make ranged characters overpowered (Rageweaver, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1489334529750728738)).
- **Pacing:** with 5x XP, Itadori killed Diablo on dungeon level 16 at clvl 30 ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1535849448129953882)).
- **Misconceptions:**
  - gazoo, 16 Aug: the mod only exists to make Psion overpowered ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1538326216572936305)). You replied on 17 Sep that it has many other additions.
  - Ketsuen, 14 Aug: Anti-Grind already offers everything people ask for here, and the creator has never posted in the channel ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1537900354551677019)).
- **What some don't:** Danycron liked some changes but finds there are too many sweeping ones (see [Danycron's feedback](#danycrons-feedback-priority)).
- **Abandonment and AI:** from 6 Apr players noticed you had gone quiet. Azzi and Danycron guessed you were "bullied out" or ran out of steam, and that much of the code and docs were AI-written ([thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490548356609609818)). Gorvex said the same in `#the-heaven` on 18 Jul ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1527931229406625884)). Others (Ponder, Marcototech) are still waiting for updates.
- **Sigils** (relevant once rebased on newer TH4): Danycron and Azzi find them too rare with too-small bonuses; see [Danycron's feedback](#danycrons-feedback-priority).
- **How other sub-mods' players see The Heaven:**
  - **What AG players miss from it:**
    - auto belt refill (Itadori, UbergamR, Marcototech; Yuki is adding it to AG, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538181685001322537), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538175403682697279))
    - rare items and socketing magic and rare items (UbergamR prefers The Heaven's approach, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538323784350830622))
    - identify all at Cain ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538188431619002539))
    - gold pickup from 2 tiles, spells in town and faster game speed (Ketsuen, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1556740173692141701))

    Yuki won't add a shared stash to AG because of morphing ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538140120237080677)).
  - **Critics:**
    - Itadori's group moved from The Heaven to TH4 with AG ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1537843383857451098)).
    - Gorvex, 25 Aug: The Heaven is "pretty bugged" and "your items aren't safe" ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1541685327880658944)).
    - Mordor called it "clearly abandoned" ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1541748812467535893)). Earlier he said the "cheat mod" label fits The Heaven more than AG ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508058457138528266)).
  - **Why players quit TH4:** Gorvex says the main reason is a lack of damage and clear speed ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1542206834171977748)).

## Other notes

- **Where to report:** players weren't sure whether to post bugs in the `#modding` thread or `#the-heaven` (Marcototech, [thread](https://discord.com/channels/1261720897975357460/1487937850824593611/1490304695967809628)). Name one place, for example in the pinned message.
- **Sub-mods listing:** the mod isn't in the server's TH4 sub-mods section because no one has asked for a subforum yet. Ask the server staff for one (Nikodemsky, 5 Nov, [channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1567674951891157114)).
- **Compatibility question:** a Russian-speaking player asked which TH4 version the mod works with and never got a clear answer ([channel](https://discord.com/channels/1261720897975357460/1507669410330972260/1508931328601755790)). The README on main names TH4 v0.208; on `th4-v0.774-base` it names v0.774.
