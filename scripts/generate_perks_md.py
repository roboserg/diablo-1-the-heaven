from __future__ import annotations

import argparse
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ENUMS_PATH = ROOT / "include" / "enums.h"
COMMON_PERKS_PATH = ROOT / "src" / "PerksData.cpp"
SYNERGY_PERKS_PATH = ROOT / "src" / "SynergyData.cpp"
DEFAULT_OUTPUT_PATH = ROOT / "docs" / "PERKS.md"
ALL_CLASSES: list[str] = []
CLASS_FAMILY_ORDER = ["Warrior", "Archer", "Mage", "Monk", "Rogue", "Savage"]
CLASS_TO_FAMILY = {
    "Warrior": "Warrior",
    "Inquisitor": "Warrior",
    "Guardian": "Warrior",
    "Templar": "Warrior",
    "Archer": "Archer",
    "Scout": "Archer",
    "Sharpshooter": "Archer",
    "Trapper": "Archer",
    "Mage": "Mage",
    "Elementalist": "Mage",
    "Demonologist": "Mage",
    "Necromancer": "Mage",
    "Beastmaster": "Mage",
    "Warlock": "Mage",
    "Monk": "Monk",
    "Kensei": "Monk",
    "Shugoki": "Monk",
    "Shinobi": "Monk",
    "Rogue": "Rogue",
    "Assassin": "Rogue",
    "Iron Maiden": "Rogue",
    "Bombardier": "Rogue",
    "Savage": "Savage",
    "Berserker": "Savage",
    "Executioner": "Savage",
    "Thraex": "Savage",
    "Murmillo": "Savage",
    "Dimachaerus": "Savage",
    "Secutor": "Savage",
    "Druid": "Savage",
}


def strip_comments(text: str) -> str:
    result: list[str] = []
    i = 0
    in_string = False
    while i < len(text):
        ch = text[i]
        nxt = text[i + 1] if i + 1 < len(text) else ""
        if in_string:
            result.append(ch)
            if ch == "\\" and i + 1 < len(text):
                result.append(text[i + 1])
                i += 2
                continue
            if ch == '"':
                in_string = False
            i += 1
            continue
        if ch == '"':
            in_string = True
            result.append(ch)
            i += 1
            continue
        if ch == "/" and nxt == "/":
            i += 2
            while i < len(text) and text[i] != "\n":
                i += 1
            continue
        if ch == "/" and nxt == "*":
            i += 2
            while i + 1 < len(text) and not (text[i] == "*" and text[i + 1] == "/"):
                i += 1
            i += 2
            continue
        result.append(ch)
        i += 1
    return "".join(result)


def find_matching(text: str, start: int, opener: str, closer: str) -> int:
    depth = 0
    in_string = False
    i = start
    while i < len(text):
        ch = text[i]
        if in_string:
            if ch == "\\":
                i += 2
                continue
            if ch == '"':
                in_string = False
            i += 1
            continue
        if ch == '"':
            in_string = True
            i += 1
            continue
        if ch == opener:
            depth += 1
        elif ch == closer:
            depth -= 1
            if depth == 0:
                return i
        i += 1
    raise ValueError(f"Unmatched {opener}{closer}")


def split_top_level(text: str, delimiter: str = ",") -> list[str]:
    parts: list[str] = []
    start = 0
    brace = paren = bracket = 0
    in_string = False
    i = 0
    while i < len(text):
        ch = text[i]
        if in_string:
            if ch == "\\":
                i += 2
                continue
            if ch == '"':
                in_string = False
            i += 1
            continue
        if ch == '"':
            in_string = True
            i += 1
            continue
        if ch == "{":
            brace += 1
        elif ch == "}":
            brace -= 1
        elif ch == "(":
            paren += 1
        elif ch == ")":
            paren -= 1
        elif ch == "[":
            bracket += 1
        elif ch == "]":
            bracket -= 1
        elif ch == delimiter and brace == 0 and paren == 0 and bracket == 0:
            parts.append(text[start:i].strip())
            start = i + 1
        i += 1
    tail = text[start:].strip()
    if tail:
        parts.append(tail)
    return parts


def decode_c_string(token: str) -> str:
    token = token.strip()
    if not (token.startswith('"') and token.endswith('"')):
        raise ValueError(f"Expected quoted string, got: {token}")
    inner = token[1:-1]
    return bytes(inner, "utf-8").decode("unicode_escape")


def normalize_spaces(text: str) -> str:
    return re.sub(r"\s+", " ", text).strip()


