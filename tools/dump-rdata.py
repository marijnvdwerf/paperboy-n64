#!/usr/bin/env -S uv run python
# /// script
# requires-python = ">=3.10"
# dependencies = ["pyelftools"]
# ///
"""Dump a section (default .rdata) of a MIPS ELF object with relocations and symbol labels.

Useful for inspecting C++ vtables and rodata blobs from MIPS N64 decomp builds.
"""

from __future__ import annotations

import argparse
import struct
import sys
from dataclasses import dataclass

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
from elftools.elf.sections import SymbolTableSection
from elftools.elf.enums import ENUM_RELOC_TYPE_MIPS


# Build reverse map: number -> name (e.g. 2 -> "R_MIPS_32").
_RELOC_NAME = {v: k for k, v in ENUM_RELOC_TYPE_MIPS.items()}


@dataclass
class SymLabel:
    offset: int
    name: str
    bind: str  # "local" | "global" | "weak"


@dataclass
class Reloc:
    offset: int
    type_name: str
    target: str
    addend: int  # 0 for REL


def reloc_type_name(r_info_type: int) -> str:
    return _RELOC_NAME.get(r_info_type, f"R_MIPS_{r_info_type}")


def bind_label(bind: str) -> str:
    return {
        "STB_LOCAL": "local",
        "STB_GLOBAL": "global",
        "STB_WEAK": "weak",
    }.get(bind, bind.lower())


def collect_section_symbols(elf: ELFFile, sec_index: int) -> list[SymLabel]:
    out: list[SymLabel] = []
    for section in elf.iter_sections():
        if not isinstance(section, SymbolTableSection):
            continue
        for sym in section.iter_symbols():
            if sym["st_shndx"] == "SHN_UNDEF":
                continue
            try:
                shndx = int(sym["st_shndx"])
            except (TypeError, ValueError):
                continue
            if shndx != sec_index:
                continue
            stt = sym["st_info"]["type"]
            # Skip the section symbol itself and file symbols.
            if stt in ("STT_SECTION", "STT_FILE"):
                continue
            name = sym.name or "<noname>"
            out.append(
                SymLabel(
                    offset=sym["st_value"],
                    name=name,
                    bind=bind_label(sym["st_info"]["bind"]),
                )
            )
    out.sort(key=lambda s: (s.offset, s.name))
    return out


def collect_section_relocs(elf: ELFFile, section_name: str) -> dict[int, Reloc]:
    """Return offset -> Reloc for relocations targeting section_name."""
    out: dict[int, Reloc] = {}
    target_names = {f".rel{section_name}", f".rela{section_name}"}
    for section in elf.iter_sections():
        if not isinstance(section, RelocationSection):
            continue
        if section.name not in target_names:
            continue
        symtab = elf.get_section(section["sh_link"])
        is_rela = section.is_RELA()
        for reloc in section.iter_relocations():
            sym = symtab.get_symbol(reloc["r_info_sym"])
            sym_name = sym.name
            if not sym_name:
                # Section symbol: fall back to section name.
                try:
                    sh = elf.get_section(int(sym["st_shndx"]))
                    sym_name = sh.name or "<section>"
                except Exception:
                    sym_name = "<unknown>"
            addend = reloc["r_addend"] if is_rela else 0
            out[reloc["r_offset"]] = Reloc(
                offset=reloc["r_offset"],
                type_name=reloc_type_name(reloc["r_info_type"]),
                target=sym_name,
                addend=addend,
            )
    return out


def printable_byte(b: int) -> str:
    return chr(b) if 32 <= b < 127 else "."


def ascii_for_word(word_bytes: bytes) -> str:
    return "".join(printable_byte(b) for b in word_bytes)


