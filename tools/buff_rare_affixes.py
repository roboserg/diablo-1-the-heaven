"""
buff_rare_affixes.py - Scale BaseEffect value fields and set DoubleChance in AffixesRare.cpp.

Applies two quality-tiered multipliers to the six numeric value fields of every
Affix entry (minLow, minHigh, maxLow, maxHigh, chanceLow, chanceHigh).
Zero values are left unchanged so that unused fields stay silent.
Entries whose quality meets or exceeds --threshold also get DoubleChance forced to 1,
increasing their weight in the random-selection pool.

Affix entry layout (src/structs.h — struct Affix / struct BaseEffect):
    { id, "Name", AE_TYPE, type, minLow, minHigh, maxLow, maxHigh, chanceLow, chanceHigh,
      quality, IA_MASK, IS_MASK, ExcludedCombinations, DoubleChance, NotCursed,
      MinGoldValue, MaxGoldValue, Multiplier, minReqClvl, maxReqClvl }

Usage:
    python tools/buff_rare_affixes.py [options] [file]

    file            Path to AffixesRare.cpp (default: src/AffixesRare.cpp)

Options:
    --low MULT      Multiplier for entries below --threshold  (default: 1.25)
    --high MULT     Multiplier for entries at/above --threshold (default: 1.5)
    --threshold N   Quality level that selects the high multiplier (default: 50)
    --dry-run       Print how many entries would change, but do not write the file
"""

import argparse
import re
import sys

# ---------------------------------------------------------------------------
# Regex — matches one Affix data-table row, capturing every named field.
# The IA_MASK field may be a bitwise-OR expression (e.g. IA_MELEE | IA_KNIFE).
# ---------------------------------------------------------------------------
ENTRY_RE = re.compile(
    r'^'
    r'(?P<indent>\s*(?:/\*[^*]*\*/\s*)?)'      # leading whitespace + optional /* comment */
    r'\{'
    r'\s*(?P<id_>\d+)\s*,'
    r'\s*(?P<name>"[^"]*")\s*,'
    r'\s*(?P<ae>AE_\w+)\s*,'
    r'\s*(?P<etype>\d+)\s*,'                    # effect.type  (always 0 in practice)
    r'\s*(?P<minLow>-?\d+)\s*,'
    r'\s*(?P<minHigh>-?\d+)\s*,'
    r'\s*(?P<maxLow>-?\d+)\s*,'
    r'\s*(?P<maxHigh>-?\d+)\s*,'
    r'\s*(?P<cLow>-?\d+)\s*,'
    r'\s*(?P<cHigh>-?\d+)\s*,'
    r'\s*(?P<quality>\d+)\s*,'
    r'\s*(?P<ia>IA_\w+(?:\s*\|\s*IA_\w+)*)\s*,'
    r'\s*(?P<is_>IS_\w+)\s*,'
    r'\s*(?P<excl>-?\d+)\s*,'
    r'\s*(?P<dbl>-?\d+)\s*,'
    r'\s*(?P<nc>-?\d+)\s*,'
    r"\s*(?P<minG>[\d']+)\s*,"
    r"\s*(?P<maxG>[\d']+)\s*,"
    r'\s*(?P<mult>-?\d+)\s*,'
    r'\s*(?P<minC>-?\d+)\s*,'
    r'\s*(?P<maxC>-?\d+)\s*'
    r'\}'
    r'(?P<tail>.*)',
)

VALUE_FIELDS = ('minLow', 'minHigh', 'maxLow', 'maxHigh', 'cLow', 'cHigh')


# ---------------------------------------------------------------------------
# Core helpers
# ---------------------------------------------------------------------------

def parse_entry(line):
    """Return a groupdict for a matched Affix data row, or None."""
    m = ENTRY_RE.match(line.rstrip('\r\n'))
    return m.groupdict() if m else None


def apply_multipliers(d, low_mult, high_mult, threshold):
    """
    Scale value fields in-place on a parsed entry dict.

    Returns the (possibly modified) dict.  Zero values are never scaled so
    unused fields remain zero.  DoubleChance is set to '1' for high-tier
    entries regardless of its original value.
    """
    quality = int(d['quality'])
    factor = high_mult if quality >= threshold else low_mult

    for field in VALUE_FIELDS:
        v = int(d[field])
        if v != 0:
            d[field] = str(round(v * factor))

    if quality >= threshold:
        d['dbl'] = '1'

    return d


def format_entry(d):
    """Reconstruct an Affix row string from a parsed (and possibly modified) dict."""
    return (
        f"{d['indent']}"
        f"{{{d['id_']},\t{d['name']},\t{d['ae']},\t"
        f"{d['etype']},\t{d['minLow']},\t{d['minHigh']},\t"
        f"{d['maxLow']},\t{d['maxHigh']},\t{d['cLow']},\t{d['cHigh']},\t"
        f"{d['quality']},\t{d['ia']},\t{d['is_']},\t"
        f"{d['excl']},\t{d['dbl']},\t{d['nc']},\t"
        f"{d['minG']},\t{d['maxG']},\t{d['mult']},\t"
        f"{d['minC']},\t{d['maxC']}"
        f"}}{d['tail']}"
    )


def transform_line(line, low_mult, high_mult, threshold):
    """
    Parse, transform, and reformat a single source line.

    Returns the transformed line (with original line ending preserved) or the
    original line unchanged if it is not a data-table row.
    """
    d = parse_entry(line)
    if d is None:
        return line

    apply_multipliers(d, low_mult, high_mult, threshold)

    ending = '\r\n' if line.endswith('\r\n') else ('\n' if line.endswith('\n') else '')
    return format_entry(d) + ending


def process_file(path, low_mult, high_mult, threshold, dry_run=False):
    """
    Apply multipliers to every Affix entry in *path*.

    Returns the number of transformed entries.  When *dry_run* is True the
    file is read but never written.
    """
    with open(path, 'r', encoding='utf-8', newline='') as f:
        lines = f.readlines()

    changed = 0
    result = []
    for line in lines:
        new_line = transform_line(line, low_mult, high_mult, threshold)
        result.append(new_line)
        if new_line != line:
            changed += 1

    if not dry_run:
        with open(path, 'w', encoding='utf-8', newline='') as f:
            f.writelines(result)

    return changed


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description='Scale Affix value fields in AffixesRare.cpp.',
    )
    parser.add_argument(
        'file', nargs='?', default='src/AffixesRare.cpp',
        help='Path to AffixesRare.cpp (default: src/AffixesRare.cpp)',
    )
    parser.add_argument(
        '--low', type=float, default=1.25, metavar='MULT',
        help='Multiplier for quality < threshold (default: 1.25)',
    )
    parser.add_argument(
        '--high', type=float, default=1.5, metavar='MULT',
        help='Multiplier for quality >= threshold (default: 1.5)',
    )
    parser.add_argument(
        '--threshold', type=int, default=50, metavar='N',
        help='Quality level that selects the high multiplier (default: 50)',
    )
    parser.add_argument(
        '--dry-run', action='store_true',
        help='Report how many entries would change without writing the file',
    )
    args = parser.parse_args()

    changed = process_file(args.file, args.low, args.high, args.threshold, args.dry_run)
    status = '[dry-run] would transform' if args.dry_run else 'transformed'
    print(f"{status} {changed} entries in {args.file}")


if __name__ == '__main__':
    main()
