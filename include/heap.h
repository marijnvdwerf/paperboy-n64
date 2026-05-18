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

struct FreeRegion {
    /* 0x00 */ u32 size;
    /* 0x04 */ FreeRegion* next;
    /* 0x08 */ FreeRegion* prev;
};

struct HeapPool {
    /* 0x000 */ void* base;
    /* 0x004 */ u32 usableSize;
    /* 0x008 */ FreeRegion* freeList;
    /* 0x00C */ Heap heaps[16];
    /* 0x18C */ u32 idCounter;
    /* 0x190 */ u32 ids[16];
    /* 0x1D0 */ u32 used[16];

    HeapPool();
    s32 alloc(u32 size, u32 alignShift);
    void free(s32 idx);
    u32 maxFreeSize();
    void bumpId(s32 idx);
    void mergeIds(s32 idxA, s32 idxB);
    void freePtr(void* ptr);
    void* allocFrom(s32 idx, u32 size);
    void initHeap(s32 idx);
    s32 bind(u8* base, u32 size, u32 alignShift);
    void reset();
    void init(u8* base, u32 size, u32 alignShift);
};

#endif
