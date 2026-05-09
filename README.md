# Paperboy (N64) Decompilation

A work-in-progress decompilation of Paperboy for the Nintendo 64.

## Setup

Requires `wibo`, `mips-linux-gnu` cross toolchain, and `uv`.

```bash
# Place your ROM as baserom.z64
uv sync
uv run python configure.py --clean
ninja
```

Builds both a matching NTSC ROM (`build/ntsc/paperboy.z64`) and a fake PAL variant (`build/pal/paperboy.z64`).

## Diffing

```bash
ninja && uv run asm-differ -o func_80029900
```
