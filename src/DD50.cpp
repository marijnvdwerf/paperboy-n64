#include "common.h"

extern "C" {
char* strncpy(char* dst, const char* src, unsigned n);
char* strcat(char* dst, const char* src);
unsigned strlen(const char* s);
int tolower(int c);
void* memcpy(void* dst, const void* src, unsigned n);
OSTime osGetTime(void);
}

extern "C" u32 D_8006B5E0;

// fixed-point string-to-float parser; reg-alloc diff (a1<->a2 swap) on the
// scan/accumulate loop, structurally matches otherwise
#ifdef NON_MATCHING
extern "C" f32 func_8000D150(u8* str) {
    s32 mult = 0x1000;
    s32 frac = 0xA;
    if (*str == 0x2D) {
        str += 1;
        mult = -0x1000;
        frac = -0xA;
    }
    u8* end;
    u8 v1;
    u8* p = str;
    v1 = *p;
    end = p;
    if (v1 == 0x2E) {
        goto block_9;
    }
    for (;;) {
        if (v1 == 0) {
            if (p != str) {
                end = p;
                goto block_9;
            }
            return 0.0f;
        }
        p += 1;
        end = p;
        v1 = *p;
        if (v1 == 0x2E) {
            break;
        }
    }
block_9:;
    s32 sum = 0;
    if (str < end) {
        do {
            end -= 1;
            sum += (*end - 0x30) * mult;
            mult *= 0xA;
        } while (str < end);
    }
    if (*p == 0) {
        return (f32)sum;
    }
    u8* f = p + 1;
    if (*(p + 1) != 0) {
        do {
            sum += (s32)((*f - 0x30) << 12) / frac;
            f += 1;
            frac *= 0xA;
        } while (*f != 0);
    }
    return (f32)sum;
}
#else
INCLUDE_ASM("asm/nonmatchings/DD50", func_8000D150);
#endif

extern "C" void func_8000D270(char* dst, const char* src) {
    strncpy(dst, src, 8);
    unsigned i = 0;
    do {
        char* p = dst + i;
        if (*p == 0x2E) {
            *p = 0;
        }
        i += 1;
    } while (i < 8);
}

extern "C" const char D_80000CE4[];

extern "C" void func_8000D2C0(char* dst, const void* src) {
    if (src == NULL) {
        src = D_80000CE4;
    }
    memcpy(dst, src, 8);
    dst[8] = 0;
}

extern "C" void func_8000D2FC(char* dst, const void* src) {
    s32 i = 8;
    if (src == NULL) {
        *dst = 0;
    } else {
        memcpy(dst, src, 8);
    }
    i -= 1;
    if (i == -1) {
        return;
    }
    char v0 = *dst;
    do {
        if (v0 == 0x2E) {
            *dst = 0;
            return;
        }
        i -= 1;
    } while (i != -1);
}

extern "C" s32 func_8000D374(const char* a, const char* b) {
    if (a == NULL) {
        a = D_80000CE4;
    }
    s32 i = 7;
    if (b == NULL) {
        b = D_80000CE4;
    }
    do {
        u8 v1 = *a;
        u8 v0 = *b;
        if (v1 == v0) {
            if (v1 == 0) {
                return 0;
            }
            a += 1;
            i -= 1;
        } else {
            s32 la = tolower(*a);
            s32 lb = tolower(*b);
            if (la != lb) {
                v1 = *a;
                v0 = *b;
                return v1 - v0;
            }
            a += 1;
            i -= 1;
        }
        b += 1;
    } while (i != -1);
    return 0;
}

extern "C" char* func_8000D438(char* dst, const char* src) {
    return strncpy(dst, src, 12);
}

extern "C" void func_8000D454(char* dst, const void* src) {
    if (src == NULL) {
        src = D_80000CE4;
    }
    memcpy(dst, src, 12);
    dst[12] = 0;
}

extern "C" void func_8000D490(char* dst, const void* src) {
    s32 i = 12;
    if (src == NULL) {
        *dst = 0;
    } else {
        memcpy(dst, src, 12);
    }
    i -= 1;
    if (i == -1) {
        return;
    }
    char v0 = *dst;
    do {
        if (v0 == 0x2E) {
            *dst = 0;
            return;
        }
        i -= 1;
    } while (i != -1);
}

extern "C" s32 func_8000D508(const char* a, const char* b) {
    if (a == NULL) {
        a = D_80000CE4;
    }
    s32 i = 11;
    if (b == NULL) {
        b = D_80000CE4;
    }
    do {
        u8 v1 = *a;
        u8 v0 = *b;
        if (v1 == v0) {
            if (v1 == 0) {
                return 0;
            }
            a += 1;
            i -= 1;
        } else {
            s32 la = tolower(*a);
            s32 lb = tolower(*b);
            if (la != lb) {
                v1 = *a;
                v0 = *b;
                return v1 - v0;
            }
            a += 1;
            i -= 1;
        }
        b += 1;
    } while (i != -1);
    return 0;
}

extern "C" {
__asm__(
    ".section .rdata\n"
    ".align 2\n"
    ".globl D_80000CE0\n"
    "D_80000CE0:\n"
    "    .asciz \".\"\n"
    "    .align 2\n"
    ".section .text\n"
);
extern char D_80000CE0[];
}

extern "C" char* func_8000D5CC(char* dst, const char* a, const char* b) {
    strncpy(dst, a, 8);
    dst[8] = 0;
    strcat(dst, D_80000CE0);
    return strcat(dst, b);
}

extern "C" void func_8000D620(char* dst, const char* src, u32 maxLen) {
    u32 len = strlen(dst);
    s32 remaining = maxLen - len;
    dst += len;
    if (remaining == 0) {
        return;
    }
    do {
        u8 c = *src;
        if (c == 0) {
            break;
        }
        *dst = c;
        dst += 1;
        remaining -= 1;
        src += 1;
    } while (remaining != 0);
    if (remaining != 0) {
        *dst = 0;
    }
}

extern "C" void func_8000D690(void) {
    u32 lo = (u32)osGetTime();
    D_8006B5E0 = lo & 0x3FF;
}

INCLUDE_RODATA("asm/nonmatchings/DD50", D_80000CE4);
