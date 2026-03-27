# Classes & Sub-Classes — Diablo: The Heaven

This document covers every playable class and sub-class in the mod, including stat tables and mechanical identity.

## Class Selection System

Character creation uses a three-tier selection:

1. **Class** — 6 base archetypes (Warrior, Archer, Mage, Monk, Rogue, Savage)
2. **Sub-class** — 4–5 variants per class with distinct stat caps
3. **Specialization** — only for Mage › Summoner (3 options) and Savage › Gladiator (4 options)

Stats listed as **Start → Max**.

---

## Key Mechanics Reference

| Stat | Warrior | Archer | Mage | Monk | Rogue | Savage |
|---|---|---|---|---|---|---|
| HP per VIT point | ×3.0 | ×2.0 | ×1.0 | ×2.5 | ×2.0 | ×3.5 |
| To-Hit Bonus | +40 | +30 | +30 | +50 | +55 | +45 |
| Block Bonus | +10 | +1 | +1 | +1 | +5 | +2 |
| Unique Skill | Item Repair | Infravision | Staff Recharge | Telekinesis | Identify | Reflect |
| Damage Formula | STR | STR + DEX | STR | (STR + DEX) / 1.5 | STR + DEX | STR |

---

## Warrior

**Identity:** Frontline melee combatant. The game's premier shield user — only Warriors get a meaningful block bonus (+10), making them the best at absorbing hits. Solid HP scaling (×3/VIT) and the highest starting strength. Trade-off is low magic ceiling and mediocre accuracy (+40 to-hit).

**Damage formula:** Strength-only (`STR / 100 × CharLevel`). Dexterity does not contribute to damage.

**Skill:** Item Repair — repair equipped gear without a cost.

---

### Warrior (base)

> The classic melee fighter. Well-rounded with no hard weaknesses.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 25 → 600 | 5 → 100 | 15 → 400 | 20 → 400 |

**Starting gear:** Light Dagger, Buckler, Light Hammer, ×2 Healing Potion

The generalist warrior. Decent stat caps across the board with no glaring holes. Good entry point for players learning melee gameplay. Mid-tier STR cap (600) leaves it behind the more specialized sub-classes in raw output.

---

### Inquisitor

> A warrior who hunts the corrupt — part soldier, part judge. Balanced melee with room for some magic.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 25 → 550 | 5 → 175 | 15 → 375 | 20 → 400 |

**Starting gear:** Light Dagger, Buckler, Light Hammer, ×2 Healing Potion

Trades 50 STR compared to the base Warrior for a higher magic ceiling (175), letting it occasionally use support spells and equip magic-required gear. Slightly lower DEX cap. Best choice for a Warrior who wants minor spell utility without going full Templar.

---

### Guardian

> An unyielding wall of flesh and steel. Pure tank.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 25 → 750 | 0 → 50 | 15 → 200 | 25 → 500 |

**Starting gear:** Estoc, Long Mace, ×2 Healing Potion (no shield slot item)

Highest STR cap of any Warrior sub-class (750) and the highest VIT cap (500). Starts with 0 base magic and caps at just 50 — essentially no spell access. Very low DEX cap (200) means poor accuracy outside of the +10 block bonus. Built to stand in the thick of combat and outlast everything. Strong, but requires finding good gear to cover its accuracy gap.

---

### Templar

> A holy warrior who blends faith with force. Melee fighter with real magic capability.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 25 → 500 | 5 → 200 | 15 → 400 | 20 → 400 |

**Starting gear:** Light Dagger, Buckler, Light Hammer, ×2 Healing Potion

Lowest STR cap of the Warrior sub-classes (500), but the highest magic ceiling (200) — enough to cast meaningful spells and equip magic-gated items. Good DEX and VIT caps. Suits players who want the block bonus and warrior HP scaling while keeping a secondary casting identity. The hybrid pick.

---

---

## Archer

**Identity:** Ranged damage dealer. Extremely high DEX ceiling is the defining feature — DEX contributes directly to both accuracy and damage. Low STR and VIT caps mean archers are fragile in melee. Skill is Infravision, useful for spotting hidden threats.

