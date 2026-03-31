#!/usr/bin/env python3
"""Print distribution of ITEM_PLAYER_CLASS_MASK (RequiredClassMask) across BaseItems, UniqueItems, and UniqueSets."""

import re
from collections import Counter

# IPCM named masks from enums.h
IPCM_MAP = {
    "IPCM_WARRIOR": "Warrior",
    "IPCM_INQUISITOR": "Inquisitor",
    "IPCM_GUARDIAN": "Guardian",
    "IPCM_TEMPLAR": "Templar",
    "IPCM_ARCHER": "Archer",
    "IPCM_SCOUT": "Scout",
    "IPCM_SHARPSHOOTER": "Sharpshooter",
    "IPCM_TRAPPER": "Trapper",
    "IPCM_MAGE": "Mage",
    "IPCM_ELEMENTALIST": "Elementalist",
    "IPCM_DEMONOLOGIST": "Demonologist",
    "IPCM_NECROMANCER": "Necromancer",
    "IPCM_BEASTMASTER": "Beastmaster",
    "IPCM_WARLOCK": "Warlock",
    "IPCM_MONK": "Monk",
    "IPCM_KENSEI": "Kensei",
    "IPCM_SHUGOKI": "Shugoki",
    "IPCM_SHINOBI": "Shinobi",
    "IPCM_ROGUE": "Rogue",
    "IPCM_ASSASSIN": "Assassin",
    "IPCM_IRON_MAIDEN": "Iron Maiden",
    "IPCM_BOMBARDIER": "Bombardier",
    "IPCM_SAVAGE": "Savage",
    "IPCM_BERSERKER": "Berserker",
    "IPCM_EXECUTIONER": "Executioner",
    "IPCM_THRAEX": "Thraex",
    "IPCM_MURMILLO": "Murmillo",
    "IPCM_DIMACHAERUS": "Dimachaerus",
    "IPCM_SECUTOR": "Secutor",
    "IPCM_DRUID": "Druid",
    "IPCM_ANY_WARRIOR": "Any Warrior",
    "IPCM_ANY_ARCHER": "Any Archer",
    "IPCM_ANY_MAGE": "Any Mage",
    "IPCM_ANY_SUMMONER": "Any Summoner",
    "IPCM_ANY_MONK": "Any Monk",
    "IPCM_ANY_ROGUE": "Any Rogue",
    "IPCM_ANY_GLADIATOR": "Any Gladiator",
    "IPCM_ANY_EXILE": "Any Exile",
    "IPCM_ANY_SAVAGE": "Any Savage",
    "IPCM_ALL_CLASSES": "All Classes",
}


def parse_base_items():
    """Parse BaseItems.cpp -> {baseId: classMaskLabel}"""
    with open("src/BaseItems.cpp", "r") as f:
        content = f.read()

    base_map = {}  # baseId -> mask label
    for line in content.split("\n"):
        line = line.strip()
        if not line.startswith("{"):
            continue
        # baseId is first field after '{'
        id_m = re.match(r"\{\s*(\d+),", line)
        mask_m = re.search(r"(IPCM_\w+)", line)
        if id_m and mask_m:
            base_id = int(id_m.group(1))
            mask_name = mask_m.group(1)
            base_map[base_id] = IPCM_MAP.get(mask_name, mask_name)
    return base_map


def parse_unique_items():
    """Parse UniqueItems.cpp -> {uniqId: baseId}"""
    with open("src/UniqueItems.cpp", "r") as f:
        content = f.read()

    uniq_map = {}  # uniqId -> baseId
    for line in content.split("\n"):
        line = line.strip()
        if not line.startswith("{"):
            continue
        # Format: { uniqId, "Name"[/*comment*/], baseId, ...
        m = re.match(r"\{\s*(\d+),\s*\"[^\"]*\"(?:/[^,]*)?,\s*(\d+)", line)
        if m:
            uniq_id = int(m.group(1))
            base_id = int(m.group(2))
            uniq_map[uniq_id] = base_id
    return uniq_map


def parse_unique_sets():
    """Parse UniqueSets.cpp -> list of (setName, [uniqIds])"""
    with open("src/UniqueSets.cpp", "r") as f:
        content = f.read()

    sets = []
    set_pattern = re.compile(r'\{\s*"([^"]+)"')
    id_pattern = re.compile(r"(\d+)\s*(?:/\*[^*]*\*/)?")

    # Split by set markers /*N*/
    set_blocks = re.split(r"/\*\d+\*/", content)
    for block in set_blocks:
        nm = set_pattern.search(block)
        if not nm:
            continue
        set_name = nm.group(1)

        # Find the UniquesList section: lines between "UniquesList" comment and "EffectCount"
        lines = block.split("\n")
        in_list = False
        ids = []
        for line in lines:
            if "UniquesList" in line:
                in_list = True
                continue
            if in_list:
                if "EffectCount" in line:
                    break
                # Extract numbers from this line
                for m in id_pattern.finditer(line):
                    val = int(m.group(1))
                    # Skip very small numbers that are likely counts (UniquesCount is before UniquesList)
                    ids.append(val)
        if ids:
            sets.append((set_name, ids))

    return sets


def print_table(base_counter, uniq_counter, set_counter):
    all_masks = sorted(
        set(base_counter) | set(uniq_counter) | set(set_counter),
        key=lambda m: (
            base_counter.get(m, 0) + uniq_counter.get(m, 0) + set_counter.get(m, 0)
        ),
        reverse=True,
    )

    base_total = sum(base_counter.values())
    uniq_total = sum(uniq_counter.values())
    set_total = sum(set_counter.values())

    def pct(count, total):
        return f"{100 * count / total:.1f}%" if total else "0.0%"

    header = f"{'Mask':<18} {'BaseItems':>10} {'%':>6} {'UniqItems':>10} {'%':>6} {'UniqSets':>10} {'%':>6}"
    sep = "-" * len(header)

    print(header)
    print(sep)
    for mask in all_masks:
        bc = base_counter.get(mask, 0)
        uc = uniq_counter.get(mask, 0)
        sc = set_counter.get(mask, 0)
        print(
            f"{mask:<18} {bc:>10} {pct(bc, base_total):>6} {uc:>10} {pct(uc, uniq_total):>6} {sc:>10} {pct(sc, set_total):>6}"
        )
    print(sep)
    print(
        f"{'TOTAL':<18} {base_total:>10} {'100%':>6} {uniq_total:>10} {'100%':>6} {set_total:>10} {'100%':>6}"
    )
    print()


def main():
    base_map = parse_base_items()
    uniq_map = parse_unique_items()
    unique_sets = parse_unique_sets()

    base_counter = Counter(base_map.values())

    uniq_counter = Counter()
    for uniq_id, base_id in uniq_map.items():
        mask = base_map.get(base_id, f"Unknown(baseId={base_id})")
        uniq_counter[mask] += 1

    set_counter = Counter()
    for set_name, uniq_ids in unique_sets:
        for uid in uniq_ids:
            base_id = uniq_map.get(uid)
            if base_id is not None:
                mask = base_map.get(base_id, f"Unknown(baseId={base_id})")
                set_counter[mask] += 1
            else:
                set_counter[f"Unknown(uniqId={uid})"] += 1

    print_table(base_counter, uniq_counter, set_counter)


if __name__ == "__main__":
    main()
