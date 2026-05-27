#include "common.h"
#include "wstring.h"
#include "render_context.h"
#include "surfaces.h"

extern "C" {
void qsort(void* base, s32 nmemb, s32 size, s32 (*compar)(const void*, const void*));
}

struct Slug;

extern "C" void* D_8006F620;
extern "C" const char D_80001530[];
extern "C" void func_8004B3BC(void*);
extern "C" void func_8004B390(void);
extern "C" u8* D_8006F628;

extern "C" s32 func_800178D0(Slug* self, u32* a1, u8* a2, s32 stride, s32 arg4);

// Alignment-1 byte blobs copied to/from the 0x2C region of Slug.
struct Blob4 {
    u8 b[4];
};

struct Blob8 {
    u8 b[8];
};

// Two aligned words at 0x2C (a draw position).
struct Coords2 {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
};

// The 0x2C region is accessed two ways: as aligned coords, and as a raw
// byte blob (an unaligned 8-byte copy / a single flag byte).
union SlugPos {
    Coords2 coords;
    Blob8 raw;
};

struct Rect {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 w;
    /* 0xC */ s32 h;
};

// The two global render targets at D_8006F630 / D_80070508. Large polymorphic
// objects; only unk44 and virtual slots 3 & 4 are touched here. SN places the
// vptr after the data, so it lands at 0x5AC.
struct RenderTarget {
    char pad0[0x44];
    /* 0x44 */ s32 unk44;
    char pad48[0x5AC - 0x48];
    /* vptr at 0x5AC */
    virtual void vf1();
    virtual void vf2();
    virtual void vf3(void* a); // slot 3
    virtual void vf4(); // slot 4
};

extern "C" RenderTarget D_8006F630;
extern "C" RenderTarget D_80070508;

// The active draw surface base: PixelFormat header at 0x0, vptr at 0x2C, at
// least 16 virtual slots (slots 15 & 16 are used here).
struct SurfaceBase {
    /* 0x00 */ PixelFormat hdr;
    char pad18[0x2C - 0x18];
    /* vptr at 0x2C */
    virtual void vf1();
    virtual void vf2(u8** outAddr, s32* outPitch, s32 mode); // slot 2 (lock)
    virtual void vf3(); // slot 3 (unlock)
    virtual void vf4();
    virtual void vf5();
    virtual void vf6();
    virtual void vf7();
    virtual void vf8();
    virtual void vf9();
    virtual void vf10(s32 a, s32 b, SurfaceBase* src, Rect* clip); // slot 10 (blit)
    virtual void vf11();
    virtual void vf12();
    virtual void vf13();
    virtual void vf14();
    virtual void vf15(); // slot 15
    virtual void vf16(void* a, void* b); // slot 16
};

// Derived surface adding the 0x30/0x36 tail after the base vptr.
struct DrawSurface : SurfaceBase {
    /* 0x30 */ Blob4 unk30;
    char pad34[0x36 - 0x34];
    /* 0x36 */ u16 unk36;
};

extern "C" DrawSurface D_8008CA40;
extern "C" DrawSurface* D_8006F624;

// One glyph entry in the Slug glyph table (0xC bytes).
struct GlyphEntry {
    /* 0x0 */ u16 unk0; // character code
    /* 0x2 */ u16 unk2;
    /* 0x4 */ u16 unk4;
    /* 0x6 */ u16 unk6;
    /* 0x8 */ u16 unk8;
    /* 0xA */ u16 unkA; // glyph width
};

// Slug has a 9-entry vtable (vptr placed at 0x3C, after the data, per SN).
// Only the ctor/dtor are expressed as C++ here; the virtual slot bodies live
// as func_NNNN (see the vtable below) and aren't wired up as members.
struct Slug {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ u32 unk04;
    /* 0x08 */ u32 unk08;
    /* 0x0C */ u32 unk0C;
    /* 0x10 */ u32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ GlyphEntry* unk24;
    /* 0x28 */ u32 unk28;
    /* 0x2C */ SlugPos unk2C;
    /* 0x34 */ Blob4 unk34;
    /* 0x38 */ Blob4 unk38;
    /* vptr at 0x3C */

    Slug(); // __4Slug
    virtual void vfunc1(void* a, void* b); // slot 1 = func_80017520
    virtual void vfunc2(void* a, void* b) = 0; // slot 2
    virtual SurfaceBase* vfunc3(s32 ch) = 0; // slot 3
    virtual void vfunc4() = 0; // slot 4
    virtual void vfunc5() = 0; // slot 5
    virtual void vfunc6() = 0; // slot 6
    virtual void vfunc7() = 0; // slot 7
    virtual ~Slug(); // slot 8 = _._4Slug
    virtual void vfunc9(); // slot 9 = func_800198C4
};

