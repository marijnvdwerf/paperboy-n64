#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"

#ifndef NULL
#define NULL 0
#endif

#define TEXT_PAD(size)                                                                                                  \
    __asm__(".section .text\n"                                                                                         \
            "    .set noat\n"                                                                                          \
            "    .set noreorder\n"                                                                                     \
            "    .rept " #size " / 4\n"                                                                                \
            "    nop\n"                                                                                                \
            "    .endr\n"                                                                                              \
            "    .set reorder\n"                                                                                       \
            "    .set at\n")
#define DATA_PAD(size) static char pad[size] = {0}

#endif
