#!/usr/bin/env python3
"""Compare class-mask accessibility between two BaseItems revisions.

Reads the "before" BaseItems.cpp directly from git, so no branch switching or
duplicate snapshot file is needed.

Examples:
    python tools/class_mask_diff.py
    python tools/class_mask_diff.py --before-ref abb89ee
    python tools/class_mask_diff.py --before-ref abb89ee --svg out/class-mask-diff.svg
"""

from __future__ import annotations

import argparse
import os
import re
import subprocess
import sys
from collections import Counter


MASK_LABELS = {
    "IPCM_ALL_CLASSES": "All Classes",
    "IPCM_ANY_WARRIOR": "Any Warrior",
    "IPCM_ANY_ROGUE": "Any Rogue",
    "IPCM_ANY_ARCHER": "Any Archer",
    "IPCM_ANY_MAGE": "Any Mage",
    "IPCM_ANY_MONK": "Any Monk",
    "IPCM_ANY_SAVAGE": "Any Savage",
}

DISPLAY_ORDER = [
    "IPCM_ALL_CLASSES",
    "IPCM_ANY_WARRIOR",
    "IPCM_ANY_ROGUE",
    "IPCM_ANY_ARCHER",
    "IPCM_ANY_MAGE",
    "IPCM_ANY_MONK",
    "IPCM_ANY_SAVAGE",
]


def repo_root() -> str:
    return os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def load_text(path: str) -> str:
    with open(path, "r", encoding="utf-8") as handle:
        return handle.read()


def git_show(ref: str, path: str) -> str:
    cmd = ["git", "show", f"{ref}:{path.replace(os.sep, '/')}"]
    result = subprocess.run(
        cmd,
        cwd=repo_root(),
        capture_output=True,
        text=True,
        encoding="utf-8",
        check=False,
    )
    if result.returncode != 0:
        raise RuntimeError(result.stderr.strip() or f"git show failed for {ref}:{path}")
    return result.stdout


def parse_base_masks(content: str) -> dict[int, str]:
    base_map: dict[int, str] = {}
    for raw_line in content.splitlines():
        line = raw_line.strip()
        if not line.startswith("{"):
            continue
        id_match = re.match(r"\{\s*(\d+),", line)
        mask_match = re.search(r"\b(IPCM_\w+)\b", line)
        if id_match and mask_match:
            base_map[int(id_match.group(1))] = mask_match.group(1)
    return base_map


def parse_unique_items(content: str) -> dict[int, int]:
    uniq_map: dict[int, int] = {}
    pattern = re.compile(r'\{\s*(\d+),\s*"[^"]*"(?:/\*[^*]*\*/)?\s*,\s*(\d+)')
    for raw_line in content.splitlines():
        line = raw_line.strip()
        if not line.startswith("{"):
            continue
        match = pattern.match(line)
        if match:
            uniq_map[int(match.group(1))] = int(match.group(2))
    return uniq_map


def parse_unique_sets(content: str) -> list[tuple[str, list[int]]]:
    sets: list[tuple[str, list[int]]] = []
    set_pattern = re.compile(r'\{\s*"([^"]+)"')
    id_pattern = re.compile(r"(\d+)\s*(?:/\*[^*]*\*/)?")
    for block in re.split(r"/\*\d+\*/", content):
        name_match = set_pattern.search(block)
        if not name_match:
            continue
        set_name = name_match.group(1)
        ids: list[int] = []
        in_list = False
        for line in block.splitlines():
            if "UniquesList" in line:
                in_list = True
                continue
            if in_list:
                if "EffectCount" in line:
                    break
                ids.extend(int(match.group(1)) for match in id_pattern.finditer(line))
        if ids:
            sets.append((set_name, ids))
    return sets


def changed_base_ids(before: dict[int, str], after: dict[int, str]) -> dict[int, tuple[str, str]]:
    changed: dict[int, tuple[str, str]] = {}
    for base_id, new_mask in after.items():
        old_mask = before.get(base_id)
        if old_mask and old_mask != new_mask:
            changed[base_id] = (old_mask, new_mask)
    return changed


def count_by_new_mask(changed: dict[int, tuple[str, str]]) -> Counter[str]:
    counter: Counter[str] = Counter()
    for _base_id, (_old_mask, new_mask) in changed.items():
        counter[new_mask] += 1
    return counter


def count_unique_items(changed: dict[int, tuple[str, str]], uniq_map: dict[int, int]) -> Counter[str]:
    counter: Counter[str] = Counter()
    for _uniq_id, base_id in uniq_map.items():
        if base_id in changed:
            counter[changed[base_id][1]] += 1
    return counter


def count_unique_sets(
    changed: dict[int, tuple[str, str]],
    uniq_map: dict[int, int],
    sets: list[tuple[str, list[int]]],
) -> Counter[str]:
    counter: Counter[str] = Counter()
    for _set_name, uniq_ids in sets:
        for uniq_id in uniq_ids:
            base_id = uniq_map.get(uniq_id)
            if base_id in changed:
                counter[changed[base_id][1]] += 1
    return counter


