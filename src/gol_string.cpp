#include "common.h"

static inline s32 wideLength(const u16* str) {
    s32 length = 0;
    while (*str++ != 0) {
        length++;
    }
    return length;
}

class GolString {
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
    s32 func_80010280(GolString* src);
    s32 func_800102FC(u16* src);
    s32 func_80010374(GolString* src);
    s32 func_800103F0(u16* src);
    s32 func_800104B8(GolString* src);
    void func_80010564();
    void func_800105E0();
    void func_80010640();
    s32 func_80010680(GolString* src);
    s32 func_800106BC(u16* d, u16 cap);
    u16* func_8001075C(s32 index);
    ~GolString();
    GolString();

    u16 size(void) {
        u16 result = length - unk6;
        return result;
    }
};

static inline u16* wideAt(GolString* str, s32 index) {
    return str->data + str->unk6 + index;
}

s32 GolString::func_800100C0() {
    s32 i = wideLength(this->data);
    s32 lines = 1;
    for (i--; i >= 0; i--) {
        if (this->data[i] == 0xA) {
            lines++;
        }
    }
    return lines;
}

void GolString::func_80010118(u8* dst) {
    s32 strlen;
    u8* s = dst;

    strlen = size();
    memset(dst, 0, 8);

    if ((strlen < 0) || (strlen >= 9)) {
        strlen = 8;
    }

    for (strlen--; strlen >= 0; strlen--) {
        s[strlen] = this->data[strlen];
    }
}

void GolString::func_800101A4(u8* dst) {
    s32 n = size();
    dst[n] = 0;
    for (n--; n >= 0; n--) {
        dst[n] = this->data[n];
    }
}

void GolString::func_800101F0() {
}

s32 GolString::func_800101F8(u16* src) {
    s32 srcLen = wideLength(src);
    if (srcLen >= this->capacity) {
        return 0;
    }
    memcpy(this->data, src, srcLen * 2);
    this->data[srcLen] = 0;
    this->length = srcLen;
    return 1;
}

s32 GolString::func_80010280(GolString* src) {
    s32 srcLen = wideLength(src->data);
    if (srcLen >= this->capacity) {
        return 0;
    }
    memcpy(this->data, src->data, srcLen * 2);
    this->unk6 = src->unk6;
    this->length = src->length;
    return 1;
}

s32 GolString::func_800102FC(u16* src) {
    s32 i = size();
    if (i != wideLength(src)) {
        return 0;
    }
    for (i--; i >= 0; i--) {
        if (this->data[i] != src[i]) {
            return 0;
        }
    }
    return 1;
}

s32 GolString::func_80010374(GolString* src) {
    s32 i = size();
    if (i != src->size()) {
        return 0;
    }
    for (i--; i >= 0; i--) {
        if (this->data[i] != *wideAt(src, i)) {
            return 0;
        }
    }
    return 1;
}

s32 GolString::func_800103F0(u16* src) {
    s32 newSize = size() + wideLength(src);
    if (newSize >= this->capacity) {
        return 0;
    }
    memcpy(&this->data[this->length], src, wideLength(src) * 2);
    this->length = newSize;
    this->data[this->length] = 0;
    return 1;
}

s32 GolString::func_800104B8(GolString* src) {
    s32 newSize = size() + src->size();
    if (newSize >= this->capacity) {
        return 0;
    }
    memcpy(&this->data[this->length], &src->data[src->unk6], src->size() * 2);
    this->length = newSize;
    this->data[this->length] = 0;
    return 1;
}

void GolString::func_80010564() {
    if (this->data[this->length] == 0) {
        return;
    }
    this->length++;
    this->unk6 = this->length;
    while (this->data[this->length] != 0 && this->data[this->length] != 0xA) {
        this->length++;
    }
}

void GolString::func_800105E0() {
    this->length = 0;
    this->unk6 = 0;
    while (this->data[this->length] != 0 && this->data[this->length] != 0xA) {
        this->length++;
    }
}

void GolString::func_80010640() {
    this->length = 0;
    this->unk6 = 0;
    while (this->data[this->length] != 0) {
        this->length++;
    }
}

s32 GolString::func_80010680(GolString* src) {
    this->data = NULL;
    this->length = 0;
    this->unk6 = 0;
    this->capacity = 0;
    this->data = src->data;
    this->capacity = src->capacity;
    this->func_80010640();
    return 1;
}

s32 GolString::func_800106BC(u16* d, u16 cap) {
    this->data = NULL;
    this->length = 0;
    this->unk6 = 0;
    this->capacity = 0;
    this->data = d;
    this->func_80010640();
    if (cap != 0) {
        if (size() >= cap) {
            this->data = NULL;
            this->length = 0;
            this->unk6 = 0;
            this->capacity = 0;
            return 0;
        }
    } else {
        s32 required = size() + 1;
        cap = required;
    }
    this->capacity = cap;
    return 1;
}

u16* GolString::func_8001075C(s32 index) {
    return wideAt(this, index);
}

GolString::~GolString() {
    this->data = NULL;
    this->length = 0;
    this->unk6 = 0;
    this->capacity = 0;
}

GolString::GolString() {
    this->data = NULL;
    this->length = 0;
    this->unk6 = 0;
    this->capacity = 0;
}

extern "C" void func_800107C4(u16* a, u8* b) {
    while (*(volatile u16*)a != 0) {
        (void)*(volatile u8*)b;
        b += 2;
    }
    *b = 0;
}

extern "C" void func_800107E8(u8* src, u16* dst) {
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
}

extern "C" void func_80010814(s32 n, u16* dst) {
    s32 place = 10;
    while (n / place != 0) {
        place *= 10;
    }
    place /= 10;
    if (n != 0) {
        do {
            *dst++ = n / place + '0';
            n %= place;
            place /= 10;
        } while (n != 0);
    }
    *dst = 0;
}

extern "C" s32 func_8001091C(u16* a0, u16* a1) {
    do {
        if (*a0 != *a1) {
            return 0;
        }

        a1++;
    } while (*(a0++) != 0);

    return 1;
}

extern "C" s32 func_80010944(u16* s) {
    return wideLength(s);
}

extern "C" s32 func_80010964(void) {
    return 1;
}
