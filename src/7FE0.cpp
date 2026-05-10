#include "common.h"

extern "C" {
s32 toupper(s32);
}

extern "C" void func_800073E0(u8* src, s8* dest, s32 len) {
    u8* buf;
    u8* end;
    u8 c;

    for (buf = src, end = src + len; buf < end; buf++) {
        c = *buf;
        if (c == '\0') {
            if (buf != src) {
                *(dest++) = '\0';
                return;
            } else {
                *(dest++) = ' ';
                continue;
            }
        } else if ((u8)(c - 0x1A) < 0x1A) {
            *(dest++) = c + 0x27;
        } else if ((u8)(c - 0x10) < 0xA) {
            *(dest++) = c + 0x20;
        } else if (c == 0xF) {
            *(dest++) = ' ';
        } else if ((u8)(c - 0x38) < 6) {
            *(dest++) = c + 0xF2;
        } else if ((u8)(c - 0x34) < 3) {
            *(dest++) = c + 0xED;
        } else if (c == 0x37) {
            *(dest++) = '\'';
        } else if (c == 0x3E) {
            *(dest++) = ':';
        } else if (c == 0x3F) {
            *(dest++) = '=';
        } else if (c == 0x40) {
            *(dest++) = '?';
        } else if (c == 0x41) {
            *(dest++) = '@';
        } else {
            *(dest++) = '*';
        }
    }
}

extern "C" void func_8000752C(u8* src, s8* dest, s32 len) {
    u8* end;
    u8 c;

    for (end = src + len; src < end; src++) {
        if (*src == '\0') {
            *dest = '\0';
            return;
        }
        c = toupper(*src);
        if ((u8)(c - 0x41) < 0x1A) {
            *(dest++) = c + 0xD9;
        } else if ((u8)(c - 0x30) < 0xA) {
            *(dest++) = c + 0xE0;
        } else if (c == 0x20) {
            *(dest++) = 0xF;
        } else if ((u8)(c - 0x2A) < 6) {
            *(dest++) = c + 0xE;
        } else if ((u8)(c - 0x21) < 3) {
            *(dest++) = c + 0x13;
        } else if (c == 0x27) {
            *(dest++) = 0x37;
        } else if (c == 0x3A) {
            *(dest++) = 0x3E;
        } else if (c == 0x3D) {
            *(dest++) = 0x3F;
        } else if (c == 0x3F) {
            *(dest++) = 0x40;
        } else if (c == 0x40) {
            *(dest++) = 0x41;
        } else {
            *(dest++) = 0x38;
        }
    }
}
