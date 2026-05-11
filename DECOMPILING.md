# Decompiling Paperboy 64

A working guide based on what we've learned matching functions in this project.

## TL;DR loop

1. Pick a function. Read its asm at `asm/nonmatchings/<segment>/<name>.s`.
2. Write a C/C++ approximation. Drop it into the corresponding source file, replacing the `INCLUDE_ASM(...)` line.
3. `ninja` — this builds *and* validates against `checksum.sha1`. If `build/ntsc/paperboy.z64: OK` prints at the end, you've matched.
4. If not, `uv run asm-differ -o <function_name>` shows a side-by-side diff. Iterate on the C until it matches.
5. Format and commit.

There is no separate "matching" build target — the regular `ninja` is the match check. A non-zero `asm-differ` score does not necessarily mean the ROM is wrong (asm-differ can be fooled by symbol renames, see below), and a zero score does not necessarily mean the ROM matches (the rodata or BSS may differ even if the text is byte-identical). The sha1sum at the end of `ninja` is the source of truth.

## Build pipeline

```
splat (splat.yaml + symbol_addrs.txt)
   ↓        ↓
   ↓     paperboy.ld
   ↓        ↓
 asm/    src/*.{c,cpp}
   ↓        ↓
   └→ assembler / cc1pln64.exe (SN compiler via wibo)
      ↓
   build/{ntsc,pal}/src/*.o
      ↓
   mips-linux-gnu-ld → paperboy.elf → paperboy.z64
      ↓
   sha1sum -c checksum.sha1
```

The SN compiler we're using is the original Sony/Software Creations toolchain (`cc1pln64.exe` for C++, `cc1n64.exe` for C). They're run via `wibo` because the binaries are PE32 executables. This compiler has quirks (see below) that the diff process is sensitive to.

`-O2 -G0` everywhere. Both NTSC and PAL builds run; PAL builds also have to pass.

### `INCLUDE_ASM` and "nonmatching"

When splat sees a function it doesn't know how to decompile, it emits the body as `asm/nonmatchings/<segment>/<name>.s` and writes an `INCLUDE_ASM(...)` line in the corresponding source file. That macro inlines the asm at the point it appears in the C file, preserving instruction order and addresses.

To decompile, you replace the `INCLUDE_ASM` line with C/C++ source that compiles to the same bytes.

If the C source is good enough to match exactly, leave it. If it's *almost* matching but you can't get the last few instructions right and you don't want to block, park the draft inside `#if 0 ... #else INCLUDE_ASM(...); #endif` so the build keeps using the asm. (Or `#if 1 ... #else ... #endif` if you want the C draft to be active while you iterate.)

## Anatomy of an asm function

```
nonmatching func_8003DCC0, 0xC          # name, byte size

glabel func_8003DCC0
    /* 3E8C0 8003DCC0 8C820070 */  lw      $v0, 0x70($a0)
    /* 3E8C4 8003DCC4 03E00008 */  jr      $ra
    /* 3E8C8 8003DCC8 00000000 */   nop
endlabel func_8003DCC0
```

Each line: `ROM offset, VRAM address, raw instruction word, mnemonic`. The MIPS o32 calling convention is in use:

- `$a0..$a3` — first 4 word-sized args (incl. `this` for C++ member functions)
- `$v0`, `$v1` — return value
- `$s0..$s7` — callee-saved
- `$t0..$t9` — caller-saved scratch
- `$sp`, `$ra`, `$fp` — as usual
- 5th+ arg slots at `$sp+0x10..` (caller reserves)
- Branch delay slots execute *before* the branch is taken

## C++ in this codebase

The original game is C++ compiled with the SN toolchain. We try to preserve that — C++ classes, member functions, virtual dispatch, etc. — rather than flatten everything to C structs and free functions. Key reasons:

- Virtual function calls have a very recognizable asm pattern. Reproducing them as struct-of-function-pointer hacks tends to fight the compiler.
- Class destructors take an implicit "mode" argument (see below) which is hard to mimic without C++ syntax.
- Stack-allocated objects auto-call ctor/dtor, matching the `jal __<Class>` / `jal _._<Class>` patterns in the asm.

When in doubt, model what you see in the asm as a C++ class.

### SN compiler quirks worth remembering

#### Vtable position

> The SN compiler places the vtable pointer **after** all data members of the class declaring the virtuals.

So you can't have data members at offsets after the vtable in the same class. To get a class with both data-before-vtable and data-after-vtable, split it across an inheritance boundary: the parent holds data + virtuals (which fix the vtable at its tail), and the derived class adds data members after the inherited vtable.

Example pattern we use for `StructWWBase`:

