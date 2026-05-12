#ifndef SURFACES_H
#define SURFACES_H

#include "common.h"

// TODO: real names unknown. Looks like a Surface/Bitmap class with a virtual
// "lock/unlock" pair (slots 2/3), a palette accessor (slot 8) and a blit
// (slot 11). Local stubs while we work out the real hierarchy.
class Surface16970;

class Surface16970Palette {
  public:
    virtual void vfunc1() = 0;
    virtual void vfunc2() = 0;
    virtual void vfunc3() = 0;
    virtual void vfunc4(Surface16970Palette*) = 0;
};

struct Rect16970 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
};

struct Surface16970Header {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u16 unk16;
};

class Surface16970 {
  public:
    /* 0x00 */ Surface16970Header hdr;
    /* 0x18 */ u8* unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u16 unk22;
    /* 0x24 */ u16 unk24;
    /* 0x26 */ u16 unk26;
    /* 0x28 */ u16 unk28;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ // vptr (SN places it after data)

    Surface16970();

    // slot 1 = dtor (func_80016548)
    virtual ~Surface16970();
    // slot 2 — lock unk18 (func_800164FC)
    virtual void vfunc2(u8** outAddr, s32* outPitch, s32 mode);
    // slot 3 — unlock unk22 (func_800164E0)
    virtual void vfunc3();
    // slot 4 — lock unk1C (func_80016494)
    virtual void vfunc4(u8** outAddr, s32* outPitch, s32 mode);
    // slot 5 — unlock unk24 (func_80016478)
    virtual void vfunc5();
    // slot 6 — empty stub (func_80016470)
    virtual void vfunc6();
    // slot 7 — empty stub (func_80016468)
    virtual void vfunc7();
    // slot 8 — palette getter, base returns NULL (func_80016460)
    virtual Surface16970Palette* vfunc8();
    // slot 9 — fill (func_80015D70)
    virtual void vfunc9(u32 fillVal);
    // slot 10 — blit-with-rect (func_800162D4)
    virtual void vfunc10(s32 a, s32 b, Surface16970* src, Rect16970* clip);
    // slot 11 — raw blit with addr/pitch (func_80015FD4)
    virtual void vfunc11(s32 a, s32 b, u8* addr, s32 pitch, Rect16970* clip);
    // slot 12 — empty stub (func_80016458)
    virtual void vfunc12();
    // slot 13 — empty stub (func_80016450); real signature is from the
    // derived "buffer init" override which takes (ctx, w, h, bpp)
    virtual void vfunc13(void* ctx, s16 w, s16 h, s32 bpp);
};

// Surface16970 with a 2-field tail: unk30 (some buffer/handle, perhaps an
// owned VRAM allocation index) and unk34 (flags). Adds vfunc14 to set the
// pointer + dimensions + format and vfunc15 to clear them.
class Surface177B0 : public Surface16970 {
  public:
    /* 0x30 */ void* unk30;
    /* 0x34 */ s32 unk34;

    Surface177B0();
    virtual ~Surface177B0();
    virtual void vfunc14(void* buf, u16 w, u16 h, u32 bpp);
    virtual void vfunc15();

    s32 func_80016C58();
    s32 func_80016C64();
    void* func_80016C70();
};

#endif // SURFACES_H
