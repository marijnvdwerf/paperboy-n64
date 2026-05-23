#!/usr/bin/env python3
# /// script
# requires-python = ">=3.11"
# dependencies = ["mapfile-parser"]
# ///
"""TU dependency tree per top-level mapfile segment.

Green = decompiled, yellow = all deps decompiled (ready), grey = ignored,
default = pending with blocking deps. Run from the project root:

    uv run tools/tu_deps.py [segment_name]
"""
from __future__ import annotations

import functools
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

import mapfile_parser


# cfront/SN demangler — ported from segvis/src/demangle.ts

_LEN_RE = re.compile(r"^(\d+)")


def _parse_class(s: str) -> str | None:
    m = _LEN_RE.match(s)
    if not m:
        return None
    n = int(m.group(1))
    start = m.end()
    if len(s) < start + n:
        return None
    return s[start:start + n]


@functools.lru_cache(maxsize=None)
def demangle(name: str) -> tuple[str, str | None]:
    """Return (display_name, kind). kind ∈ {'ctor','dtor','method','data',None}.
    'data' covers section/vtable/jumptable/D_-globals; None is a loose function."""
    if name.startswith(".") or name.startswith("_vt.") \
            or name.startswith("jtbl_") or name.startswith("D_"):
        return name, "data"
    if name.startswith("_._"):
        c = _parse_class(name[3:])
        if c:
            return c, "dtor"
    if name.startswith("__") and not name.startswith("__."):
        c = _parse_class(name[2:])
        if c:
            return c, "ctor"
    sep = name.find("__")
    if sep > 0:
        c = _parse_class(name[sep + 2:])
        if c:
            return c, "method"
    return name, None


MAP_FILE = Path("build/ntsc/paperboy.map")
BUILD_DIR = Path("build/ntsc")

# ROM offsets (vrom) of TUs considered "decompiled". Edit this list as files
# get matched. The vrom is the first column you see in the output.
DECOMPILED_VROMS: set[int] = {
    0x007660,
    0x007FE0,
    0x008260,
    0x008E30,
    0x008EB0,
    0x009E10,
    0x00D260,
    0x00D2C0,
    0x00D730,
    0x00E2C0,
    0x0109C0,
    0x010CC0,
    0x011570,
    0x011890,
    0x016110,
    0x0168C0,
    0x016970,
    0x017440,
    0x0177B0,
    0x017880,
    0x025B30,
    0x028F80,
    0x029B80,
    0x029ED0,
    0x02A500,
    0x02B9C0,
    0x02C6A0,
    0x02C9A0,
    0x03D5E0,
    0x03DE40,
    0x03E8E0,
    0x040D10,
    0x041CE0,
    0x042940,
    0x043DD0,
    0x0455F0,
    0x0462D0,
    0x0463C0,
    0x0465A0,
    0x046B40,
    0x0476E0,
    0x0477C0,
    0x048AD0,
    0x049630,
    0x049A80,
    0x04A000,
    0x04A9C0,
    0x04B2F0,
    0x04B7D0,
    0x088330,
    0x313F0,
}

# Hidden from output; their exports are treated as already-decompiled so that
# dependents can still become "ready".
IGNORED_VROMS: set[int] = {
    0x003440,
    0x006BD0,
    0x00A0B0,
    0x00A670,
    0x00B010,
    0x00B720,
    0x00B7A0,
    0x00C380,
    0x00C400,
    0x00C450,
    0x00C4B0,
    0x00C6B0,
    0x00C780,
    0x00C820,
    0x00C8C0,
    0x00C950,
    0x00CA60,
    0x00CB10,
    0x00CBA0,
    0x00CCA0,
    0x00DD50,
    0x04BF70,
    0x04D0D0,
    # libmus
    0x051160,
    0x0545A0,
    0x054DC0,
    0x055220,
    0x0553E0,
    0x055680,
    # libultra
    0x0558D0,
    0x060270,
    0x063A60,
    # cpp runtime
    0x064F20,
    0x064F40,
}

