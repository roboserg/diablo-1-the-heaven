# Ideas from Other Mods

Features that players praise or ask for in the channels of two other TH4 sub-mods. Feedback about The Heaven itself is in [COMMUNITY_FEEDBACK.md](COMMUNITY_FEEDBACK.md). Problems reported in these channels that affect The Heaven are in [OPEN_BUGS.md](OPEN_BUGS.md) and [FIXED_BUGS.md](FIXED_BUGS.md).

The two sub-mods, both on **The Hell 4 mod** Discord server:
- **Anti-Grind** (AG) by Yuki: `#antigrind`, 2,115 messages, 23 May – 5 Nov 2026
- **Godroll** by StArchon, a patched TH4.exe: `#godroll`, 762 messages, 3 Oct – 5 Nov 2026

These are features their players praise or ask for that The Heaven doesn't have and that aren't already in COMMUNITY_FEEDBACK.md. Each was checked against the README, the changelog and the code after the move to TH4 v0.774. Since 17 Oct, AG 7.4 runs on TH4 v0.774, the same base as The Heaven, and Yuki says it loads vanilla TH4 saves ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1561047153697099878)).

Links marked *antigrind* and *godroll* open those channels in Discord; you must be signed in and a member of the server. Every linked message is also in a local snapshot: see [Sources and how to re-check them](COMMUNITY_FEEDBACK.md#sources-and-how-to-re-check-them).

## Most praised: every change is optional (AG)
AG's main selling point is that every change is either a game changer picked when creating a character or a config option. Yuki calls it a sandbox, from trivially easy to harder than vanilla, and Masterteo89 calls this versatility the best thing about AG ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1561092952564957245), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508073977820614797), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538144330160799744)). In The Heaven, no-penalty uniques, the rare item buffs and the doubled stat cap are always on. Mordor, Masterteo89 and Danycron criticize exactly those, so making them optional would answer that criticism.

## Loot, selling and inventory
- **Loot filter and auto-sell:**
  - AG has a loot filter in its config, and Rageweaver says it's needed with +100% loot ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508155691284566028)).
  - UbergamR wants to hide items his class can't use ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538164656135340052)).
  - Godroll is adding `AutoSellFilter.txt`: junk sells for the shop price the moment you pick it up ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1567702919107837964)). Darkbone115 asked for a loot filter there ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1567622772425883739)).
- **Sell from anywhere** (Godroll): Ctrl+Shift-click sells an item from the inventory, belt, sigil bag, stash or the ground ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1563376614077632633), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1565927978158784552)).
- **Auto attack and auto loot** (Godroll, 2–4 Nov): click a monster once and the hero keeps fighting and moves on to the next monster. When nothing is left, it picks up drops and opens chests and shrines. Both can be turned off in the config. Matt called it one of the best QoL features ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1566897157301207190), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1566666441317556229)).
- **Backpack sort** (Godroll): Backspace packs the inventory and groups items ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1566356389494915172)). The Heaven only sorts the stash, by quality, level, price or picture; ZemoG asked for alphabetical sorting ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1560094942032953345)).
- **More visible loot:**
  - socket counts shown on items on the ground (AG, praised by bigguyforyou, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1507815571901907114)). **Done for the next release.**
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

## Crafting and town services
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

## Perks, stats and classes
- **Perk points for clearing a difficulty:** coming to AG as a config option, with about 8 players for it ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1528686649251463179), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1528946409926295592)). AG also has a double-perks game changer ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1528114451226689676)).
- **+5 stats per level and a perk at level 1** (AG). Ketsuen says the stat game changer is the main reason he won't play without a sub-mod ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1529229407498141948), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1507815598430883861)).
- **Synergy rework** (AG): Ketsuen calls it a major point ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1556740173692141701), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508987112840757248)).
- **Looser trait limits** (AG), such as Lord of the Rings for every class (only Monk gets it in TH4) and Giant for Rogue. Zerathyl names it as the reason to install AG ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1522938413865500762), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508071412970164354)). Slack Daddy wants a config option to turn off trait penalties ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1560041334738264207)).
- **Class reworks** (AG):
  - Iron Maiden buffs, and a Rogue that blocks with two claws. Both are on Masterteo89's list of AG's winning points ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1560143047692386366)).
  - a Trapper rework ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1559988408942600223))
  - auras whose "standing still" bonus also counts attacking and casting ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538372085137801297))

  In Godroll's channel, Marcototech asked for melee cleave, and Gorvex suspects melee progresses slower than ranged ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557399944275427391), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1563845602117156897)).

## Combat flow and movement (Godroll)
- **Pause while a panel is open,** only in battle. It paused everywhere until Marcototech objected ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557063418731569223), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1557180033414467658)).
- **Cooldowns reset out of battle, and buffs last.** Buffs carry across stairs, and Fury, Seeing, Etherealize, Reflect and Mana Shield can be cast in town and stay on ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1565808660582764555), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1556440664609263636), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1563769457631821987)). The Heaven only allows Teleport and Mana Shield in town.
- **2x speed and faster jogging:** the speed button goes up to 2x, and the game starts at the speed you left it at ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1564886495280496672)). **The 2x speed is done for the next release.** The game still starts at 1x, and jogging is unchanged.
- **Long walks and WASD:** one click walks any reachable path, and Shift+WASD walks ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1556921339087028344)).
- **Shift-click on a stat's + spends 5 points.** It was Slack Daddy's idea ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1562576488547614720)). **Done for the next release.**

## Map and information (Godroll)
- **Map overhaul:**
  - markers for shrines, bookcases, levers and fountains
  - arrows on every exit
  - the full map once a level is cleared
  - markers on monsters still fighting

  urza got lost in Horazon's areas without it ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1560942135040016384), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1562028804933816341)).
- **Loot and uniques left on the level,** shown while holding Alt and unlocked for a difficulty after killing Uber Diablo there. Gorvex first called it "on the edge of cheating", and Masterteo89 called it a spoiler ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1561201576075591761), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1561299210148642940)).
- **Game time per hero:** Marcototech asked for it, and Gorvex wants it to compare classes ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1563841690437951532)).
- **Small touches:**
  - a sound when a level is cleared, and HP bars marked in quarters (Godroll, [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1564174780658221176), [godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1567678973779910778)). **The sound is done for the next release.**
  - the player's pain cries when hit by spells and traps, restored (AG, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1559892585064038401))
  - renaming a character (AG, [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1527867845998940170))

## Difficulty and content
- **Monster HP and damage sliders from 0 to 1000%** (AG), with stun threshold scaling planned ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508807297659437226), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1538370564832362506)).
- **Hard-to-reach content opened up** (AG): the alternative route in every mode, AR levels and Uber bosses ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1508078322070847508), [antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1527860107558518875)).
- **New game changers** (Godroll): No Reinforcements (monsters don't summon or raise others) and Paladin's Bane ([godroll](https://discord.com/channels/1261720897975357460/1555832068272033864/1564063611159056384)). Ketsuen quit Dark Campaign because it has no game changers ([antigrind](https://discord.com/channels/1261720897975357460/1507664341145620480/1559985152384565280)).

## How their players see The Heaven
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
