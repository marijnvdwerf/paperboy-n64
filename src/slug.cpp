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

// The active source image/font object (only the bit-depth field is needed here).
struct SlugImage {
    char pad0[0x16];
    /* 0x16 */ u16 depth;
    char pad18[0x26 - 0x18];
    /* 0x26 */ u16 unk26;
};
extern "C" SlugImage* D_8006F624;

extern "C" s32 func_800178D0(Slug* self, u32* a1, u8* a2, s32 stride, s32 arg4);


// Alignment-1 byte blobs copied to/from the 0x2C region of Slug.
struct Blob4 { u8 b[4]; };
struct Blob8 { u8 b[8]; };

// Two aligned words at 0x2C (a draw position).
struct Coords2 {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
};

struct Rect {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 w;
    /* 0xC */ s32 h;
};

// One glyph entry in the Slug glyph table (0xC bytes).
struct GlyphEntry {
    /* 0x0 */ u16 unk0;  // character code
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
    /* 0xA */ u16 unkA;  // glyph width
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
    /* 0x2C */ Coords2 unk2C;
    /* 0x34 */ Blob4 unk34;
    /* 0x38 */ Blob4 unk38;
    /* vptr at 0x3C */

    Slug();                       // __4Slug
    virtual void vfunc1(void* a, void* b);  // slot 1 = func_80017520
    virtual void vfunc2() = 0;    // slot 2
    virtual void vfunc3() = 0;    // slot 3
    virtual void vfunc4() = 0;    // slot 4
    virtual void vfunc5() = 0;    // slot 5
    virtual void vfunc6() = 0;    // slot 6
    virtual void vfunc7() = 0;    // slot 7
    virtual ~Slug();              // slot 8 = _._4Slug
    virtual void vfunc9();        // slot 9 = func_800198C4
};

INCLUDE_ASM("asm/nonmatchings/slug", func_80017520);

INCLUDE_RODATA("asm/nonmatchings/slug", D_80001530);

INCLUDE_ASM("asm/nonmatchings/slug", func_800176EC);

INCLUDE_ASM("asm/nonmatchings/slug", func_800178D0);

INCLUDE_ASM("asm/nonmatchings/slug", func_800179CC);

INCLUDE_ASM("asm/nonmatchings/slug", func_80017DA8);

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

extern "C" s32 func_80019398(Slug* self, s32 ch) {
    s32 lo = 0;
    s32 hi = self->unk20 - 1;
    s32 mid = self->unk20 >> 1;

    if (hi >= 0) {
        GlyphEntry* glyphs = self->unk24;
        u32 key = ch & 0xFFFF;
        do {
            u16 entry = glyphs[mid].unk0;
            if (entry == key) {
                break;
            }
            if (key < entry) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
            mid = (lo + hi) >> 1;
        } while (hi >= lo);
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

INCLUDE_ASM("asm/nonmatchings/slug", func_80019800);

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
    u32 i = 1;
    if (i < self->unk20) {
        do {
            self->unk24[i].unk0 = *str->func_8001075C(i - 1);
            i++;
        } while (i < self->unk20);
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
    u32 i = 1;
    if (i < self->unk20) {
        do {
            self->unk24[i].unk0 = str[i - 1];
            i++;
        } while (i < self->unk20);
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
    u32 i = 1;
    if (i < self->unk20) {
        do {
            self->unk24[i].unk0 = *str->func_8001075C(i - 1);
            i++;
        } while (i < self->unk20);
    }
    SlugDrawPos pos;
    pos.xy = self->unk2C;
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
    *(u8*)&this->unk2C = 0;
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
    *(Blob8*)&self->unk2C = *arg1;
}

extern "C" void func_80019E28(Slug* self, Blob4* arg1) {
    self->unk34 = *arg1;
    self->unk28 |= 0x800;
}

extern "C" void func_80019E48(Slug* self, u32 arg1) {
    self->unk28 = arg1 & 0xFFFF;
}

INCLUDE_RODATA("asm/nonmatchings/slug", _vt.4Slug);