def titleize_identifier(identifier: str) -> str:
    identifier = re.sub(r"^(PFC|IPCM|PERK|SYNERGY|SYMP|PS)_\d+_", "", identifier)
    identifier = re.sub(r"^(PFC|IPCM|PERK|SYNERGY|SYMP|PS)_", "", identifier)
    words = identifier.split("_")
    keep_upper = {"AOE", "HP", "XP"}
    titled: list[str] = []
    for word in words:
        if not word:
            continue
        titled.append(word if word in keep_upper else word.capitalize())
    return " ".join(titled)


def parse_enum_block(text: str, enum_name: str) -> str:
    marker = f"enum {enum_name}"
    start = text.index(marker)
    brace_start = text.index("{", start)
    brace_end = find_matching(text, brace_start, "{", "}")
    return text[brace_start + 1:brace_end]


def parse_player_classes(enums_text: str) -> tuple[list[str], dict[str, int]]:
    block = parse_enum_block(enums_text, "PLAYER_FULL_CLASS : char")
    classes: list[str] = []
    values: dict[str, int] = {}
    current_value = 0
    for entry in split_top_level(block):
        if not entry:
            continue
        if "=" in entry:
            name, raw_value = entry.split("=", 1)
            name = name.strip()
            current_value = int(raw_value.strip())
        else:
            name = entry.strip()
        if name == "PFC_COUNT":
            continue
        classes.append(titleize_identifier(name))
        values[name] = current_value
        current_value += 1
    return classes, values


def safe_eval(expr: str, names: dict[str, int], full_mask: int) -> int:
    expr = expr.replace("static_cast<uint>( -1 )", str(full_mask))
    expr = expr.strip()
    value = eval(expr, {"__builtins__": {}}, names)
    return int(value) & full_mask


def parse_class_masks(enums_text: str, classes: list[str], class_values: dict[str, int]) -> dict[str, int]:
    block = parse_enum_block(enums_text, "ITEM_PLAYER_CLASS_MASK : uint")
    full_mask = (1 << len(classes)) - 1
    values: dict[str, int] = dict(class_values)
    for entry in split_top_level(block):
        if not entry or "=" not in entry:
            continue
        name, expr = entry.split("=", 1)
        values[name.strip()] = safe_eval(expr, values, full_mask)
    return values


def mask_to_classes(mask: int, classes: list[str]) -> list[str]:
    return [name for index, name in enumerate(classes) if mask & (1 << index)]


def extract_array_body(text: str, prefix: str) -> str:
    start = text.index(prefix)
    brace_start = text.index("{", start)
    brace_end = find_matching(text, brace_start, "{", "}")
    return text[brace_start + 1:brace_end]


def parse_description_block(block: str) -> list[str]:
    block = block.strip()
    if not (block.startswith("{") and block.endswith("}")):
        raise ValueError(f"Unexpected description block: {block[:40]}")
    return [decode_c_string(token) for token in split_top_level(block[1:-1])]


def parse_req_block(block: str, kind: str) -> list[tuple[str, int]]:
    block = block.strip()
    if not (block.startswith("{") and block.endswith("}")):
        return []
    tokens = [token.strip() for token in split_top_level(block[1:-1]) if token.strip()]
    pairs: list[tuple[str, int]] = []
    for i in range(0, len(tokens), 2):
        if i + 1 >= len(tokens):
            break
        name = tokens[i]
        level_token = tokens[i + 1]
        if kind == "perk" and name == "PERK_NO_PERK":
            continue
        if kind == "spell" and name == "PS_NONE":
            continue
        level = int(level_token)
        if level <= 0:
            continue
        pairs.append((name, level))
    return pairs


def parse_value_tokens(tokens: list[str]) -> list[int]:
    values: list[int] = []
    for token in tokens:
        token = token.strip()
        if not token:
            continue
        if token.startswith("{") and token.endswith("}"):
            values.extend(parse_value_tokens(split_top_level(token[1:-1])))
            continue
        values.append(int(token))
    return values


def format_description(template: str, values: list[int]) -> str:
    lines = [normalize_spaces(part) for part in template.split("::") if normalize_spaces(part)]
    formatted: list[str] = []
    value_iter = iter(values)
    for line in lines:
        count = line.count("%i")
        replacements = tuple(next(value_iter) for _ in range(count))
        if count:
            line = line % replacements
        line = line.replace("%%", "%")
        formatted.append(line)
    return " ".join(formatted)


