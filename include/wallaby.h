#ifndef WALLABY_H
#define WALLABY_H

#include "marsupial.h"

class Parrot;

struct Wallaby : Marsupial {
    Wallaby();
    ~Wallaby(); // TODO: remove — auto-generated, not an override

    void vfunc2(Parrot* parrot) CXX_OVERRIDE;
    void vfunc3(s32 newCount) CXX_OVERRIDE;
    void vfunc4(void) CXX_OVERRIDE;
    void vfunc6(s32 index, Vec3f* out) CXX_OVERRIDE;
    void vfunc7(s32 index, Vec3f* out) CXX_OVERRIDE;
    void vfunc8(s32 index, Vec3f* out) CXX_OVERRIDE;
    void vfunc9(s32 index, u8* out) CXX_OVERRIDE;
    void vfunc10(s32 index, Vec3f* src) CXX_OVERRIDE;
    void vfunc11(s32 index, Vec3f* src) CXX_OVERRIDE;
    void vfunc12(s32 index, Vec3f* src) CXX_OVERRIDE;
    void vfunc13(void) CXX_OVERRIDE;
};

#endif
