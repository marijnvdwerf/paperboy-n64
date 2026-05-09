# Paperboy N64 Decomp

N64 decompilation project for Paperboy using splat, SN toolchain (cc1n64/cc1pln64 via wibo), and ninja build system.

## Build

```bash
ninja
```

After YAML changes, run `uv run python configure.py --clean` before building.

## Verify

Build produces `build/ntsc/paperboy.z64` — checksum must match `checksum.sha1`.
