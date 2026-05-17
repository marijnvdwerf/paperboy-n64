#ifndef COCKATOO_H
#define COCKATOO_H

#include "parrot.h"

struct Cockatoo : public Parrot {
    /* 0x650 */ void* unk650;

    Cockatoo();
    virtual s32 close();
    virtual void selectDriver(const char* path);
    virtual char* getExtension();
    virtual void parseError(s32 code);
    virtual s32 nextToken();
    virtual s32 isStreaming();

    void resetStream();
    s32 readBytes(s32 nbytes);
    void readStructDef();
};

#endif
