#!/usr/bin/env -S uv run python
# /// script
# requires-python = ">=3.10"
# ///
"""Summarize asm sections (or src files) from the `entry` segment of paperboy.yaml
using m2c + claude (haiku, max effort). Appends to OUT, skipping offsets already present.
"""
import argparse, glob, re, subprocess, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
YAML = ROOT / "paperboy.yaml"
RODATA = sorted(glob.glob(str(ROOT / "asm/data/*.rodata.s")))

PROMPT_ASM = (
    "You are looking at decompiled output from m2c of one .s file from a partial "
    "Nintendo 64 ROM split. The ROM is only partially split, so a single .s file "
    "may contain many unrelated functions glued together by the linker, or it may "
    "be a coherent translation unit. Summarize in 1-3 sentences what this file "
    'appears to be about (e.g. "string formatting helpers", "audio playback init"). '
    'If the functions look unrelated or you cannot identify a coherent theme, just '
    'answer "various things" (optionally with a short hint). Be terse. No preamble.'
)
PROMPT_SRC = (
    "You are looking at a partially-decompiled C/C++ source file from a Nintendo 64 "
    "ROM. Some functions are already written in C/C++; others have been filled in "
    "by m2c (a heuristic decompiler whose output is approximate). Summarize in 1-3 "
    'sentences what this file appears to be about (e.g. "Surface blitting helpers", '
    '"audio sequence player init"). If functions look unrelated, answer "various '
    'things" with a short hint. Be terse. No preamble.'
)
INCLUDE_ASM_RE = re.compile(
    r'^\s*INCLUDE_ASM\("([^"]+)",\s*([A-Za-z0-9_.$]+)\)\s*;\s*$'
)


def entry_block() -> str:
    text = YAML.read_text().splitlines()
    start = next(i for i, l in enumerate(text) if l.startswith("  - name: entry"))
    end = next(i for i, l in enumerate(text[start + 1 :], start + 1)
               if l.startswith("  - "))
    return "\n".join(text[start:end])


def asm_offsets() -> list[str]:
    return re.findall(r"\[0x([0-9A-Fa-f]+),\s*asm\]", entry_block())


def src_entries() -> list[tuple[str, str]]:
    return re.findall(r"\[0x([0-9A-Fa-f]+),\s*(c|cpp)\]", entry_block())


def run_m2c(asm_path: Path) -> str:
    r = subprocess.run(
        ["uv", "run", "m2c", str(asm_path), *RODATA],
        capture_output=True, text=True, timeout=120, cwd=ROOT,
    )
    return r.stdout.strip()


def expand_src(src: Path) -> str:
    out = []
    for line in src.read_text().splitlines(keepends=True):
        m = INCLUDE_ASM_RE.match(line)
        if not m:
            out.append(line); continue
        d, fn = m.group(1), m.group(2)
        path = ROOT / d / f"{fn}.s"
        if not path.is_file():
            out.append(f"// {line.rstrip()}  // (asm not found)\n"); continue
        try:
            draft = run_m2c(path) or "// m2c produced no output"
        except Exception as e:
            draft = f"// m2c failed: {e}"
        out.append(f"// ---- m2c({fn}) ----\n{draft}\n// ---- end m2c ----\n")
    return "".join(out)


def ask_claude(prompt: str, body: str, fence: str) -> str:
    payload = f"{prompt}\n\n```{fence}\n{body}\n```\n"
    r = subprocess.run(
        ["claude", "-p", "--model", "haiku", "--effort", "max",
         "--no-session-persistence"],
        input=payload, capture_output=True, text=True, cwd=ROOT,
    )
    return r.stdout.strip()


def already_done(out_path: Path, marker: str) -> bool:
    if not out_path.exists():
        return False
    return any(line.startswith(f"## {marker}  ") for line in out_path.read_text().splitlines())


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("mode", choices=["asm", "src"])
    ap.add_argument("--out", default="entry_summaries.md",
                    help="output markdown file (default: entry_summaries.md)")
    args = ap.parse_args()

    out_path = ROOT / args.out
    out_path.touch()

    if args.mode == "asm":
        items = [(f"0x{o.upper()}", ROOT / f"asm/{o.upper()}.s") for o in asm_offsets()]
    else:
        items = [(f"0x{o.upper()}", ROOT / f"src/{o.upper()}.{ext}")
                 for o, ext in src_entries()]

    total = len(items)
    for i, (marker, path) in enumerate(items, 1):
        if not path.is_file():
            print(f"[{i}/{total}] {marker}: MISSING {path.relative_to(ROOT)}", file=sys.stderr)
            continue
        if already_done(out_path, marker):
            print(f"[{i}/{total}] {marker}: skip (already in {out_path.name})", file=sys.stderr)
            continue
        print(f"[{i}/{total}] {marker} ({path.relative_to(ROOT)}) ...", file=sys.stderr)

        if args.mode == "asm":
            try:
                body = run_m2c(path)
            except Exception:
                body = ""
            if not body:
                summary = "(m2c produced no output)"
            else:
                summary = ask_claude(PROMPT_ASM, body, "c")
        else:
            body = expand_src(path)
            summary = ask_claude(PROMPT_SRC, body, "cpp")

        with out_path.open("a") as f:
            f.write(f"## {marker}  (`{path.relative_to(ROOT)}`)\n\n{summary}\n\n")

    # Re-sort the file by offset so new entries land in the right place.
    text = out_path.read_text()
    parts = re.split(r"(?=^## 0x[0-9A-Fa-f]+  )", text, flags=re.M)
    chunks = []
    for c in parts:
        m = re.match(r"## 0x([0-9A-Fa-f]+)  ", c)
        if m:
            chunks.append((int(m.group(1), 16), c.rstrip() + "\n"))
    chunks.sort(key=lambda x: x[0])
    out_path.write_text("\n".join(c for _, c in chunks))

    print(f"Done -> {out_path}", file=sys.stderr)


if __name__ == "__main__":
    main()
