# Class hierarchy

Living document tracking the OO structure recovered from m2c output.
Names are placeholders (mostly Australian fauna) until the real symbol
names are recovered. Specific addresses and `func_XXXXX` identifiers
live in `symbol_addrs.txt` — this file describes *structure* (class
relationships, struct layouts, roles, open questions).

## Inheritance forest

### File I/O / parsing

```
AbstractFile        (43DD0.c)
└─ RomFile          (43DD0.c)
   ├─ Pelican       (RomFile subclass with full open/close lifecycle — 462D0.c)
   ├─ Frogmouth     (inline subclass for Kookaburra string-table loader — 465A0.c)
   ├─ [the "File" pattern]
   │                — many small one-off RomFile subclasses where the author
   │                  left out a destructor, so cfront auto-generated one.
   │                  Each TU that declares one emits its own copy of the
   │                  same shape; lots of near-identical vtables in the
   │                  binary. Pelican is the one that grew real methods;
   │                  Frogmouth is the explicit inline-stub case.
   └─ Parrot        (text parser .adf/.tdf/.mdf/.gdf/.sdf/.bdf/.maf — 46B40.c)
      ├─ Magpie     (sorted-name dictionary — 4A000.c)
      ├─ Cockatoo   (binary parser .adb/.bin — 40D10.c)
      ├─ Galah      (inline .adb subclass; emitted in every consuming TU)
      ├─ Rosella    (inline .gdb subclass; emitted in every consuming TU)
      ├─ Cockatiel  (inline .sdb subclass; for Cicada particle emitter — EA5D0.c)
      └─ Budgie     (inline .tdb subclass; for Wedgetail texture registry — 28F80.c)
```

### Scene-graph tree node

```
Joey                (intrusive parent-child node — 168C0.c)
└─ Kangaroo         (Joey + 4×4 local + 4×4 world matrix — 2B9C0.c)
```

### Transform / animator

```
Dingo               (bounding sphere + velocity, dirty-flag recompute — EA5D0.c)
├─ Pademelon        (Dingo + 3×3 rotation matrix — 1F7F0.c)
│  └─ Wallaroo      (Pademelon + integer animation rates — 1F7F0.c)
│     └─ Cassowary  (Wallaroo + channel-based frame interpolator — 1F7F0.c)
└─ Brushtail        (Dingo + array of child Dingos = BVH node — EA5D0.c)
```

### Status holders

```
Quoll               (8-byte flag + vtable — 17880.c)
└─ Boodie           (Quoll + 3-byte-record array — 28F80.c)

Sentry              (status holder, vtable at unk8 — 462D0.c)
└─ Otter            (file stream handle — 4A000.c)
```

### Input devices

```
InputDeviceBase     (abstract; 38 virtuals, repeat / listener mgmt — include/input.h)
└─ ControllerDevice (N64 controller with OSContPad + OSPfs — include/input.h)

EventListener       (pure-virtual: onPressed / onReleased — include/input.h)

ControllerSystem    (forward-declared in include/input.h; manages many
                     ControllerDevices, back-referenced via each device's
                     `system` pointer — implementation file unread)
```

### Image loaders / render contexts

```
???                 (image-base / render-context; ctor at func_8001E884
                     — file unread, see Gaps; contains 6-plane frustum at
                     unk6C..unkCC, embedded file at unk10, palette area
                     at unkA4..unk4A4, vtable slot at unk5AC)
├─ Albatross          (BMP file loader — 12480.c; instance at D_8006F630)
└─ Cormorant        (TGA file loader — 27D10.c; instance at D_80070508)
```

Both loaders are global singletons built by a startup ctor
(`func_8001AD40`). Wedgetail's rebuild path picks between them by a
per-surface flag bit (BMP vs TGA asset).

### Lookup / hash dictionaries

```
Bandicoot           (8-char name → s32 hash — 25690.c)
├─ Numbat           (.adb scene loader; HAS-A Goanna at +0xC — 11B40.c)
├─ Antechinus       (scene-level named-asset manager, tree-walks via unk0 — EA5D0.c)
└─ Wedgetail          (.tdb texture / surface registry; entries are Bunyips — 28F80.c)

Skink               (8-char material-name → handle — 1F7F0.c)
└─ Platypus         (renderable geometry; commands + face planes — 1F7F0.c)

Pigeon              (PJW/ELF string-keyed hash with inline string pool — 43650.c)

Kookaburra          (wide-string localization table; uses WString — 465A0.c)
```