def normalize_class_segments(levels: list[dict]) -> list[tuple[int, int, list[str]]]:
    segments: list[tuple[int, int, list[str]]] = []
    start = 1
    current = levels[0]["classes"]
    for index, level in enumerate(levels[1:], start=2):
        if level["classes"] != current:
            segments.append((start, index - 1, current))
            start = index
            current = level["classes"]
    segments.append((start, len(levels), current))
    return segments


def placeholder_letters(count: int) -> list[str]:
    alphabet = "XYZUVWQRST"
    return [alphabet[index] if index < len(alphabet) else f"V{index + 1}" for index in range(count)]


def split_description_lines(template: str) -> list[str]:
    return [normalize_spaces(part) for part in template.split("::") if normalize_spaces(part)]


def summarize_sequence(values: list[int]) -> str:
    if not values:
        return ""
    if len(values) == 1:
        return f"fixed at {values[0]}"

    diffs = [values[i] - values[i - 1] for i in range(1, len(values))]
    if all(diff == diffs[0] for diff in diffs):
        sign = "+" if diffs[0] >= 0 else ""
        return f"starts at {values[0]}, {sign}{diffs[0]} per level"

    if len(values) >= 5:
        average = round((values[-1] - values[0]) / (len(values) - 1))
        sign = "+" if average >= 0 else ""
        return f"starts at {values[0]}, ~{sign}{average} per level"

    segments: list[tuple[int, int, int]] = []
    seg_start = 1
    current_diff = diffs[0]
    for index, diff in enumerate(diffs[1:], start=2):
        if diff != current_diff:
            segments.append((seg_start, index, current_diff))
            seg_start = index + 1
            current_diff = diff
    segments.append((seg_start, len(values), current_diff))

    parts = [f"starts at {values[0]}"]
    for start, end, diff in segments:
        sign = "+" if diff >= 0 else ""
        if start == end:
            parts.append(f"{sign}{diff} to Lv{end}")
        else:
            parts.append(f"{sign}{diff} per level (Lv{start}-Lv{end})")
    return "; ".join(parts)


def build_effect_summary(perk: dict) -> tuple[str, list[str]]:
    total_placeholders = sum(template.count("%i") for template in perk["descriptions"])
    letters = placeholder_letters(total_placeholders)
    effect_parts: list[str] = []
    growth_parts: list[str] = []
    letter_index = 0

    for template in perk["descriptions"]:
        line_parts = split_description_lines(template)
        local_effect_parts: list[str] = []
        for line in line_parts:
            count = line.count("%i")
            local_letters = letters[letter_index:letter_index + count]
            for local_idx, letter in enumerate(local_letters):
                line = line.replace("%i", f"`{letter}`", 1)
                sequence = [level["values"][letter_index + local_idx] for level in perk["levels"]]
                growth_parts.append(f"{letter}: {summarize_sequence(sequence)}")
            letter_index += count
            line = line.replace("%%", "%")
            local_effect_parts.append(line)
        effect_parts.append(" ".join(local_effect_parts))

    return " ".join(effect_parts), growth_parts


def perk_families(perk: dict) -> list[str]:
    families: list[str] = []
    for level in perk["levels"]:
        for class_name in level["classes"]:
            family = CLASS_TO_FAMILY.get(class_name, class_name)
            if family not in families:
                families.append(family)
    families.sort(key=CLASS_FAMILY_ORDER.index)
    return families


def perk_group(perk: dict) -> str:
    families = perk_families(perk)
    if len(families) == len(CLASS_FAMILY_ORDER):
        return "All Classes"
    if len(families) == 1:
        return families[0]
    return "Mixed"


def format_group_label(perk: dict, group: str) -> str:
    return group


def parse_level_block(block: str, classes: list[str], class_masks: dict[str, int]) -> list[dict]:
    block = block.strip()
    if not (block.startswith("{") and block.endswith("}")):
        raise ValueError("Unexpected level block")
    levels: list[dict] = []
    for entry in split_top_level(block[1:-1]):
        entry = entry.strip()
        if not entry:
            continue
        if not (entry.startswith("{") and entry.endswith("}")):
            continue
        fields = split_top_level(entry[1:-1])
        if len(fields) < 8:
            continue
        char_level = int(fields[0])
        if char_level == 0:
            continue
        class_mask = safe_eval(fields[1], class_masks, (1 << len(classes)) - 1)
        perk_reqs = parse_req_block(fields[2], "perk")
        spell_reqs = parse_req_block(fields[3], "spell")
        values = parse_value_tokens(fields[8:])
        levels.append({
            "char_level": char_level,
            "classes": mask_to_classes(class_mask, classes),
            "perk_reqs": perk_reqs,
            "spell_reqs": spell_reqs,
            "values": values,
        })
    return levels


