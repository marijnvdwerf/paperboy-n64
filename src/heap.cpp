#include "heap.h"

#ifdef NON_MATCHING
void* Heap::alloc(u32 reqSize) {
    u32 hdrSize = 8;
    if (this->alignShift != 0) {
        u32 alignment = 1 << this->alignShift;
        u32 alignM1 = alignment - 1;
        reqSize += alignM1;
        u32 mask = ~alignM1;
        reqSize &= mask;
        hdrSize = (alignment + 7) & mask;
    }
    reqSize += hdrSize;
    if (reqSize < 0x10) {
        reqSize = 0x10;
    }

    HeapBlock* b = this->freeList;
    u32 blkSize;
    while (b != NULL) {
        blkSize = b->size;
        if (blkSize >= reqSize) {
            break;
        }
        b = b->nextFree;
    }
    if (b == NULL) {
        return NULL;
    }

    u32 consumed;
    if (reqSize + 0x10 < blkSize) {
        consumed = reqSize;
        b->size = reqSize;
        HeapBlock* newBlock = (HeapBlock*)((u8*)b + reqSize);
        HeapBlock* phys = (HeapBlock*)((u8*)b + blkSize);
        newBlock->size = blkSize - reqSize;
        newBlock->nextFree = b->nextFree;
        newBlock->prevFree = b->prevFree;
        if (b->nextFree != NULL) {
            b->nextFree->prevFree = newBlock;
        }
        if (this->freeList == b) {
            this->freeList = newBlock;
        } else if (b->prevFree != NULL) {
            b->prevFree->nextFree = newBlock;
        }
        newBlock->prevPhys = b;
        if ((u8*)phys < this->base + this->size) {
            phys->prevPhys = newBlock;
        }
        newBlock->used = 0;
        newBlock->id = this->id;
    } else {
        consumed = blkSize;
        if (this->freeList == b) {
            this->freeList = b->nextFree;
        } else {
            b->prevFree->nextFree = b->nextFree;
        }
        if (b->nextFree != NULL) {
            b->nextFree->prevFree = b->prevFree;
        }
    }

    this->used += consumed;
    b->used = 1;
    b->id = this->id;
    return (u8*)b + hdrSize;
}
#else
extern "C" {
INCLUDE_ASM("asm/nonmatchings/heap", alloc__4HeapUl);
}
#endif

#ifdef NON_MATCHING
void Heap::free(void* ptr) {
    int zero;
    u32 hdrSize = 8;
    zero = 0;
    if (this->alignShift != 0) {
        u32 alignment = 1 << this->alignShift;
        hdrSize = alignment + 7;
        hdrSize &= -alignment;
    }
    HeapBlock* b = (HeapBlock*)((u8*)ptr - hdrSize);
    u32 blkSize = b->size;
    HeapBlock* next = (HeapBlock*)((u8*)b + blkSize);
    b->used = 0;
    this->used -= blkSize;
    u8* end = this->base + this->size;

    s32 mergedNext = 0;
    if ((u8*)next < end && next->used == zero) {
        blkSize += next->size;
        b->size = blkSize;
        b->nextFree = next->nextFree;
        b->prevFree = next->prevFree;
        if (this->freeList == next) {
            this->freeList = b;
        } else if (next->prevFree != NULL) {
            next->prevFree->nextFree = b;
        }
        if (next->nextFree != NULL) {
            next->nextFree->prevFree = b;
        }
        HeapBlock* after = (HeapBlock*)((u8*)b + blkSize);
        if ((u8*)after < end) {
            after->prevPhys = b;
        }
        mergedNext = 1;
    }

    HeapBlock* prev = b->prevPhys;
    if (prev != NULL && prev->used == zero) {
        blkSize += prev->size;
        prev->size = blkSize;
        if (mergedNext) {
            if (this->freeList == b) {
                this->freeList = b->nextFree;
            } else {
                b->prevFree->nextFree = b->nextFree;
            }
            if (b->nextFree != NULL) {
                next = b->prevFree;
                b->nextFree->prevFree = next;
            }
        }
        HeapBlock* after = (HeapBlock*)((u8*)prev + blkSize);
        if ((u8*)after < end) {
            after->prevPhys = prev;
        }
        return;
    }

    if (!mergedNext) {
        b->prevFree = NULL;
        b->nextFree = this->freeList;
        if (this->freeList != NULL) {
            this->freeList->prevFree = b;
        }
        this->freeList = b;
    }
}
#else
extern "C" {
INCLUDE_ASM("asm/nonmatchings/heap", free__4HeapPv);
}
#endif

s32 Heap::freeBlockCount() {
    HeapBlock* b = this->freeList;
    s32 count = 0;
    while (b != NULL) {
        count++;
        b = b->nextFree;
    }
    return count;
}

u32 Heap::maxFreeBlockSize() {
    HeapBlock* b = this->freeList;
    u32 maxSize = 0;
    if (b != NULL) {
        do {
            u32 sz = b->size;
            if (maxSize < sz) {
                maxSize = sz;
            }
            b = b->nextFree;
        } while (b != NULL);
    }
    return maxSize;
}

void Heap::reset() {
    this->base = NULL;
    this->size = 0;
    this->used = 0;
    this->alignShift = 0;
    this->id = 0;
    this->freeList = NULL;
}

void Heap::init() {
    this->used = 0;
    this->freeList = (HeapBlock*)this->base;
    this->freeList->size = this->size;
    this->freeList->used = 0;
    this->freeList->id = this->id;
    this->freeList->prevPhys = NULL;
    this->freeList->nextFree = NULL;
    this->freeList->prevFree = NULL;
}

void Heap::bind(u8* base, u32 size, u32 id, u32 alignShift) {
    this->base = base;
    this->size = size;
    this->id = id;
    this->alignShift = alignShift;
    this->init();
}

Heap::Heap() {
    this->base = NULL;
    this->size = 0;
    this->used = 0;
    this->alignShift = 0;
    this->id = 0;
    this->freeList = NULL;
}