def dump(path: str, section_name: str) -> int:
    with open(path, "rb") as f:
        elf = ELFFile(f)

        section = elf.get_section_by_name(section_name)
        if section is None:
            print(f"error: section {section_name!r} not found in {path}", file=sys.stderr)
            return 1

        sec_index = None
        for i, s in enumerate(elf.iter_sections()):
            if s.name == section_name:
                sec_index = i
                break

        data = section.data()
        size = len(data)

        # big-endian MIPS
        endian = ">"

        symbols = collect_section_symbols(elf, sec_index)
        relocs = collect_section_relocs(elf, section_name)

        print(f"{path}  {section_name}  size=0x{size:X}")

        # Map offset -> list of symbols starting there.
        sym_at: dict[int, list[SymLabel]] = {}
        for s in symbols:
            sym_at.setdefault(s.offset, []).append(s)

        # Word-aligned dump, two words per row (8 bytes), aligned to symbol boundaries.
        WORDS_PER_ROW = 2
        WORD = 4
        ROW = WORDS_PER_ROW * WORD

        # Determine current symbol context to break rows when a new symbol starts.
        sym_offsets = sorted(sym_at.keys())

        offset = 0
        # Pad: if first content isn't at offset 0 and there's no symbol there, still print.
        cur_sym_idx = 0

        # Helper: next symbol offset strictly greater than `o`.
        def next_sym_after(o: int) -> int:
            for so in sym_offsets:
                if so > o:
                    return so
            return size

        while offset < size:
            # Print symbol headers that start at this offset.
            if offset in sym_at:
                for s in sym_at[offset]:
                    print()
                    print(f"{s.name} ({s.bind}):")

            row_end = min(offset + ROW, size, next_sym_after(offset))
            # If the row would span past a symbol boundary, truncate to that boundary
            # so each new symbol starts cleanly. row_end already accounts for that.

            # Build the words for this row.
            words_in_row: list[tuple[int, bytes, int | None]] = []
            o = offset
            while o < row_end:
                wlen = min(WORD, row_end - o)
                wbytes = data[o : o + wlen]
                # Pad to 4 bytes for display if short tail.
                word_int = None
                if wlen == WORD:
                    word_int = struct.unpack(f"{endian}I", wbytes)[0]
                words_in_row.append((o, wbytes, word_int))
                o += wlen

            # Format the row.
            addr = f"  0x{offset:04X}:"
            word_strs = []
            ascii_strs = []
            reloc_notes: list[str] = []
            for wo, wbytes, wint in words_in_row:
                if wo in relocs:
                    word_strs.append("XXXXXXXX")
                    ascii_strs.append("...." if len(wbytes) == 4 else "." * len(wbytes))
                    r = relocs[wo]
                    addend_part = f"+0x{r.addend:X}" if r.addend else ""
                    reloc_notes.append(f"-> {r.target}{addend_part}  {r.type_name}")
                elif wint is not None:
                    word_strs.append(f"{wint:08X}")
                    ascii_strs.append(ascii_for_word(wbytes))
                else:
                    # short tail
                    word_strs.append(wbytes.hex().upper().ljust(8))
                    ascii_strs.append(ascii_for_word(wbytes))

            # Pad word_strs to WORDS_PER_ROW for alignment if the row is short.
            while len(word_strs) < WORDS_PER_ROW:
                word_strs.append(" " * 8)
                ascii_strs.append("    ")

            words_col = "  ".join(word_strs)
            ascii_col = " ".join(ascii_strs)
            left = f"{addr}  {words_col}   {ascii_col}"
            if reloc_notes:
                # Align reloc info column.
                print(f"{left:<60}  {'; '.join(reloc_notes)}")
            else:
                print(left)

            offset = row_end

    return 0


def main() -> int:
    p = argparse.ArgumentParser(
        description="Dump a section (default .rdata) of a MIPS ELF object with relocations and symbol labels."
    )
    p.add_argument("obj", help="path to ELF object file")
    p.add_argument("--section", default=".rdata", help="section name (default: .rdata)")
    args = p.parse_args()
    return dump(args.obj, args.section)


if __name__ == "__main__":
    sys.exit(main())
