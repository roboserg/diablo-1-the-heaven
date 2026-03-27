# Traits Guide

Traits are permanent character modifiers chosen at character creation. Each character can select up to **4 traits**. Every trait grants a benefit and imposes a penalty. Some trait combinations are mutually exclusive.

> **CLVL** = current character level. **MAG** = magic stat. **DFE** = damage from enemies.

---

## Universal Traits

### Gifted
- **Classes:** All
- **Benefit:** +5 to base stats and +25 to maximum stats
- **Penalty:** -(CLVLx2) to mana
- Excellent for physical builds where mana barely matters. Strong early game boost.

### Skilled
- **Classes:** All
- **Benefit:** +CLVL+(CLVL²/150) to all current stats
- **Penalty:** Perk rate reduced by CLVL/2
- Great stat scaling but fewer perks long-term. Best on stat-hungry classes.

### Barbarism
- **Classes:** All
- **Benefit:** Magical damage resistance cap increased to 90%
- **Penalty:** -(CLVL/5) mana regeneration
- Extremely powerful in late-game where magic damage is lethal.

### Survivor
- **Classes:** All
- **Benefit:** +CLVLx2 to life regeneration
- **Penalty:** -CLVL to all resistances
- Meaningful regen, but resistance loss becomes painful deep in the dungeon.

### Weird
- **Classes:** All
- **Benefit:** +(CLVLx2) to life
- **Penalty:** Spell manacost is doubled
- Near-useless for casters. Good for physical classes that want extra HP.

### Domesticated
- **Classes:** All
- **Benefit:** +30% to experience gains
- **Penalty:** 1 less stat per level-up
- Losing a stat point per level compounds significantly over time.

### Scrounger
- **Classes:** All
- **Benefit:** +50% magic find
- **Penalty:** -(1+CLVL/15) to stun threshold
- Solid for farming builds. Stun threshold loss gets noticeable at high levels.

### Adventurer
- **Classes:** All
- **Benefit:** 1 more stat point per level-up
- **Penalty:** -20% life, -20% mana
- Extra stats accumulate nicely over 50 levels. The HP/mana cut requires gear compensation.

### Forgetful
- **Classes:** All
- **Benefit:** +CLVL²+100 to experience gains
- **Penalty:** Constantly lose experience points: (CLVL²/10)
- Passive XP drain likely outweighs the bonus in most scenarios.

### Fast Metabolism
- **Classes:** All
- **Benefit:** +(CLVL/10) to life & mana regeneration
- **Penalty:** -CLVL to Acid Resistance
- Modest regen bonus; acid resistance loss becomes significant at higher levels.

### Treasure Hunter
- **Classes:** All
- **Benefit:** Unique monsters drop one more item on death
- **Penalty:** Goldfind and Magicfind reduced by (CLVL/2+75)%
- Trades general loot quality for extra unique drops. Poor if you rely on MF gear.

### Prodigy
- **Classes:** All
- **Benefit:** Get Synergy points every 4th level
- **Penalty:** Base spell levels reduced by CLVL/18
- More frequent synergies allow versatile builds. Spell level loss mostly irrelevant for physical classes.

### Unshakable
- **Classes:** All
- **Benefit:** Stun threshold increased by (40%CLVL)
- **Penalty:** Armor class reduced by CLVL
- Not being stunned is invaluable. AC loss can be offset by gear.

### Crowd Seeker
- **Classes:** All (single-player only)
- **Benefit:** Additional 25% chance to avoid item durability loss
- **Penalty:** Monster chain activation radius increased by +2
- More monsters aggro at once. Durability savings rarely justify the increased crowd risk.

### Blue Blood
- **Classes:** All except Savages
- **Benefit:** All starting attributes raised by 10 points, +10% XP gains
- **Penalty:** -1 to stats to distribute on level-up
- Strong early game boost and XP bonus. Per-level stat reduction is relatively mild.

### Insensitive
- **Classes:** All except Mages
- **Benefit:** Armor class raised by (CLVL/2)
- **Penalty:** Synergy points only every 7th level; mana regen reduced by (CLVL/9)
- AC is welcome but slower synergy gain and reduced mana regen are real costs.

