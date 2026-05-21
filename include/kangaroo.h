#ifndef KANGAROO_H
#define KANGAROO_H

#include "common.h"
#include "scene.h"

class Kangaroo : public Joey {
  public:
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ f32 unk5C;
    /* 0x60 */ f32 unk60;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ f32 unk68;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 unk70;
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ f32 unk80;
    /* 0x84 */ f32 unk84;
    /* 0x88 */ f32 unk88;
    /* 0x8C */ f32 unk8C;

    virtual void vfunc1() CXX_OVERRIDE; // func_8002B2CC
    virtual void vfunc2(f32* src, f32* dst) CXX_OVERRIDE; // func_8002B590
    virtual void vfunc3(f32* src, f32* dst) CXX_OVERRIDE; // func_8002ADC0
    virtual void vfunc4(f32* src, f32* dst) CXX_OVERRIDE; // func_8002B4CC
    virtual void vfunc5(f32* src, f32* dst) CXX_OVERRIDE; // func_8002B408
    virtual void vfunc6(f32* quat) CXX_OVERRIDE; // func_8002AF90
    virtual void vfunc7(f32* a, f32* b, f32* c) CXX_OVERRIDE; // func_8002B964
    virtual void vfunc8(f32* a, f32* b) CXX_OVERRIDE; // func_8002B930
    virtual void vfunc9(f32* a, f32* b) CXX_OVERRIDE; // func_8002B8FC
    virtual void vfunc10(f32* a, f32* b) CXX_OVERRIDE; // func_8002B7C8
    virtual void vfunc11(f32* a, f32* b) CXX_OVERRIDE; // func_8002B684
    virtual void vfunc12(f32* quat) CXX_OVERRIDE; // func_8002B0BC
    virtual void vfunc13(f32* vec) CXX_OVERRIDE; // func_8002B0A0
    virtual void vfunc14(f32* vec) CXX_OVERRIDE; // func_8002B084
    virtual void vfunc15(f32* vec) CXX_OVERRIDE; // func_8002B068
    virtual f32 vfunc16(f32* vec) CXX_OVERRIDE; // func_8002B02C
    virtual void vfunc17(f32* vec) CXX_OVERRIDE; // func_8002B010
    virtual void vfunc18(f32* vec) CXX_OVERRIDE; // func_8002AFF4
    virtual void vfunc19(Joey* src) CXX_OVERRIDE; // func_8002AF2C

    void func_8002B1D0(f32 scale);
    void func_8002B3B0(Kangaroo* parent);
    Kangaroo();
    void func_8002BA6C(f32* vec);
    f32* func_8002BA88();
    f32* func_8002BA90();
};

#endif
