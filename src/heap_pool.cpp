#include "heap.h"

extern "C" const char D_80005410[] = "";

#ifdef NON_MATCHING
// search loop shape: target uses `j` back-edge + separate cached pool+i*0x18 register
s32 HeapPool::alloc(u32 size, u32 alignShift) {
    u32 alignedSize = ALIGN_UP(size, 0x40);
    s32 i = 0;
    while (i < 16 && this->heaps[i].size != 0) {
        i++;
    }
    if (i >= 16) {
        __assert(D_80005410, 0, 0, 0);
    }

    FreeRegion* r = this->freeList;
    while (r != NULL && (s32)r->size < (s32)alignedSize) {
        r = r->next;
    }
    if (r == NULL) {
        __assert(D_80005410, 0, 0, 0);
    }

    u32 regionSize = r->size;
    u32 remaining = regionSize - alignedSize;
    if (remaining >= 0x41) {
        FreeRegion* nr = (FreeRegion*)((u8*)r + alignedSize);
        nr->size = remaining;
        nr->next = r->next;
        nr->prev = r;
        if (r->next != NULL) {
            r->next->prev = nr;
        }
        r->size = alignedSize;
        r->next = nr;
    } else {
        alignedSize = regionSize;
    }

    if (this->freeList == r) {
        this->freeList = r->next;
        if (r->next != NULL) {
            r->next->prev = NULL;
        }
    } else {
        r->prev->next = r->next;
        if (r->next != NULL) {
            r->next->prev = r->prev;
        }
    }

    this->heaps[i].bind((u8*)r, alignedSize, i, alignShift);
    this->used[i] = 1;
    this->idCounter++;
    this->ids[i] = this->idCounter;
    return i;
}
#else
INCLUDE_ASM("asm/nonmatchings/heap_pool", alloc__8HeapPoolUlUl);
#endif

#ifdef NON_MATCHING
// reg alloc s0/s1 swap (pool/idx) — opaque allocator choice; tried many variants
void HeapPool::free(s32 idx) {
    this->bumpId(idx);
    if (this->used[idx] != 0) {
        u32 sz = this->heaps[idx].size;
        FreeRegion* r = (FreeRegion*)this->heaps[idx].base;
        FreeRegion* prev = NULL;
        FreeRegion* cur = this->freeList;
        while (cur != NULL && cur < r) {
            prev = cur;
            cur = cur->next;
        }
        r->size = sz;
        r->next = cur;
        r->prev = prev;
        if (prev == NULL) {
            this->freeList = r;
        } else {
            prev->next = r;
        }
        if (cur != NULL) {
            if ((u8*)r + sz == (u8*)cur) {
                r->next = cur->next;
                r->size += cur->size;
                if (cur->next != NULL) {
                    cur->next->prev = r;
                }
            } else {
                cur->prev = r;
            }
        }
        if (prev != NULL && (u8*)prev + prev->size == (u8*)r) {
            prev->next = r->next;
            prev->size += r->size;
            if (r->next != NULL) {
                r->next->prev = prev;
            }
        }
    }
    this->heaps[idx].reset();
}
#else
INCLUDE_ASM("asm/nonmatchings/heap_pool", free__8HeapPooll);
#endif

u32 HeapPool::maxFreeSize() {
    FreeRegion* r = this->freeList;
    u32 maxSize = 0;
    if (r != NULL) {
        do {
            u32 sz = r->size;
            if ((s32)maxSize < (s32)sz) {
                maxSize = sz;
            }
            r = r->next;
        } while (r != NULL);
    }
    return maxSize & ~0x3F;
}

void HeapPool::bumpId(s32 idx) {
    this->idCounter++;
    this->ids[idx] = this->idCounter;
}

void HeapPool::mergeIds(s32 idxA, s32 idxB) {
    this->idCounter++;
    u32 newId = this->idCounter;
    u32 oldId;
    u32 j;
    oldId = this->ids[idxA];
    for (j = 0; j < 16; j++) {
        if (this->ids[j] == oldId) {
            this->ids[j] = newId;
        }
    }
    oldId = this->ids[idxB];
    for (j = 0; j < 16; j++) {
        if (this->ids[j] == oldId) {
            this->ids[j] = newId;
        }
    }
}

#ifdef NON_MATCHING
// target uses sizeOff (offset number) and heapOff (offset number) as separate registers;
// idiomatic C++ collapses pool+sizeOff into one pointer register
void HeapPool::freePtr(void* ptr) {
    u32 i = 0;
    while (1) {
        if (i >= 16) {
            break;
        }
        if (this->heaps[i].size != 0) {
            if (!((u8*)ptr < this->heaps[i].base)) {
                if ((u8*)ptr < this->heaps[i].base + this->heaps[i].size) {
                    this->heaps[i].free(ptr);
                    return;
                }
            }
        }
        i++;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/heap_pool", freePtr__8HeapPoolPv);
#endif

#ifdef NON_MATCHING
// target caches this+idx*4 across the loop; idiomatic C++ doesn't surface that pattern
void* HeapPool::allocFrom(s32 idx, u32 size) {
    void* p = this->heaps[idx].alloc(size);
    if (p != NULL) {
        return p;
    }
    u32 i = 0;
    while (1) {
        if (i >= 16) {
            return NULL;
        }
        if (i != (u32)idx) {
            if (this->heaps[i].size != 0) {
                if (this->ids[i] == this->ids[idx]) {
                    p = this->heaps[i].alloc(size);
                    if (p != NULL) {
                        return p;
                    }
                }
            }
        }
        i++;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/heap_pool", allocFrom__8HeapPoollUl);
#endif

void HeapPool::initHeap(s32 idx) {
    this->heaps[idx].init();
}

#ifdef NON_MATCHING
// target search loop uses `j` back-edge + separate cached this+i*0x18 register
s32 HeapPool::bind(u8* base, u32 size, u32 alignShift) {
    u32 alignedSize = ALIGN_UP(size, 0x40);
    u8* adjustedBase = base - (alignedSize - size);
    s32 i;
    for (i = 0; i < 16; i++) {
        if (this->heaps[i].size == 0) {
            break;
        }
    }
    if (i >= 16) {
        __assert(D_80005410, 0, 0, 0);
    }
    this->heaps[i].bind(adjustedBase, alignedSize, i, alignShift);
    this->used[i] = 0;
    this->idCounter++;
    this->ids[i] = this->idCounter;
    return i;
}
#else
INCLUDE_ASM("asm/nonmatchings/heap_pool", bind__8HeapPoolPUcUlUl);
#endif

#ifdef NON_MATCHING
// compiler collapses this+offset into one s0 (target keeps offset and this separate)
void HeapPool::reset() {
    u32 i = 0;
    while (1) {
        if (i >= 16) {
            break;
        }
        if (this->heaps[i].size != 0) {
            this->heaps[i].reset();
        }
        i++;
    }
    this->base = NULL;
    this->usableSize = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/heap_pool", reset__8HeapPool);
#endif

void HeapPool::init(u8* base, u32 size, u32 alignShift) {
    if (this->usableSize != 0) {
        this->reset();
    }
    this->base = (u8*)(ALIGN_UP(base, 0x40));
    this->usableSize = size - ((u8*)this->base - base);
    this->freeList = (FreeRegion*)this->base;
    this->freeList->size = size;
    this->freeList->next = NULL;
    this->freeList->prev = NULL;
    memset(&this->ids[0], 0, 0x40);
}

HeapPool::HeapPool() {
    this->base = NULL;
    this->freeList = NULL;
    this->usableSize = 0;
    this->idCounter = 0;
}