### Sisyphean Task
- **Classes:** Most (excludes Templar, pure Mage)
- **Benefit:** Current life and mana increased by (2xCLVL)
- **Penalty:** Total XP gains reduced by (75+CLVL/10)% — includes perks and item effects
- The XP reduction is devastating. Only consider for specific challenge builds.

---

## Warrior Traits

### Kamikaze
- **Classes:** Warriors, Monks, Savages
- **Benefit:** +CLVL to accuracy
- **Penalty:** -(CLVL/3) to armor class
- Accuracy is generally easy to cap from other sources. AC loss is a steep price.

### Heavy Handed
- **Classes:** Warriors, Monks, Savages
- **Benefit:** +(80%CLVL) to physical melee damage
- **Penalty:** -(1+CLVL/26) to all spell levels
- Excellent physical damage scaling. Spell penalty is irrelevant for physical builds.

### Thick Skinned
- **Classes:** Warriors, Archer, Scout, Monks, Savages
- **Benefit:** +(CLVL/9) to melee and arrow damage resistance
- **Penalty:** -66%CLVL to accuracy
- The accuracy penalty is severe (-33 at level 50). Resistance gains are modest by comparison.

### Ratel Hide
- **Classes:** Warriors, Monks, Rogues, Savages
- **Benefit:** -(CLVL/7) to DFE
- **Penalty:** -35% to knockback resistance
- Reduces enemy armor effectiveness. KB reduction is annoying in corridors.

### Sandman
- **Classes:** Warriors, Monks, Iron Maiden, Savages
- **Benefit:** Melee damage resist +5; caps +10% to minimum and maximum
- **Penalty:** Minimum chance to be hit in melee raised by 1+CLVL/15%
- Autohit increase means enemies always land some hits regardless of AC.

### Fechtmeister
- **Classes:** Warrior only
- **Benefit:** 1 frame faster melee attacks, +(CLVL/2) to armor, melee autohit reduced by 4%
- **Penalty:** Cannot use shields
- Fast unshielded Warrior. Attack speed and armor bonuses are strong. Losing block entirely is the trade.

### Ranger
- **Classes:** Warrior only
- **Benefit:** Embraces the unique skills of an Archer
- **Penalty:** Does not use any weapons but bows
- Turns the Warrior into a bow character with Warrior stat growth.

---

## Archer Traits

### Finesse
- **Classes:** Warriors, Archer, Sharpshooter, Monks, Rogue, Assassin, Berserker, Executioner, Murmillo, Thraex, Dimachaerus, Secutor, Druid
- **Benefit:** Critical strike chance increased by (CLVL/27+10)%
- **Penalty:** -50% base damage
- Very dependent on crit damage scaling. The 50% base damage cut is steep.

### Cautious
- **Classes:** Sharpshooter only
- **Benefit:** +30% damage to targets further than point blank range
- **Penalty:** -50% damage at point blank range
- Encourages kiting. Strong if you can consistently stay at range.

### Arrow Dancing
- **Classes:** Scout, Monk, Kensei, Rogue, Berserker, Dimachaerus
- **Benefit:** +(CLVL/2) to arrow damage resistance
- **Penalty:** -(CLVL/2) to melee damage resistance
- Pure resistance repositioning. Good if you face more ranged than melee enemies.

### Point Blank
- **Classes:** Archer only
- **Benefit:** +65% total damage at point blank range
- **Penalty:** -25% damage to targets further than point blank
- Aggressive Archer playstyle. Works well with melee-range bow skills.

### Piercing Shot
- **Classes:** Sharpshooter only
- **Benefit:** (CLVL/3)% chance for arrows to pass through enemies
- **Penalty:** -(2xCLVL) to accuracy
- Great for corridor crowd control. Accuracy penalty is harsh (-100 at level 50).

### Strafer
- **Classes:** Scout only
- **Benefit:** Multishot upgraded to 5 arrows with manual control
- **Penalty:** Attacking takes 1 frame longer
- Transforms the Scout's signature skill. Manual control is excellent for precise targeting.

### Crossbow Training
- **Classes:** Guardian only
- **Benefit:** 150% of Dexterity added to base damage when using crossbows
- **Penalty:** +5% to autohit in ranged combat
- Massive DEX-to-damage conversion. Guardian has access to high DEX, making this scale very well.

