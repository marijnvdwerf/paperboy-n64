#include "pixel_format.h"

static inline s32 ctzOf(u32 mask) {
    u32 m;
    if (mask == 0) {
        return 32;
    }
    m = mask;
    s32 count = 0;
    while (!(m & 1)) {
        m >>= 1;
        count++;
    }
    return count;
}

static inline s32 bitWidthOf(u32 mask) {
    u32 m;
    if (mask == 0) {
        return 0;
    }
    m = mask;
    while (!(m & 1)) {
        m >>= 1;
    }
    s32 count = 0;
    while (m & 1) {
        m >>= 1;
        count++;
    }
    return count;
}

u32 PixelFormat::packColor(const u8* rgba) {
    u32 rShift = 8 - bitWidthOf(this->maskRed);
    u32 gShift = 8 - bitWidthOf(this->maskGreen);
    u32 bShift = 8 - bitWidthOf(this->maskBlue);
    u32 aShift = 8 - bitWidthOf(this->maskAlpha);
    u32 r = (u32)rgba[0] >> rShift;
    u32 g = (u32)rgba[1] >> gShift;
    u32 b = (u32)rgba[2] >> bShift;
    u32 a = (u32)rgba[3] >> aShift;
    u32 result;
    result = r << ctzOf(this->maskRed);
    result |= g << ctzOf(this->maskGreen);
    result |= b << ctzOf(this->maskBlue);
    result |= a << ctzOf(this->maskAlpha);
    return result;
}

void PixelFormat::unpackColor(u32 pixel, u8* rgba) {
    u32 r = (pixel & this->maskRed) >> ctzOf(this->maskRed);
    u32 g = (pixel & this->maskGreen) >> ctzOf(this->maskGreen);
    u32 b = (pixel & this->maskBlue) >> ctzOf(this->maskBlue);
    u32 a = (pixel & this->maskAlpha) >> ctzOf(this->maskAlpha);
    rgba[0] = r << (8 - bitWidthOf(this->maskRed));
    rgba[1] = g << (8 - bitWidthOf(this->maskGreen));
    rgba[2] = b << (8 - bitWidthOf(this->maskBlue));
    rgba[3] = a << (8 - bitWidthOf(this->maskAlpha));
}

s32 PixelFormat::ctzPaletteMask() {
    u32 mask;
    if (this->paletteMask == 0) {
        return 32;
    }
    mask = this->paletteMask;
    s32 count = 0;
    while (!(mask & 1)) {
        mask >>= 1;
        count++;
    }
    return count;
}

s32 PixelFormat::ctzUnk10() {
    return ctzOf(this->unk10);
}

s32 PixelFormat::ctzMaskAlpha() {
    return ctzOf(this->maskAlpha);
}

s32 PixelFormat::ctzMaskBlue() {
    return ctzOf(this->maskBlue);
}

s32 PixelFormat::ctzMaskGreen() {
    return ctzOf(this->maskGreen);
}

s32 PixelFormat::ctzMaskRed() {
    return ctzOf(this->maskRed);
}

s32 PixelFormat::bitWidthPaletteMask() {
    u32 mask;
    if (this->paletteMask == 0) {
        return 0;
    }
    mask = this->paletteMask;
    while (!(mask & 1)) {
        mask >>= 1;
    }
    s32 count = 0;
    while (mask & 1) {
        mask >>= 1;
        count++;
    }
    return count;
}

s32 PixelFormat::bitWidthUnk10() {
    return bitWidthOf(this->unk10);
}

s32 PixelFormat::bitWidthMaskAlpha() {
    return bitWidthOf(this->maskAlpha);
}

s32 PixelFormat::bitWidthMaskBlue() {
    return bitWidthOf(this->maskBlue);
}

s32 PixelFormat::bitWidthMaskGreen() {
    return bitWidthOf(this->maskGreen);
}

s32 PixelFormat::bitWidthMaskRed() {
    return bitWidthOf(this->maskRed);
}
