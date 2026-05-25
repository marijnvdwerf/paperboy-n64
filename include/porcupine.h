#ifndef PORCUPINE_H
#define PORCUPINE_H

#include "common.h"
#include "bunyip.h"
#include "possum.h"

class Porcupine : public Bunyip {
  public:
    /* 0x40 */ Possum palette;

    virtual ~Porcupine();
    Surface16970Palette* vfunc8() CXX_OVERRIDE;
    void vfunc14(BunyipRenderer* ctx, PixelFormat* pf, s32 w, s32 h) CXX_OVERRIDE;
    void vfunc15() CXX_OVERRIDE;
    void vfunc16(BunyipRenderer* ctx, BunyipScene* scene) CXX_OVERRIDE;

    Possum* func_800270E8();

    static void func_80026F10(s32 val);
    static s32 func_800270D8();
};

#endif // PORCUPINE_H
