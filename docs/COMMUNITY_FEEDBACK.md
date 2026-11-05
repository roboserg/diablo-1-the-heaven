# Community Feedback

Bug reports and requests from players, collected from the `#the-heaven` channel on
**The Hell 4 mod** Discord server (all 144 messages, 25 May – 5 Nov 2026) and from
[GitHub issues](https://github.com/roboserg/diablo-1-the-heaven/issues).

None of these are fixed yet: the latest release is still v0.2.0 (30 March 2026).

The message links open in Discord. You must be signed in and a member of the server.

## Bugs

### 1. Griswold's personal stash loses items
Items put into Griswold's stash disappear after the stash is closed. This is the most-reported bug.
- Marcototech, 25 May: "Stash from griswold: Caution is bugged" ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1508488088580788375))
- Blackheart, 18 Jul: anything put in Griswold's stash in Purgatory disappears after closing it ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1527881491592253521))
- GitHub [issue #3](https://github.com/roboserg/diablo-1-the-heaven/issues/3): rings and potions disappear. The reporter suggests removing the option.

### 2. Magic items change when moved through the shared stash
A magic item put into Gillian's shared stash comes out as a different item on another character.
- Rageweaver, 25 May: a magic ring changed after switching characters, even though both were Dimachaerus and could use it ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1508581640606322860))
- Mordor (TH4 author), 26 May: this is one of the reasons loot must not be shared ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1508761056175063112))
- Gorvex, 17 Sep, explains the cause ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1550231476996538450)):
  - Magic items are regenerated from their seed when loaded.
  - The affix pool differs per class/subclass. Affix slots that are not allowed for one subclass get filled by valid ones, which shifts the pool.
  - The seed is also updated on the fly to fix invalid rolls, so the same item rolls differently for another class.
  - The root cause is in base TH4; the shared stash just exposes it.
- Marcototech (Discord) and Gron21 (GitHub issue #3) can't reproduce it moving items between Paladin, Archer and Mage. Marcototech suspects it only happens between two characters of the same class.

### 3. Items lost when a window is closed or a save is loaded with an item still in it
- Marcototech, 21 Jun: pick an item up from the stash, keep it on the cursor with the stash open, press ESC and load a game, and the item is gone forever ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1518073293121519646))
- GitHub issue #3:
  - A gem put into Griswold's crafting window disappears if the window is closed suddenly.
  - A ring transmuted at Cain disappears if it isn't moved to the inventory before closing the window.
  - Quick save, enchant at Adria, then quick load leaves two copies of the item (enchanted and not), and up to three if repeated. Loading from the menu makes the item disappear.

### 4. Multiplayer gaps
- Marcototech, 21 Jun, two PCs on the same LAN: time speed (Acceleration game changer) can't be changed, and other players can't see dropped items ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1518059339343069234)). Separate loot per player is probably base TH4 behavior.
- Itadori, 14 Aug: the 5x XP option doesn't work in multiplayer and they'd like it to ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1537844932948263002)). Their group plays TH4 with the Anti-Grind submod in multiplayer and The Heaven in single player.

### 5. Removing negative modifiers also removed -light radius
Darkbone115, 15 Aug: -light radius is useful in some situations and should stay ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1537953948856614943)).

### 6. Sisyphean Task silently hides Swift Learner
UbergamR, 12 Aug: a character with the Sisyphean Task trait can't pick the Swift Learner perk, and nothing in the game says why. Worth noting in the trait or perk description ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1536870226858082446)).

### 7. Other open GitHub issues
- [Issue #3](https://github.com/roboserg/diablo-1-the-heaven/issues/3): the pause menu title is hard to read in some zones, and the "Symptoms" tree looks unfinished ("work in progress?").
- [Issue #5](https://github.com/roboserg/diablo-1-the-heaven/issues/5): pixel light doesn't work.

## Requests

- **Rebase on the latest TH4.** Mordor has released the latest TH4 source in the downloads section of [patreon.com/thmod](https://www.patreon.com/thmod/about) (Slack Daddy, 14 Oct, [message](https://discord.com/channels/1261720897975357460/1507669410330972260/1559691391121104967); Nikodemsky, 4 Nov, [message](https://discord.com/channels/1261720897975357460/1507669410330972260/1567663591039959160)). This would also bring:
  - the Wirt power-up and sigils, which came in TH4 versions after 0.208 (Ponder, 2 Jun, [message](https://discord.com/channels/1261720897975357460/1507669410330972260/1511302320934228008))
- **5x XP in multiplayer** (Itadori, see bug 4).
- **Keep -light radius** (see bug 5).
- From GitHub issue #3:
  - the original town NPC greetings (Cain, Griswold, Adria)
  - a special drop sound for unique and set items
  - a shared stash in multiplayer
  - more players per multiplayer game

## Later: check the unofficial 0.2.0 patch

Marcototech posted an unofficial build with fixes by someone named **Yuki**. Check what it changes and whether we need any of the fixes.

- **File:** `TH4Heaven - 0.2.0 stats uniques fixed.zip` (4.62 MB)
- **Where:** attached to Marcototech's message in `#the-heaven`, 26 May 2026, 16:24:
  https://discord.com/channels/1261720897975357460/1507669410330972260/1508838222828929086
- **Announced here** ("something fixed by Yuki"): [message](https://discord.com/channels/1261720897975357460/1507669410330972260/1508760621473071125)
- Right after it (16:26) Marcototech posted something captioned "important for play it", probably install notes or a screenshot: [message](https://discord.com/channels/1261720897975357460/1507669410330972260/1508838742654189739)
- **How to download:** open the message link while signed in to Discord and click the file to download it. Link to the message, not the file: Discord's direct file links expire.
- The name suggests fixes to unique-item stats. It is probably a compiled build (exe and data files), not source, so the check is a diff against our v0.2.0 release.
- It is a third-party binary. Unzip it into its own empty folder outside the repo, and don't run the exe before checking it.

## Other notes

- **Sub-mods listing:** the mod isn't in the server's TH4 sub-mods section because no one has asked for a subforum yet. Ask the server staff for one (Nikodemsky, 5 Nov, [message](https://discord.com/channels/1261720897975357460/1507669410330972260/1567674951891157114)).
- **Compatibility question:** a Russian-speaking player asked which TH4 version the mod works with and never got a clear answer ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1508931328601755790)). The README now names TH4 v0.208.
- **Black Death game changer:** Marcototech saw it in a game where he says he only picked Easy and the Acceleration game changers. Rageweaver says it's intended ([message](https://discord.com/channels/1261720897975357460/1507669410330972260/1508841792731086868)). Unclear whether it's a bug.
- **Perception:** players like the mod (less grind, the QoL features) and ask for updates (Ponder, Marcototech). Others think it's abandoned and "mostly programmed by AI" (Gorvex, 18 Jul, [message](https://discord.com/channels/1261720897975357460/1507669410330972260/1527931229406625884)).
