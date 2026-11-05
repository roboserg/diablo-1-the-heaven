# Saves made by v0.2.0

The autotest's `oldwarrior`, `oldmage`, `oldarcher`, `oldclassic` and `oldveteran` phases load these saves to check that players can upgrade from v0.2.0 with their characters, Griswold's stash and the shared stash (`scripts\autotest.ps1`, see `docs\BUILD.md`). Each folder is one phase: `oldmage` loads `mage`.

| Folder | Hero | What it's for |
|---|---|---|
| `warrior` | OldWarrior, a level 20 Warrior in Normal mode, 95 unspent stat points | Magic items and uniques in the inventory and both stashes |
| `mage` | OldMage, a level 25 Mage in Normal mode | Books and spell staves, and spells the Mage has learned |
| `archer` | OldArcher, a level 25 Archer in Normal mode | The same for an Archer |
| `classic` | OldClassic, a level 25 Mage in Classic mode | Classic's save name and folder, and Classic's affix table. Its save is `th4_save_cls\th3cls_0.ksv`, the name v0.2.0 used |
| `veteran` | OldVeteran, a level 120 Savage in Normal mode, with every stat point spent | 46 different perks and synergies, rare items, magic and white items with gems in their sockets, and loose gems, in the inventory and both stashes. Its items were made at Hell difficulty |

In each folder:

- `th4_save_sp\th4sp_0.ssv`: the character (Classic: `th4_save_cls\th3cls_0.ksv`).
- `th4_save_sp\th4shared.stv`: the shared stash.
- `oldsave-manifest.txt`: everything as v0.2.0 had it, one per line. An item line has where it is, its seed and the autotest's `Describe()` line (names, base, damage, armour, durability, requirements, every effect and any gems). The `hero` line has the level, base stats, unspent points and experience. The `perks` and `spells` lines have each one's number, rank and name, so a perk or spell TH4 moved shows up; the Warrior's list is older and doesn't have them.

The test allows for one thing TH4 v0.774 itself changed: two affixes were renamed, with the same effect (`WithNewNames` in `src\AutoTest.cpp`).

`tests\saves\v0.3.0` has the Classic hero as v0.3.0 saved it.

The saves were made on 5 Nov 2026 by the v0.2.0 release (tag `v0.2.0`) with a small driver added for the occasion, which was never committed: it created each hero, raised the level, spent the points by the game's own rules, made the items with the game's own item generator, put them in place through the stash panels, saved and wrote the list.

These are save files of The Heaven itself, with nothing from a third party in them.
