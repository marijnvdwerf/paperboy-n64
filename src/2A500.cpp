#include "common.h"
#include "structs.h"

extern "C" {
extern u8 D_80072BA0[];
extern OSViMode D_80077C30[];
}

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