### Pistoleer
- **Classes:** Guardian only
- **Benefit:** Use pistol for a weapon
- **Penalty:** No other weapons can be used
- Unique weapon type. Mutually exclusive with Crossbow Training.

---

## Mage Traits

### Wild Sorcery
- **Classes:** Mage, Elementalist, Warlock, Druid
- **Benefit:** All spell damage increased by 30%
- **Penalty:** You cannot drink potions that restore life or mana
- Forces reliance on regeneration and other survival means. Glass cannon defining trait.

### Psion
- **Classes:** Mage only
- **Benefit:** Nearby visible monsters take 3+CLVL×MAG/100 damage per second
- **Penalty:** Reduced base mana, no natural mana regeneration
- Passive AOE damage aura. Completely changes mana management. Unlocks Amplify Damage and Psychokinesis perks.

### Cleric
- **Classes:** Mage only
- **Benefit:** Damage of Holy spell types increased by 50%
- **Penalty:** Damage of other spell types decreased by 20%
- Full Holy magic specialization. 50% bonus greatly outweighs the 20% penalty if you commit to the school.

### Mana Flux
- **Classes:** Mage, Warlock, Summoners, Druid
- **Benefit:** +(1+2×CLVL/21) to spell levels
- **Penalty:** Maximum mana decreased by 20%
- Spell level bonuses directly boost damage. Max mana loss can be compensated with items.

### Avatar of Cold
- **Classes:** Elementalist only
- **Benefit:** +25% to damage of cold spells and to cold resistance
- **Penalty:** -25% to damage of fire spells and to fire resistance
- Pure cold specialization. Balanced trade if you avoid fire spells entirely.

### Mamluk
- **Classes:** Elementalist only
- **Benefit:** Embrace the unique skills of a desert warrior (melee)
- **Penalty:** Can't use offensive spells effectively
- Turns the Elementalist into a melee fighter with Mage stat growth.

### Hydramancer
- **Classes:** Warlock only
- **Benefit:** Get Hydra as skill with various elemental attacks
- **Penalty:** Most offensive spells have cooldowns
- Enables Hydra summon. Shifts playstyle toward pet-assisted combat.

### Dark Pact
- **Classes:** Warlock only
- **Benefit:** Damage of acid spells increased by CLVL%
- **Penalty:** Mana regeneration reduced by CLVL/2
- Huge acid damage scaling (+50% at level 50). Works well with high-mana builds.

### Necropathy
- **Classes:** Necromancer only
- **Benefit:** Monsters receive 20% more damage from minions when in your light radius
- **Penalty:** -50% basic mana regeneration, -CLVL to mana
- Requires keeping enemies in light radius. Mana pool and regen hurt significantly.

### The Best Defense...
- **Classes:** Any Summoner
- **Benefit:** +(25+CLVL/15)% to damage of summoned minions
- **Penalty:** -(25+CLVL/15)% to total hit points of summoned minions
- Glass cannon minions. They kill faster but die faster. Synergizes with Cannibalism.

### Breaker of Stones
- **Classes:** Demonologist only
- **Benefit:** Summons will attack Stonecursed monsters in melee (50% damage)
- **Penalty:** Stone Curse has a 120 second cooldown
- Very niche utility. 2-minute Stone Curse cooldown is a huge cost.

### Cannibalism
- **Classes:** Beastmaster only
- **Benefit:** Summoned minions leech 20% life from melee hits
- **Penalty:** Minimum chance to be hit in melee raised by 9% for minions
- Minion sustain at the cost of minion survivability. Pairs well with The Best Defense.

### Feral
- **Classes:** Beastmaster only
- **Benefit:** No summoner-specific offensive spell cooldowns
- **Penalty:** Cannot learn Teleport spell; half natural mana regen
- Removes spell cooldown limits. Loss of Teleport hurts mobility.

---

## Monk Traits

### Bloodless
- **Classes:** Monk only
- **Benefit:** Monsters cannot leech life from you
- **Penalty:** You cannot leech life from them either
- Symmetric trade. Useful in areas with heavy-leech enemies. Neutral for non-leech builds.

### Lord of the Rings
- **Classes:** Monk only
- **Benefit:** You can equip up to 8 rings
- **Penalty:** Perk rate reduced by CLVL/2
- Massive itemization potential. Perk rate reduction is steep. Best with powerful rings.