extern "C" int sprintf(char* dst, const char* fmt, ...);

extern "C" void func_800176EC(Slug* self, void* arg1);
extern "C" void func_800179CC(Slug* self, void* arg2, PixelFormat* fmt);
extern "C" void func_80017DA8(Slug* self);
extern "C" void func_80019800(Slug* self, s32* buf, u8* addr, s32 pitch);
extern "C" s32 func_80019648(Slug* self, s32* buf, u8* addr, s32 pitch, s32 x);
extern "C" s32 func_80019724(Slug* self, s32* buf, u8* addr, s32 pitch, s32 x);
extern "C" s32 func_8001908C(const void* a, const void* b);

extern "C" void func_80017520(Slug* self, void* arg1, void* arg2) {
    PixelFormat fmt;

    RenderTarget* ctx;
    if (self->unk28 & 0x10) {
        ctx = &D_80070508;
    } else {
        ctx = &D_8006F630;
    }

    ctx->vf3(arg1);

    self->unk18 = ctx->unk44;

    D_8008CA40.unk36 = self->unk28;
    D_8006F624 = &D_8008CA40;

    if (self->unk28 & 0x20) {
        D_8008CA40.unk30 = self->unk34;
        D_8008CA40.unk30.b[3] = 0;
        D_8008CA40.unk36 |= 0x800;
    }

    D_8006F624->vf16(arg2, ctx);

    fmt = D_8006F624->hdr;

    ctx->vf4();

    func_800176EC(self, arg1);
    func_800179CC(self, arg2, &fmt);

    self->vfunc2(arg2, &fmt);

    func_80017DA8(self);
    qsort(self->unk24, self->unk20, 0xC, func_8001908C);

    D_8006F624->unk36 = 0;
    D_8006F624->vf15();
}

INCLUDE_RODATA("asm/nonmatchings/slug", D_80001530);

// Glyph-table layout builder. Structurally complete (score ~1904, all `r`/shift
// markers). Blocked on one interlocked codegen decision: the target materializes
// the constant 1 once (the vf2 lock mode) and shares that register for `i` and the
// `i != 1` compare, leaving LICM no free register so the format string is recomputed
// in the cold branch and the loop stays top-test. Our build allocates the compare
// constant late, frees a register, LICM hoists the format string (8th callee-saved
// reg), and the loop rotates to a guarded do-while. No source lever found to force
// the early/shared const-1 allocation.
#ifdef NON_MATCHING
extern "C" void func_800176EC(Slug* self, void* arg1) {
    char buf[256];
    u8* addr;
    s32 pitch;
    u32 i = 1;

    s32* glyphBuf = new s32[self->unk18];
    if (glyphBuf == NULL) {
        __assert(D_80001530, NULL, 0, NULL);
    }
    D_8006F624->vf2(&addr, &pitch, 1);
    func_80019800(self, glyphBuf, addr, pitch);
    s32 r = func_80019648(self, glyphBuf, addr, pitch, 0);
    self->unk14 = r;
    self->unk24[0].unk2 = 0;
    self->unk24[0].unkA = self->unk14;
    s32 x = r + 1;
    for (; i < self->unk20; i++) {
        if (i != 1) {
            x += func_80019648(self, glyphBuf, addr, pitch, x);
        }
        self->unk24[i].unk2 = x;
        self->unk24[i].unkA = func_80019724(self, glyphBuf, addr, pitch, x);
        if (self->unk24[i].unkA == 0) {
            sprintf(buf, "Incomplete font image: %s\nmissing character %x\n", arg1, self->unk24[i].unk0);
            __assert(D_80001530, NULL, 0, NULL);
        }
        x += self->unk24[i].unkA;
    }
    D_8006F624->vf3();
    delete[] glyphBuf;
}
#else
INCLUDE_ASM("asm/nonmatchings/slug", func_800176EC);
#endif

INCLUDE_ASM("asm/nonmatchings/slug", func_800178D0);

INCLUDE_ASM("asm/nonmatchings/slug", func_800179CC);

