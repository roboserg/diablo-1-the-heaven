#!/usr/bin/env python3
"""Replace individual subclass IPCM_ masks with base-class ANY_ or ALL_CLASSES.

Usage:
    python3 tools/fix_class_mask.py

Scans src/BaseItems.cpp for RequiredClassMask fields containing individual
subclass IPCM_* values and replaces them with their base-class IPCM_ANY_*
grouping. Cross-class compounds (e.g. Warrior+Monk) become IPCM_ALL_CLASSES.

Subclass -> base class mapping:
    ANY_WARRIOR: Warrior, Inquisitor, Guardian, Templar, Savage
    ANY_ROGUE:   Rogue, Assassin, Iron Maiden, Bombardier
    ANY_ARCHER:  Archer, Scout, Sharpshooter, Trapper
    ANY_MAGE:    Mage, Elementalist, Necromancer, Demonologist, Beastmaster, Warlock
    ANY_MONK:    Monk, Kensei, Shinobi, Shugoki
    ANY_SAVAGE:  Berserker, Executioner, Thraex, Murmillo, Dimachaerus, Secutor, Druid
"""

import re
import sys
import os

# Subclass -> base class family mapping
SUBCLASS_MAP = {
    "WARRIOR": "WARRIOR",
    "INQUISITOR": "WARRIOR",
    "GUARDIAN": "WARRIOR",
    "TEMPLAR": "WARRIOR",
    "SAVAGE": "WARRIOR",
    "ROGUE": "ROGUE",
    "ASSASSIN": "ROGUE",
    "IRON_MAIDEN": "ROGUE",
    "BOMBARDIER": "ROGUE",
    "ARCHER": "ARCHER",
    "SCOUT": "ARCHER",
    "SHARPSHOOTER": "ARCHER",
    "TRAPPER": "ARCHER",
    "MAGE": "MAGE",
    "ELEMENTALIST": "MAGE",
    "NECROMANCER": "MAGE",
    "DEMONOLOGIST": "MAGE",
    "BEASTMASTER": "MAGE",
    "WARLOCK": "MAGE",
    "MONK": "MONK",
    "KENSEI": "MONK",
    "SHINOBI": "MONK",
    "SHUGOKI": "MONK",
    "BERSERKER": "SAVAGE",
    "EXECUTIONER": "SAVAGE",
    "THRAEX": "SAVAGE",
    "MURMILLO": "SAVAGE",
    "DIMACHAERUS": "SAVAGE",
    "SECUTOR": "SAVAGE",
    "DRUID": "SAVAGE",
    # Intermediate compound masks
    "ANY_GLADIATOR": "SAVAGE",
    "ANY_SUMMONER": "MAGE",
    "ANY_EXILE": "SAVAGE",
}

# Already-target values (no change needed)
TARGET_VALUES = {
    "ALL_CLASSES",
    "ANY_WARRIOR",
    "ANY_ROGUE",
    "ANY_ARCHER",
    "ANY_MAGE",
    "ANY_MONK",
    "ANY_SAVAGE",
}

IPC_PATTERN = re.compile(r"IPCM_(\w+)")


def get_family(ipcm_name):
    """Get base class family for an IPCM_ name, or None if unknown."""
    if ipcm_name in TARGET_VALUES:
        return ipcm_name
    return SUBCLASS_MAP.get(ipcm_name)


def map_expression(expr):
    """Map an IPCM_ mask expression to its simplified form.

    All components within the same base class family -> IPCM_ANY_<family>.
    Mixed families -> IPCM_ALL_CLASSES.
    """
    components = [m.group(1) for m in IPC_PATTERN.finditer(expr)]
    if not components:
        return None

    families = set()
    for comp in components:
        fam = get_family(comp)
        if fam is None:
            return None  # unknown component, skip
        families.add(fam)

    if len(families) == 1:
        fam = families.pop()
        if fam in TARGET_VALUES:
            return f"IPCM_{fam}"
        return f"IPCM_ANY_{fam}"

    return "IPCM_ALL_CLASSES"


# Pattern to find the IPCM_ mask field (before ,\tfalse or ,\tFALSE)
ITEM_PATTERN = re.compile(r"(,\t)(IPCM_[^,\t]+?)(,\t(?:false|FALSE),)")


def process_file(filepath):
    with open(filepath, "r", newline="") as f:
        lines = f.readlines()

    changed = 0
    output = []

    for lineno, line in enumerate(lines, 1):
        if not line.lstrip().startswith("{"):
            output.append(line)
            continue

        match = ITEM_PATTERN.search(line)
        if not match:
            output.append(line)
            continue

        old_expr = match.group(2)
        new_expr = map_expression(old_expr)
        if new_expr and new_expr != old_expr:
            new_line = line[: match.start(2)] + new_expr + line[match.end(2) :]
            changed += 1
            output.append(new_line)
        else:
            output.append(line)

    with open(filepath, "w", newline="") as f:
        f.writelines(output)

    return changed


if __name__ == "__main__":
    script_dir = os.path.dirname(os.path.abspath(__file__))
    repo_root = os.path.dirname(script_dir)
    path = os.path.join(repo_root, "src", "BaseItems.cpp")

    if not os.path.exists(path):
        print(f"Error: {path} not found", file=sys.stderr)
        sys.exit(1)

    n = process_file(path)
    print(f"Modified {n} lines in {path}")
