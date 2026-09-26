#include "heap.h"

s32 HeapPool::alloc(s32 size, u32 alignShift) {
    size += 0x3F;
    size &= ~0x3F;
    u32 i;
    for (i = 0; i < 16; i++) {
        if (this->heaps[i].getSize() == 0) {
            break;
        }
    }
    if (i >= 16) {
        __assert("", 0, 0, 0);
    }

    FreeRegion* r = this->freeList;
    while (r != NULL && r->size < size) {
        r = r->next;
    }
    if (r == NULL) {
        __assert("", 0, 0, 0);
    }

    if (r->size - size >= 0x41U) {
        FreeRegion* nr = (FreeRegion*)((u8*)r + size);
        nr->size = r->size - size;
        nr->next = r->next;
        if (nr->next != NULL) {
            nr->next->prev = nr;
        }
        nr->prev = r;
        r->size = size;
        r->next = nr;
    } else {
        size = r->size;
    }

    if (this->freeList == r) {
        this->freeList = r->next;
        if (this->freeList != NULL) {
            this->freeList->prev = NULL;
        }
    } else {
        r->prev->next = r->next;
    }
    if (r->next != NULL) {
        r->next->prev = r->prev;
    }

    this->heaps[i].bind((u8*)r, size, i, alignShift);
    this->used[i] = 1;
    this->idCounter++;
    this->ids[i] = this->idCounter;
    return i;
}

void HeapPool::free(s32 idx) {
    this->bumpId(idx);
    if (this->used[idx] != 0) {
        u32 sz = this->heaps[idx].getSize();
        FreeRegion* r = (FreeRegion*)this->heaps[idx].getBase();
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

u32 HeapPool::maxFreeSize() {
    FreeRegion* r = this->freeList;
    s32 maxSize = 0;
    if (r != NULL) {
        do {
            if (maxSize < r->size) {
                maxSize = r->size;
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

void HeapPool::freePtr(void* ptr) {
    for (u32 i = 0; i < 16; i++) {
        if (this->heaps[i].getSize() != 0) {
            if (ptr >= this->heaps[i].getBase() && ptr < this->heaps[i].getEnd()) {
                this->heaps[i].free(ptr);
                return;
            }
        }
    }
}

void* HeapPool::allocFrom(s32 idx, u32 size) {
    void* p = this->heaps[idx].alloc(size);
    if (p != NULL) {
        return p;
    }
    for (u32 i = 0; i < 16; i++) {
        if (i != idx) {
            if (this->heaps[i].getSize() != 0) {
                if (this->ids[i] == this->ids[idx]) {
                    p = this->heaps[i].alloc(size);
                    if (p != NULL) {
                        return p;
                    }
                }
            }
        }
    }
    return NULL;
}

void HeapPool::initHeap(s32 idx) {
    this->heaps[idx].init();
}

s32 HeapPool::bind(u32 size, u8* base, u32 alignShift) {
    u8* alignedBase = (u8*)ALIGN_UP(base, 0x40);
    u32 adjustedSize = size - (alignedBase - base);
    u32 i;
    for (i = 0; i < 16; i++) {
        if (this->heaps[i].getSize() == 0) {
            break;
        }
    }
    if (i >= 16) {
        __assert("", 0, 0, 0);
    }
    this->heaps[i].bind(alignedBase, adjustedSize, i, alignShift);
    this->used[i] = 0;
    this->idCounter++;
    this->ids[i] = this->idCounter;
    return i;
}

void HeapPool::reset() {
    for (u32 i = 0; i < 16; i++) {
        if (this->heaps[i].getSize() != 0) {
            this->heaps[i].reset();
        }
    }
    this->base = NULL;
    this->usableSize = 0;
}

void HeapPool::init(u8* base, u32 size, u32 alignShift) {
    if (this->usableSize != 0) {
        this->reset();
    }
    this->base = (u8*)ALIGN_UP(base, 0x40);
    this->usableSize = size - (this->base - base);
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