### Surfaces / vertex buffers

```
Surface16970        (drawing surface base; fill / blit / lock — src/16970.cpp)
├─ Echidna          (specialized surface — 3A950.c)
└─ Bunyip           (textured / colored surface with flag word — 28F80.c)

Potoroo             (vec3-buffer base; count + vec3 array — 28F80.c)
└─ Marsupial        (Potoroo + vertex array + recolor / color-rollback — 3B720.c)
   ├─ Wombat        (flat-shaded verts, text loader — 3A0B0.c)
   ├─ Wallaby       (colored verts, text loader — 3A950.c)
   └─ Bettong       (colored verts, BINARY loader — 2EEA0.c)
      └─ Currawong  (Bettong + per-vertex normals = lit binary verts — 3B720.c)
```

`Surface16970` HAS-A `Surface16970Header` (0x18-byte embedded metadata
sub-object at offset 0) and may have a `Surface16970Palette` (returned
by a virtual; NULL by default). `Rect16970` is the clip-rect type used
by the blit virtuals.

## HAS-A composition

- **Numbat → Goanna** at offset 0xC. The Numbat-loader functions that
  operate on the sub-object at +0xC are Goanna's own methods.
- **Cassowary → Goanna** via its channel array — each channel's `unk8C`
  points to a Goanna keyframe mesh, and Cassowary uses Goanna's
  interpolators directly.
- **Cassowary → Platypus** — calls Platypus virtuals from animator update
  paths.
- **Bilby → Echidna[]** — Bilby's array at `unk1C` contains Echidna
  instances (0x4C-byte elements).
- **(Texture-like class) → Possum / Dunnart** — both palette classes are
  sized by source's `unk16` bits-per-pixel field; one of them is
  embedded in a texture or similar, swapped by color format.
- **(Same containing class) → field at `unk50`** — referenced by an
  outlier accessor in 313E0.c; the containing class has a Dunnart-shaped
  palette and at least one more field 0x50 in.
- **Cicada → Wallaroo** at offset 0x10 (composition, not inheritance —
  the ctor calls `__8Wallaroo(this + 0x10)` directly). Pre-Wallaroo
  bytes hold externally-set scene / material / source pointers; post-
  Wallaroo bytes hold the particle-system state.
- **Brushtail → Dingo[]** — Brushtail's child array at `unk40` holds
  pointers to other Dingo-derived nodes, making it a bounding-volume-
  hierarchy node.

## Standalones (no inheritance / dependents established)

| class           | file     | role                                             |
| --------------- | -------- | ------------------------------------------------ |
| Lyrebird        | 3D640.c  | 3D positional audio source (Doppler)             |
| Bowerbird       | 2B9C0.c  | N64 RSP display-list builder (no vtable)         |
| Goanna          | 1F7F0.c  | mesh data: positions / colors / indices          |
| Possum          | 27750.c  | RGBA8888 palette                                 |
| Dunnart         | 313E0.c  | RGBA5551 palette (sister to Possum)              |
| Brolga          | 42D90.c  | 4-direction neighbor-aware tile-state animator   |
| Cuscus          | 41770.c  | fixed-capacity ring buffer / FIFO queue          |
| Cicada          | EA5D0.c  | particle emitter (HAS-A Wallaroo at +0x10)       |
| Quokka          | 3C030.c  | resource manager (10 typed arrays)               |
| Hydra           | 3C030.c  | (TBD — placeholder)                              |
| StructWWParent  | 3C030.c  | (TBD — placeholder)                              |
| Node3           | 3C030.c  | 343.0f / 171.5f sound-related node               |
| Clone           | 3A950.c  | proxy / transform wrapper                        |
| Bilby           | 3A0B0.c  | object container holding Echidnas                |
| InputDeviceBase | 41CE0.c  | input device base (recent commit, not re-read)   |
| LocalIOParent   | 43DD0.c  | local I/O parent (recent commit, not re-read)    |

## Confidence map

