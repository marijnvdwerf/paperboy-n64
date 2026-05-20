#ifndef BOODIE_H
#define BOODIE_H

#include "common.h"
#include "quoll.h"

struct BoodieEntry {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
};

class Boodie : public Quoll {
  public:
    /* 0x08 */ BoodieEntry* unk8;

    Boodie();
    virtual ~Boodie();
    virtual void vfunc2(Parrot* parrot);
    virtual void func_80016C80();
    virtual void func_8002851C(s32 count);

    BoodieEntry* func_8002863C();
    void func_80028648(s32 idx, const BoodieEntry* src);
    BoodieEntry* func_80028680(s32 idx);
};

#endif