**Damage formula:** `(STR + DEX) / 200 × CharLevel`. Both stats contribute equally.

**Skill:** Infravision.

---

### Archer (base)

> A well-trained ranged fighter. Reliable and adaptable.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 15 → 250 | 5 → 150 | 25 → 800 | 20 → 300 |

**Starting gear:** Small Bow, ×2 Healing Potion

The baseline ranged class. Ties Scout for the highest DEX cap (800), and has more VIT (300 vs 250). Some magic room (150) for light spell use. The most balanced archer for general play.

---

### Scout

> A swift skirmisher who relies on speed and precision above all else.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 15 → 250 | 5 → 200 | 30 → 800 | 15 → 250 |

**Starting gear:** Small Bow, ×2 Healing Potion

Starts with the highest DEX (30) of any archer and ties Archer at the DEX cap (800). The cost is the lowest VIT cap of all archers (250) and even lower starting VIT (15). Slightly more magic room than base Archer. Extremely squishy — must stay at range or die fast. Best for experienced players who can manage positioning.

---

### Sharpshooter

> A marksman who favors heavier weapons and calculated shots.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 15 → 300 | 5 → 200 | 25 → 700 | 20 → 300 |

**Starting gear:** Small Bow, ×2 Healing Potion

Highest STR cap of all archers (300), which helps with heavier bow requirements and melee fallback. Slightly lower DEX cap (700) than Archer/Scout. Same VIT as base Archer. The more STR-forward archer — better at equipping high-requirement ranged weapons, slightly less accurate at peak but more versatile gear-wise.

---

### Trapper

> An unconventional archer who combines ranged attacks with magical traps and explosives.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 15 → 200 | 15 → 400 | 20 → 550 | 15 → 350 |

**Starting gear:** Arrow Trap I, Buckler, ×2 Healing Potion

The most unique archer — a magic/trap hybrid. Highest magic cap of any archer by far (400), lowest STR and DEX caps. HP formula is `1.75 × VIT + 1.5 × CharLevel + 24` (with a -11 mana formula penalty at baseline). The Engineer trait removes level-based scaling in exchange for a flat formula. Playstyle revolves around trap placement and spell augmentation rather than pure bow output. Effective but requires understanding the trap system.

---

---

## Mage

**Identity:** Pure spellcaster. Highest magic cap in the game (900) and double magic scaling on mana. Terrible physical stats — lowest HP gain (×1/VIT), weakest block, lowest STR/DEX. Entirely dependent on spells and staves. Skill is Staff Recharge.

**Damage formula:** Strength-only (`STR / 100 × CharLevel`). Rarely relevant.

**Skill:** Staff Recharge — restore charges to staves.

**Base mana:** 60 (highest of all classes).

---

### Mage (base)

> The classic arcane scholar. Fragile but devastatingly powerful at range.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 10 → 150 | 30 → 900 | 10 → 150 | 15 → 300 |

**Starting gear:** Wand of Holy Bolts, Orb, ×2 Healing Potion

The reference caster. Max magic (900) with the full `2 × MAG + 2 × CharLevel + 3` mana formula. Lowest HP ceiling of any class due to the ×1/VIT scaling. All combat is spell-based. Strong throughout the game but requires careful resource management.

---

### Elementalist

> A mage who has mastered the raw forces of fire, ice, and lightning.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 10 → 150 | 30 → 900 | 10 → 150 | 15 → 300 |

**Starting gear:** Smoked Sphere, Orb, ×2 Mana Potion

Identical stat caps to the base Mage. Differentiated by starting gear (Smoked Sphere instead of Wand of Holy Bolts) and a focus on elemental spell damage. Thematically, the go-to pick for fire/ice/lightning builds. Mechanically interchangeable with base Mage aside from starting items.

---

## Mage › Summoner

**Identity:** Summons minions to fight on their behalf. Shares the same stat caps as other mages (900 MAG) but has a different HP formula: Demonologist and Necromancer get `BaseVit + 2 × CharLevel + 8` (slightly more HP than base Mage per level), while Beastmaster uses `2 × BaseVit + CharLevel/2 - 5` (more VIT-dependent, different level scaling).

