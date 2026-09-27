#ifndef GOL_BMP_FILE_H
#define GOL_BMP_FILE_H

#include "gol_img_file.h"

struct GolSurface;
class GolPaletteBase;
struct TiledSurface;

struct GolBmpFile : public GolImgFile {
    /* 0x5B0 */ char pathBuf[0x40];
    /* 0x5F0 */ u8 unk5F0[0x300];
    /* 0x8F0 */ u8 unk8F0[0x5DC];
    /* 0xECC */ s32 unkECC;
    /* 0xED0 */ s32 unkED0;
    /* 0xED4 */ s32 unkED4;

    GolBmpFile();
    void vfunc1() CXX_OVERRIDE;
    void vfunc3(const char* filename) CXX_OVERRIDE;
    const char* vfunc5() CXX_OVERRIDE;
    void vfunc6(GolSurface* dstSurface, s32 flipFlag, u8* transColor) CXX_OVERRIDE;
    void vfunc7(TiledSurface* dstSurface, s32 flipFlag, u8* transColor) CXX_OVERRIDE;
    void vfunc8(u8* dst) CXX_OVERRIDE;
    void vfunc9(u8* srcBuffer, GolSurface* dstSurface, s32 flipFlag, u8* transColor) CXX_OVERRIDE;
};

#endif