extern "C" void func_80017DA8(Slug* self) {
    Rect rect;
    u8* addr;
    s32 pitch;
    s32 ch = 0;

    SurfaceBase* src = self->vfunc3(ch);
    rect.y = 0;
    rect.h = self->unk18;
    D_8006F624->vf2(&addr, &pitch, 1);
    src->vf2(&addr, &pitch, 2);

    for (u32 i = 0; i < self->unk20; i++) {
        if (self->unk24[i].unk4 != ch) {
            src->vf3();
            ch = self->unk24[i].unk4;
            src = self->vfunc3(ch);
            src->vf2(&addr, &pitch, 2);
        }
        rect.x = self->unk24[i].unk2;
        rect.w = rect.x + self->unk24[i].unkA;
        src->vf10(self->unk24[i].unk6, self->unk24[i].unk8, &D_8008CA40, &rect);
    }
    src->vf3();
    D_8006F624->vf3();
}

INCLUDE_ASM("asm/nonmatchings/slug", func_80017F80);

INCLUDE_ASM("asm/nonmatchings/slug", func_800183AC);

INCLUDE_ASM("asm/nonmatchings/slug", func_800187B0);

INCLUDE_ASM("asm/nonmatchings/slug", func_80018A1C);

INCLUDE_ASM("asm/nonmatchings/slug", func_80018B94);

INCLUDE_ASM("asm/nonmatchings/slug", func_80018F2C);

INCLUDE_ASM("asm/nonmatchings/slug", func_80018F6C);

extern "C" void func_80018F90(void* arg0, Rect* a1, Rect* a2, Rect* a3, f32 sx, f32 sy) {
    s32 lo, hi;
    lo = a1->h;
    hi = a3->h;
    if (lo < hi) {
        f32 f = sy * (f32)(hi - lo);
        a3->h = lo;
        a2->h -= (s32)f;
    }
    lo = a1->y;
    hi = a3->y;
    if (hi < lo) {
        f32 f = sy * (f32)(lo - hi);
        a3->y = lo;
        a2->y += (s32)f;
    }
    lo = a1->w;
    hi = a3->w;
    if (lo < hi) {
        f32 f = sx * (f32)(hi - lo);
        a3->w = lo;
        a2->w -= (s32)f;
    }
    {
        s32 lo2 = a1->x, hi2 = a3->x;
        if (hi2 < lo2) {
            f32 f = sx * (f32)(lo2 - hi2);
            a3->x = lo2;
            a2->x += (s32)f;
        }
    }
}

extern "C" void func_80019080(void* arg0) {
    D_8006F620 = arg0;
}

