#ifndef GDB_MODEL_INDEX_ARRAY_H
#define GDB_MODEL_INDEX_ARRAY_H

#include "common.h"
#include "gdb_model_index_array_base.h"

class GdbModelIndexArray : public GdbModelIndexArrayBase {
  public:
    struct Indices {
        /* 0x0 */ u8 unk0;
        /* 0x1 */ u8 unk1;
        /* 0x2 */ u8 unk2;
        /* 0x3 */ u8 unk3;
    };

    /* 0x08 */ GdbModelIndexArray::Indices* unk8;

    GdbModelIndexArray();
    virtual ~GdbModelIndexArray();
    virtual void vfunc2(GolFileParser* parser);
    virtual void func_80016C80();
    virtual void func_8002851C(s32 count);

    GdbModelIndexArray::Indices* func_8002863C();
    void func_80028648(s32 idx, const GdbModelIndexArray::Indices* src);
    GdbModelIndexArray::Indices* func_80028680(s32 idx);
};

#endif