### Juggernaut
- **Classes:** Warriors, Monks, Savages (except pure Savage)
- **Benefit:** Knockback resistance set to 100%
- **Penalty:** Cannot learn Teleport spell
- Never being knocked back is very strong for melee. Losing Teleport is the mobility cost.

### Blistered Skin
- **Classes:** Kensei only
- **Benefit:** Base life regen depends on max life: +1 regen per 25 life
- **Penalty:** Magical damage resistance cap set to 75%
- Strong scaling regen for high-HP Kensei. Reduced magic resist cap is dangerous late.

### Hunger
- **Classes:** Shugoki only
- **Benefit:** Melee hits always leech additional 1% of total life and mana
- **Penalty:** Monsters have thicker armor (DFE) the deeper you venture
- Passive sustain on every hit. DFE scaling hurts damage output at deep levels.

### Petrifier
- **Classes:** Shinobi only
- **Benefit:** You can damage Stonecursed monsters (halved damage)
- **Penalty:** Stone Curse has a 60 second cooldown
- Very situational utility. Cooldown significantly impacts Shinobi's Stone Curse usage.

---

## Rogue Traits

### Gold Digger
- **Classes:** Rogue only
- **Benefit:** +(2×CLVL) gold dropped
- **Penalty:** Armor class reduced by (120%CLVL)
- The AC penalty is crippling on a class that relies on DEX-based defense.

### Big Frame
- **Classes:** Rogue only
- **Benefit:** +3 life per level and +3 life per Vitality point
- **Penalty:** Autohit increased by 3% melee, 2% ranged
- Makes Rogue significantly tankier in HP. Being easier to hit offsets some of that.

### Axepertise
- **Classes:** Rogue only
- **Benefit:** Use two-handed axes as melee weapon; stun threshold +2
- **Penalty:** You no longer use blunts and sharps
- Enables STR-based Rogue builds. Must plan gear entirely around axes.

### Black Witchery
- **Classes:** Assassin only
- **Benefit:** Significantly improved magical powers for ranged combat
- **Penalty:** No throwing knives
- Transforms the Assassin into a ranged magic user. Completely different playstyle.

### Puncturing Stab
- **Classes:** Assassin only
- **Benefit:** (CLVL/5)% chance to bypass melee damage resistance and DFE
- **Penalty:** -CLVL to accuracy
- Armor penetration is excellent against heavy targets. Accuracy penalty mitigable through DEX.

### A Rose with Thorns
- **Classes:** Iron Maiden only
- **Benefit:** Thorns damage from every item increased by (CLVL/2)
- **Penalty:** -(CLVL/3) to current and maximum block chance
- Enables thorns/reflect builds. Block loss is painful for a shield class.

### Wrecking Block
- **Classes:** Iron Maiden only
- **Benefit:** Blocking melee hits reflects damage, increased by (CLVL/2)%
- **Penalty:** Shields lose durability twice as fast
- Reflect on block is a fun offensive-defensive synergy. Ongoing repair cost.

### Two Towers
- **Classes:** Iron Maiden only
- **Benefit:** Wield dual shields; +(CLVL/6) to current and max block chance
- **Penalty:** Cannot use any melee weapons; melee autohit increased by 10%
- Pure defense build — very high block with no offensive weapons. Relies on thorns/reflect.

### Pyromaniac
- **Classes:** Bombardier only
- **Benefit:** Total damage from fire flasks increased by 25%
- **Penalty:** Dexterity doesn't increase armor class
- Good for flask-focused builds. Losing DEX-to-AC is significant.

### Toxic at Heart
- **Classes:** Bombardier only
- **Benefit:** Acid burns monsters 80% faster
- **Penalty:** Ability to score critical hits is lost; -40% acid damage
- Faster acid burn but losing 40% acid damage and all crits is a steep cost.

### Engineer
- **Classes:** Trapper only
- **Benefit:** Trap damage passively increased by (15+CLVL/2)%
- **Penalty:** No gains to life or mana from level-ups
- Strong trap scaling but the Trapper stays fragile through the game.

---

## Savage Traits

### Rabid
- **Classes:** Savage, Executioner, Druid
- **Benefit:** Base damage is doubled for melee attacks
- **Penalty:** You cannot drink potions that restore life or mana
- Enormous damage multiplier. Requires regeneration or leech for survival instead of potions.