Specialization is chosen at character creation and determines the type of creatures summoned.

---

### Demonologist

> A summoner who binds demons to their will through dark contracts.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 10 → 150 | 30 → 900 | 10 → 150 | 15 → 300 |

**Starting gear:** Hatred Effigy, Corpse Head, ×2 Holy Water

Summons demonic minions. Uses the improved summoner HP formula (`+2×CharLevel`), making it slightly tankier than base Mage at higher levels. Best for players who want an aggressive minion army with a hellish aesthetic.

---

### Necromancer

> A summoner who raises the dead to serve as undying soldiers.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 10 → 150 | 30 → 900 | 10 → 150 | 15 → 300 |

**Starting gear:** Bone Wand, Cranium Shield, ×2 Holy Water

Summons undead minions. Same HP formula as Demonologist. The classic minion-master fantasy. Starting gear (Bone Wand, Cranium Shield) gives it a slightly different early-game feel.

---

### Beastmaster

> A summoner who bonds with wild creatures, commanding them as extensions of their will.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 10 → 150 | 30 → 900 | 10 → 150 | 15 → 300 |

**Starting gear:** Ceremonial Dagger, Quasit Heart, ×2 Holy Water

Summons animal companions. Same magic ceiling but uses a different HP formula: `2 × BaseVit + CharLevel/2 - 5`. The `CharLevel/2` scaling means HP grows at half the per-level rate compared to Demonologist/Necromancer.

---

### Warlock

> A practitioner of forbidden magic who draws power from dark pacts.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 10 → 150 | 30 → 900 | 10 → 150 | 15 → 300 |

**Starting gear:** Light Rod, Booklet of Black Magic, ×2 Mana Potion

Identical stat caps and HP/mana formulas to the base Mage. The most thematically distinct starting kit (dark magic items), but mechanically there is no difference from Mage at the stat level. Its identity lives entirely in item and spell choices.

---

---

## Monk

**Identity:** Martial arts fighter. Best accuracy of the physical classes (+50 to-hit), above-average HP scaling (×2.5/VIT), and a blended damage formula that benefits from both STR and DEX. Skill is Telekinesis. Lacks heavy armor access; fights with staves, fists, and light weapons.

**Damage formula:** `(STR + DEX) / 150 × CharLevel` — more DEX-efficient than Archer or Rogue.

**Skill:** Telekinesis — pick up items at range.

---

### Monk (base)

> A disciplined martial artist who has mastered body and mind.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 15 → 400 | 5 → 100 | 25 → 600 | 20 → 400 |

**Starting gear:** Staff, ×2 Healing Potion

Balanced entry into the monk class. Solid STR (400) and DEX (600) caps with good VIT (400). The reference point for all monk sub-classes.

---

### Kensei

> A master of the blade who has perfected a single weapon style.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 20 → 350 | 0 → 100 | 25 → 700 | 20 → 350 |

**Starting gear:** Light Kanabo, Tanto, ×2 Healing Potion

Sacrifices STR cap (350) for the highest DEX of any monk (700). HP formula improves over base Monk: `3 × BaseVit + 2 × CharLevel + 13`. Lower VIT cap (350) balances out the better per-VIT scaling. Pure precision fighter. Excels with fast, light weapons.

---

### Shugoki

> An immovable behemoth in human form. Slow, unstoppable, devastating.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 20 → 500 | 0 → 50 | 25 → 450 | 20 → 500 |

**Starting gear:** Long Hatchet, ×2 Healing Potion

Highest STR (500) and VIT (500) caps of any Monk. HP formula is the best in the monk family: `3.5 × BaseVit + 3 × CharLevel + 12`. Virtually no magic access (MAG cap 50). The monk tank — slower and harder-hitting than Kensei/Shinobi, built to absorb punishment.

---

### Shinobi

