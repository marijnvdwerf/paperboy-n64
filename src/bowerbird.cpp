#include "bowerbird.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);
extern s32 D_80072BB0;

void Bowerbird::disableClipping() {
    if (clipping == 0)
        return;
    if (count >= capacity - 2)
        return;
    gSPClearGeometryMode(&buffer[count++], G_CLIPPING);
    clipping = 0;
}

void Bowerbird::enableClipping() {
    if (clipping != 0)
        return;
    if (count >= capacity - 2)
        return;
    gSPSetGeometryMode(&buffer[count++], G_CLIPPING);
    clipping = 1;
}

void Bowerbird::close(s32 arg1) {
    if (count >= capacity - 2) {
        count = capacity - 2;
    }
    Gfx* gfx = &buffer[count++];
    if (arg1) {
        gDPFullSync(gfx++);
        count++;
    }
    gSPEndDisplayList(gfx++);
    osWritebackDCache(buffer, count * 8);
}

void Bowerbird::branch(Bowerbird* other) {
    if (count >= capacity - 2)
        return;
    gSPDisplayList(buffer, OS_K0_TO_PHYSICAL(other->buffer));
}

void Bowerbird::func_8002BC14(s32 n) {
    active = 0;
    count += n;
}

s32 Bowerbird::reserve(Gfx** out, u32 needed) {
    if (capacity - count - 2 < needed) {
        return 0;
    }
    *out = &buffer[count];
    active = 1;
    return 1;
}

void Bowerbird::clear() {
    if (buffer != NULL) {
        delete[] buffer;
        buffer = NULL;
    }
    capacity = 0;
    count = 0;
}

void Bowerbird::open(s32 id, u32 size) {
    capacity = size >> 3;
    count = 0;
    func_8004B3BC(D_80072BB0);
    buffer = new Gfx[capacity];
    func_8004B390();
    if (buffer == NULL) {
        __assert("", NULL, 0, NULL);
    }
    segment = id;
    active = 0;
}

void Bowerbird::func_8002BD30(s32 value) {
    D_80072BB0 = value;
}

Bowerbird::~Bowerbird() {
    clear();
}

Bowerbird::Bowerbird() {
    segment = 0;
    buffer = NULL;
    active = 0;
    capacity = 0;
    count = 0;
}
