# Changelog

All notable changes to Diablo: The Heaven will be documented in this file.

## [0.3.0] - 2026-11-05

**Needs The Hell 4 v0.774.** The Heaven is now built on The Hell 4 v0.774 (October 2026). Copy `TH4data.mor` from TH4 v0.774 into the game folder; one from an older TH4 won't work. The README links a backup copy. Characters, Griswold's stash and the shared stash from v0.2.0 carry over, except for Classic characters (see Known issues).

### New from The Hell 4 v0.774
- **Outbreak**: a new game mode where you fight off waves of monsters.
- **Dark Campaign**: a new game mode with 26 missions in the service of the Lord of Terror. Single player only.
- **Sigils**: rare drops with their own inventory that give extra bonuses.
- **Gladiatrix**: a new Rogue subclass.
- **7 new traits**: Scissor, Capitalist, Frenzied, Late Bloomer, Warlord, Raw Power and Thief.
- **3 new game changers**: Terminator, Super Mutant and Enclave Soldier.
- **Hundreds of new items**, including 3 new item sets: Crimson Arsenal, War Arsenal and Sisters' Blood.
- **Secrets of Magic**: a new quest.
- **Per-pixel lighting**: smoother lighting, on by default.

### Changed
- **Shared stash locks items that would change**: TH4 rebuilds items for the character who loads them, so a magic staff or book, an item in Classic mode, a socketed white item, a shop item or Night Kin and Clean Hands gold can come out different for another character. Such items are drawn red, can't be taken out, and stay as they were for the character who stashed them.
- **Perk reset keeps perks whose benefit stays**: Here and Now (a full level), Educated, Higher Education and Overwhelm (stat points at every level-up) and Heavy Drinker (elixirs). Raise the Bar, Thaumaturgy and Ostentation are kept when dropping them would cut stats. The tooltip lists them.
- **-light radius is back on unique and set items**: v0.1.0 removed it with the penalties, but archers and mages use it to pull fewer monsters.
- **Quick save (F9) refuses while an item is held** on the cursor.

### Fixed
- **Stash items lost or duplicated**: Griswold's stash no longer loses or duplicates items around Gillian's shared stash. The shared stash is saved together with the character.
- **Items lost in saves and loads**: F9/F12 quick save and load, loading a save from the menu, the once-a-minute autosave and quitting no longer lose or duplicate an item held on the cursor or left in Cain's, Adria's or Griswold's windows.
- **Magic items flipping**: magic items no longer switch between two versions every 5 seconds, with affixes and "Not Identified" coming and going.
- **Penalties left on unique and set items**: +damage from enemies, -stun threshold, -melee and -ranged absorption, -elemental, spell and weapon %, and negative armour are gone from the stats and the tooltips. Trade-offs, like moving life to mana, stay.
- **Doubled stat cap**: elixirs and shrines raise stats up to it and they stay there, and stats at the cap turn gold. Before, the game cut them back to the old cap.
- **Perk reset took away base stats** above the limits Raise the Bar, Thaumaturgy and Ostentation raise.
- **Sound and music volume** no longer go up a bar on every restart (base TH4).
- **Unarmed damage** keeps its base with items that change base damage (base TH4).
- **Sisyphean Task** lists the perks it blocks.
- **Pixel light** works: the shipped `config.ini` turns it on.
- **Pause menu title** is readable in the dungeon.

### Known issues
- A magic item can come out different for another class or in Classic mode (base TH4). The shared stash locks such items.
- Most magic items change once, a few seconds after they drop and before they're identified.
- A crash at the exact moment the shared stash and the character are saved can duplicate or lose items.
- F12 quick load closes the game with "Unable to open save file" if the hero has no saved game: a new hero before its first save, or any Nightmare or Speedrun hero. The next update fixes this.
- Classic characters from v0.2.0 aren't in the hero list, because TH4 v0.774 renamed Classic save files. The game offers a new hero instead, and the old save file stays on disk. The next update lists them again.
- Some Classic magic items from v0.2.0 get another affix, because TH4 v0.774 added rows to the Classic affix table. The next update fixes this for items v0.3.0 hasn't saved.
- Infernal Bargain gives no perk points with the traits Skilled, Lord of the Rings or Grim Deal, but its penalties still apply. The next update fixes this.
- Without `TH4data.mor` the error blames `DIABDAT.MPQ`. The next update names the right file.

## [0.2.0] - 2026-03-30

### Added
- **Shared Stash**: Gillian now offers a stash shared by all your characters, single-player and multiplayer. Griswold continues to offer the per-character personal stash.

## [0.1.0] - 2026-03-29

