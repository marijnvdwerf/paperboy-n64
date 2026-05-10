#!/usr/bin/env python3
"""Apply byte-level patches to a ROM file.

Usage: patch_rom.py ROM offset:hexbytes [offset:hexbytes ...]
"""
import sys


def main():
    if len(sys.argv) < 2:
        print(__doc__, file=sys.stderr)
        sys.exit(2)
    rom = sys.argv[1]
    patches = sys.argv[2:]
    if not patches:
        return
    with open(rom, "rb+") as f:
        for spec in patches:
            offset_str, hex_str = spec.split(":")
            offset = int(offset_str, 0)
            data = bytes.fromhex(hex_str)
            f.seek(offset)
            f.write(data)


if __name__ == "__main__":
    main()
