#ifndef BOWERBIRD_H
#define BOWERBIRD_H

#include "common.h"
#include <PR/mbi.h>
#include <PR/gbi.h>

struct Bowerbird {
    /* 0x00 */ s32 segment;
    /* 0x04 */ s32 active;
    /* 0x08 */ s32 clipping;
    /* 0x0C */ u32 capacity;
    /* 0x10 */ u32 count;
    /* 0x14 */ Gfx* buffer;

    Bowerbird();
    ~Bowerbird();

    void disableClipping();
    void enableClipping();
    void close(s32 arg1);
    void branch(Bowerbird* other);
    void func_8002BC14(s32 n);
    s32 reserve(Gfx** out, u32 needed);
    void clear();
    void open(s32 id, u32 size);

    static void func_8002BD30(s32 value);
};

#endif
