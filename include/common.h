#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"

#ifndef NULL
#define NULL 0
#endif

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

#define TEXT_PAD(size)                  \
    __asm__(".section .text\n"          \
            "    .set noat\n"           \
            "    .set noreorder\n"      \
            "    .rept " #size " / 4\n" \
            "    nop\n"                 \
            "    .endr\n"               \
            "    .set reorder\n"        \
            "    .set at\n")
#define DATA_PAD(size) static char pad[size] = { 0 }

#endif
