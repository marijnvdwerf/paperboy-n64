#!/usr/bin/env python3
"""List functions ranked by how many times they are called from asm.

Walks asm/ for `jal <name>` instructions and tallies them. Optionally
filters out functions that already have a definition (decompiled in src/
or referenced from a cpp/c file).

Usage:
    tools/most_called.py [--top N] [--undecompiled-only]
"""
import argparse
import re
from collections import Counter
from pathlib import Path

JAL_RE = re.compile(r"\bjal\s+([A-Za-z_.][\w.]*)")


def count_jals(asm_dir: Path) -> Counter:
    counts: Counter = Counter()
    for path in asm_dir.rglob("*.s"):
        for line in path.read_text().splitlines():
            m = JAL_RE.search(line)
            if not m:
                continue
            counts[m.group(1)] += 1
    return counts


def find_decompiled(src_dir: Path) -> set[str]:
    """Heuristic: any function name appearing as a definition in C/C++ source."""
    decompiled: set[str] = set()
    def_re = re.compile(r"\b(func_[0-9A-Fa-f]{8})\s*\(")
    for path in src_dir.rglob("*.c*"):
        for line in path.read_text().splitlines():
            line = line.strip()
            if line.startswith("INCLUDE_ASM") or line.startswith("//"):
                continue
            for m in def_re.finditer(line):
                decompiled.add(m.group(1))
    return decompiled


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--top", type=int, default=30)
    ap.add_argument("--undecompiled-only", action="store_true")
    args = ap.parse_args()

    counts = count_jals(Path("asm"))
    if args.undecompiled_only:
        decompiled = find_decompiled(Path("src"))
        counts = Counter({n: c for n, c in counts.items() if n not in decompiled})

    for name, count in counts.most_common(args.top):
        print(f"{count:5d}  {name}")


if __name__ == "__main__":
    main()
