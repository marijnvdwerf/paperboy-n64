#ifndef BUNYIP_H
#define BUNYIP_H

#include "common.h"
#include "surfaces.h"

struct BunyipMark {
    u8 data[4];
};

struct BunyipTrack {
    u8 data[8];
};

struct BunyipRenderer;
struct BunyipScene;

struct Bunyip : public Surface16970 {
    /* 0x30 */ BunyipMark unk30;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u16 flags;
    /* 0x38 */ BunyipTrack unk38;

    Bunyip();
    virtual void vfunc14(BunyipRenderer* ctx, PixelFormat* pf, s32 a, s32 b) = 0;
    virtual void vfunc15() = 0;
    virtual void vfunc16(BunyipRenderer* ctx, BunyipScene* arg2);

    void func_80029194(u16 val);
    u16 func_8002919C();
    void func_800291A8(u16 val);
    void func_800291B0(BunyipTrack* dst);
    void func_800291D8(BunyipTrack* src);
    void func_80029200(BunyipMark* dst);
    void func_80029218(BunyipMark* src);
    u16 func_8002923C();
    u16 func_80029248();
    u16 func_80029254();
    u16 func_80029260();
    u16 func_8002926C();
    u16 func_80029278();
    u16 func_80029284();
    u16 func_80029290();
    u16 func_8002929C();
    u16 func_800292A8();
};

#endif
