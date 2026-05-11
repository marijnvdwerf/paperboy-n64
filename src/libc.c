#include "common.h"

extern s32 D_80064460;

INCLUDE_ASM("asm/nonmatchings/libc", qsort);

INCLUDE_ASM("asm/nonmatchings/libc", strtok);

int memcmp(const void* s1, const void* s2, u32 n) {
    const u8* u1 = (const u8*)s1;
    const u8* u2 = (const u8*)s2;
    u32 i;

    for (i = 0; i < n; i++) {
        if (u1[i] == u2[i]) {
            continue;
        }

        if (u2[i] < u1[i])
            return 1;
        return -1;
    }

    return 0;
}

void* memset(void* dst, s32 c, u32 n) {
    u8* p;
    u8* end;

    for (p = (u8*)dst, end = p + n; p < end; p++) {
        *p = c;
    }

    return dst;
}

INCLUDE_ASM("asm/nonmatchings/libc", strcmp);

char* strcpy(char* dst, const char* src) {
    s32 i = 0;
    if (*src != 0) {
        do {
            dst[i++] = *src++;
        } while (*src != 0);
    }
    dst[i] = 0;
    return dst;
}

s32 rand(void) {
    s32 next = (D_80064460 * 0x343FD) + 0x269EC3;
    D_80064460 = next;
    return (next >> 16) & 0x7FFF;
}

void srand(s32 seed) {
    D_80064460 = seed;
}

INCLUDE_ASM("asm/nonmatchings/libc", atoi);

INCLUDE_ASM("asm/nonmatchings/libc", atof);

char* strcat(char* dst, const char* src) {
    s32 dst_len = 0;
    s32 i;

    if (*dst != 0) {
        do {
            dst_len++;
        } while (dst[dst_len] != 0);
    }
    i = 0;
    if (*src != 0) {
        do {
            dst[dst_len + i++] = *src++;
        } while (*src != 0);
    }
    dst[dst_len + i] = 0;
    return dst;
}

INCLUDE_ASM("asm/nonmatchings/libc", stricmp);

INCLUDE_ASM("asm/nonmatchings/libc", strncat);

int strncmp(const char* str1, const char* str2, u32 num) {
    u32 i;
    u8 temp_a3;
    u8 temp_v1;

    for (i = 0; i < num; i++) {
        temp_a3 = (u8)str1[i];
        temp_v1 = (u8)str2[i];

        if (temp_a3 != temp_v1) {
            if (temp_v1 < temp_a3) {
                return 1;
            } else {
                return -1;
            }
        }
        if (temp_a3 == 0) {
            return 0;
        }
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/libc", strncpy);

INCLUDE_ASM("asm/nonmatchings/libc", strnicmp);

INCLUDE_ASM("asm/nonmatchings/libc", strrchr);

INCLUDE_ASM("asm/nonmatchings/libc", strstr);

char* strupr(char* str) {
    char temp_a1;
    char* var_v1;

    for (var_v1 = str; *var_v1 != 0; var_v1++) {
        temp_a1 = *var_v1;
        if ((temp_a1 >= 'a') && (temp_a1 <= 'z')) {
            *var_v1 = temp_a1 + ('A' - 'a');
        }
    }

    return str;
}

s32 tolower(s32 c) {
    if ((u32)(c - 'A') < 26) {
        return c + 0x20;
    }
    return c;
}

s32 toupper(s32 c) {
    if ((u32)(c - 'a') < 26) {
        return c - 0x20;
    }
    return c;
}
void swap(void* a, void* b, s32 width);

void shortsort(void* lo, void* hi, s32 width, s32 (*comp)(void*, void*)) {
    void* p;
    void* max;
    void* cur;

    while (lo < hi) {
        cur = lo + width;
        max = lo;
        if (hi >= cur) {
            p = cur;
            do {
                if (comp(p, max) > 0) {
                    max = cur;
                }
                cur += width;
                p = cur;
            } while (hi >= cur);
        }
        swap(max, hi, width);
        hi -= width;
    }
}

void swap(void* a, void* b, s32 width) {
    u8* pa = (u8*)a;
    u8* pb = (u8*)b;
    s32 i = width - 1;

    if (pa == pb) {
        return;
    }

    while (i != -1) {
        u8 tmp = *pa;
        i--;
        *pa++ = *pb;
        *pb++ = tmp;
    }
}

INCLUDE_ASM("asm/nonmatchings/libc", func_80006A4C);
