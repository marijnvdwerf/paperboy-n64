#include "pelican.h"

extern "C" {
extern void* D_80076504;
void func_8004B3BC(void*);
void func_8004B390();
}

s32 Pelican::close() {
    if (!(this->openFlags & 0x20)) {
        if (this->buffer != NULL) {
            delete[] this->buffer;
            this->buffer = NULL;
        }
    }
    s32 r = this->rawClose();
    this->bufferStart = 0;
    this->bufferEnd = 0;
    this->openFlags = 0;
    this->state = 0;
    return r;
}

s32 Pelican::vfunc18(Otter* otter, const char* path, s32 openFlags, s32 capacity, s32 length) {
    this->openFlags = openFlags;
    this->state = 0;
    this->readCursor = 0;
    this->bufferStart = 0;
    this->bufferEnd = 0;
    this->fileOffset = 0;
    s32 r = this->vfunc17(otter, path, length);
    if (r != 0)
        return r;
    this->state = 1;
    if (openFlags & 0x20 && this->buffer) {
    } else {
        if (this->buffer != NULL) {
            delete[] this->buffer;
            this->buffer = NULL;
        }
        if (capacity != 0) {
            capacity += (capacity & 1);
            func_8004B3BC(D_80076504);
            this->buffer = new u8[capacity];
            func_8004B390();
            if (this->buffer == NULL) {
                this->close();
                return 4;
            }
        }
        this->bufferCapacity = capacity;
    }

    return 0;
}

s32 Pelican::open(const char*, s32, s32) {
    return 8;
}

s32 Pelican::func_80045934() {
    return 8;
}

Pelican::~Pelican() {
    if (this->buffer != NULL) {
        delete[] this->buffer;
        this->buffer = NULL;
    }
    this->close();
}