> A shadow warrior who strikes without warning and vanishes before retaliation.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 20 → 250 | 0 → 150 | 25 → 750 | 20 → 350 |

**Starting gear:** Light Kanabo, Rounded Shield, Tanto, ×2 Healing Potion

Highest DEX cap of any Monk (750) — second-highest of any class after the archer line. Lowest STR of the monk sub-classes (250). Has some magic room (150). HP formula matches base Monk (×2.5/VIT). Plays like a rogue-monk hybrid — hit fast, hit hard with DEX, avoid getting cornered.

---

---

## Rogue

**Identity:** The highest base accuracy in the game (+55 to-hit). Good damage formula (STR + DEX), moderate HP scaling (×2/VIT), and the extremely useful Identify skill. Sits between Archer and Warrior in combat range — effective with both melee and ranged approaches depending on sub-class.

**Damage formula:** `(STR + DEX) / 200 × CharLevel`. Same split as Archer.

**Skill:** Identify — identify unknown items for free.

---

### Rogue (base)

> A versatile operative who blends combat skill with street-smarts.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 15 → 300 | 10 → 400 | 20 → 500 | 20 → 300 |

**Starting gear:** Light Hammer, Sharp Knife, ×2 Healing Potion

The only Rogue sub-class with a high magic cap (400) — essentially a DEX/MAG hybrid. Good DEX (500) and meaningful spell access. Versatile but neither the best physical attacker nor the best caster. Best for players who want to mix combat and spells.

---

### Assassin

> A deadly close-range striker who overwhelms targets with precision and speed.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 15 → 350 | 10 → 200 | 20 → 600 | 20 → 350 |

**Starting gear:** Claw I, Buckler, ×2 Healing Potion

Highest DEX (600) and highest STR (350) caps of the physical rogue builds. Lower magic cap (200) than base Rogue. Good VIT (350). Pure physical damage focus with claw weapons. The straightforward high-damage melee/rogue.

---

### Iron Maiden

> A battle-hardened warrior who wears her scars as armor.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 15 → 300 | 10 → 150 | 20 → 550 | 20 → 500 |

**Starting gear:** Light Hammer, Thorned Bundle, Sharp Knife, ×2 Healing Potion

Highest VIT cap of any Rogue (500) — this is the tank variant. HP formula improves to `3 × BaseVit + CharLevel/5 + 25` (note: limited per-level scaling, compensated by high VIT cap). Lowest magic cap (150). Unique mechanic: can switch her class skill from Identify to Item Repair. The defensive, self-sufficient rogue.

---

### Bombardier

> An alchemist-warrior who turns the battlefield into a killing field of traps and explosions.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 15 → 300 | 15 → 450 | 20 → 500 | 15 → 250 |

**Starting gear:** Toxic Vials, LWS, ×2 Healing Potion

Highest magic cap of any Rogue (450) — a magical rogue who leans on explosive consumables and trap-type items. Lowest VIT cap (250) of all rogues. HP formula is `1.5 × BaseVit + CharLevel + 22`.

---

---

## Savage

**Identity:** The game's brawler class. Best HP scaling in the game (×3.5/VIT, higher in some sub-classes). High strength, minimal or zero magic. Skill is Reflect. Damage is strength-only. Takes double vitality-loss penalties like Warriors. Has a unique critical hit mechanic (2× crit damage at 10% chance per level).

**Damage formula:** Strength-only (`STR / 100 × CharLevel`).

**Skill:** Reflect — reflect projectiles back at enemies.

---

### Savage (base)

> A primal warrior who fights on instinct and raw muscle.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 30 → 800 | 0 → 50 | 10 → 200 | 25 → 450 |

**Starting gear:** Long Mace, Estoc, ×2 Healing Potion

The reference brawler. Very high STR ceiling (800), almost no magic or DEX. Good VIT cap (450). The savage playstyle in pure form — charge in, hit hard, rely on HP to survive.

---

### Berserker

> A warrior consumed by battle rage who fights harder as wounds mount.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 30 → 650 | 0 → 50 | 10 → 300 | 25 → 500 |

**Starting gear:** Hatchet, ×2 Healing Potion

