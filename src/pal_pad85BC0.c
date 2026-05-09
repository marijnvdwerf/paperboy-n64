#include "common.h"

#ifdef PAL
__asm__(".section .text\n"
        "    .set noat\n"
        "    .set noreorder\n"
        "          .rept 0x50 / 4\n"
        "          nop\n"
        "          .endr\n"
        "    .set reorder\n"
        "    .set at\n");
#endif