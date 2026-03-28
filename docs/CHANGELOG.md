# Changelog

All notable changes to Diablo: The Heaven will be documented in this file.

## [0.1.0] - 2026-03-29

### Added
- **Quick Save / Quick Load Hotkeys**: F9 for quick save, F12 for quick load during gameplay (single-player, saving-enabled modes only). Mirrors the menu save/load with "Saving"/"Loading" on-screen feedback.

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
