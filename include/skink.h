#ifndef SKINK_H
#define SKINK_H

#include "common.h"

class RenderContext;
class Parrot;

struct SkinkEntry {
    /* 0x00 */ u8 pad[0x14];
    /* 0x14 */ char name[8];
};

struct Skink {
    /* 0x00 */ RenderContext* context;
    /* 0x04 */ u32 count;
    /* 0x08 */ SkinkEntry** entries;

    Skink();
    ~Skink();

    void func_80021380(RenderContext* ctx, Parrot* file);
    static void func_8002151C(s32 value);
    void func_80021528(s32 index, SkinkEntry* value);
    void func_8002153C(s32 index, const char* name);
    s32 func_800215DC(const char* name);
    void func_80021680();
    void func_800216C0(RenderContext* ctx, const char* path, s32 useCockatoo);
    void func_80021808(RenderContext* ctx, u32 count);
};

#endif
