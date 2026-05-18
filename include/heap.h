#ifndef HEAP_H
#define HEAP_H

#include "common.h"

struct HeapBlock {
    /* 0x00 */ u32 id : 6;
    /* 0x00 */ u32 used : 1;
    /* 0x00 */ u32 size : 25;
    /* 0x04 */ HeapBlock* prevPhys;
    /* 0x08 */ HeapBlock* nextFree;
    /* 0x0C */ HeapBlock* prevFree;
};

struct Heap {
    /* 0x00 */ u8* base;
    /* 0x04 */ u32 size;
    /* 0x08 */ u32 used;
    /* 0x0C */ u32 alignShift;
    /* 0x10 */ u32 id;
    /* 0x14 */ HeapBlock* freeList;

    Heap();
    void* alloc(u32 reqSize);
    void free(void* ptr);
    s32 freeBlockCount();
    u32 maxFreeBlockSize();
    void reset();
    void init();
    void bind(u8* base, u32 size, u32 id, u32 alignShift);
};

#endif
