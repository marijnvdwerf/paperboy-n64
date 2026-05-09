#include "common.h"

#ifndef PAL
INCLUDE_ASM("asm/nonmatchings/88330", func_800BFF50);

INCLUDE_ASM("asm/nonmatchings/88330", func_800C01EC);
#else
__asm__(".section .text\n"
        "    .set noat\n"
        "    .set noreorder\n"
        "          .rept 0x2D0 / 4\n"
        "          nop\n"
        "          .endr\n"
        "    .set reorder\n"
        "    .set at\n");
#endif