# Paperboy N64 Decomp

N64 decompilation project for Paperboy using splat, SN toolchain (cc1n64/cc1pln64 via wibo), and ninja build system.

## Build

Build only the file you're working on, not the full ROM:

```bash
ninja build/ntsc/src/<FILE>.o
```

asm-differ works off the .o (no link needed). Do NOT run plain `ninja` to relink everything — it's slow and the byte-mismatched checksum is irrelevant to per-function diff work.

After YAML changes, run `uv run python configure.py --clean` before building. `configure.py` may ONLY be invoked with `--clean` — never plain.

## Verify

Build produces `build/ntsc/paperboy.z64` — checksum must match `checksum.sha1`.

## Editing files

Do NOT use `sed`, `perl`, or `awk` to modify project files — use the `Edit` tool instead. They mangle whitespace and bypass review.

## The `expected/` directory

`expected/build/` is the user's known-good build, used as the TARGET side by `asm-differ`. NEVER write to `expected/` — no `cp` into it, no `mv`, no edits. If you suspect it's stale, ask the user; do not regenerate it.

## `symbol_addrs.txt`

Single list ordered by address. No blank lines, no comment-only separator lines. Entries are `name = 0xADDR; // type:func` (or no comment for non-funcs). Use `uv run tools/format_symbol_addrs.py` to re-sort after edits.

`symbol_addrs.txt` does NOT control asm-differ matching — `expected/build/` was produced at a particular moment with a particular set of symbol names, and asm-differ uses *those* names as anchors. Renaming a symbol in `symbol_addrs.txt` only affects the next build of `expected/` (which the user owns).
