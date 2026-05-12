#include "common.h"

extern "C" {
#include "sched.h"
extern void* memset(void*, s32, u32);
}

class StructYYSubA8Inner1;
class StructYYSubA8Inner2;
class StructYYSubA8;

extern "C" {
extern u8 D_80072BA0[];
extern OSViMode D_80077C30[];
}

class StructYYSubA8Base {
  public:
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ void* unk10;
    /* 0x14 */ // vtable

    StructYYSubA8Base();
    virtual s32 vfunc1();
    virtual ~StructYYSubA8Base();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9();
    virtual s32 vfunc10();
    virtual void vfunc11();
    virtual void vfunc12();
    virtual void vfunc13();
    virtual void vfunc14();
    virtual void vfunc15();
    virtual void vfunc16();
    virtual void vfunc17();
    virtual void vfunc18();
    virtual void vfunc19();
    virtual void vfunc20();
    virtual void vfunc21();
    virtual void vfunc22();
    virtual void vfunc23();
    virtual void vfunc24();
    virtual void vfunc25();
    virtual void vfunc26();
    virtual void vfunc27();
    virtual void vfunc28();
    virtual void vfunc29();
    virtual void vfunc30();
    virtual void vfunc31();
    virtual void vfunc32();
    virtual void vfunc33();
};

class StructYYSubA8Inner1 {
  public:
    /* 0x00 */ char pad[0x40];

    StructYYSubA8Inner1();
    ~StructYYSubA8Inner1();
};

class StructYYSubA8Inner2 {
  public:
    /* 0x00 */ s32 unk0;
    /* 0x04 */ char pad4[0xCD4C];

    StructYYSubA8Inner2();
    ~StructYYSubA8Inner2();

    s32 func_80036A68();
    s32 func_80031B58(StructYYSubA8*, StructYYSubA8Inner1*, s32, s32);
};

class StructYYSubA8 : public StructYYSubA8Base {
  public:
    /* 0x18 */ char pad18[0x4];
    /* 0x1C */ void* unk1C;
    /* 0x20 */ OSSched scheduler;
    /* 0x2A8 */ StructYYSubA8Inner1 inner1;
    /* 0x2E8 */ StructYYSubA8Inner2 inner2;
    /* 0xD038 */ s32 unkD038;
    /* 0xD03C */ s32 unkD03C;

    StructYYSubA8();
    virtual s32 vfunc1();
    virtual ~StructYYSubA8();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc7();
    virtual s32 vfunc10();

    void func_80029AEC();
    void func_80029CDC();
    s32 func_80029CEC();
    OSSched* func_80029D00();
    OSMesgQueue* func_80029D08();
};

// clang-format off
static const s32 D_800026E0[] = {
    OS_VI_NTSC_LPN1, OS_VI_NTSC_LPF1, OS_VI_NTSC_LAN1, OS_VI_NTSC_LAF1,
    OS_VI_NTSC_LPN2, OS_VI_NTSC_LPF2, OS_VI_NTSC_LAN2, OS_VI_NTSC_LAF2,
    OS_VI_NTSC_HPF1, OS_VI_NTSC_HPN1, OS_VI_NTSC_HAF1, OS_VI_NTSC_HAN1,
    OS_VI_NTSC_HPF2, OS_VI_NTSC_HPN2,
    -1, -1,
    OS_VI_PAL_LPN1, OS_VI_PAL_LPF1, OS_VI_PAL_LAN1, OS_VI_PAL_LAF1,
    OS_VI_PAL_LPN2, OS_VI_PAL_LPF2, OS_VI_PAL_LAN2, OS_VI_PAL_LAF2,
    OS_VI_PAL_HPF1, OS_VI_PAL_HPN1, OS_VI_PAL_HAF1, OS_VI_PAL_HAN1,
    OS_VI_PAL_HPF2, OS_VI_PAL_HPN2,
    -1, -1,
    OS_VI_MPAL_LPN1, OS_VI_MPAL_LPF1, OS_VI_MPAL_LAN1, OS_VI_MPAL_LAF1,
    OS_VI_MPAL_LPN2, OS_VI_MPAL_LPF2, OS_VI_MPAL_LAN2, OS_VI_MPAL_LAF2,
    OS_VI_MPAL_HPF1, OS_VI_MPAL_HPN1, OS_VI_MPAL_HAF1, OS_VI_MPAL_HAN1,
    OS_VI_MPAL_HPF2, OS_VI_MPAL_HPN2,
    -1, -1,
};
// clang-format on

