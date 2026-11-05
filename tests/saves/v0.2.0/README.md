# A save made by v0.2.0

The autotest's `oldsave` phase loads this save to check that players can upgrade from v0.2.0 with their characters, Griswold's stash and the shared stash (`scripts\autotest.ps1`, see `docs\BUILD.md`).

- `th4_save_sp\th4sp_0.ssv`: OldWarrior, a level 20 Warrior in Normal mode, with 95 unspent stat points. He has 5 items in the inventory (magic items and a unique) and 6 in Griswold's stash (magic items and two uniques).
- `th4_save_sp\th4shared.stv`: the shared stash, with 6 items (magic items and two uniques).
- `oldsave-manifest.txt`: every item as v0.2.0 had it, one per line: where it is, its seed, and the autotest's `Describe()` line (names, base, damage, armour, durability, requirements and every effect). The `hero` line has the level, base stats, unspent points and experience.

Made on 5 Nov 2026 by the v0.2.0 release (tag `v0.2.0`) with a small driver added for the occasion: it created the hero, raised him to level 20, made the items with the game's own item generator, put them in place through the stash panels, saved and wrote the list. v0.2.0 ran on the TH4 v0.774 `TH4data.mor`.

These are save files of The Heaven itself, with nothing from a third party in them.
