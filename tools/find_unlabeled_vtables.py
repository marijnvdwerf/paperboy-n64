#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.10"
# dependencies = []
# ///
"""Detect vtables in the baserom that are not labeled as _vt.* in the map file.

A vtable has the SN-compiler layout:
  word +00: 0x00000000  (ti-offset, always 0 for primary vtable)
  word +04: 0x00000000  (typeinfo pointer high-word, SN stores as 0)
  word +08: 0x00000000  (typeinfo pointer low-word, SN stores as 0)
  word +0C: <func-ptr>  (first virtual function slot, high half)
  word +10: 0x00000000  (...)
  word +14: <func-ptr>  (second slot)
  ...

Each virtual-function slot is 8 bytes: (0x00000000, address).
We require at least MIN_SLOTS function slots after the 3-word header.
"""
import re
import struct
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MAP = ROOT / "build" / "ntsc" / "paperboy.map"
ROM = ROOT / "baserom.z64"

# Minimum consecutive (0, func_ptr) pairs required after the 3-word header.
MIN_SLOTS = 3

MAP_LINE = re.compile(r"^\s+0x([0-9a-fA-F]{16})\s+(\S+)\s*$", re.MULTILINE)
SEG_BOUND = re.compile(
    r"^\s+0x([0-9a-fA-F]{16})\s+(\w+_(?:ROM_START|ROM_END|VRAM_END))\s*=",
    re.MULTILINE,
)


def parse_map(path: Path):
    text = path.read_text()

    # Collect all symbols: name -> addr, addr -> [names]
    name_to_addr: dict[str, int] = {}
    addr_to_names: dict[int, list[str]] = defaultdict(list)
    for m in MAP_LINE.finditer(text):
        addr = int(m.group(1), 16)
        name = m.group(2)
        if addr == 0:
            continue
        if name.endswith(".NON_MATCHING"):
            base = name[: -len(".NON_MATCHING")]
            if base not in name_to_addr:
                name_to_addr[base] = addr
            continue
        name_to_addr[name] = addr
        addr_to_names[addr].append(name)

    # Collect segment boundaries for ROM-offset <-> VRAM conversion
    bounds: dict[str, int] = {}
    for m in SEG_BOUND.finditer(text):
        bounds[m.group(2)] = int(m.group(1), 16)

    return name_to_addr, addr_to_names, bounds


def build_scan_segments(bounds: dict[str, int]) -> list[tuple[int, int, int]]:
    """Return list of (rom_start, rom_end, vram_start) for code/data segments."""
    segs: list[tuple[int, int, int]] = []
    known = [
        ("entry", 0x001000, 0x80000400),
        ("first", None, None),
        ("F19C0", None, None),
    ]
    # Compute VRAM start for each segment from its predecessor's VRAM_END.
    # entry starts at 0x80000400 (fixed by the ROM header).
    prev_vram_end = 0x80000400

    for seg_name, fixed_rom_start, fixed_vram_start in known:
        rom_start = fixed_rom_start if fixed_rom_start is not None else bounds.get(f"{seg_name}_ROM_START")
        rom_end = bounds.get(f"{seg_name}_ROM_END")
        vram_start = fixed_vram_start if fixed_vram_start is not None else prev_vram_end

        if rom_start is None or rom_end is None:
            continue

        segs.append((rom_start, rom_end, vram_start))
        prev_vram_end = bounds.get(f"{seg_name}_VRAM_END", prev_vram_end)

    return segs


def detect_vtables(
    rom_data: bytes,
    segments: list[tuple[int, int, int]],
    func_vram_min: int,
    func_vram_max: int,
) -> list[tuple[int, int, int, int]]:
    """Return list of (rom_offset, vram, slot_count, seg_idx) for each vtable candidate."""

    def is_func(v: int) -> bool:
        return func_vram_min <= v <= func_vram_max

    results: list[tuple[int, int, int, int]] = []
    for seg_idx, (rom_start, rom_end, vram_start) in enumerate(segments):
        i = rom_start
        while i <= rom_end - 4 * (3 + MIN_SLOTS * 2):
            w0, w1, w2, w3 = struct.unpack_from(">4I", rom_data, i)
            if not (w0 == 0 and w1 == 0 and w2 == 0 and is_func(w3)):
                i += 4
                continue
            # Count consecutive (0, func) pairs after the header
            slots = 1  # w3 is the first function slot
            j = i + 16
            while j + 8 <= rom_end:
                z, v = struct.unpack_from(">2I", rom_data, j)
                if z == 0 and is_func(v):
                    slots += 1
                    j += 8
                else:
                    break
            if slots >= MIN_SLOTS:
                vram = vram_start + (i - rom_start)
                results.append((i, vram, slots, seg_idx))
                # Skip past this vtable to avoid overlapping detections
                i = j
            else:
                i += 4
    return results


def main() -> int:
    rom_data = ROM.read_bytes()
    name_to_addr, addr_to_names, bounds = parse_map(MAP)

    segments = build_scan_segments(bounds)
    if not segments:
        print("ERROR: could not determine scan segments from map file", file=sys.stderr)
        return 1

    # Determine valid function-pointer range from all known symbols in the map.
    code_addrs = [a for a in addr_to_names if 0x80000000 <= a <= 0x80200000]
    if not code_addrs:
        print("ERROR: no code symbols found in map", file=sys.stderr)
        return 1
    func_vram_min = min(code_addrs)
    func_vram_max = max(code_addrs)

    candidates = detect_vtables(rom_data, segments, func_vram_min, func_vram_max)

    # Classify each candidate
    known_vt_addrs = {addr for name, addr in name_to_addr.items() if "_vt." in name}

    unlabeled: list[tuple[int, int, int, int, list[str]]] = []
    for rom_off, vram, slots, seg_idx in candidates:
        if vram in known_vt_addrs:
            continue  # already labeled as _vt.*
        existing = addr_to_names.get(vram, [])
        unlabeled.append((rom_off, vram, slots, seg_idx, existing))

    if not unlabeled:
        print(f"All {len(candidates)} vtable candidates are already labeled.")
        return 0

    seg_names = ["entry", "first", "F19C0"]
    print(
        f"Found {len(unlabeled)} unlabeled vtable(s) "
        f"(out of {len(candidates)} total candidates, {len(known_vt_addrs)} already labeled)\n"
    )

    prev_seg = -1
    for rom_off, vram, slots, seg_idx, existing in unlabeled:
        if seg_idx != prev_seg:
            if prev_seg != -1:
                print()
            seg_name = seg_names[seg_idx] if seg_idx < len(seg_names) else f"seg{seg_idx}"
            rom_start, rom_end, vram_start = segments[seg_idx]
            print(f"  [{seg_name}  ROM 0x{rom_start:07X}–0x{rom_end:07X}  VRAM 0x{vram_start:08X}]")
            print(f"  {'ROM offset':<13}  {'VRAM':<12}  {'slots':<6}  current label")
            print(f"  {'-'*13}  {'-'*12}  {'-'*6}  {'-'*30}")
            prev_seg = seg_idx
        label = ", ".join(existing) if existing else "<no map label>"
        print(f"  0x{rom_off:07X}    0x{vram:08X}  {slots:>6}  {label}")

    return 1


if __name__ == "__main__":
    sys.exit(main())