extern "C" s32 func_8001908C(const void* a, const void* b) {
    u16 x = ((const GlyphEntry*)a)->unk0;
    u16 y = ((const GlyphEntry*)b)->unk0;
    if (x > y) {
        return 1;
    }
    if (x == y) {
        return 0;
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/slug", func_800190B8);

INCLUDE_ASM("asm/nonmatchings/slug", func_80019250);

extern "C" s32 func_80019398(Slug* self, u16 ch) {
    s32 lo = 0;
    s32 hi = self->unk20 - 1;
    s32 mid;

    for (mid = self->unk20 >> 1; lo <= hi; mid = (lo + hi) >> 1) {
        if (self->unk24[mid].unk0 == ch) {
            break;
        }
        if (ch < self->unk24[mid].unk0) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    if (hi < lo) {
        return 0;
    }
    return mid;
}

INCLUDE_ASM("asm/nonmatchings/slug", func_80019408);

INCLUDE_ASM("asm/nonmatchings/slug", func_80019520);

INCLUDE_ASM("asm/nonmatchings/slug", func_80019648);

INCLUDE_ASM("asm/nonmatchings/slug", func_80019724);

extern "C" void func_80019800(Slug* self, s32* buf, u8* addr, s32 pitch) {
    u16 depth = D_8006F624->hdr.bitDepth;
    u32 i;

    if (depth == 4) {
        for (i = 0; i < self->unk18; i++, addr += pitch) {
            buf[i] = *addr >> 4;
        }
    } else {
        u32 mask = (1 << depth) - 1;
        for (i = 0; i < self->unk18; i++, addr += pitch) {
            buf[i] = (addr[0] | (addr[1] << 8) | (addr[2] << 16) | (addr[3] << 24)) & mask;
        }
    }
}

// Slug vtable slot 9 (last entry).
extern "C" void func_800198C4(Slug* self) {
    if (self->unk24 != NULL) {
        delete[] self->unk24;
        self->unk24 = NULL;
    }
    self->unk00 = 0;
    self->unk18 = 0;
    self->unk20 = 0;
    self->unk24 = NULL;
    self->unk14 = 0;
    self->unk38.b[0] = 0xFF;
    self->unk38.b[1] = 0xFF;
    self->unk38.b[2] = 0xFF;
    self->unk38.b[3] = 0xFF;
}

extern "C" void func_80019924(Slug* self, s32 arg1) {
    self->unk20 = arg1;
    func_8004B3BC(D_8006F620);
    self->unk24 = new GlyphEntry[self->unk20];
    func_8004B390();
    if (self->unk24 == NULL) {
        __assert(D_80001530, NULL, 0, NULL);
    }
}

extern "C" void func_80019990(Slug* self, void* a1, void* a2, u8* color, WString* str, s32 count) {
    if (self->unk24 != NULL) {
        self->vfunc9();
    }
    func_80019924(self, count + 1);
    self->unk24[0].unk0 = 0x20;
    for (u32 i = 1; i < self->unk20; i++) {
        self->unk24[i].unk0 = *str->func_8001075C(i - 1);
    }
    self->unk28 = 8;
    if (color != NULL) {
        self->unk28 = 0x28;
        self->unk34.b[0] = color[0];
        self->unk34.b[1] = color[1];
        self->unk34.b[2] = color[2];
        self->unk34.b[3] = color[3];
    }
    self->vfunc1(a1, a2);
}

extern "C" void func_80019ABC(Slug* self, void* a1, void* a2, u8* color, u8* str, s32 count) {
    if (self->unk24 != NULL) {
        self->vfunc9();
    }
    if (str == NULL) {
        str = D_8006F628;
        count = 0x3F;
    }
    func_80019924(self, count + 1);
    self->unk24[0].unk0 = 0x20;
    for (u32 i = 1; i < self->unk20; i++) {
        self->unk24[i].unk0 = str[i - 1];
    }
    self->unk28 = 8;
    if (color != NULL) {
        self->unk28 = 0x28;
        self->unk34.b[0] = color[0];
        self->unk34.b[1] = color[1];
        self->unk34.b[2] = color[2];
        self->unk34.b[3] = color[3];
    }
    self->vfunc1(a1, a2);
}

struct SlugDrawPos {
    /* 0x0 */ Coords2 xy;
    /* 0x8 */ s8 flag;
};

extern "C" void func_80019BE8(Slug* self, void* a1, WString* str, s32 count) {
    if (self->unk24 != NULL) {
        self->vfunc9();
    }
    func_80019924(self, count + 1);
    self->unk24[0].unk0 = 0x20;
    for (u32 i = 1; i < self->unk20; i++) {
        self->unk24[i].unk0 = *str->func_8001075C(i - 1);
    }
    SlugDrawPos pos;
    pos.xy = self->unk2C.coords;
    pos.flag = 0;
    self->vfunc1(&pos, a1);
}

extern "C" void func_80019CDC(Slug* self) {
    self->unk00 = 0;
    self->unk18 = 0;
    self->unk20 = 0;
    self->unk24 = NULL;
    self->unk14 = 0;
    self->unk38.b[0] = 0xFF;
    self->unk38.b[1] = 0xFF;
    self->unk38.b[2] = 0xFF;
    self->unk38.b[3] = 0xFF;
}

Slug::~Slug() {
    func_800198C4(this);
}

Slug::Slug() {
    func_80019CDC(this);
    this->unk2C.raw.b[0] = 0;
    this->unk28 = 0;
    this->unk1C = 0;
}

extern "C" void func_80019D98(Slug* self, Blob4* arg1) {
    *arg1 = self->unk38;
}

extern "C" void func_80019DB0(Slug* self, Blob4* arg1) {
    self->unk38 = *arg1;
}

extern "C" s32 func_80019DC8(Slug* self) {
    return self->unk28 & 0x20;
}

extern "C" void func_80019DD4(Slug* self, s32 arg1) {
    self->unk1C = arg1;
}

extern "C" s32 func_80019DDC(Slug* self) {
    return self->unk1C;
}

extern "C" s32 func_80019DE8(Slug* self) {
    return self->unk24 != NULL;
}

extern "C" s32 func_80019DF4(Slug* self) {
    return self->unk18;
}

extern "C" void func_80019E00(Slug* self, Blob8* arg1) {
    self->unk2C.raw = *arg1;
}

extern "C" void func_80019E28(Slug* self, Blob4* arg1) {
    self->unk34 = *arg1;
    self->unk28 |= 0x800;
}

extern "C" void func_80019E48(Slug* self, u16 arg1) {
    self->unk28 = arg1;
}

INCLUDE_RODATA("asm/nonmatchings/slug", _vt.4Slug);
