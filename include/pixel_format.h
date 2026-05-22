#ifndef PIXEL_FORMAT_H
#define PIXEL_FORMAT_H

#include "common.h"

class PixelFormat {
  public:
    /* 0x00 */ u32 maskRed;
    /* 0x04 */ u32 maskGreen;
    /* 0x08 */ u32 maskBlue;
    /* 0x0C */ u32 maskAlpha;
    /* 0x10 */ u32 unk10;
    /* 0x14 */ u16 paletteMask;
    /* 0x16 */ u16 bitDepth;

    PixelFormat()
        : maskRed(0), maskGreen(0), maskBlue(0), maskAlpha(0),
          unk10(0), paletteMask(0), bitDepth(0) {
    }

    u32 packColor(const u8* rgba);
    void unpackColor(u32 pixel, u8* rgba);

    s32 ctzPaletteMask();
    s32 ctzUnk10();
    s32 ctzMaskAlpha();
    s32 ctzMaskBlue();
    s32 ctzMaskGreen();
    s32 ctzMaskRed();

    s32 bitWidthPaletteMask();
    s32 bitWidthUnk10();
    s32 bitWidthMaskAlpha();
    s32 bitWidthMaskBlue();
    s32 bitWidthMaskGreen();
    s32 bitWidthMaskRed();
};

#endif
