#include "common.h"
#include "file.h"

class WString {
  public:
    /* 0x0 */ u16* data;
    /* 0x4 */ u16 capacity;
    /* 0x6 */ u16 unk6;
    /* 0x8 */ u16 length;

    s32 func_800100C0();
    void func_80010118(u8* dst);
    void func_800101A4(u8* dst);
    void func_800101F0();
    s32 func_800101F8(u16* src);
    s32 func_80010280(WString* src);
    s32 func_800102FC(u16* src);
    s32 func_80010374(WString* src);
    s32 func_800103F0(u16* src);
    s32 func_800104B8(WString* src);
    void func_80010564();
    void func_800105E0();
    void func_80010640();
    s32 func_80010680(WString* src);
    s32 func_800106BC(u16* d, u16 cap);
    u16* func_8001075C(s32 index);
    ~WString();
    WString();

    u16 size(void) {
        return length - unk6;
    }
};

extern "C" {
extern s32 D_80076630;

void func_8004B390(void);
void func_8004B3BC(s32);
s32 func_8001091C(u16* a, u16* b);
}


class Kookaburra {
  public:
    /* 0x00 */ u8 loaded;
    /* 0x04 */ u16* data;
    /* 0x08 */ u16* indices;
    /* 0x0C */ u16 count;

    virtual ~Kookaburra();
    virtual s32 cleanup();
    virtual s32 load(const char* filename);
    virtual void freeBuffers();

    Kookaburra();
};

#ifdef NON_MATCHING
// reg-alloc: 4 saved regs vs target's 5; second loop ptr+=2 scheduling
extern "C" s32 func_800459A0(Kookaburra* self, const char* filename) {
    u8 buf[2];
    File file;
    s32 bytesRead;
    u16 dataCount;
    s32 offset;

    if (self->indices) {
        self->freeBuffers();
    }

    if (file.open(filename, 2, 0)) {
        return 0;
    }
    if (file.readAt(0, buf, 2, &bytesRead)) {
        return 0;
    }
    if (bytesRead != 2) {
        return 0;
    }

    self->count = buf[0] + (buf[1] << 8);
    offset = 2;

    if (file.readAt(offset, buf, offset, &bytesRead)) {
        return 0;
    }
    if (bytesRead != offset) {
        return 0;
    }

    dataCount = buf[0] | (buf[1] << 8);

    func_8004B3BC(D_80076630);
    self->indices = new u16[self->count];
    self->data = new u16[dataCount];
    func_8004B390();

    offset = 4;

    if (self->indices == NULL || self->data == NULL) {
        __assert("", 0, 0, 0);
    }

    if (file.readAt(offset, self->indices, self->count * 2, &bytesRead)) {
        return 0;
    }

    offset += bytesRead;

    u8* ptr = (u8*)self->indices;
    for (s32 i = 0; i < self->count; i++) {
        self->indices[i] = ptr[0] + (ptr[1] << 8);
        ptr += 2;
    }

    s32 total = (u16)dataCount;
    if (file.readAt(offset, self->data, total * 2, &bytesRead)) {
        return 0;
    }

    file.close();

    ptr = (u8*)self->data;
    for (s32 i = 0; i < total; i++) {
        self->data[i] = ptr[0] + (ptr[1] << 8);
        ptr += 2;
    }

    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/kookaburra", func_800459A0);
#endif

extern "C" u16 func_80045C18(Kookaburra* self, u16* key) {
    for (s32 i = 0; i < self->count; i++) {
        if (func_8001091C(key, self->data + self->indices[i])) {
            return i;
        }
    }
    return 0xFFFF;
}

extern "C" u16* func_80045C9C(Kookaburra* self, u16 index) {
    if (self->data == NULL) {
        return NULL;
    }
    if (self->count < index) {
        return NULL;
    }
    return self->data + self->indices[index];
}

extern "C" s32 func_80045CE0(Kookaburra* self, WString* dst, u16 index) {
    if (self->data == NULL) {
        return 0;
    }
    if (self->count < index) {
        return 0;
    }
    return dst->func_800106BC(self->data + self->indices[index], 0);
}

extern "C" void func_80045D3C(Kookaburra* self) {
    if (self->indices) {
        delete[] self->indices;
    }
    if (self->data) {
        delete[] self->data;
    }
    self->indices = NULL;
    self->data = NULL;
}

extern "C" s32 func_80045D8C(Kookaburra* self) {
    if (self->loaded) {
        if (self->data) {
            delete self->data;
        }
        if (self->indices) {
            delete self->indices;
        }
        self->data = NULL;
        self->indices = NULL;
    }
    return 1;
}

extern "C" s32 func_80045DEC(Kookaburra* self) {
    if (self->loaded) {
        self->cleanup();
    }
    if (D_80076630 == -1) {
        return 0;
    }
    self->loaded = 1;
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

extern "C" void func_80045ED0(s32 value) {
    D_80076630 = value;
}

extern "C" u16 func_80045EDC(Kookaburra* self) {
    return self->count;
}

extern "C" u16 func_80045EE8(Kookaburra* self, u16** key) {
    return func_80045C18(self, *key);
}

extern "C" u8 func_80045F0C(Kookaburra* self) {
    return self->loaded;
}

