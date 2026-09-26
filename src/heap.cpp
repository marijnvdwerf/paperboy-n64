#include "heap.h"

void* Heap::alloc(u32 reqSize) {
    u32 hdrSize = 8;
    if (this->alignShift != 0) {
        u32 alignM1 = (1U << this->alignShift) - 1;
        reqSize += alignM1;
        reqSize &= ~alignM1;
        hdrSize = (1U << this->alignShift) + 7;
        hdrSize &= ~alignM1;
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

    if (reqSize + 0x10 < blkSize) {
        b->size = reqSize;
        HeapBlock* newBlock = (HeapBlock*)((u8*)b + reqSize);
        HeapBlock* phys = (HeapBlock*)((u8*)b + blkSize);
        newBlock->size = blkSize - reqSize;
        newBlock->nextFree = b->nextFree;
        newBlock->prevFree = b->prevFree;
        if (newBlock->nextFree != NULL) {
            newBlock->nextFree->prevFree = newBlock;
        }
        if (this->freeList == b) {
            this->freeList = newBlock;
        } else if (newBlock->prevFree != NULL) {
            newBlock->prevFree->nextFree = newBlock;
        }
        newBlock->prevPhys = b;
        if ((u8*)phys < this->base + this->size) {
            phys->prevPhys = newBlock;
        }
        newBlock->used = 0;
        newBlock->id = this->id;
        blkSize = reqSize;
    } else {
        if (this->freeList == b) {
            this->freeList = b->nextFree;
        } else {
            b->prevFree->nextFree = b->nextFree;
        }
        if (b->nextFree != NULL) {
            b->nextFree->prevFree = b->prevFree;
        }
    }

    this->used += blkSize;
    b->setUsed(1);
    b->id = this->id;
    return (u8*)b + hdrSize;
}

void Heap::free(void* ptr) {
    u32 hdrSize = 8;
    if (this->alignShift != 0) {
        hdrSize = (1U << this->alignShift) + 7;
        hdrSize &= -(1U << this->alignShift);
    }
    HeapBlock* b = (HeapBlock*)((u8*)ptr - hdrSize);
    u32 blkSize = b->size;
    HeapBlock* next = (HeapBlock*)((u8*)b + blkSize);
    b->used = 0;
    this->used -= blkSize;
    u8* end = this->base + this->size;

    bool merged = false;
    if ((u8*)next < end && next->getUsed() == 0) {
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
        next = (HeapBlock*)((u8*)b + blkSize);
        if ((u8*)next < end) {
            next->prevPhys = b;
        }
        merged = true;
    }

    HeapBlock* prev = b->prevPhys;
    if (prev != NULL && prev->getUsed() == 0) {
        blkSize += prev->size;
        prev->size = blkSize;
        if (merged) {
            if (this->freeList == b) {
                this->freeList = b->nextFree;
            } else {
                b->prevFree->nextFree = b->nextFree;
            }
            if (b->nextFree != NULL) {
                b->nextFree->prevFree = b->prevFree;
            }
        }
        next = (HeapBlock*)((u8*)prev + blkSize);
        if ((u8*)next < end) {
            next->prevPhys = prev;
        }
        merged = true;
        return;
    }
    if (!merged) {
        b->nextFree = this->freeList;
        b->prevFree = NULL;
        if (this->freeList != NULL) {
            this->freeList->prevFree = b;
        }
        this->freeList = b;
    }
}

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
            if (maxSize < b->size) {
                maxSize = b->size;
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
