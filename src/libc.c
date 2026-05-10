#include "common.h"

extern s32 D_80064460;

INCLUDE_ASM("asm/nonmatchings/libc", qsort);

INCLUDE_ASM("asm/nonmatchings/libc", strtok);

INCLUDE_ASM("asm/nonmatchings/libc", memcmp);

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

INCLUDE_ASM("asm/nonmatchings/libc", strncmp);

INCLUDE_ASM("asm/nonmatchings/libc", func_80006730);

INCLUDE_ASM("asm/nonmatchings/libc", strnicmp);

INCLUDE_ASM("asm/nonmatchings/libc", strrchr);

INCLUDE_ASM("asm/nonmatchings/libc", strstr);

INCLUDE_ASM("asm/nonmatchings/libc", strupr);

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

INCLUDE_ASM("asm/nonmatchings/libc", func_80006960);

INCLUDE_ASM("asm/nonmatchings/libc", func_80006A10);

INCLUDE_ASM("asm/nonmatchings/libc", func_80006A4C);

INCLUDE_ASM("asm/nonmatchings/libc", func_80006A58);