Lower STR cap than base Savage (650 vs 800) but more VIT (500) and more DEX (300). Still ×3.5 HP scaling. The more balanced brawler — trades peak damage for better survivability and slightly wider gear options from the DEX boost.

---

### Executioner

> A merciless killing machine with no concept of defense.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 30 → 900 | 0 → 25 | 10 → 25 | 25 → 550 |

**Starting gear:** Hatchet, ×2 Healing Potion

Highest STR cap in the entire game (900). Nearly zero DEX (cap 25) and minimal magic (cap 25). But the HP formula is the best available: `4.5 × BaseVit + 3 × CharLevel` — can reach enormous HP totals with 550 VIT. An extreme specialist: pick this only if you intend to max strength and never look back. Gear restrictions due to DEX floor will be significant.

---

## Savage › Gladiator

**Identity:** Roman gladiator archetypes. Each variant has distinct armor and weapon styles drawn from historical fighter categories. Slightly more moderate stats than the base Savage but more specialized identities. HP scaling is ×3.0/VIT (slightly less than base Savage's ×3.5 for most variants).

---

### Thraex

> Light and agile, the Thraex strikes fast with a curved blade and small shield.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 20 → 550 | 0 → 150 | 20 → 400 | 25 → 400 |

**Starting gear:** Light Dagger, Buckler, ×2 Healing Potion

The most versatile Gladiator — some magic access (150), decent DEX (400), solid STR (550). HP formula: `3 × BaseVit + 2 × CharLevel + 18`. Note: the "Blood for Blood" trait removes per-level HP scaling entirely, which can be a significant downside if taken unintentionally. The Gladiator option for players who want some magical flexibility.

---

### Murmillo

> A heavily armored fighter who hides behind a great shield and strikes with overwhelming force.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 25 → 700 | 0 → 50 | 10 → 250 | 30 → 500 |

**Starting gear:** Light Dagger, Buckler, Light Hammer, ×2 Healing Potion

Starts with the highest base VIT of any class in the game (30). High STR cap (700) and strong VIT cap (500) with a solid HP formula: `3.5 × BaseVit + 3 × CharLevel + 2`. Very low DEX. The Gladiator tank — best compared to Shugoki (Monk) but with slightly better mana scaling and the Reflect skill.

---

### Dimachaerus

> The two-weapon fighter of the arena — two blades, relentless offense.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 25 → 450 | 0 → 100 | 20 → 550 | 20 → 400 |

**Starting gear:** Light Mace, Light Dagger, ×2 Healing Potion

The dual-wield specialist. Highest DEX cap of the Gladiator variants (550) but the lowest STR cap (450). Some magic (100). HP formula matches standard Warrior at `3 × BaseVit + 2 × CharLevel + 18`. Starts with two weapons in hand.

---

### Secutor

> A pursuer who never lets prey escape. Fast for a heavily armored fighter.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 25 → 550 | 0 → 100 | 15 → 400 | 25 → 450 |

**Starting gear:** Light Dagger, Buckler, Light Hammer, ×2 Healing Potion

Balanced Gladiator variant. STR (550), DEX (400), VIT (450) are all solid without extremes. Some magic room (100). HP formula: `3 × BaseVit + 2 × CharLevel + 18`. The all-around pick for Gladiator — no weaknesses but no standout strength either.

---

### Druid

> A primal shaman who blends savage strength with nature magic.

| STR | MAG | DEX | VIT |
|---|---|---|---|
| 20 → 400 | 10 → 350 | 15 → 350 | 20 → 400 |

**Starting gear:** (nature-themed starting item), ×2 Healing Potion

The most unique Savage sub-class — a genuine STR/MAG hybrid. The only Savage with meaningful magic access (350 cap). Also gets the highest to-hit bonus of the Savage family (+55, matching Rogue). HP formula matches Mage Summoner: `BaseVit + 2 × CharLevel + 8` — notably weaker than other Savages despite being a Savage sub-class. A deliberate trade: give up the brawler's HP advantage to gain spells.

---
