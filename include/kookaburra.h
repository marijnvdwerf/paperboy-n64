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

    u16 find(u16* key);
    u16* getEntry(u16 index);
    s32 copyTo(WString* dst, u16 index);
    s32 init();
    static void setHeapId(s32 value);
    u16 getCount();
    u16 findIndirect(u16** key);
    u8 isLoaded();
};

#endif