// #ifndef PAL
s32 StructYYSubA8::vfunc1() {
    s32 idx = 0;
    if (this->unkC & 4) {
        idx |= 1;
    }
    if (this->unkC & 8) {
        idx |= 2;
    }
    if (this->unk8 == 0x20) {
        idx |= 4;
    }
    if ((u32)this->unk4 > 0xF0U) {
        idx |= 8;
    }
    if (this->unkC & 0x10) {
        idx |= 0x10;
    } else if (this->unkC & 0x100000) {
        idx |= 0x20;
    }
    s32 vimode = D_800026E0[idx];
    OSViMode* vi_mode = &D_80077C30[vimode];
    if (this->unkD038 == 0) {
        osCreateScheduler(&this->scheduler, D_80072BA0, 0x13, vimode, 1);
        this->unkD038 = 1;
    } else {
        osViSetMode(vi_mode);
    }
    osViBlack(1);

#ifndef PAL
    this->unkD03C = 1;
#endif

    if (idx & 8) {
        if (this->unk0 != 0x280) {
            vi_mode->comRegs.width = this->unk0;
            vi_mode->comRegs.xScale = (s32)((this->unk0 << 10) / 640.0f);
            vi_mode->fldRegs[1].origin = vi_mode->fldRegs[0].origin = this->unk0 << 1;
        }
        if (this->unk4 != 0x1E0) {
            vi_mode->fldRegs[1].yScale = vi_mode->fldRegs[0].yScale = (s32)((this->unk4 << 10) / 480.0f);
        }
    } else {
        vi_mode->comRegs.width = this->unk0;
        vi_mode->comRegs.xScale = (s32)((this->unk0 << 10) / 640.0f);
        vi_mode->fldRegs[1].origin = vi_mode->fldRegs[0].origin = this->unk0 << 1;
    }
    this->func_80029AEC();
    return 0;
}
// #else
// TEXT_PAD(0x1DC);
// #endif

void StructYYSubA8::func_80029AEC() {
    u32 flags = 0;
    flags |= (this->unkC & 0x60) ? OS_VI_GAMMA_ON : OS_VI_GAMMA_OFF;
    flags |= (this->unkC & 0x40) ? OS_VI_GAMMA_DITHER_ON : OS_VI_GAMMA_DITHER_OFF;
    flags |= (this->unkC & 0x80) ? OS_VI_DITHER_FILTER_ON : OS_VI_DITHER_FILTER_OFF;
    flags |= (this->unkC & 0x100) ? OS_VI_DIVOT_ON : OS_VI_DIVOT_OFF;
    osViSetSpecialFeatures(flags);
}

void StructYYSubA8::vfunc4() {
    this->unkC &= ~0x80;
    this->func_80029AEC();
}

void StructYYSubA8::vfunc3() {
    this->unkC |= 0x80;
    this->func_80029AEC();
}

s32 StructYYSubA8::vfunc10() {
    StructYYSubA8Inner2* inner = &this->inner2;
    s32 ret;
    if (inner->unk0 & 1) {
        ret = inner->func_80036A68();
    } else {
        inner->func_80031B58(this, &this->inner1, this->unkC & 0x1000, 0x280);
        ret = 0;
    }
    return ret;
}

StructYYSubA8::~StructYYSubA8() {
    this->vfunc6();
}

StructYYSubA8::StructYYSubA8() {
    memset(&this->scheduler, 0, sizeof(OSSched));
    this->unk10 = &this->inner1;
    this->unk1C = &this->inner2;
    this->unkD038 = 0;
    this->unkD03C = 0;
}

void StructYYSubA8::func_80029CDC() {
    this->unkD03C = 0;
}

s32 StructYYSubA8::func_80029CEC() {
    return this->unkD03C;
}

OSSched* StructYYSubA8::func_80029D00() {
    return &this->scheduler;
}

OSMesgQueue* StructYYSubA8::func_80029D08() {
    return osScGetCmdQ(&this->scheduler);
}