DATA_SECTION_TYPES = {".rodata", ".data", ".rdata", ".ctors", ".dtors"}

GREEN = "\033[32m"
YELLOW = "\033[33m"
GREY = "\033[90m"
BOLD = "\033[1m"
RESET = "\033[0m"


def parse_text_relocs_batch(o_paths: list[Path]) -> dict[str, list[str]]:
    """One readelf invocation across all .o files. Returns {o_path_str: [sym...]}."""
    if not o_paths:
        return {}
    result = subprocess.run(
        ["mips-linux-gnu-readelf", "-rW", *map(str, o_paths)],
        capture_output=True, text=True,
    )
    out: dict[str, list[str]] = {}
    current: list[str] | None = None
    in_text = False
    for line in result.stdout.splitlines():
        if line.startswith("File: "):
            current = out.setdefault(line[6:].strip(), [])
            in_text = False
            continue
        if line.startswith("Relocation section"):
            in_text = ".text" in line
            continue
        if not in_text or current is None:
            continue
        parts = line.split()
        if len(parts) < 5:
            continue
        try:
            int(parts[0], 16)
        except ValueError:
            continue
        current.append(parts[-1])
    return out


def load_tus() -> tuple[dict[str, dict], list[tuple[str, list[str]]]]:
    """Returns ({file: tu}, [(segment_name, [tu_keys_in_order])])."""
    mf = mapfile_parser.MapFile()
    mf.readMapFile(MAP_FILE)

    tus: dict[str, dict] = {}
    segments: list[tuple[str, list[str]]] = []
    o_paths: list[Path] = []
    text_files: list[str] = []
    for segment in mf:
        seen_keys: dict[str, None] = {}
        for section in segment:
            sec_type = str(section.sectionType)
            if not section.filepath or not section.vram or section.size == 0:
                continue
            if sec_type != ".text" and sec_type not in DATA_SECTION_TYPES:
                continue
            file_key = section.filepath.as_posix()
            tu = tus.setdefault(file_key, {
                "file": file_key,
                "vrom": section.vrom or 0,
                "symbols": [],
                "extern_calls": [],
                "exports": [],
                "has_text": False,
            })
            for sym in section:
                if not sym.name or sym.name.endswith(".NON_MATCHING"):
                    continue
                tu["symbols"].append(sym.name)
            if sec_type == ".text":
                tu["has_text"] = True
                if section.vrom is not None:
                    tu["vrom"] = section.vrom
                o_path = BUILD_DIR / file_key
                if o_path.exists() and file_key not in seen_keys:
                    o_paths.append(o_path)
                    text_files.append(file_key)
                seen_keys[file_key] = None
        if seen_keys:
            segments.append((segment.name, list(seen_keys)))

    reloc_map = parse_text_relocs_batch(o_paths)
    for path, file_key in zip(o_paths, text_files):
        tus[file_key]["extern_calls"] = reloc_map.get(str(path), [])

    for tu in tus.values():
        tu["exports"] = sorted({
            demangle(s)[0] for s in tu["symbols"]
            if demangle(s)[1] in ("ctor", "dtor", "method")
        })

    return tus, segments


def build_graph(tus: dict[str, dict]) -> tuple[dict[str, set[str]], dict[str, str]]:
    sym_to_tu: dict[str, str] = {}
    for key, tu in tus.items():
        for sym in tu["symbols"]:
            sym_to_tu[sym] = key

    deps: dict[str, set[str]] = defaultdict(set)
    for key, tu in tus.items():
        for sym in tu["extern_calls"]:
            owner = sym_to_tu.get(sym)
            if owner and owner != key:
                deps[key].add(owner)
    return deps, sym_to_tu


def short_name(file_key: str) -> str:
    return Path(file_key).stem