### Added
- **No Penalties on Unique/Set Items**: Negative stat penalties on unique and set items are no longer applied or shown in tooltips. Items like Nosferatu's Brooch retain only their positive bonuses, while beneficial negative effects (e.g. reduced damage from enemies) are preserved.
- **Add Socket Service at Griswold**: Magic, and rare items can now have sockets added at Griswold. Price scales with item level and base price.
- **Auto Belt Refill**: When using a potion from the belt, matching potions are automatically pulled from inventory (exact match → same type → holy water → any potion).
- **Quick Save / Quick Load Hotkeys**: F9 for quick save, F12 for quick load during gameplay (single-player, saving-enabled modes only).
- **Item Info Tooltips Improvements**: Item quality level ("item level: X") and base item cost ("base cost: X gold") are now displayed in tooltips.
- **Oil and Book Color Changes**: Oils now display in brown and books display in gold (matching unique item color) for easier visual identification.
- **Open Stash at Griswold**: Griswold now offers "Open stash" option in his menu, same as Gillian's stash service.
- **Identify All Items at Cain**: Cain's shop now has a new "Identify all items (X gold)" option that identifies all unidentified items in inventory at once.
- **Heal and Restore Mana at Cain**: When talking to Cain, the player's HP is fully restored and mana is restored (outside Classic mode), similar to Pepin and Adria's services.

### Changed
- **Gem insertion is now free**: Gem insertion into sockets is now free (only socketable slot creation and gem extraction cost gold)

## [0.0.4] - 2026-03-28

### Added
- **Teleport and Mana Shield now usable in town**: Both spells can now be cast while in town.
- **Perk Reset and Stat Reset services at Adria**: Adria now offers two new services via buttons in her shop: **Reset Perks** (refunds all spent perk points) and **Reset Stats** (refunds all spent stat points back to class starting values). Both cost `1000 × character level` gold.
- **Type-Specific Floor Item Text Colors**: Potions, gems, relics, and rare items now display in distinctive colors for easier identification.

### Changed
- **Stat Allocation Cap Doubled**: Players can now allocate twice as many stat points per character level. The level-based stat sum limit formula is doubled (`(32×level/3 + 90) × 2`), while trait bonuses (Gifted, Giant, Paladin, etc.) remain unchanged.
- **Gold Auto-Pickup Radius Increased**: Gold is now picked up from up to 2 tiles away (was 1 tile).
- **Acceleration Game Changer**: Now available in all single-player game modes (previously Easy mode only)
- **Magic Find now fully scales rare item drop chance**: MF applies 1:1 to rare drop chance (same as magic items). Previously used the unique item divisor (÷4), giving only +25% per 100 MF. Now 100 MF doubles the rare drop chance.

## [0.0.3] - 2026-03-27

### Added
- **Independent rare item drops**: Rare items now have their own drop chance (2% for mobs, 3% for bosses) separate from unique items, matching Diablo 2's priority chain: unique > rare > magic > normal.
- **Rare Item Affixes Buffed**: All rare item affix values are now higher (×1.25 for low/mid tier, ×1.5 for high tier, quality ≥ 50). High-tier affixes are twice as likely to be selected and eligible on a wider range of item levels.
- **Infernal Bargain Synergy**: Re-enabled for all classes. The synergy trades life & mana regeneration and all stats for extra perk points.

## [0.0.2] - 2026-03-27

### Changed
- **Black Mushroom Quest**: Spectral Elixir now grants +5 to all stats (Strength, Dexterity, Magic, Vitality) in non-Classic modes (reduced from +10)

### Fixed
- **Grim Deal Trait**: Removed undocumented spell damage penalty that silently reduced all spell damage by `(CharLevel/3)%`

### Added
- **Game Speed Control in all game modes**: The time speed button (cycles 1.0x → 1.25x → 1.5x) is now available in all single-player game modes. Previously only available in Ironman, Nightmare, and Survival modes.

## [0.0.1] - 2026-03-26

### Changed
- **Psion Aura Damage**: Revert damage as it was in The Hell 2. Damage output ~50% higher due to improved scaling formula: `3 + CharLevel * Magic / 100` (was `3 + CharLevel * Magic / 150` in The Hell 4)
- **Black Mushroom Quest**: Spectral Elixir now grants +10 to all stats (Strength, Dexterity, Magic, Vitality) in non-Classic modes (up from +1)
- **Acceleration Game Changer**: Experience multiplier increased from 2x to 5x (single-player, Easy mode only)
- **Swift Learner Perk**: XP gain per level increased from +5% to +20% (progression now +20%, +40%, +60%, etc.)
