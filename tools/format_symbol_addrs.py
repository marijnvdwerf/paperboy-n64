#!/usr/bin/env python3
"""Format symbol_addrs.txt: normalize lines, sort groups by min address.

Groups are blocks of consecutive non-blank lines, separated by blank line(s).
Group order is preserved; lines within each group are sorted by address.
Each line becomes `<name> = 0xHEX;` with optional ` // comment`.
"""
import re
import sys
from pathlib import Path

LINE_RE = re.compile(r"^\s*([^=\s]+)\s*=\s*([^;]+);\s*(?://\s*(.*?))?\s*$")


def parse_line(line: str):
    m = LINE_RE.match(line)
    if not m:
        raise ValueError(f"cannot parse: {line!r}")
    name, addr, comment = m.group(1), int(m.group(2), 0), m.group(3)
    return name, addr, (comment.strip() if comment else None)


def format_line(name: str, addr: int, comment: str | None) -> str:
    s = f"{name} = 0x{addr:08X};"
    if comment:
        s += f" // {comment}"
    return s


def format_file(text: str) -> str:
    groups: list[list[tuple[str, int, str | None]]] = []
    current: list[tuple[str, int, str | None]] = []
    for raw in text.splitlines():
        if raw.strip() == "":
            if current:
                groups.append(current)
                current = []
        else:
            current.append(parse_line(raw))
    if current:
        groups.append(current)

    for g in groups:
        g.sort(key=lambda entry: entry[1])

    out_lines = []
    for i, group in enumerate(groups):
        if i:
            out_lines.append("")
        for entry in group:
            out_lines.append(format_line(*entry))
    return "\n".join(out_lines) + "\n"


def main():
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("symbol_addrs.txt")
    formatted = format_file(path.read_text())
    path.write_text(formatted)


if __name__ == "__main__":
    main()