```cpp
class StructWWParent {            // size 0x18: data 0x00..0x13 + vtable at 0x14
  public:
    s32 unk0;
    char pad04[0x10];

    StructWWParent();
    virtual ~StructWWParent();
};

class StructWWBase : public StructWWParent {   // adds data at 0x18..0x77
  public:
    musConfig config;              // 0x18 (size 0x44)
    u8* heap;                      // 0x5C
    /* ... */

    StructWWBase();
    virtual ~StructWWBase();       // overrides parent dtor; same slot
    virtual void vfunc1();         // appends slots
    /* ... */
};
```

The parent needs at least one virtual to force the vtable to be emitted at the right offset. The destructor counts.

#### Destructor slot is wherever you declare it

> Virtual destructors occupy a vtable slot in source declaration order — they are *not* implicitly at slot 0/1.

If the rodata vtable has the dtor in slot 7, declare six other virtuals before `~Class()` in the class definition. We do this for `LocalIOBase`, where slot 7 is the dtor wrapper.

#### Destructors take a "delete mode" argument

When the compiler invokes a destructor it passes a mode value in `$a1`:

- `2` — run dtor but do not deallocate (stack object going out of scope)
- `3` — run dtor and deallocate (`delete obj`)

So you'll see `jal _._<Class>; li $a1, 2` (or `3`) all over the place. This is normal; declare the destructor and the compiler handles the convention automatically.

#### `delete[]` and `new[]` map to specific allocators in this project

We've mapped:

```
__builtin_new        = 0x8004B414     (operator new)
__builtin_delete     = 0x8004B3F0     (operator delete)
__builtin_vec_new    = 0x80064320     (operator new[])
__builtin_vec_delete = 0x80064340     (operator delete[])
```

The `vec` variants are the project's custom heap allocator. Use `new u8[N]` / `delete[] ptr` for the right pattern.

#### Static dispatch happens for stack-allocated objects

> If a virtual is called on an object whose dynamic type is known statically (e.g. a stack local), the SN compiler **devirtualizes** the call to a direct `jal`.

This is critical for matching. The same line `obj.virt8(...)`:

- If `obj` is a stack local `LocalIO2 sp10;`, the compiler emits `jal virt8__11LocalIOBasePCclll` — direct.
- If `obj` is `LocalIOBase* arg1`, the compiler emits `lw vptr; lh offset; lw fn; jalr` — virtual through vtable.

Match the asm: stack local for direct, pointer for indirect.

#### Instruction scheduling is the hard part

The SN compiler aggressively reorders instructions to fill branch delay slots and hide load-use stalls. Two C programs that are semantically equivalent often produce *different* asm because of different scheduling. When you have a function that's "structurally" right but not byte-identical, look at:

- Where `lw` (load) instructions land — the compiler tries to schedule them ahead of their use so the load completes in time.
- Branch-likely (`bnel`, `beqzl`) vs regular branches — the compiler picks one based on its (often opaque) prediction heuristics.
- Order of struct field stores — the compiler often interleaves `li` constants with `sw` stores.

Small source rewrites can shift these. Examples we've used:

- Hoisting a local variable declaration to the top of the function to control when its load happens.
- Combining a NULL check with `&&` vs splitting into two `if`s.
- Replacing `while (P) { ... advance ... }` with `for (;;) { ... }` or `do { ... } while (P)` — generates different loop layouts.
- Swapping case order in `switch` — the compiler emits cases in source declaration order; the rodata jump table is in target order.

If you've tried five rearrangements and the diff still has 1–3 instruction-position issues, it's often best to commit the structurally-correct C and move on, or park behind `#if 0`.

#### Name mangling

SN cfront-style mangling. Examples:

| C++ declaration                             | Mangled name                            |
| ------------------------------------------- | --------------------------------------- |
| `StructWWBase::vfunc1()`                    | `vfunc1__12StructWWBase`                |
| `StructWWBase::vfunc6(StructVV*)`           | `vfunc6__12StructWWBaseP8StructVV`      |
| `LocalIOBase::virt11(void*, s32)`           | `virt11__11LocalIOBasePvl`              |
| `StructVV::vfunc4(s32)` returning `StructUU*` | `vfunc4__8StructVVl`                  |
| `~Class()`                                  | `_._<N><ClassName>`                     |
| `Class()`                                   | `__<N><ClassName>`                      |
| Vtable for `Class`                          | `_vt.<N><ClassName>`                    |

