#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.10"
# dependencies = []
# ///
"""Verify symbol_addrs.txt against build/ntsc/paperboy.map."""
import re
import sys
from pathlib import Path
from collections import defaultdict

ROOT = Path(__file__).resolve().parent.parent
SYMS = ROOT / "symbol_addrs.txt"
MAP = ROOT / "build" / "ntsc" / "paperboy.map"

SYM_LINE = re.compile(r"^\s*([^=\s]+)\s*=\s*0x([0-9A-Fa-f]+)\s*;")
MAP_LINE = re.compile(r"^\s+0x([0-9a-fA-F]{16})\s+(\S+)\s*$")


def parse_symbol_addrs(path: Path) -> list[tuple[str, int]]:
    entries = []
    for line in path.read_text().splitlines():
        s = line.split("//", 1)[0].strip()
        if not s:
            continue
        m = SYM_LINE.match(s)
        if m:
            entries.append((m.group(1), int(m.group(2), 16)))
    return entries


def parse_map(path: Path) -> tuple[dict[str, int], dict[int, list[str]]]:
    """Return (name->addr, addr->[names]). Skips addr 0.
    Pairs `foo` and `foo.NON_MATCHING` at the same addr; prefers the bare name."""
    raw: dict[str, int] = {}
    for line in path.read_text().splitlines():
        m = MAP_LINE.match(line)
        if not m:
            continue
        addr = int(m.group(1), 16)
        if addr == 0:
            continue
        name = m.group(2)
        # Last occurrence wins (shouldn't matter for normal symbols).
        raw[name] = addr

    # Merge .NON_MATCHING shims: if base exists, drop the shim; else rename to base.
    name_to_addr: dict[str, int] = {}
    for name, addr in raw.items():
        if name.endswith(".NON_MATCHING"):
            base = name[: -len(".NON_MATCHING")]
            if base in raw:
                continue  # both present: bare name wins
            name_to_addr[base] = addr
        else:
            name_to_addr[name] = addr

    addr_to_names: dict[int, list[str]] = defaultdict(list)
    for name, addr in name_to_addr.items():
        addr_to_names[addr].append(name)
    return name_to_addr, addr_to_names


def main() -> int:
    entries = parse_symbol_addrs(SYMS)
    name_to_addr, addr_to_names = parse_map(MAP)

    problems: list[tuple[int, str]] = []
    for name, addr in entries:
        if name not in name_to_addr:
            here = addr_to_names.get(addr, [])
            here_str = ", ".join(here) if here else "<none>"
            problems.append((
                addr,
                f"MISSING  {name} @ 0x{addr:08X} (map has at this addr: {here_str})",
            ))
            continue
        map_addr = name_to_addr[name]
        if map_addr != addr:
            others = [n for n in addr_to_names.get(addr, []) if n != name]
            other_str = ", ".join(others) if others else "<none>"
            problems.append((
                addr,
                f"MISMATCH {name}: addrs=0x{addr:08X}  map=0x{map_addr:08X}  "
                f"(map symbol at this addr: {other_str})",
            ))

    problems.sort(key=lambda p: p[0])
    for _, msg in problems:
        print(msg)
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
