#include "common.h"
#include "pixel_format.h"

// --- Forward declaration ---
struct Dunnart;

// --- External references ---
extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390();
extern "C" void func_80030B50(Dunnart* self);

extern s32 D_80072C10;
extern "C" const char D_80002CF0[];

// RGBA5551 pixel format (big-endian bitfield)
struct DunnartPixel {
    u16 r : 5;
    u16 g : 5;
    u16 b : 5;
    u16 a : 1;
};

struct DunnartColor {
    u8 r, g, b, a;
};

struct DunnartBase {
    /* 0x00 */ // vtable

    virtual void vfunc1(u8* dst, s32 start, u32 num);
};

// --- Dunnart class ---
struct Dunnart  : DunnartBase{
    /* 0x04 */ u16* data;
    /* 0x08 */ s32 count;

    // Virtual functions (vtable slot order):
    // slot 1: func_800307EC (batch read pixels)
    virtual void vfunc1(u8* dst, s32 start, u32 num) CXX_OVERRIDE;
    // slot 2: func_80030AD4 (batch write pixels)
    virtual void vfunc2(u8* src, s32 start, u32 num);
    // slot 3: func_80030A2C (read single pixel)
    virtual void vfunc3(u8* dst, s32 index);
    // slot 4: func_80030960 (copy from another Dunnart)
    virtual void vfunc4(Dunnart* src);
    // slot 5: func_800308E4 (find pixel index)
    virtual s32 vfunc5(u8* color);
    // slot 6: func_800308D0 (returns 0)
    virtual s32 vfunc6();
    // slot 7: func_800308C4 (return count)
    virtual s32 vfunc7();
    // slot 8: func_800308B8 (return count)
    virtual s32 vfunc8();

    Dunnart();
    ~Dunnart();
};

// --- Non-member function ---
extern "C" s32 func_800307E0(u8* arg0) {
    return *(s32*)(arg0 + 0x50);
}