Where `<N>` is the length of the class name. Class types in args use the same `<N><Name>` form prefixed by `P` for pointer. Primitive types: `c` (char), `i` (int), `l` (long, includes `s32`), `Pv` (void*), `PCc` (const char*). Return types are *not* part of the mangling. The `T<n>` shorthand can substitute a previous arg type (e.g. `lPvlT2` = `(long, void*, long, void*)` — `T2` reuses arg 2's type).

#### Mangling vs the asm symbol

The asm files reference functions by their mangled names. If you change a C++ signature, the emitted mangled name changes, and the link fails because the rodata vtable (and other call sites) reference the old name.

Workflow when adding/renaming a class method:

1. Declare the method in the header.
2. Define it (or leave it as `INCLUDE_ASM` of the old name).
3. Compile once. The link will fail with an "undefined reference to <mangled>" error.
4. Find the asm address. Add `<mangled> = 0xADDR; // type:func` to `symbol_addrs.txt`.
5. Run `uv run python configure.py --clean` (a.k.a. "reconfigure") so splat regenerates with the new name. The asm file is renamed from `func_NNNN.s` to `<mangled>.s`, and any `INCLUDE_ASM` references in source files have to be updated.
6. `ninja` again.

**Important caveat:** splat only emits an asm file for a symbol if a corresponding `INCLUDE_ASM` macro exists for it in some source file. If you add a mangled name to `symbol_addrs.txt` and the original `INCLUDE_ASM(..., func_NNNN)` doesn't get updated, splat will delete the old `func_NNNN.s` *without* creating a `<mangled>.s`. The build will then fail because the asm body is gone. Always update the `INCLUDE_ASM` line(s) before reconfiguring.

### Header conventions

`include/structs.h` is the shared C++ class definitions header. Classes that cross TU boundaries (used in multiple .cpp files) live there. Classes used by only one TU can be defined inline in that .cpp.

Field offsets in comments (`/* 0x18 */`) are mandatory — they're the truth, not the field name. Field names start as `unkXX` and get renamed when you understand their role. Don't rename fields opportunistically; do it when there's evidence (e.g. when you've identified what an external library expects, like libmus's `musConfig`).

## The asm-differ workflow

```
uv run asm-differ -o <function_name>
```

The `-o` flag means "look for the function in the .o files I just built". The output is a side-by-side diff of TARGET (baserom disassembly) vs CURRENT (your build).

Reading the diff:

- Identical lines: white.
- Symbol substitution (e.g. `func_8003E1C0` ↔ `__8StructVV` resolving to the same address): annotated with `i` flag, low score impact.
- Register diff (`s0` ↔ `s1`): annotated with `r`, low/mid score.
- Different instruction: heavy red/green markers.
- Inserted line in CURRENT (extra instruction): `>` marker.

The score is a heuristic — non-zero doesn't always mean the ROM is wrong. Look at *what* is different. If asm-differ shows label-substitution-only differences (an `i` marker on every otherwise-matching line), the ROM almost certainly matches; `ninja`'s sha1sum confirms.

### When `asm-differ` says score 0 but ROM still fails

This usually means the **rodata** differs even though the text matches: a literal string ended up in a different segment, or an inline `""` produced a new entry in this TU's `.rodata` instead of merging with an existing global string. Look at the literals you used in C and where the corresponding asm references them.

### When the link fails with `defined in discarded section`

The compiler emitted a vtable referencing symbols that don't exist anywhere. Two common cases:

1. You declared a class with N virtuals but only some of them are mapped to actual addresses. The vtable references the missing ones, the linker can't resolve, the rodata section is dropped, and any code that references the vtable now points to nowhere.
2. You added a class method without updating `symbol_addrs.txt` and reconfiguring.

Fix: map every virtual function the class declares to a real address (in `symbol_addrs.txt`), or remove the virtual declarations until the rest matches.

## Class anatomy in this project

There's a recurring structure:

```
                            ┌─ data (before vtable)
ParentClass                 │
                            └─ virtual <only one or two, to force vtable here>

                            ┌─ inherits parent
DerivedClass : Parent       │
                            ├─ more data (after inherited vptr)
                            └─ virtual <main API>

LeafClass : DerivedClass    └─ optional further specialization, often empty
```

Examples:

- `StructWWParent` → `StructWWBase`. Parent: unk0 + pad + virtual dtor. Base: lots of data + 13 virtuals (vfunc1..vfunc13) and a dtor.
- `StructVVParent` → `StructVV`. Parent: unk0 + virtual dtor. VV: owner, next, etc. + dtor + vfunc1..vfunc6.
- `StructUUBase` → `StructUU`. Base: dtor + 4 virtuals. UU: owner, next, padding.
- `LocalIOBase` → `LocalIO` / `LocalIO2`. Base: data + 16 virtuals (with dtor at slot 7). LO/LO2: empty leaf classes used to instantiate stack-locals with their own vtable (different dtor wrappers).

When you find a vtable in rodata that's `N` slots big, the class hierarchy has `N` virtuals. The dtor is at some slot (count where the wrapper-style dtor lives in the rodata).

## Splitting TUs

Sometimes a single asm range is actually two separate translation units glued together by the linker. You can detect this when:

- A literal (typically `""` for crash messages) appears as two separate addresses, even though they're the same string. If they were the same TU, the SN compiler would deduplicate.
- A function references a vtable in rodata that's offset oddly.

To split:

1. Find an aligned address between the last function of TU1 and the first function of TU2.
2. Add a new entry in `paperboy.yaml`: `- [0xNNNN, cpp]`.
3. Reconfigure. Splat creates a new `src/<offset>.cpp` with `INCLUDE_ASM` stubs for the new range.
4. Move any C++ source for functions in the new range from the old `.cpp` to the new one.
5. If the two TUs share helper types (e.g. `LocalIO2`), move them to `include/structs.h`.
6. The rodata may also need splitting at a corresponding aligned offset. Add a new `rodata` entry in the yaml.

## Don't reconfigure without consent

`configure.py --clean` regenerates a lot of asm files and source-file stubs. Run it deliberately, not casually — the regenerated state is often what you want, but it can also delete in-progress asm files (see the caveat under "Mangling vs the asm symbol"). If the project is collaborative, communicate before reconfiguring.

## Glossary of project-specific gotchas

- **The empty crash string `""`** is referenced everywhere via `func_800079A8(D_80003EB8, 0, 0, 0)`. The `D_80003EB8` symbol is just an empty asciz in rodata. Whether two functions resolve the *same* `D_80003EB8` address or two different empty strings tells you whether they were originally in the same TU (single string is reused) or different TUs (each got its own).
- **`libmus` is Software Creations' audio library** and shows up extensively. The `MusInitialize` / `MusStop` / `MusAsk` / etc. symbols are part of it. `musConfig` is its init struct, 0x44 bytes.
- **Audio scheduler messages**: `OS_SC_RETRACE_MSG` (1), `OS_SC_PRE_NMI_MSG` (4), plus custom message type `0x63` ("start playback") drive the audio thread loop in `func_8003D6AC`.
- **PAL build differences**: source compiled with `-DPAL=1`. Some functions have `#ifdef PAL` blocks. Both NTSC and PAL must build for the checksum check to pass.

## Style

- We're not naming things until we have evidence of what they do. Field names start as `unkXX`. Functions are `func_NNNN` until identified. When you rename, do it in a single commit so the diff is reviewable.
- Comments describe *why*, not *what*. Don't write `// returns owner` next to `return this->owner;`.
- Run `clang-format` before committing. Configuration is in `.clang-format`.
- Don't introduce backwards-compat shims when a rename is the cleaner fix.

## A worked example

Decomp `func_8003DCC0`:

```
glabel func_8003DCC0
    lw   $v0, 0x70($a0)
    jr   $ra
    nop
```

Reads word at offset 0x70 of `a0`, returns it. `a0` is the first arg, which for a free function is just `arg0`. Offset 0x70 of `StructWWBase` is `unk70` (the pause flag we identified).

```cpp
extern "C" s32 func_8003DCC0(StructWWBase* self) {
    return self->unk70;
}
```

`ninja`. ROM OK. Done.

Decomp something harder, like a virtual method that calls another virtual:

```cpp
// vv vfunc1 — opens a .mbk file via the LocalIO2 file I/O class
void StructVV::vfunc1(const char* arg1) {
    LocalIO2 sp10;                                  // stack-local: ctor auto-runs, dtor auto-runs at scope end
    char sp40[0x40];

    if (this->vfunc3()) {                           // devirtualized: direct jal
        this->vfunc2();
    }
    strcpy(sp40, arg1);
    strcat(sp40, D_80003EB0);                       // ".mbk"
    if (sp10.virt8(sp40, 0xA, 0x1000, 0)) {         // devirtualized: direct jal virt8__11LocalIOBasePCclll
        func_800079A8(D_80003EB8, 0, 0, 0);
    }
    this->func_8003DCE0(&sp10);
    sp10.virt9();                                   // devirtualized: direct jal virt9__11LocalIOBase
}
```

The dtor for `sp10` and `this->vfunc3()` calls are all direct because the dynamic type is known statically. Three asm calls disappear into clean C++.

## Where to look next when stuck

- Other decomp projects using the same SN toolchain (e.g. `/Users/marijn/Projects/_lego-racers/newrepo/src` if you have it locally) — same patterns, often the same problems solved.
- The N64 SDK reference (`~/Downloads/n64sdkmod-master`) for libultra and libmus signatures.
- `uv run m2c <asm_file> [<rodata_file>]` — produces a starting C approximation. The output is rough but useful for the overall structure.
- `bunx ultra64 identify baserom.z64 --base 0x80000400` — identifies known libultra functions in the ROM.
