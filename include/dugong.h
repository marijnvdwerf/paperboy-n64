#ifndef DUGONG_H
#define DUGONG_H

#include "common.h"
#include "file.h"
#include "pixel_format.h"

struct DugongColor {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 a;
};

struct Dugong {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ u8* unkC;
    /* 0x010 */ File file;
    /* 0x040 */ u32 unk40;
    /* 0x044 */ u32 unk44;
    /* 0x048 */ u32 unk48;
    /* 0x04C */ PixelFormat pixelFormat;
    /* 0x064 */ s32 unk64;
    /* 0x068 */ s32 unk68;
    /* 0x06C */ s32 unk6C;
    /* 0x070 */ s32 unk70;
    /* 0x074 */ s32 unk74;
    /* 0x078 */ s32 unk78;
    /* 0x07C */ s32 unk7C;
    /* 0x080 */ s32 unk80;
    /* 0x084 */ s32 unk84;
    /* 0x088 */ s32 unk88;
    /* 0x08C */ s32 unk8C;
    /* 0x090 */ s32 unk90;
    /* 0x094 */ s32 unk94;
    /* 0x098 */ u8 unk98;
    /* 0x099 */ u8 unk99;
    /* 0x09A */ u8 unk9A;
    /* 0x09B */ u8 unk9B;
    /* 0x09C */ u8 unk9C;
    /* 0x09D */ u8 unk9D;
    /* 0x09E */ u8 unk9E;
    /* 0x09F */ u8 unk9F;
    /* 0x0A0 */ u32 unkA0;
    /* 0x0A4 */ DugongColor palette[256];
    /* 0x4A4 */ u8 unk4A4[0x100];
    /* 0x5A4 */ s32 unk5A4;
    /* 0x5A8 */ s32 unk5A8;
    /* 0x5AC */ // vtable

    Dugong();
    virtual void vfunc1();
    virtual ~Dugong();
    virtual void vfunc3(const char* filename);
    virtual void vfunc4();
    virtual const char* vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9();

    void func_8001B230(u8* palette, u8* transColor);
    s32 func_8001B5C8(u8* color);
    void func_8001B794(PixelFormat* dstPf, u8* transColor);
    void func_8001BAE0(u8* src, u8* dst, PixelFormat* dstPf);
    void func_8001BD54(u8* src, u8* dst);
    void func_8001BF54(u8* src, u8* dst);
    void func_8001C158(u8* src, u8* dst);
    void func_8001C3AC(u8* src, u8* dst);
    void func_8001C5B0(u8* src, u8* dst);
    void func_8001C8E4(u8* src, u16* dst);
    void func_8001CC1C(u8* src, u8* dst);
    void func_8001D040(u8* src, u32* dst);
    void func_8001D378(u8* src, u8* dst);
    void func_8001D500(u8* buf, s32 scaleX, s32 dstWidth, s32 bitDepth);
    void func_8001D738(u8* src1, u8* src2, u8* dst);
    void func_8001DCFC(u8* src1, u8* src2, u8* dst);
    void func_8001DD90(u8* src, u8* dst, u32 dstWidth, u32 dstHeight, s32 dstStride, PixelFormat* dstPf, s32 unused, u8* transColor);
    void func_8001DEC0(u8* src, u8* dst, u32 dstWidth, u32 dstHeight, s32 dstStride, PixelFormat* dstPf, u8* palette, s32 flipVertical, u8* transColor);
    void func_8001E0BC(PixelFormat* pf, u32 width, u32 height, u32 arg4, u8* palette, s32 paletteCount);
    void func_8001E1DC(u8* src, u32* dst);
    void func_8001E320(u8* src, u16* dst);
    void func_8001E464(u8* src, u8* dst);
    void func_8001E5A4(u8* src, u8* dst);
    void func_8001E5E0(u8* src, u8* dst);
    void func_8001E65C(u8* src, u8* dst);
    void func_8001E6C0(u8* src, u8* dst);
    void func_8001E94C();
    void func_8001E954();
    void func_8001E960();
    void func_8001E968();
    s32 func_8001E974();
    void func_8001E98C(u8* src);
    void func_8001E9A4(u8* src);
    u8* func_8001EA40();
    u16 func_8001EA48();
    u32 func_8001EA54();
    u32 func_8001EA60();
    u32 func_8001EA6C();
    u32 func_8001EA78();
    u32 func_8001EA84();
    s32 func_8001EA90();
    s32 func_8001EAAC();
    s32 func_8001EAC8();
    s32 func_8001EAE4();
    s32 func_8001EB00();
    s32 func_8001EB1C();
    s32 func_8001EB38();
    s32 func_8001EB44();
    s32 func_8001EB50();
    u16 func_8001EB5C();
    void func_8001EB68(PixelFormat* out);
    s32 func_8001EBA0();
    s32 func_8001EBAC();
    s32 func_8001EBB8();
    s32 func_8001EBC4();
};

#endif