### Bouncer
- **Classes:** Savage only
- **Benefit:** Total melee damage increased by 50%
- **Penalty:** Attack speed decreased by 2 frames
- 50% damage is excellent but 2 frames of speed loss is severe. Net DPS depends on base speed.

### Destroyer
- **Classes:** Savage only
- **Benefit:** Total damage increased by 15%
- **Penalty:** Weapons break twice as fast
- 15% damage is modest for the constant repair costs. Especially bad with Barbarian (white items).

### Barbarian
- **Classes:** Savage only
- **Benefit:** +180%CLVL additional perk points; +CLVL/2 stun threshold
- **Penalty:** Uses only white items; can't read books (except Fury, Heal, Portal)
- Massive perk compensation for white-item restriction. A unique challenge run experience.

### Monkey Grip
- **Classes:** Savage only
- **Benefit:** Wield two-handed melee weapons with one hand
- **Penalty:** -(10+CLVL) accuracy and AC; -20% maximum chance to hit
- Enables dual-wielding giant weapons. Accuracy and AC penalties are severe.

### Leper
- **Classes:** Executioner only
- **Benefit:** +75 maximum base dexterity; +75 maximum base magic
- **Penalty:** Townspeople are afraid of you (except Gillian)
- Major stat cap increases with only a social/convenience downside.

### Old Habit
- **Classes:** Executioner only
- **Benefit:** Deal +200% damage to non-unique monsters in melee when they have 25% life or less
- **Penalty:** +(CLVL/8) damage from enemies
- Excellent farming and speed-clearing. Enemies die faster when low on HP.

### Armadillo
- **Classes:** Berserker only
- **Benefit:** Reflect layer amount increased by CLVL/13+2
- **Penalty:** Melee damage reduced by CLVL-1
- Reflect build — return more damage to attackers at the cost of direct damage.

### Adrenaline
- **Classes:** Berserker only
- **Benefit:** Life regeneration increased during active fighting
- **Penalty:** Life regeneration drops when standing still
- Rewards aggressive, continuous combat. Berserker playstyle is inherently active.

### Psychotic
- **Classes:** Berserker only
- **Benefit:** Fury makes you stronger and more resilient
- **Penalty:** You lose control under Fury and go berserking until it ends
- High-risk amplifier for Fury. Pairs naturally with Adrenaline.

### Bruiser
- **Classes:** Shinobi, Thraex, Dimachaerus, Secutor
- **Benefit:** +150 maximum base strength
- **Penalty:** -150 maximum base dexterity
- Stat redistribution favoring STR damage over DEX evasion.

### Bloody Mess
- **Classes:** Dimachaerus only
- **Benefit:** Melee crit damage increased by (CLVL/2)²+20 with extra bleeding
- **Penalty:** Melee autohit chance raised by 2%
- Crit damage scales enormously (645+20 at level 50). The 2% autohit increase is minor.

### Bestiarius
- **Classes:** Secutor only
- **Benefit:** Sharp weapon damage against beasts increased by 30%
- **Penalty:** Blunt weapon damage against undead lowered by 25%
- Situationally strong. Penalty only applies to blunt vs undead, which can be avoided.

### Old Fashioned
- **Classes:** Any Gladiator (Murmillo, Thraex, Dimachaerus, Secutor)
- **Benefit:** +(CLVL²/45+CLVL) armor, +(CLVL²/25) damage, +40 max base DEX
- **Penalty:** Can't wear body armor
- Unarmored Gladiator build. Naturally generates AC and damage through trait scaling.

### Rudiarius
- **Classes:** Any Gladiator
- **Benefit:** Start game with +750 gold
- **Penalty:** Start game with -5 to base vitality
- One-time gold bonus vs permanent stat loss. Gold loses value quickly.

### Blood for Blood
- **Classes:** Thraex only
- **Benefit:** Total melee damage increased by 30% when life drops below 40%
- **Penalty:** No gains to life from level-ups
- High-risk high-reward. Stay intentionally low on life to keep the damage bonus active.

### Blood and Sand
- **Classes:** Murmillo only
- **Benefit:** DFE can reduce incoming melee damage by 75% (standard limit is 50%)
- **Penalty:** -(CLVL/5) to life regeneration
- 75% max damage reduction is excellent. Makes Murmillo significantly tankier.