- **Confident** — verified directly from m2c reading:
  - AbstractFile / RomFile / Parrot family and the inline Parrot subclasses
  - Joey → Kangaroo
  - Bandicoot → Numbat
  - Skink → Platypus
  - Pademelon → Wallaroo → Cassowary
  - Magpie under Parrot (vtable at `unk28` is Parrot's slot)
  - File under RomFile
  - Marsupial → Wombat / Wallaby (Wombat / Wallaby's parent ctor matches
    Marsupial's address)
  - Marsupial → Bettong → Currawong (Bettong's ctor in 2EEA0.c calls
    Marsupial's ctor; Currawong's ctor in 3B720.c calls Bettong's ctor)
  - Dingo → Pademelon → Wallaroo → Cassowary (Pademelon's ctor in
    1F7F0.c calls Dingo's ctor in EA5D0.c)
  - Dingo → Brushtail (Brushtail's ctor in EA5D0.c calls Dingo's ctor
    in the same file)
  - Bandicoot → Antechinus / Wedgetail
  - Quoll → Boodie
  - Potoroo → Marsupial (Marsupial's ctor in 3B720.c calls Potoroo's
    ctor in 28F80.c — closes the vertex-buffer chain to a real root)
  - Surface16970 → Echidna / Bunyip (Surface16970 itself confirmed via
    src/16970.cpp — fill / blit / lock interface)

- **Moderate** — carried from earlier session, not re-verified:
  - Sentry → Otter

- **Low** — open questions:
  - Pademelon's parent class (its file is unread — see Gaps)
  - Marsupial's parent class (file unread — see Gaps)
  - Whether Quoll has subclasses we haven't yet seen
  - Hydra / StructWWParent / Node3 real roles

## Per-class reference

Layouts use instance-relative offsets (`unk0`, `unk4`, ...). For
addresses of ctor / dtor / vtable, see `symbol_addrs.txt`.

### AbstractFile (43DD0.c)

File I/O base. Provides the external `open` interface used by all
RomFile-derived classes.

### RomFile (43DD0.c)

ROM-backed file class. Subclassed by Parrot, File, and the inline
Frogmouth pattern.

### File (462D0.c)

Header-defined RomFile subclass. Vtable at instance offset `unk28`. The
canonical TU (462D0.c) emits the dtor body and the open/close virtuals;
the ctor is inline in the header. Has an `arg1 & 0x20` "transient" flag
in its open path that suppresses buffer reallocation.

### Parrot (46B40.c)

Text-format token parser inheriting RomFile. Vtable at `unk28`. Parses
`.adf/.tdf/.mdf/.gdf/.sdf/.bdf/.maf` via token codes. Filename buffer at
`unk1A4` with a 0x40-byte inline / heap-fallback storage.

### Magpie (4A000.c)

Sorted-name dictionary. Parrot subclass — vtable at `unk28`. An alphabet
lookup table accelerates name searches.

### Cockatoo (40D10.c)

Binary-token Parrot subclass for `.adb` / `.bin`. Vtable at `unk28`. Big
binary opcode switch reading u8 / s16 / s32 / f32 fields and nested
arrays. Class size 0x654.

### Galah — inline `.adb` Parrot subclass

Header-defined Parrot subclass whose only override is the vtable. The
ctor is inlined into the using site (raw `new` + `Parrot::Parrot()` +
vtable swap). The destructor is a trivial stub that tail-calls
`Parrot::~Parrot`. Emitted in every consuming TU; the linker keeps each
copy at its own address.

Observed in: 11B40.c (Numbat loader), 1F7F0.c (Goanna loader).

### Rosella — inline `.gdb` Parrot subclass

Same shape as Galah, for `.gdb` files.

Observed in: 1F7F0.c (Skink loader), 1F7F0.c (Platypus loader).

### Cockatiel — inline `.sdb` Parrot subclass

Same shape as Galah / Rosella, for `.sdb` files. Used by Cicada's
particle-spec loader.

Observed in: EA5D0.c (Cicada loader).

### Budgie — inline `.tdb` Parrot subclass

Same shape, for `.tdb` files. Used by Wedgetail's texture / surface
registry loader.

Observed in: 28F80.c (Wedgetail loader).

### Frogmouth — inline RomFile subclass

First "inline subclass" pattern we've seen rooted at **RomFile** instead
of Parrot. Same shape: header-defined RomFile subclass whose only
override is the vtable; the ctor is inlined at the using site
(stack-allocated `RomFile::RomFile()` + vtable assignment to `unk28`);
the destructor is a trivial stub that tail-calls `RomFile::~RomFile`.
Used to read raw bytes from files without going through Parrot's token
machinery.

Observed in: 465A0.c (Kookaburra loader).

### Joey (168C0.c)

Intrusive parent-child tree node. 16-byte struct: `unk0` parent
backlink, `unk4` next-sibling, `unk8` head-of-children, `unkC` vtable.
Operations: attach-as-head and detach. Root class — base for everything
participating in a scene / owner tree. Dtor body not present in this
TU — header-defined, lives elsewhere.

### Kangaroo (2B9C0.c)

Joey + transforms. Adds a local 4×4 matrix at `unk10..unk4C` and a
cached world 4×4 at `unk50..unk8C`. Root-update functions multiply
parent → children and recurse into grandchildren via the Joey list. Dtor
body not in this TU.

### Dingo (EA5D0.c)

Bounding sphere + velocity, ~0x34-byte root class. Layout: center
vec3 at `unk0..unk8`, radius at `unkC` (negative-sentinel "dirty"
flag), axis bounds `(center.x − r, center.x + r)` at `unk10..unk14`,
velocity vec3 at `unk18..unk20`, auxiliary vec3 at `unk24..unk2C`,
vtable at `unk30`.

The dirty-flag pattern: methods that read bounds first check
`unkC < 0` and call the vtable's "recompute" virtual (`unk30->unkC`)
to refresh the sphere from whatever the subclass uses as its true
geometry. Provides standalone sphere ops: set / get center, set
radius (with derived bounds recompute), get / set velocity,
sphere-vs-sphere intersect, sphere-vs-point distance², integrate-by-
velocity, ray-vs-sphere query.

Base of the transform / animator chain and the BVH-node chain.

### Brushtail (EA5D0.c)

Dingo subclass — bounding-volume-hierarchy node. Adds a pointer-array
of child Dingos at `unk40` (count `unk3C`, capacity `unk34`,
precomputed `1.0/count` at `unk38`).

Its compute-bounds override builds a sphere enclosing all children:
walks every child, averages their centers (weighted by `1/N`) to get
the parent center, then computes the max `(center_distance + child_radius)`
to get the parent radius. Writes the result back into Dingo's center
and radius via the same setters Dingo exposes.

Adds child-dispatch helpers that broadcast various vtable methods to
all children — the "tick all", "update all", "render all" pattern
typical of scene-graph aggregates. Has its own heap-id slot.

### Pademelon (1F7F0.c)

Dingo + 3×3 rotation matrix at `unk34..unk54` (translation continues
through `unk60` either reusing Dingo's center or in the matrix's `t`
row — to confirm against asm). Vtable at `unk30`. Methods for basis
construction from vectors, quaternion ↔ matrix conversion, inverse
transform, matrix multiply.

### Wallaroo (1F7F0.c)

Pademelon + integer angular / translation rates (`unk6C/6E/70/74`),
scale `unk64`, flag word `unk68`, asset / skeleton ptrs `unk78`/`unk7C`,
max-distance² `unk80`. Adds LOD distance check and rate integration.

### Cassowary (1F7F0.c)

Wallaroo + channel-based animator. Adds per-channel state at
`unk84..unkC8`: parser ptr, frame cursor, mesh data ptr, current/target
rotation, blend weight, blend mesh. Frame interpolation, segment start /
tick / LOD all live here.

### Quoll (17880.c)

8-byte flag + vtable. Trivial getter/setter for the flag. Confirmed
parent of at least Boodie (28F80.c).

### Boodie (28F80.c)

Quoll subclass with a 4-byte-record array at `unk8` (each record has
`unk0` zero + three u8 fields at `unk1`/`unk2`/`unk3`). Reuses Quoll's
`unk0` flag field semantically as the array count. Loader reads
`count * 3` u8 values from a Parrot stream. Could be RGB triples,
material IDs, or another small tag array — exact role TBD.

### Sentry (462D0.c) — from prior summary

Small status holder, vtable at `unk8`. Parent of Otter.

### Otter (4A000.c) — from prior summary

File stream handle. Vtable at `unk8`. Sentry subclass.

### Bandicoot (25690.c)

Open-addressing linear-probe hash table mapping 8-char names to s32
values. 0xC-byte slots (8-byte name + 4-byte value). Rolling-shift hash:
`hash += key[i] << shift; shift = (shift + 7) & 0x1F`.

### Antechinus (EA5D0.c)

Bandicoot subclass — scene-level named-asset manager. Adds fields
beyond Bandicoot's base: counter at `unkC`, entry count at `unk10`,
scene / set id at `unk14`.

Loaders for both `.sdb` binary streams (via the inline Cockatiel
Parrot subclass) and raw count-based init. Each registered entry gets
hashed into the Bandicoot base by 8-char name, but entries can also
form a tree (linked via each entry's `unk0`) — recursive dispatch
methods walk the tree top-down for "register all" / "release all"
operations.

### Wedgetail (28F80.c)

Bandicoot subclass — `.tdb` texture / surface registry. Adds: scene /
parent pointer at `unkC` (registers self with the scene on construction),
fields at `unk10..unk14`, entry count at `unk18`. Vtable at `unk8`.

Loader opens a `.tdb` file via Cockatoo or the inline Budgie Parrot
subclass, then iterates 8-char-named entries; for each, allocates a
slot via a subclass virtual (`unk30`), inserts the name → slot mapping
into the Bandicoot base, and parses tokens 40-46 to populate the
slot's color (RGB at `unk30`), size (`unk34`), and a 13-bit-or-so
flag word at `unk36`.

Entries are **Bunyips** (or Bunyip-shaped Surface16970 subclasses) —
the offset layout matches exactly. A "rebuild" pass walks all entries
and refreshes per-entry state from current scene context.

### Numbat (11B40.c)

Bandicoot + Goanna sub-object + two parallel arrays loaded from `.adb`:

| range        | role                                                       |
| ------------ | ---------------------------------------------------------- |
| 0x00-0x0B    | Bandicoot base (name → entry-index hash)                   |
| 0x0C-0x1F    | Goanna sub-object (geometry mesh data)                     |
| 0x20-0x27    | small-record array (0x14 stride, count at `unk24`)         |
| 0x28-0x2F    | named-node array (0x34 stride, count at `unk2C`)           |

The named-node array's 8-char names get registered into the Bandicoot
hash for fast lookup by name.

### Skink (1F7F0.c)

8-char material-name → handle resolver. No vtable despite a
virtual-dtor-shaped destructor mangling. Holds an `s32 *` array at
`unk8` of resolved material handles. Two loaders (parser-based and raw
count/data).

### Platypus (1F7F0.c)

Skink + renderable geometry. Vtable at `unk48`. Adds a packed command
stream at `unk20` (opcodes `0x10000000` / `0x40000000` / `0x50000000` /
`0x60000000` for triangle / material / sync / end), face-plane data at
`unk18` (16-byte planes), bounds, scale. Loads from `.gdb` via Rosella.

### Pigeon (43650.c) — from prior summary

String-keyed hash table with PJW/ELF hash and an inline string pool.
Vtable at `unk1C`. Standalone, no inheritance found.

### Kookaburra (465A0.c)

Wide-string localization table. Root class. Holds a u16-char content
buffer at `unk4`, a u16 index array at `unk8` (each entry is
`byte_offset / 2` into the content), a u16 string count at `unkC`, and
an init flag at `unk0`. Vtable at `unk10`. Loaded from a binary file via
a temporary Frogmouth. Search is linear; the get-as-WString path
constructs a `WString`.

Quirk: one cleanup helper uses plain `delete` on `unk4`/`unk8` even
though the loader allocated them with `vec_new` (the other cleanup
helper correctly uses `vec_delete`). Could be m2c misreading or a real
mismatched-deleter bug — worth checking against asm before fixing names.

### WString (src/10CC0.cpp)

Wide-string class — the **real** name (already decompiled). 0xA-byte
struct: `u16 *data` (buffer pointer), `u16 capacity`, `u16 unk6`
(window-start offset), `u16 length`. The `size()` accessor returns
`length - unk6` — i.e., the visible substring is `data[unk6 .. length]`,
so `unk6` acts as a sliding window into the underlying buffer.

Methods cover:
- Assign from `u16*` / from another WString (with capacity checks).
- Equality vs `u16*` / vs WString (length-then-elementwise).
- Append from `u16*` / from WString.
- Window advancement: skip past a newline, reset and find first
  newline, reset and find string end (collectively a line-iterator
  pattern).
- Bind / wrap an existing `u16*` buffer with a given capacity.
- Copy out to a `u8*` (narrows u16 chars to bytes).

The file also contains C-linkage free functions for u16 ↔ u8
conversions, a u16 `strcmp` / `strlen`, and an int → u16 decimal
formatter. The `strcmp`-equivalent is the one Kookaburra uses for its
localized-string lookups.

### Goanna (1F7F0.c)

Mesh data. Vec3 positions at `unk8` (count `unk0`, stride 0xC), RGBA8
colors at `unkC` (count `unk2`, stride 4), u16 indices / keyframe times
at `unk10` (count `unk4`, stride 2). Standalone, no vtable. Loaded from
`.adb` via Galah; offers interpolation helpers used by Cassowary's
channel system.

### Possum (27750.c)

RGBA**8888** palette. Three-field struct (vtable at offset 0, array ptr
`unk4`, count `unk8`). Size set to `1 << source->unk16` — i.e., 2^bits
indexed-color palette typical for N64 textures.

### Dunnart (313E0.c)

RGBA**5551** palette — sister class to Possum. Identical 0xC-byte struct
shape and identical allocator pattern, but stores 2-byte 5551 entries
instead of 4-byte 8888. Pack / unpack uses the classic N64
5-bit-to-8-bit MSB-replication trick. The get / set / find / batch APIs
all convert to RGBA8888 at the boundary, so callers can stay
format-agnostic — Possum and Dunnart are interchangeable from the
outside, presumably swapped based on the texture's color-format field.

### Outlier in 313E0.c

A 0x10-aligned getter at the very start of 313E0.c returns a field at
`unk50` — way past Dunnart's 0xC-byte footprint. Almost certainly a
method of the *containing* texture / material class that owns the
Dunnart, emitted in this TU because cfront placed the inline here.
Worth pairing with whichever class owns the texture format field at
`unk16`.

### Orphan stubs in 3DE00.c

A whole TU consisting of nothing but eight 8-byte default-virtual stubs
(two `return 0`, six empty void). Sits right after Lyrebird's TU ends.
No ctor / dtor / vtable in the TU itself — these are the *target bodies*
for some other class's vtable slots. Candidates: a Listener class
collaborating with Lyrebird (whose home file we still haven't found),
an overflow TU for Lyrebird's own vtable, or some abstract base whose
canonical TU is elsewhere. Unnameable until the vtable referencing
these stubs surfaces.

### Lyrebird (3D640.c)

3D positional audio source. Position / velocity / gain / frequency state
plus inner / outer range (squared). Handles NULL / single / multi-listener
cases via a linked list of listeners (`Listener.unk4C` = next). Constants
give it away: 343.0f (speed of sound), 171.5f (Doppler clamp), 0.7f
(damping).

### Bowerbird (2B9C0.c)

N64 RSP display-list builder. Standalone (no vtable). Pushes 8-byte
opcodes: `0xD9` (SETOTHERMODE_H), `0xDE` (G_DL), `0xDF` (G_ENDDL), `0xE9`
(G_TILESYNC).

### Cuscus (41770.c)

Fixed-capacity ring buffer / FIFO queue. Root class, 0x1C-byte struct:
vtable at offset 0, init flag at `unk4`, storage ptr at `unk8`, tail
index at `unkC`, head index at `unk10`, capacity at `unk14`, size at
`unk18`. Slots are 0x10 bytes each: three s32 payload fields plus two
u8 flag fields.

Four distinct push entry points all share the "write payload + advance
write index + bump size" core; they differ only in what they set the
slot's two u8 flags to after the write — `(1, 1)`, `(0, 0)`, `(1, 0)`,
or a fourth no-flags variant. So the flag bits are *metadata for
downstream consumers* (priority / processing-class), not used by the
queue itself. Two pop variants drain from head and tail. The allocator
fires a parent-overridable virtual on `unk0->unk34` before allocating —
a pre-alloc hook for subclasses.

### Cicada (EA5D0.c)

Particle emitter. No own vtable. Layout:

| range | meaning |
| ----- | ------- |
| `0x00..0x0F` | externally-set pointers (source, material, scene, current bounds) |
| `0x10..~0x8F` | embedded Wallaroo sub-object (transform, animation) |
| `0x90..` | particle-pool state |

The pool is a fixed-capacity array of 0x44-byte slots at `unkA0`, with
intrusive free / active linked lists threaded through each slot's
`unk40`. Per-particle state: position, velocity, age, lifetime,
material binding, next-in-list. Motion parameters (linear / angular
acceleration, drag, lifetime bounds, color-over-life) live at
`unkB0..unkD8` and select between "constant force" and "directional
spawn" via flag bits at `unkAC`.

The tick path decrements ages, releases expired particles to the
free list, integrates active particles. The emit-batch path walks the
active list and emits 4 vertices + 2 indexed triangles per particle
into a Platypus-style command stream — particles are billboard quads
in screen space.

Loads particle specs from `.sdb` files via a temporary Cockatoo or
an inline Cockatiel Parrot subclass (similar pattern to Goanna /
Platypus loaders).

### Brolga (42D90.c)

4-direction neighbor-aware state animator. Root class, 0x24-byte struct:

| offset | meaning |
| ------ | ------- |
| `unk0`  | vtable |
| `unk4`  | init flag |
| `unk8` / `unkC` / `unk10` / `unk14` | four cardinal-direction match IDs (u32) |
| `unk18` | current state index (0-8) |
| `unk1C` | per-instance id used as `<< 24` shift for packed DL commands |
| `unk20` | recompute-cooldown timer |

Every event-handler entry point runs the same scan: for each of the 4
directions, ask the scene (passed in via `arg1`, with a "are these IDs
adjacent" predicate accessible through its vtable) whether this slot's
ID matches. Then check prev / next neighbors (mod 4) to refine the state
index. Encoding is `(matchedDir * 2) + 1`, then `-1` if previous neighbor
matches, `+1` if next neighbor matches — 8 distinct states for the 8
combinations.

On state change, emits a packed DL command
`(this->unk1C << 24) | (state | 0x50000000)` to the scene's event
dispatcher. Same `0x50` opcode family as Platypus's animation-event
commands — hooks into the same DL machinery. A cooldown path in the
tick handler debounces rapid changes by 0x32 ticks.

Classic auto-tiling / connector pattern (pipes, fences, road segments,
or any visual element whose frame depends on its four cardinal
neighbors).

### Quokka (3C030.c) — from prior summary

Resource manager. Vtable at `unkEC`. Holds 10 typed arrays.

### Node3 (3C030.c) — from prior summary

Contains 343.0f / 171.5f constants — likely sound-related (same
constants as Lyrebird). Real role TBD.

### Hydra (3C030.c) — from prior summary

Vtable at offset 0. Real role TBD.

### StructWWParent (3C030.c) — from prior summary

Vtable at offset 0x14. Real role TBD.

### Wombat (3A0B0.c) — from prior summary

Vertex buffer. Marsupial subclass — vtable at `unk8`. 0x10-byte vertex
format, flat-shaded.

### Bilby (3A0B0.c) — from prior summary

Object container with array at `unk1C` of 0x4C-byte elements (Echidnas).

### Wallaby (3A950.c) — from prior summary

Colored vertex buffer. Marsupial subclass — vtable at `unk8`. Sibling of
Wombat — reads RGB from file and scales by 127.

### Potoroo (28F80.c)

Vec3-buffer root class. Layout: count at `unk0`, secondary u16 at `unk2`
(defaults to 5 — a type / capacity hint), vec3 array pointer at `unk4`,
vtable at `unk8`. Loader reads `count` float-triples from a Parrot
stream. Standalone setters / getters for vec3-at-index. A cluster of
trivial default-virtual stubs that subclasses override.

Base of the Marsupial → {Wombat / Wallaby / Bettong → Currawong} family.

### Marsupial (3B720.c)

Potoroo + vertex array. Owns the 0x10-byte vertex array at `unkC`
(s16 pos[3], pad, s16 uv[2], u8 rgba[4]); manages a "saved colors" temp
buffer at `unk14` and a dirty flag at `unk10` for in-place recolor /
rollback. Vtable at `unk8` (Potoroo's slot, overridden).

Recolor entry saves the original RGBA into `unk14` on first call, then
applies a per-channel `(c >> shift) + offset` transform to each vertex
RGBA. A separate restore function undoes it by copying the saved temp
colors back.

### Bettong (2EEA0.c)

Marsupial subclass that adds a "loaded" state flag at `unk2` and a
**binary** loader for the standard 0x10-byte Marsupial vertex (pos +
uv + rgba). Vertex bytes come straight off the stream as u8 RGBA (read
via Parrot's u16 reader and truncated), versus Wallaby's text/float
loader that reads each channel as a float scaled by 127.

Standalone setters/getters for position, UV (with the ×127×32 ↔ 1/127
roundtrip), and RGBA at index `i`. Parent of Currawong.

### Currawong (3B720.c)

Bettong + per-vertex normals. Same 0x10-byte Marsupial vertex layout
plus a parallel normal array at `unk18` (0xC-stride vec3 f32). Loaded
from `.gdf/.mdf`-style streams: s16 positions, s16 uv (×127×32), RGBA
fixed to `0xFF` (lit, not vertex colored), then a vec3 normal per
vertex. Inheritance chain: Marsupial → Bettong → Currawong; the ctor's
parent call lands on Bettong (2EEA0.c), which in turn calls Marsupial.

### Surface16970 (src/16970.cpp)

Drawing-surface root class. Confirmed (real cfront-mangled name —
`__12Surface16970`). Layout: a 0x18-byte embedded `Surface16970Header`
at offset 0 (format / state metadata, including `unk16` = bit depth at
4/8/16/24/32), then primary buffer pointer at `unk18`, secondary
buffer pointer at `unk1C`, pitch / stride at `unk20`, two u16 lock
flag words at `unk22` / `unk24`, width / height at `unk26` / `unk28`.
Vtable lives at `unk2C`. Total size 0x30.

Virtual interface (vfuncs 2-13):
- `vfunc2/3`, `vfunc4/5` — lock / unlock the primary (`unk18`) and
  secondary (`unk1C`) buffers; bit-flag protocol in `unk22` / `unk24`.
- `vfunc8` — get optional `Surface16970Palette*` (default NULL;
  indexed-color subclasses override).
- `vfunc9(fill)` — fill the surface with a constant color, with
  per-bit-depth code paths (4/8/16/24/32). The 4-bit path does nibble
  packing.
- `vfunc10(dstX, dstY, src, clip)` — copy from another Surface16970
  with clip-rect, after format check + palette propagation.
- `vfunc11(dstX, dstY, srcAddr, srcPitch, clip)` — low-level blit
  primitive backing vfunc10; also has a 4-bit nibble path.
- `vfunc6/7/12/13` — empty defaults, subclass hooks.

A family of related types defined / used in this file:
- **`Surface16970Header`** (0x18-byte sub-object) — format / state.
  Has its own helper TU at 0x80015xxx with per-field validators that
  this file forwards to via thin extern-C wrappers.
- **`Surface16970Palette`** — optional palette returned by `vfunc8`.
- **`Rect16970`** — `(x1, y1, x2, y2)` clip rectangle used by blits.

### Echidna (3A950.c) — from prior summary

Specialized surface inheriting Surface16970. Vtable at `unk2C`.

### Bunyip (28F80.c)

Surface16970 subclass — textured / colored render surface. Vtable at
`unk2C` (Surface16970's slot, overridden). Adds: packed RGB at
`unk30..unk32`, spare byte at `unk33`, u16 size / type at `unk34`,
u16 flag word at `unk36` (many bits observed: 1, 2, 4, 8, 0x10, 0x20,
0x40, 0x80, 0x100, 0x200, 0x800, 0x1000), and a u64 8-char name at
`unk38..unk3C` populated by Wedgetail's loader.

Draw entry reads scene flags via the scene's `unk12C` vtable, updates
its own flag bits, then dispatches into the renderer through the
draw-context's `unk5AC` hook. Acts as the per-surface render record
that the Wedgetail registry holds and rebuilds.

### Clone (3A950.c) — from prior summary

Proxy / transform wrapper. Vtable at `unk12C`. Delegates to wrapped
object at `unk138`. Many trivial default virtuals at top of class.

## Gaps / next files to read

The biggest tree-shaped gaps. Addresses here identify *unread files*
that would resolve open questions — they belong in this section
specifically because they're search keys for "which file to read next",
not symbols we want to track long-term.

1. **41CE0.c (InputDeviceBase)** — input class family.
2. **43DD0.c (AbstractFile / LocalIOParent / RomFile)** — verify the
   AbstractFile → RomFile → File chain and surface LocalIOParent's role.
3. **File containing `func_8001E884`** — the parent ctor of both
   Albatross (BMP loader, 12480.c) and Cormorant (TGA loader, 27D10.c).
   This is the image-base / render-context class that also owns the
   6-plane frustum at `unk6C..unkCC` referenced by 28F80.c's frustum
   tests.