def blocking_by_owner(tu: dict, sym_to_tu: dict[str, str],
                      decompiled_keys: set[str], ignored_keys: set[str],
                      ) -> dict[str | None, set[str]]:
    own = set(tu["exports"])
    by_owner: dict[str | None, set[str]] = {}
    for sym in tu["extern_calls"]:
        cls, kind = demangle(sym)
        if kind == "data":
            continue
        owner = sym_to_tu.get(sym)
        if owner == tu["file"] or owner in ignored_keys:
            continue
        if owner in decompiled_keys:
            continue
        name = cls if kind is not None else sym
        if name in own:
            continue
        by_owner.setdefault(owner, set()).add(name)
    return by_owner


def render_tree(tus: dict[str, dict], segments: list[tuple[str, list[str]]],
                deps: dict[str, set[str]], sym_to_tu: dict[str, str],
                root: str) -> None:
    import shutil
    import textwrap

    cols = shutil.get_terminal_size((120, 20)).columns
    decompiled_keys = {k for k, tu in tus.items()
                       if tu["vrom"] in DECOMPILED_VROMS
                       or tu["vrom"] in IGNORED_VROMS
                       or not tu["has_text"]}
    ignored_keys = {k for k, tu in tus.items() if tu["vrom"] in IGNORED_VROMS}

    target = root.lstrip(".")
    for seg_name, keys in segments:
        if seg_name.lstrip(".") != target:
            continue
        text_keys = [k for k in keys if tus[k]["has_text"]]
        if not text_keys:
            continue
        print(f"\n{BOLD}{seg_name}{RESET}")
        text_keys.sort(key=lambda k: tus[k]["vrom"])

        for key in text_keys:
            tu = tus[key]
            vrom = tu["vrom"]

            colour = ""
            if vrom in IGNORED_VROMS:
                colour = GREY
            elif vrom in DECOMPILED_VROMS:
                colour = GREEN
            else:
                d = deps[key]
                if not d or d <= decompiled_keys:
                    colour = YELLOW

            label = f"0x{vrom:X}  {short_name(key)}"

            export_comment = ""
            by_owner: dict[str | None, set[str]] = {}
            if vrom not in IGNORED_VROMS:
                if tu["exports"]:
                    export_comment = f"  # exports: {', '.join(tu['exports'])}"
                if vrom not in DECOMPILED_VROMS:
                    by_owner = blocking_by_owner(
                        tu, sym_to_tu, decompiled_keys, ignored_keys)

            head = f"  {label}"
            if colour:
                print(f"{colour}{head}{export_comment}{RESET}")
            elif export_comment:
                print(f"{head}{GREY}{export_comment}{RESET}")
            else:
                print(head)

            if by_owner:
                print(f"{GREY}      # requires:{RESET}")
                def sort_key(item):
                    k = item[0]
                    return (1, 0, "") if k is None else (0, tus[k]["vrom"], k)
                prefix = "      #   "
                wrap_w = max(40, cols - len(prefix))
                for owner_key, names in sorted(by_owner.items(), key=sort_key):
                    if owner_key is None:
                        header = "<external>"
                    else:
                        ot = tus[owner_key]
                        header = f"0x{ot['vrom']:X}  {short_name(owner_key)}"
                    body = ", ".join(sorted(names))
                    wrapped = textwrap.fill(
                        f"{header}: {body}",
                        width=wrap_w,
                        initial_indent=prefix,
                        subsequent_indent=prefix + "    ",
                    )
                    print(f"{GREY}{wrapped}{RESET}")


def main() -> None:
    import argparse
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("root", nargs="?", default="entry",
                    help="root segment to show (default: entry)")
    args = ap.parse_args()

    if not MAP_FILE.exists():
        print(f"map file not found: {MAP_FILE}", file=sys.stderr)
        sys.exit(1)
    tus, segments = load_tus()
    deps, sym_to_tu = build_graph(tus)
    render_tree(tus, segments, deps, sym_to_tu, args.root)


if __name__ == "__main__":
    main()
