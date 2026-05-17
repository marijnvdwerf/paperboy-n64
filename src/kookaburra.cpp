#include "common.h"
#include "file.h"
#include "kookaburra.h"
#include "wstring.h"

extern "C" {
extern s32 D_80076630;

void func_8004B390(void);
void func_8004B3BC(s32);
s32 func_8001091C(u16* a, u16* b);
}

s32 Kookaburra::load(const char* filename) {
    u8 buf[2];
    File file;
    s32 bytesRead;
    s32 offset;
    u16 dataCount;
    s32 i;
    u16 value;

    if (indices) {
        freeBuffers();
    }

    if (file.open(filename, 2, 0)) {
        return 0;
    }
    if (file.readAt(0, buf, 2, &bytesRead) || bytesRead != 2) {
        return 0;
    }

    offset = 2;
    count = buf[0] + (buf[1] << 8);

    if (file.readAt(offset, buf, offset, &bytesRead)) {
        return 0;
    }
    if (bytesRead != offset) {
        return 0;
    }

    dataCount = buf[0] | (buf[1] << 8);

    func_8004B3BC(D_80076630);
    indices = new u16[count];
    data = new u16[dataCount];
    func_8004B390();

    offset = 4;

    if (indices == NULL || data == NULL) {
        __assert("", 0, 0, 0);
    }

    if (file.readAt(offset, indices, count * 2, &bytesRead)) {
        return 0;
    }

    offset += bytesRead;

    u8* ptr = (u8*)indices;
    for (i = 0; i < count; i++) {
        value = ptr[0] + (ptr[1] << 8);
        indices[i] = value;
        ptr += 2;
    }

    if (file.readAt(offset, data, dataCount * 2, &bytesRead)) {
        return 0;
    }

    file.close();

    ptr = (u8*)data;
    for (i = 0; i < dataCount;) {
        value = ptr[0] + (ptr[1] << 8);
        ptr += 2;
        data[i++] = value;
    }

    return 1;
}

u16 Kookaburra::find(u16* key) {
    for (s32 i = 0; i < count; i++) {
        if (func_8001091C(key, data + indices[i])) {
            return i;
        }
    }
    return 0xFFFF;
}

u16* Kookaburra::getEntry(u16 index) {
    if (data == NULL) {
        return NULL;
    }
    if (count < index) {
        return NULL;
    }
    return data + indices[index];
}

s32 Kookaburra::copyTo(WString* dst, u16 index) {
    if (data == NULL) {
        return 0;
    }
    if (count < index) {
        return 0;
    }
    return dst->func_800106BC(data + indices[index], 0);
}

void Kookaburra::freeBuffers() {
    if (indices) {
        delete[] indices;
    }
    if (data) {
        delete[] data;
    }
    indices = NULL;
    data = NULL;
}

s32 Kookaburra::cleanup() {
    if (loaded) {
        if (data) {
            delete data;
        }
        if (indices) {
            delete indices;
        }
        data = NULL;
        indices = NULL;
    }
    return 1;
}

s32 Kookaburra::init() {
    if (loaded) {
        cleanup();
    }
    if (D_80076630 == -1) {
        return 0;
    }
    loaded = 1;
    return 1;
}

Kookaburra::~Kookaburra() {
    if (loaded) {
        cleanup();
    }
}

Kookaburra::Kookaburra() {
    loaded = 0;
    indices = NULL;
    data = NULL;
    count = 0;
}

void Kookaburra::setHeapId(s32 value) {
    D_80076630 = value;
}

u16 Kookaburra::getCount() {
    return count;
}

u16 Kookaburra::findIndirect(u16** key) {
    return find(*key);
}

u8 Kookaburra::isLoaded() {
    return loaded;
}
