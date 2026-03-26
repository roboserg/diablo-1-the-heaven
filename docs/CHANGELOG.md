# Changelog

All notable changes to Diablo: The Heaven will be documented in this file.

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
