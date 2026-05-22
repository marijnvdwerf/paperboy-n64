---
description: Decode N64 F3DEX2 display list commands into human-readable GBI macro names using gfxdis. Use this skill whenever you encounter raw hex display list data (like 0xDE000000, 0xDF000000), Gfx struct stores with hex constants, need to identify what a GBI command does, or are decompiling functions that build display lists. Also use when you see patterns like `gfx->w0 = 0x...` or `gfx->words.w0 = 0x...` in decompiled code and need to replace them with proper gSP*/gDP* macro calls. Triggers on "gfxdis", "display list", "GBI command", "what is this Gfx command", hex patterns starting with D9/DA/DB/DC/DD/DE/DF/E4-FC in display list context, or any request to decode or identify N64 graphics commands.
---

# gfxdis — N64 Display List Decoder

Binary: `~/Projects/gfxdis/src/gfxdis/gfxdis.f3dex2` (this project uses F3DEX2 microcode).

Do not try to decode GBI commands from memory or training data. The encodings are complex (both w0 and w1 encode multiple bitfields), and gfxdis also recognizes multi-command sequences that collapse into single high-level macros. You will get wrong answers if you guess. Always run the tool.

## Step 1: Collect hex pairs into chains

A "chain" is every consecutive Gfx write within one code path — all the w0/w1 pairs from one branch, including any shared code that runs after the branch rejoins. Read the function and trace each code path, collecting the full sequence of hex pairs it would emit at runtime.

Do NOT feed gfxdis individual w0/w1 pairs one at a time. gfxdis detects patterns across consecutive commands. For example, the 5-command sequence `FD10/E8/F5/E6/F0` is recognized as a single `gDPLoadTLUT_pal16(...)` call, but only when gfxdis sees all 5 together. Feeding them individually produces 5 wrong low-level calls instead.

## Step 2: Run gfxdis once per chain

Use `-g gfx -i -d` and pass each 32-bit word as a separate **unquoted** shell argument (quoted spaces break argument parsing):

```bash
~/Projects/gfxdis/src/gfxdis/gfxdis.f3dex2 -g gfx -i -d \
  E3001001 00008000 \
  FD100000 00000000 E8000000 00000000 F5000100 07000000 E6000000 00000000 F0000000 0703C000 \
  E7000000 00000000
```

Example output showing 5 commands collapsed into one macro:
```
gDPSetTextureLUT(gfx, G_TT_RGBA16);
gDPLoadTLUT_pal16(gfx, 0, 0x00000000);
gDPPipeSync(gfx);
```

For functions with many branches that emit different command sequences, run gfxdis once per distinct branch path.
