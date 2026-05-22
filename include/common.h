#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h" // IWYU pragma: export
#include "os.h" // IWYU pragma: export

#ifndef NULL
#define NULL 0
#endif

#ifdef HAVE_CXX_OVERRIDE
#define CXX_OVERRIDE override
#else
#define CXX_OVERRIDE
#endif

typedef s32 UNK;

#define ALIGN_UP(x, n) (((u32)(x) + ((n) - 1)) & ~((n) - 1))

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

int memcmp(const void*, const void*, unsigned);
void* memcpy(void*, const void*, unsigned);
void* memset(void*, int, unsigned);

void __assert(const char* msg, const char* file, s32 line, const char* prettyMsg);

s32 lzss_decompress(u8* src, u8* dest);

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#define TEXT_PAD(size) \
    __asm__(".section .text\n" \
            "    .set noat\n" \
            "    .set noreorder\n" \
            "    .rept " #size " / 4\n" \
            "    nop\n" \
            "    .endr\n" \
            "    .set reorder\n" \
            "    .set at\n")
#define DATA_PAD(size) static char pad[size] = { 0 }

#endif
