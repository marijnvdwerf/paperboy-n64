#ifndef PENGUIN_H
#define PENGUIN_H

#ifdef __cplusplus

#include "common.h"
#include "otter.h"
#include "pelican.h"

class Penguin : public Pelican {
  public:
    /* 0x30 */ Otter* unk30;
    /* 0x34 */ s32 fileNo;

    Penguin();
    virtual s32 seek(u32);
    virtual s32 rawClose();
    virtual s32 rawRead(void* buf, s32 len, s32* out);
    virtual s32 rawWrite(void* buf, s32 len);
    virtual s32 sync();
    virtual s32 vfunc17(Otter* otter, const char* path, s32 length);
};

#endif
#endif
