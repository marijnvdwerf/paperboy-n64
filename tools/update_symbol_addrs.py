#!/usr/bin/env python3
# /// script
# requires-python = ">=3.11"
# dependencies = ["mapfile-parser"]
# ///
"""Update symbol_addrs.txt names from a linker map file.

Dry-run by default; pass --write to apply changes.
"""
import argparse
import re
from pathlib import Path

from mapfile_parser import MapFile

LINE_RE = re.compile(r"^\s*([^=\s]+)\s*=\s*([^;]+);\s*(?://\s*(.*?))?\s*$")
AUTO_NAME_RE = re.compile(r"^(func_|D_)[0-9A-Fa-f]+$")


def build_map_lookup(map_path: Path) -> dict[int, str]:
    m = MapFile()
    m.readMapFile(map_path)
    lookup: dict[int, str] = {}
    for seg in m:
        for sec in seg:
            for sym in sec:
                if sym.name.startswith("$") or sym.name.endswith(".NON_MATCHING"):
                    continue
                if AUTO_NAME_RE.match(sym.name):
                    continue
                lookup[sym.vram] = sym.name
    return lookup


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--map", type=Path, default=Path("build/ntsc/paperboy.map"))
    parser.add_argument("--symbols", type=Path, default=Path("symbol_addrs.txt"))
    parser.add_argument("--write", action="store_true", help="Apply changes in-place")
    args = parser.parse_args()

    lookup = build_map_lookup(args.map)
    text = args.symbols.read_text()
    out_lines: list[str] = []
    changes = 0
    warnings = 0

    for raw in text.splitlines():
        if raw.strip() == "":
            out_lines.append(raw)
            continue

        m = LINE_RE.match(raw)
        if not m:
            out_lines.append(raw)
            continue

        name = m.group(1)
        addr = int(m.group(2), 0)
        comment = m.group(3)

        map_name = lookup.get(addr)
        if map_name is None:
            print(f"  warning: 0x{addr:08X} ({name}) — not in map")
            warnings += 1
            out_lines.append(raw)
        elif map_name == name:
            out_lines.append(raw)
        else:
            if AUTO_NAME_RE.match(map_name):
                print(f"  warning: 0x{addr:08X} ({name}) — map has auto-name {map_name}, skipping")
                warnings += 1
                out_lines.append(raw)
            else:
                print(f"  {name} -> {map_name}  (0x{addr:08X})")
                changes += 1
                s = f"{map_name} = 0x{addr:08X};"
                if comment:
                    s += f" // {comment}"
                out_lines.append(s)

    print(f"\n{changes} change(s), {warnings} warning(s)")

    if args.write and changes:
        args.symbols.write_text("\n".join(out_lines) + "\n")
        print("Written.")
    elif changes:
        print("Dry run — pass --write to apply.")


if __name__ == "__main__":
    main()
