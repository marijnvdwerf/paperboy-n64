#ifndef GDB_MODEL_INDEX_ARRAY_BASE_H
#define GDB_MODEL_INDEX_ARRAY_BASE_H

#include "common.h"

class GolFileParser;

class GdbModelIndexArrayBase {
  public:
    /* 0x00 */ u32 unk0;
    /* 0x04 */ // vtable

    GdbModelIndexArrayBase();
    virtual ~GdbModelIndexArrayBase();
    virtual void vfunc2(GolFileParser* parser) = 0;
    virtual void func_80016C80();

    u32 func_80016CF0();
    s32 func_80016CFC();
};

#endif
