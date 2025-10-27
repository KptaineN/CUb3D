#!/usr/bin/env python3
"""
Replace color table entries matching a target hex color in an XPM file with a new hex color.
Creates a backup with .bak extension.
Usage: python3 convert_xpm_bg.py /path/to/file.xpm from_hex to_hex
Example: python3 convert_xpm_bg.py textures/ttur/alien.xpm #000000 #FF00FF
"""
import sys
import re

if len(sys.argv) < 4:
    print("Usage: convert_xpm_bg.py <file.xpm> <from_hex> <to_hex>")
    sys.exit(2)

path = sys.argv[1]
from_hex = sys.argv[2].lower()
to_hex = sys.argv[3].lower()

with open(path, 'r', encoding='utf-8') as f:
    lines = f.readlines()

# find header line like: "110 110 143 2 ", then next N lines are color table where each line contains 'c #RRGGBB' or 'c None'
header_idx = None
m = None
for i, line in enumerate(lines):
    if re.match(r"\s*\"\d+\s+\d+\s+\d+\s+\d+\s*\",?", line):
        header_idx = i
        break

if header_idx is None:
    print('XPM header not found; aborting')
    sys.exit(1)

# extract numbers
header = lines[header_idx].strip().strip(',').strip('"')
parts = header.split()
if len(parts) < 4:
    print('Unexpected XPM header format')
    sys.exit(1)

cols = int(parts[0])
rows = int(parts[1])
colors = int(parts[2])
chars = int(parts[3])

# color table starts at header_idx+1
start = header_idx + 1
end = start + colors
if end > len(lines):
    print('XPM truncated?')
    sys.exit(1)

changed = 0
newlines = lines[:]
for i in range(start, end):
    line = lines[i]
    # match color entry like "a c #000000",
    # allow multiple fields before 'c'
    m = re.match(r"(\s*\".+?\")\s*c\s+(None|#[0-9A-Fa-f]{6})(.*)", line)
    if m:
        key = m.group(1)
        col = m.group(2)
        rest = m.group(3)
        if col.lower() == from_hex:
            newlines[i] = f'{key} c {to_hex}{rest}\n'
            changed += 1

if changed == 0:
    print('No color entries matched', from_hex)
else:
    # write backup
    import shutil
    shutil.copy2(path, path + '.bak')
    with open(path, 'w', encoding='utf-8') as f:
        f.writelines(newlines)
    print(f'Updated {changed} color entries from {from_hex} to {to_hex}; backup at {path}.bak')

sys.exit(0)