### Crupellarius
- **Classes:** Murmillo only
- **Benefit:** More melee & arrow resistance caps; +150%CLVL to AC; +100 max VIT
- **Penalty:** -50% base damage; -100 max STR; -10 accuracy
- Ultimate defensive Murmillo. Massive AC and VIT, poor offense. Pairs with Blood and Sand.

---

## Multi-Class Traits

### Negotiant
- **Classes:** Warriors, Archers, Monks, Rogues
- **Benefit:** Extra 10% shop discount; opportunities for better goods
- **Penalty:** -50 maximum base vitality
- Shop discount is a minor economic advantage. Losing 50 max VIT hurts survivability.

### Zealot
- **Classes:** Inquisitor, Guardian, Templar, Archer, Scout, Monks, Rogue, Assassin, Berserker, Executioner, Thraex, Secutor
- **Benefit:** Attack speed increased by 1 frame
- **Penalty:** -(5+CLVL) accuracy; -15% max chance to hit
- Attack speed is one of the most valuable stats. Accuracy penalty compensable through DEX.

### Thrill Seeker
- **Classes:** Scout, Kensei, Shugoki, Rogue, Iron Maiden, Savage, Berserker, Dimachaerus
- **Benefit:** +50% to experience gains when life is below 35%
- **Penalty:** -25% to experience gains when life is above 65%
- Rewards high-risk play. Synergizes with Blood for Blood and similar low-life builds.

### Grim Deal
- **Classes:** Archers, Warrior, Inquisitor, Guardian, Mage, Elementalist, Summoners, Monks, Rogues, Gladiators, Savage, Berserker, Druid
- **Benefit:** Get one more perk on each level-up
- **Penalty:** AC, %life, %mana, HP regen, DFE, stun threshold: weakened by 40%CLVL
- Extra perks per level is extremely valuable. All defensive stats suffer at 40%CLVL.

### Holy Aura
- **Classes:** Templar only
- **Benefit:** Nearby visible undead take 1+CLVL×MAG/40 damage per second
- **Penalty:** -20% to experience points earned
- Passive AOE vs undead. High MAG investment amplifies damage massively.

### Chromatic Skin
- **Classes:** Guardian, Templar, Shugoki, Assassin, Druid
- **Benefit:** +(50%CLVL) to all magic resistances
- **Penalty:** Monsters cannot be stunned by your attacks
- Excellent resistance scaling. Losing stun utility is painful for crowd control.

### Tormentor
- **Classes:** Inquisitor only
- **Benefit:** Elemental damage from weapons increased by 20%
- **Penalty:** Ability to score critical hits is totally lost
- For elemental weapon builds, crits are secondary to consistent elemental output.

### Devastator
- **Classes:** Inquisitor only
- **Benefit:** Access to elemental auras that damage nearby visible monsters
- **Penalty:** Must spend perk points on these auras
- Unlocks Searing, Static, Theurgic, and Toxic Aura perks. Strong for AOE and group play.

### Paladin
- **Classes:** Templar only
- **Benefit:** You get magical and spellcasting powers
- **Penalty:** Cannot use mallets and crossbows
- Hybrid divine caster. Unlocks Sanctity, Path of Mana, Might N Magic perks.

### Abnegation
- **Classes:** Templar only
- **Benefit:** Extra -2 DFE; +(CLVL/3) to MDR/ADR/all resistance
- **Penalty:** -40%CLVL to life & mana regeneration
- Excellent resistance and damage reduction stacking. Regen loss offset by spells and gear.

### Vigorous
- **Classes:** Warriors, Archer, Trapper, Shugoki, Assassin, Berserker, Thraex, Secutor, Druid
- **Benefit:** Maximum health increased by 35%
- **Penalty:** Attack speed reduced by 1 frame
- Massive survivability boost. 1 frame of speed is acceptable for tanky builds.

### Iron Fisted
- **Classes:** Warrior, Templar, Archer, Monks, Rogue, Iron Maiden, Berserker, Executioner, Gladiators
- **Benefit:** Damage increased by CLVL (melee & ranged)
- **Penalty:** Critical strike chance reduced by (CLVL/10+1)%
- Flat damage scaling is consistent. Crit reduction matters less if crits aren't your focus.