def print_table(title: str, counter: Counter[str]) -> None:
    total = sum(counter.values())
    print(title)
    print("-" * len(title))
    if total == 0:
        print("No newly accessible items.")
        print()
        return
    for mask in DISPLAY_ORDER:
        count = counter.get(mask, 0)
        if count == 0:
            continue
        pct = 100.0 * count / total
        print(f"{MASK_LABELS[mask]:<13} {count:>5}  {pct:>5.1f}%")
    print(f"{'TOTAL':<13} {total:>5}  100.0%")
    print()


def print_transitions(changed: dict[int, tuple[str, str]]) -> None:
    transitions = Counter(changed.values())
    print("Transitions")
    print("-----------")
    for (old_mask, new_mask), count in transitions.most_common():
        old_label = MASK_LABELS.get(old_mask, old_mask.replace("IPCM_", ""))
        new_label = MASK_LABELS.get(new_mask, new_mask.replace("IPCM_", ""))
        print(f"{old_label:<18} -> {new_label:<13} {count:>5}")
    print()


def print_transition_table(changed: dict[int, tuple[str, str]]) -> None:
    transitions = Counter(changed.values())
    print("Newly Accessible BaseItems By Transition")
    print("----------------------------------------")
    print(f"{'From':<18} {'To':<13} {'Count':>5}")
    print(f"{'-' * 18} {'-' * 13} {'-' * 5}")
    for (old_mask, new_mask), count in transitions.most_common():
        old_label = MASK_LABELS.get(old_mask, old_mask.replace("IPCM_", ""))
        new_label = MASK_LABELS.get(new_mask, new_mask.replace("IPCM_", ""))
        print(f"{old_label:<18} {new_label:<13} {count:>5}")
    print()


def write_svg(path: str, counter: Counter[str], title: str) -> None:
    bars = [(MASK_LABELS[mask], counter.get(mask, 0)) for mask in DISPLAY_ORDER if counter.get(mask, 0) > 0]
    if not bars:
        bars = [("No changes", 0)]

    width = 820
    height = 100 + 56 * len(bars)
    left = 190
    right = 40
    bar_width = width - left - right
    max_value = max((value for _, value in bars), default=1) or 1

    lines = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">',
        '<style>',
        "text { font-family: Segoe UI, Arial, sans-serif; fill: #1f2937; }",
        ".title { font-size: 24px; font-weight: 700; }",
        ".label { font-size: 16px; }",
        ".value { font-size: 15px; font-weight: 600; }",
        "</style>",
        f'<rect width="{width}" height="{height}" fill="#fcfcfd" />',
        f'<text class="title" x="32" y="42">{xml_escape(title)}</text>',
    ]

    for index, (label, value) in enumerate(bars):
        y = 78 + 56 * index
        bar_len = 0 if max_value == 0 else int(bar_width * value / max_value)
        lines.append(f'<text class="label" x="32" y="{y + 18}">{xml_escape(label)}</text>')
        lines.append(f'<rect x="{left}" y="{y}" width="{bar_width}" height="24" rx="6" fill="#e5e7eb" />')
        lines.append(f'<rect x="{left}" y="{y}" width="{bar_len}" height="24" rx="6" fill="#2563eb" />')
        lines.append(f'<text class="value" x="{left + bar_len + 10}" y="{y + 18}">{value}</text>')

    lines.append("</svg>")

    with open(path, "w", encoding="utf-8", newline="\n") as handle:
        handle.write("\n".join(lines))


def xml_escape(value: str) -> str:
    return (
        value.replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace('"', "&quot;")
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--before-ref",
        default="HEAD~1",
        help="Git ref to use as the old BaseItems.cpp revision (default: HEAD~1).",
    )
    parser.add_argument(
        "--svg",
        help="Optional path for an SVG bar chart of newly accessible BaseItems by new mask.",
    )
    args = parser.parse_args()

    root = repo_root()
    before_text = git_show(args.before_ref, "src/BaseItems.cpp")
    after_text = load_text(os.path.join(root, "src", "BaseItems.cpp"))
    unique_items_text = load_text(os.path.join(root, "src", "UniqueItems.cpp"))
    unique_sets_text = load_text(os.path.join(root, "src", "UniqueSets.cpp"))

    before_map = parse_base_masks(before_text)
    after_map = parse_base_masks(after_text)
    uniq_map = parse_unique_items(unique_items_text)
    sets = parse_unique_sets(unique_sets_text)

    changed = changed_base_ids(before_map, after_map)
    base_counter = count_by_new_mask(changed)
    uniq_counter = count_unique_items(changed, uniq_map)
    set_counter = count_unique_sets(changed, uniq_map, sets)

    print(f"Compared {args.before_ref}:src/BaseItems.cpp -> working tree src/BaseItems.cpp")
    print(f"Changed base-item class masks: {len(changed)}")
    print()
    print_transitions(changed)
    print_transition_table(changed)
    print_table("Newly Accessible BaseItems By New Mask", base_counter)
    print_table("Affected UniqueItems By New Mask", uniq_counter)
    print_table("Affected UniqueSets Entries By New Mask", set_counter)

    if args.svg:
        svg_path = args.svg
        if not os.path.isabs(svg_path):
            svg_path = os.path.join(root, svg_path)
        os.makedirs(os.path.dirname(svg_path), exist_ok=True)
        write_svg(svg_path, base_counter, "Newly Accessible BaseItems By New Mask")
        print(f"Wrote SVG chart: {svg_path}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