def parse_perk_file(
    path: Path,
    array_prefix: str,
    section_title: str,
    classes: list[str],
    class_masks: dict[str, int],
) -> list[dict]:
    text = strip_comments(path.read_text(encoding="utf-8", errors="replace"))
    body = extract_array_body(text, array_prefix)
    perks: list[dict] = []
    for entry in split_top_level(body):
        entry = entry.strip()
        if not entry:
            continue
        if not (entry.startswith("{") and entry.endswith("}")):
            continue
        fields = split_top_level(entry[1:-1])
        if len(fields) < 4:
            continue
        perk_id = fields[0].strip()
        descriptions = parse_description_block(fields[1])
        name = decode_c_string(fields[2])
        levels = parse_level_block(fields[3], classes, class_masks)
        perks.append({
            "id": perk_id,
            "name": name,
            "descriptions": descriptions,
            "levels": levels,
            "section": section_title,
        })
    return perks


def render_markdown(perks: list[dict]) -> str:
    lines = [
        "# Perks Guide",
        "",
        "Generated from `src/PerksData.cpp` and `src/SynergyData.cpp`.",
        "",
        "Each entry lists only what the perk does and how it scales per level.",
        "",
        "## Contents",
        "",
        "- [Common Perks](#common-perks)",
        "  - [All Classes](#all-classes)",
        "  - [Warrior](#warrior)",
        "  - [Archer](#archer)",
        "  - [Mage](#mage)",
        "  - [Monk](#monk)",
        "  - [Rogue](#rogue)",
        "  - [Savage](#savage)",
        "  - [Mixed](#mixed)",
        "- [Synergies](#synergies)",
        "  - [All Classes](#all-classes-1)",
        "  - [Warrior](#warrior-1)",
        "  - [Archer](#archer-1)",
        "  - [Mage](#mage-1)",
        "  - [Monk](#monk-1)",
        "  - [Rogue](#rogue-1)",
        "  - [Savage](#savage-1)",
        "  - [Mixed](#mixed-1)",
    ]
    sections = ["Common Perks", "Synergies"]
    for section in sections:
        section_perks = sorted(
            (perk for perk in perks if perk["section"] == section),
            key=lambda perk: (0 if perk_group(perk) == "All Classes" else 1 if perk_group(perk) != "Mixed" else 2, perk["name"].lower()),
        )
        if not section_perks:
            continue
        lines.extend(["", "---", "", f"## {section}"])
        grouped: dict[str, list[dict]] = {
            "All Classes": [],
            "Warrior": [],
            "Archer": [],
            "Mage": [],
            "Monk": [],
            "Rogue": [],
            "Savage": [],
            "Mixed": [],
        }
        for perk in section_perks:
            grouped[perk_group(perk)].append(perk)

        for group in ["All Classes", *CLASS_FAMILY_ORDER, "Mixed"]:
            if not grouped[group]:
                continue
            lines.extend(["", f"### {format_group_label(grouped[group][0], group)}"])
            for perk in sorted(grouped[group], key=lambda perk: perk["name"].lower()):
                effect_summary, growth_summary = build_effect_summary(perk)
                lines.extend(["", f"#### {perk['name']}"])
                lines.append(f"- Effect: {effect_summary}")
                lines.append(f"- Growth: {'; '.join(growth_summary) if growth_summary else 'none'}")
                if group == "Mixed":
                    lines.append(f"- Classes: {', '.join(perk_families(perk))}")
    lines.append("")
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser(description="Generate docs/PERKS.md from perk source files.")
    parser.add_argument(
        "--output",
        type=Path,
        default=DEFAULT_OUTPUT_PATH,
        help="Markdown output path",
    )
    args = parser.parse_args()

    enums_text = strip_comments(ENUMS_PATH.read_text(encoding="utf-8", errors="replace"))
    classes, class_values = parse_player_classes(enums_text)
    global ALL_CLASSES
    ALL_CLASSES = classes
    class_masks = parse_class_masks(enums_text, classes, class_values)

    perks: list[dict] = []
    perks.extend(parse_perk_file(COMMON_PERKS_PATH, "Perk Perks[PERKS_COUNT] =", "Common Perks", classes, class_masks))
    perks.extend(parse_perk_file(SYNERGY_PERKS_PATH, "Perk SynergyPerks[] =", "Synergies", classes, class_masks))

    output_path = args.output.resolve()
    output_path.write_text(render_markdown(perks), encoding="utf-8")


if __name__ == "__main__":
    main()