### Fear the Reaper
- **Classes:** Inquisitor, Guardian, Monks, Berserker, Gladiators, Druid
- **Benefit:** Get one more perk on each level-up
- **Penalty:** Initial life is 50; no life from base VIT or level-ups
- Extreme fragility for massive perk power. Life stays at 50 regardless of investment.

### Nasty Disposition
- **Classes:** Warrior, Guardian, Archer, Sharpshooter, Trapper, Monks, Savages
- **Benefit:** +(10+CLVL/3) to accuracy; +(25+CLVL/2)% to weapon damage
- **Penalty:** Vendor prices are tripled
- Both combat bonuses are excellent. Tripled prices manageable with Gold Find gear.

### Stoneform
- **Classes:** Warriors, Archers, Monks, Assassin, Iron Maiden, Savages (except pure Savage)
- **Benefit:** +CLVL to armor class
- **Penalty:** Vitality doesn't give natural resistances
- Solid AC scaling. Losing VIT-based resistances requires gear compensation.

### Lithe Build
- **Classes:** Warriors, Archers, Monks, Rogues (except Iron Maiden), Savages
- **Benefit:** +100 max base DEX; monsters have extra 8% chance to miss
- **Penalty:** Inventory size is halved
- Great evasion stats. Halved inventory is a major quality-of-life penalty.

### Blood Oath
- **Classes:** Warrior, Inquisitor, Archers, Mage, Elementalist, Monk, Shinobi, Rogues, Savage, most Gladiators, Druid
- **Benefit:** Character states will not change life & mana regeneration rate
- **Penalty:** DFE increased by (1+CLVL/16)
- Consistent regen regardless of state. DFE increase becomes a scaling vulnerability.

### Coinbound
- **Classes:** Most Warriors, Archers, Mages, Monks, some Rogues, Savage, Executioner, Berserker
- **Benefit:** Goldfind increased by 100%CLVL
- **Penalty:** All stats decreased by CLVL/5
- Goldfind scales well but stat reduction scales equally badly (-10 all stats at level 50).

### Doomwhorl
- **Classes:** Most classes except pure Warrior
- **Benefit:** Innate ranged damage resistance absorbs 20% more damage
- **Penalty:** +8% to autohit in ranged combat
- Good against ranged enemies. Autohit increase makes ranged attacks always deal some damage.

### Resolute Guard
- **Classes:** Warrior, Inquisitor, Archers, Mages, Monk, Kensei, Shinobi, Rogue, Iron Maiden, Bombardier, Savages
- **Benefit:** All magic resistances increased by (60%CLVL)
- **Penalty:** Stun threshold decreased by (30%CLVL)
- Excellent resistance scaling. Combine with Unshakable or stun-resist gear.

### Small Frame
- **Classes:** Warrior, Inquisitor, Templar, Archers, Mages, Monk, Kensei, Shinobi, Assassin, Bombardier, Secutor, Druid
- **Benefit:** Autohit improved (-5% melee, -3% ranged); base AC formula: DEX/3
- **Penalty:** Melee and arrow damage resist lowered (current -10, max -10%)
- Evasion-focused. Works best with extremely high DEX investment.

### Bend the Rules
- **Classes:** Warriors, Archers, Mages, Monk, Kensei, Shugoki, Rogues, Savage, Berserker
- **Benefit:** All base attributes can grow up to 900
- **Penalty:** Total base attribute sum limited at 1400
- Enables extreme stat specialization. Dump everything into 1-2 stats for focused builds.

### Giant
- **Classes:** Warriors, Archers, Monks (except Shinobi), Rogues (except pure Rogue), Savages
- **Benefit:** +1 life per base VIT; max VIT increased by 100
- **Penalty:** Potions are only half effective
- Enormous HP for VIT-stacking builds. Halved potions forces reliance on regen or leech.

### Fatality
- **Classes:** Warriors (except Inquisitor), Archers (except Scout), Monks (except base Monk), Rogue, Assassin, Savages (single-player only)
- **Benefit:** Additional 10% chance to score a 300% damage hit
- **Penalty:** Monsters can do the same
- High risk/reward. Enemies also gain the 300% damage chance, which gets dangerous late.

---
