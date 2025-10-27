#!/usr/bin/env python3
"""
Replace the color table entry used at the four corners of an XPM file with a new hex color.
Usage: convert_xpm_by_corner.py path/to/file.xpm to_hex
Example: convert_xpm_by_corner.py textures/ttur/alien.xpm #FF00FF
Creates a backup file file.xpm.bak
"""
import sys, re, shutil

if len(sys.argv) < 3:
    print('Usage: convert_xpm_by_corner.py <file.xpm> <to_hex>')
    sys.exit(2)

path = sys.argv[1]
to_hex = sys.argv[2]

with open(path, 'r', encoding='utf-8') as f:
    lines = [ln.rstrip('\n') for ln in f]

# find header
header_idx = None
for i, line in enumerate(lines):
    if re.match(r"\s*\"\s*\d+\s+\d+\s+\d+\s+\d+\s*\"", line):
        header_idx = i
        break
if header_idx is None:
    print('XPM header not found')
    sys.exit(1)

header = lines[header_idx].strip().strip(',').strip('"')
parts = header.split()
cols = int(parts[0]); rows = int(parts[1]); colors = int(parts[2]); chars = int(parts[3])

color_lines = lines[header_idx+1: header_idx+1+colors]
pixel_lines = lines[header_idx+1+colors: header_idx+1+colors+rows]
if len(pixel_lines) < rows:
    print('Unexpected XPM structure: pixel lines shorter than rows')
    sys.exit(1)

# parse color table: key -> color
color_map = {}
for cl in color_lines:
    m = re.match(r"\s*\"(.+?)\"\s*(.*)", cl)
    if not m: continue
    key = m.group(1)
    rest = m.group(2)
    # find c <color>
    mc = re.search(r"\bc\s+(None|#[0-9A-Fa-f]{6})", rest)
    color = mc.group(1) if mc else None
    color_map[key] = color

# helper to get key at pixel position (0-indexed)
def get_key_at(x,y):
    line = pixel_lines[y].strip().strip(',').strip('"')
    start = x * chars
    return line[start:start+chars]

corners = [(1,1), (cols-2,1), (1,rows-2), (cols-2, rows-2)]
found_keys = set()
for cx,cy in corners:
    if cx < 0 or cx >= cols or cy < 0 or cy >= rows: continue
    k = get_key_at(cx, cy)
    found_keys.add(k)

print('Found corner keys:', found_keys)
found_colors = {}
for k in found_keys:
    c = color_map.get(k)
    found_colors[k] = c
print('Found corner colors:', found_colors)

# decide which color to replace: pick the most frequent color among corners
from collections import Counter
cnt = Counter()
for k in found_keys:
    cnt[found_colors.get(k)] += 1
most_common_color, _ = cnt.most_common(1)[0]
print('Most common corner color to replace:', most_common_color)
if not most_common_color or most_common_color.lower() == to_hex.lower():
    print('Nothing to do')
    sys.exit(0)

# update color table lines: replace color value in the color_lines where c = most_common_color
new_color_lines = []
repl_count = 0
for cl in color_lines:
    m = re.match(r"(\s*\".+?\"\s*.*\bc\s+)(None|#[0-9A-Fa-f]{6})(.*)", cl)
    if m:
        prefix = m.group(1)
        col = m.group(2)
        suffix = m.group(3)
        if col.lower() == most_common_color.lower():
            newcl = prefix + to_hex + suffix
            new_color_lines.append(newcl)
            repl_count += 1
            continue
    new_color_lines.append(cl)

if repl_count == 0:
    print('No exact color table entry matched the corner color; aborting')
    sys.exit(1)

# write backup and new file
bak = path + '.bak'
shutil.copy2(path, bak)
new_lines = lines[:header_idx+1] + new_color_lines + pixel_lines + lines[header_idx+1+colors+rows:]
with open(path, 'w', encoding='utf-8') as f:
    for ln in new_lines:
        f.write(ln + '\n')
print(f'Replaced {repl_count} color table entries ({most_common_color} -> {to_hex}), backup at {bak}')
sys.exit(0)
