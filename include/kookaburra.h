#ifndef KOOKABURRA_H
#define KOOKABURRA_H

#include "common.h"

class WString;

class Kookaburra {
  public:
    /* 0x00 */ u8 loaded;
    /* 0x04 */ u16* data;
    /* 0x08 */ u16* indices;
    /* 0x0C */ u16 count;

    virtual ~Kookaburra();
    virtual s32 cleanup();
    virtual s32 load(const char* filename);
    virtual void freeBuffers();

    Kookaburra();

    u16 func_80045C18(u16* key);
    u16* func_80045C9C(u16 index);
    s32 func_80045CE0(WString* dst, u16 index);
    s32 func_80045DEC();
    static void func_80045ED0(s32 value);
    u16 func_80045EDC();
    u16 func_80045EE8(u16** key);
    u8 func_80045F0C();
};

#endif
