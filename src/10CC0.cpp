#include "common.h"

extern "C" {
void* memset(void*, s32, unsigned);
void* memcpy(void*, const void*, unsigned);
}

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
};

// reg-alloc: target picks a1/a3 for i/lines, ours picks a2/a1; all instructions match
#ifdef NON_MATCHING
s32 WString::func_800100C0() {
    u16* p = this->data;
    s32 i;
    for (i = 0; *p++ != 0; i++) {
    }
    s32 lines = 1;
    for (i--; i >= 0; i--) {
        if (this->data[i] == 0xA) {
            lines++;
        }
    }
    return lines;
}
#else
INCLUDE_ASM("asm/nonmatchings/10CC0", func_800100C0__7WString);
#endif

// reg-alloc: target uses s2 for `this`, s1 for dst; ours swaps them
#ifdef NON_MATCHING
void WString::func_80010118(u8* dst) {
    s32 n = (u16)(this->length - this->unk6);
    memset(dst, 0, 8);
    if (n < 0 || n >= 9) {
        n = 8;
    }
    for (n--; n >= 0; n--) {
        dst[n] = ((u8*)this->data)[n * 2 + 1];
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/10CC0", func_80010118__7WStringPUc);
#endif

void WString::func_800101A4(u8* dst) {
    s32 n = (u16)(this->length - this->unk6);
    dst[n] = 0;
    for (n--; n >= 0; n--) {
        dst[n] = ((u8*)this->data)[n * 2 + 1];
    }
}

void WString::func_800101F0() {
}

// scheduling: target hoists `s1=0` between saves; can't coax
#ifdef NON_MATCHING
s32 WString::func_800101F8(u16* src) {
    s32 srcLen = 0;
    if (*src != 0) {
        u16* p = src + 1;
        do {
            srcLen++;
        } while (*p++ != 0);
    }
    if (srcLen >= this->capacity) {
        return 0;
    }
    memcpy(this->data, src, srcLen * 2);
    this->data[srcLen] = 0;
    this->length = srcLen;
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/10CC0", func_800101F8__7WStringPUs);
#endif

s32 WString::func_80010280(WString* src) {
    u16* p = src->data;
    s32 srcLen;
    for (srcLen = 0; *p++ != 0; srcLen++) {
    }
    if (srcLen >= this->capacity) {
        return 0;
    }
    memcpy(this->data, src->data, srcLen * 2);
    this->unk6 = src->unk6;
    this->length = src->length;
    return 1;
}

// inlines wcslen; same reg-alloc shape as func_80010374
#ifdef NON_MATCHING
s32 WString::func_800102FC(u16* src) {
    s32 i = (u16)(this->length - this->unk6);
    if (i != func_80010944(src)) {
        return 0;
    }
    for (i--; i >= 0; i--) {
        if (this->data[i] != src[i]) {
            return 0;
        }
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/10CC0", func_800102FC__7WStringPUs);
#endif

// reg-alloc: target moves both a0→t0 and a1→a3; loop body laid out top-tested via j-to-test
#ifdef NON_MATCHING
s32 WString::func_80010374(WString* src) {
    s32 i = (u16)(this->length - this->unk6);
    if (i != (u16)(src->length - src->unk6)) {
        return 0;
    }
    for (i--; i >= 0; i--) {
        if (this->data[i] != src->data[src->unk6 + i]) {
            return 0;
        }
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/10CC0", func_80010374__7WStringP7WString);
#endif

// inlines wcslen twice; same reg-alloc shape as func_800104B8
#ifdef NON_MATCHING
s32 WString::func_800103F0(u16* src) {
    s32 newSize = (u16)(this->length - this->unk6) + func_80010944(src);
    if (newSize >= this->capacity) {
        return 0;
    }
    memcpy(&this->data[this->length], src, func_80010944(src) * 2);
    this->length = newSize;
    this->data[this->length] = 0;
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/10CC0", func_800103F0__7WStringPUs);
#endif

// reg-alloc: target promotes src param a1→a3; can't reproduce
#ifdef NON_MATCHING
s32 WString::func_800104B8(WString* src) {
    s32 newSize = (u16)(this->length - this->unk6) + (u16)(src->length - src->unk6);
    if (newSize >= this->capacity) {
        return 0;
    }
    memcpy(&this->data[this->length], &src->data[src->unk6], (src->length - src->unk6) * 2);
    this->length = newSize;
    this->data[this->length] = 0;
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/10CC0", func_800104B8__7WStringP7WString);
#endif

void WString::func_80010564() {
    if (this->data[this->length] == 0) {
        return;
    }
    this->length++;
    this->unk6 = this->length;
    while (this->data[this->length] != 0 && this->data[this->length] != 0xA) {
        this->length++;
    }
}

void WString::func_800105E0() {
    this->length = 0;
    this->unk6 = 0;
    while (this->data[this->length] != 0 && this->data[this->length] != 0xA) {
        this->length++;
    }
}

void WString::func_80010640() {
    this->length = 0;
    this->unk6 = 0;
    while (this->data[this->length] != 0) {
        this->length++;
    }
}

s32 WString::func_80010680(WString* src) {
    this->data = NULL;
    this->length = 0;
    this->unk6 = 0;
    this->capacity = 0;
    this->data = src->data;
    this->capacity = src->capacity;
    this->func_80010640();
    return 1;
}

// compiler emits `move v0,s1` extra; target uses bnezl (branch-likely) we can't reliably coax
#ifdef NON_MATCHING
s32 WString::func_800106BC(u16* d, u16 cap) {
    this->data = NULL;
    this->length = 0;
    this->unk6 = 0;
    this->capacity = 0;
    this->data = d;
    this->func_80010640();
    if (cap != 0) {
        if ((u16)(this->length - this->unk6) < cap) {
            this->capacity = cap;
            return 1;
        }
        this->data = NULL;
        this->length = 0;
        this->unk6 = 0;
        this->capacity = 0;
        return 0;
    }
    this->capacity = (u16)(this->length - this->unk6) + 1;
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/10CC0", func_800106BC__7WStringPUsUs);
#endif

u16* WString::func_8001075C(s32 index) {
    return this->data + this->unk6 + index;
}

WString::~WString() {
    this->data = NULL;
    this->length = 0;
    this->unk6 = 0;
    this->capacity = 0;
}

WString::WString() {
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
            s32 digit = n / place;
            s32 rem = n % place;
            place /= 10;
            *dst++ = digit + '0';
            n = rem;
        } while (n != 0);
    }
    *dst = 0;
}

// compiler hoists `v0=0` init before the loop; target keeps it in the second jr ra delay slot
#ifdef NON_MATCHING
extern "C" s32 func_8001091C(u16* a, u16* b) {
    while (*a == *b++) {
        if (*a == 0) {
            return 1;
        }
        a++;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/10CC0", func_8001091C);
#endif

extern "C" s32 func_80010944(u16* s) {
    s32 n;
    for (n = 0; *s++ != 0; n++) {
    }
    return n;
}

extern "C" s32 func_80010964(void) {
    return 1;
}
