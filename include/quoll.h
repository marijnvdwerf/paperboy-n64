#ifndef QUOLL_H
#define QUOLL_H

#include "common.h"

class Parrot;

class Quoll {
  public:
    /* 0x00 */ u32 unk0;
    /* 0x04 */ // vtable

    Quoll();
    virtual ~Quoll();
    virtual void vfunc2(Parrot* parrot) = 0;
    virtual void func_80016C80();

    u32 func_80016CF0();
    s32 func_80016CFC();
};

#endif