// vfunc1 - batch read pixels from palette to RGBA
// NON_MATCHING: missing lw s0,0x10(sp) (two-entry-point: target loads unused 5th arg at
// function start for non-virtual callers), plus andi scheduling (same as func_800308E4).
// Score 160 — body matches perfectly otherwise.
#ifdef NON_MATCHING
extern "C" void func_800307EC(Dunnart* self, DunnartColor* dst, s32 start, u32 num, s32 extra) {
    u32 i = 0;
    if (num != 0) {
        do {
            dst[i].r = (self->data[i + start] & 0xF800) >> 8;

            u16 px = self->data[i + start];
            s32 g6 = px & 0x40;
            dst[i].g = ((px & 0x7C0) >> 3) | (g6 << 2) | (g6 << 1) | g6;

            u16 px2 = self->data[i + start];
            dst[i].a = 0;
            s32 bmask = px2 & 0x3E;
            s32 b1 = px2 & 0x2;
            dst[i].b = (bmask << 2) | (b1 << 2) | (b1 << 1) | b1;

            if (self->data[i + start] & 0x1) {
                dst[i].a = 0xFF;
            }
            i++;
        } while (i < num);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/dunnart", func_800307EC);
#endif

// vfunc8 - return count
extern "C" s32 func_800308B8(Dunnart* self) {
    return self->count;
}

// vfunc7 - return count
extern "C" s32 func_800308C4(Dunnart* self) {
    return self->count;
}

// vfunc6 - return 0
extern "C" s32 func_800308D0(Dunnart* self) {
    return 0;
}

// set global
extern "C" void func_800308D8(s32 val) {
    D_80072C10 = val;
}

// vfunc5 - find pixel index
// NON_MATCHING: andi a2,a2,0xffff scheduled 1 instruction later than target.
// Score 60 — all instructions present and correct, just reordered.
#ifdef NON_MATCHING
extern "C" s32 func_800308E4(Dunnart* self, u8* rgba) {
    Dunnart* t = self;
    s32 i = 0;
    u8 b = rgba[2];
    u8 a = rgba[3];
    u8 r = rgba[0];
    u8 g = rgba[1];

    u16 pixel = ((r & 0xF8) << 8) | ((g & 0xF8) << 3) | ((b & 0xF8) >> 2) | (a >> 7);

    s32 n = t->count;
    if (n != 0) {
        pixel &= 0xFFFF;
        s32 cnt = n;
        u16* p = t->data;
        do {
            if (*p == pixel) {
                return i;
            }
            i++;
            p++;
        } while ((u32)i < (u32)cnt);
    }
    return -1;
}
#else
INCLUDE_ASM("asm/nonmatchings/dunnart", func_800308E4);
#endif

// vfunc4 - copy from another Dunnart
extern "C" void func_80030960(Dunnart* self, Dunnart* src) {
    u8 rgba[4];
    u32 i = 0;

    self->count = src->vfunc7();
    if (self->count != 0) {
        do {
            src->vfunc3(rgba, i);

            u8 r = rgba[0];
            u8 g = rgba[1];
            u8 b = rgba[2];
            u8 a = rgba[3];

            self->data[i] = ((r & 0xF8) << 8) | ((g & 0xF8) << 3) | ((b & 0xF8) >> 2) | (a >> 7);
            i++;
        } while (i < (u32)self->count);
    }
}

// vfunc3 - read single pixel
extern "C" void func_80030A2C(Dunnart* self, u8* dst, s32 index) {
    s32 idx = index * 2;

    dst[0] = (self->data[index] & 0xF800) >> 8;

    u16 px = self->data[index];
    s32 g6 = px & 0x40;
    dst[1] = ((px & 0x7C0) >> 3) | (g6 << 2) | (g6 << 1) | g6;

    u16 px2 = self->data[index];
    dst[3] = 0;
    s32 bmask = px2 & 0x3E;
    s32 b1 = px2 & 0x2;
    dst[2] = (bmask << 2) | (b1 << 2) | (b1 << 1) | b1;

    if (self->data[index] & 0x1) {
        dst[3] = 0xFF;
    }
}

// vfunc2 - write batch pixels
extern "C" void func_80030AD4(Dunnart* self, DunnartColor* colors, s32 start, u32 num) {
    u32 i = 0;
    if (num != 0) {
        do {
            u8 r = colors[i].r;
            u8 g = colors[i].g;
            u8 b = colors[i].b;
            u8 a = colors[i].a;
            self->data[i + start] = ((r & 0xF8) << 8) | ((g & 0xF8) << 3) | ((b & 0xF8) >> 2) | (a >> 7);
            i++;
        } while (i < num);
    }
}

// cleanup - free data buffer
extern "C" void func_80030B50(Dunnart* self) {
    self->count = 0;
    if (self->data != NULL) {
        delete[] self->data;
        self->data = NULL;
    }
}

// allocate and init palette buffer
extern "C" void func_80030B88(Dunnart* self, PixelFormat* pf) {
    if (self->data != NULL) {
        func_80030B50(self);
    }

    s32 n = 1 << pf->bitDepth;
    self->count = n;
    func_8004B3BC(D_80072C10);
    self->data = new u16[self->count];
    func_8004B390();

    if (self->data == NULL) {
        __assert(D_80002CF0, 0, 0, 0);
    }

    u32 i = 0;
    if (self->count != 0) {
        do {
            self->data[i] = 0;
            i++;
        } while (i < (u32)self->count);
    }
}

// dtor
Dunnart::~Dunnart() {
    func_80030B50(this);
}

// ctor
Dunnart::Dunnart() {
    data = NULL;
    count = 0;
}

// get global
extern "C" s32 func_80030CB0() {
    return D_80072C10;
}

// return data pointer
extern "C" u16* func_80030CC0(Dunnart* self) {
    return self->data;
}

// return data != NULL
extern "C" s32 func_80030CCC(Dunnart* self) {
    return self->data != 0;
}

INCLUDE_RODATA("asm/nonmatchings/dunnart", D_80002CF0);

INCLUDE_RODATA("asm/nonmatchings/dunnart", _vt.7Dunnart);
