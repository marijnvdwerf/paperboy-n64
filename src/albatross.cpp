#include "albatross.h"
#include "surfaces.h"

extern "C" char* strcpy(char*, const char*);
extern "C" char* strcat(char*, const char*);
extern "C" unsigned strlen(const char*);
extern "C" int sprintf(char*, const char*, ...);

#pragma clang diagnostic ignored "-Wc++11-compat-deprecated-writable-strings"
char* D_8006F610 = ".bmp";

// A grid of Surface16970 tiles; vfunc8(row, col) returns one cell.
struct TiledSurface {
    /* 0x00 */ s32* tileWidths;
    /* 0x04 */ s32* tileHeights;
    /* 0x08 */ u8 unk8[0x20];
    /* 0x28 */ u32 rowCount;
    /* 0x2C */ u32 colCount;
    /* 0x30 */ s32 totalWidth;
    /* 0x34 */ s32 totalHeight;
    /* 0x38 */ s32 flags;
    /* 0x3C */ u8 unk3C[0x10];

    virtual ~TiledSurface() = 0;
    virtual void vfunc2() = 0;
    virtual void vfunc3() = 0;
    virtual void vfunc4() = 0;
    virtual void vfunc5() = 0;
    virtual void vfunc6() = 0;
    virtual void vfunc7() = 0;
    virtual Surface16970* vfunc8(s32 row, s32 col) = 0;
};

static inline void tileDims(TiledSurface* ts, u32 row, u32 col, s32* w, s32* h) {
    *w = ts->tileWidths[row];
    *h = ts->tileHeights[col];
}

#pragma pack(push, 1)

struct BmpHeader {
    /* 0x00 */ u16 sig;
    /* 0x02 */ u16 pad;
    /* 0x04 */ u32 fileSize;
    /* 0x08 */ u16 reserved1;
    /* 0x0A */ u16 reserved2;
    /* 0x0C */ u32 dataOffset;
    /* 0x10 */ u8 fileHdr[16];
    /* 0x20 */ u8 dibHdr[0x40];
};

#pragma pack(pop)

void Albatross::vfunc1() {
    BmpHeader bmp;

    s32 dummy;

    s32 compression = 0;
    u32 bitsPerPixel = 0;
    u32 planes;
    u32 shift;
    s32 fileOffset = 0;
    u8* palBuf;
    u8* p;
    u32 i;

    if (file.readAt(0, bmp.fileHdr, 0xE, &dummy) != 0) {
        __assert("", NULL, 0, NULL);
    }

    p = bmp.fileHdr;
    bmp.sig = p[0] + (p[1] << 8);
    if (bmp.sig != 0x4D42) {
        bitsPerPixel = p[0] & 0x3C;
        if (bitsPerPixel != 4 && bitsPerPixel != 8 && bitsPerPixel != 0x18 && bitsPerPixel != 0x20) {
            __assert("", NULL, 0, NULL);
        }
        if (bitsPerPixel >= 9) {
            this->unk68 = 0;
        } else if (p[0] & 0x80) {
            this->unk68 = 0;
        } else {
            this->unk68 = p[1] + 1;
        }
        p += 2;
        this->unk40 = p[0] + (p[1] << 8);
        p += 2;
        fileOffset += 6;
        this->unk44 = p[0] + (p[1] << 8);

        if (bitsPerPixel < 9) {
            i = this->unk68 * 3;
            palBuf = (i <= sizeof(this->unk5F0)) ? this->unk5F0 : new u8[i];
            if (palBuf == NULL) {
                __assert("", NULL, 0, NULL);
            }
            if (this->file.readAt(fileOffset, palBuf, i, &dummy) != 0) {
                __assert("", NULL, 0, NULL);
            }
            fileOffset += i;
            p = palBuf;
            for (i = 0; i < this->unk68; i++) {
                this->palette[i].b = *p++;
                this->palette[i].g = *p++;
                this->palette[i].r = *p++;
                this->palette[i].a = 0xFF;
            }
            if (palBuf != this->unk5F0 && palBuf != NULL) {
                delete[] palBuf;
            }
        }

        u32 stride = (((this->unk40 * bitsPerPixel) + 0x1F) & ~0x1F) >> 3;
        this->unkECC = fileOffset;
        this->unk48 = stride;
        this->unkED4 = 0x80;
        this->unkED0 = stride * this->unk44;
    } else {
        p += 2;
        bmp.fileSize = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
        p += 8;
        bmp.reserved1 = bmp.fileHdr[6] + (bmp.fileHdr[7] << 8);
        bmp.reserved2 = bmp.fileHdr[8] + (bmp.fileHdr[9] << 8);
        bmp.dataOffset = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
        this->unkECC = bmp.dataOffset;

        if (this->file.readAt(0xE, bmp.dibHdr, 0x40, &dummy) != 0) {
            __assert("", NULL, 0, NULL);
        }

        p = bmp.dibHdr;
        u32 biSize = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
        fileOffset = biSize + 0xE;
        p += 4;

        if (biSize == 0xC) {
            // BITMAPCOREHEADER
            this->unk40 = p[0] + (p[1] << 8);
            this->unk44 = bmp.dibHdr[6] + (bmp.dibHdr[7] << 8);
            u32 planesHi = bmp.dibHdr[9];
            u32 bitsHi = bmp.dibHdr[11];
            this->unk68 = 0;
            planes = bmp.dibHdr[8] + (planesHi << 8);
            bitsPerPixel = bmp.dibHdr[10] + (bitsHi << 8);
            shift = planes * bitsPerPixel;

            if (bitsPerPixel < 9) {
                u32 palCount = 1 << shift;
                this->unk68 = palCount;
                if (palCount >= 0x101) {
                    __assert("", NULL, 0, NULL);
                }
                u32 palSize = this->unk68 * 3;
                palBuf = (palSize <= sizeof(this->unk5F0)) ? this->unk5F0 : new u8[palSize];
                if (palBuf == NULL) {
                    __assert("", NULL, 0, NULL);
                }
                if (this->file.readAt(fileOffset, palBuf, this->unk68 * 3, &dummy) != 0) {
                    __assert("", NULL, 0, NULL);
                }
                p = palBuf;
                for (i = 0; i < this->unk68; i++) {
                    this->palette[i].b = *p++;
                    this->palette[i].g = *p++;
                    this->palette[i].r = *p++;
                    this->palette[i].a = 0xFF;
                }
                if (palBuf != this->unk5F0 && palBuf != NULL) {
                    delete[] palBuf;
                }
                u32 palCnt = this->unk68;
                if (palCnt < 0x100) {
                    memset(this->palette + palCnt, 0, (0x100 - palCnt) * 4);
                }
            }

            u32 stride = (((this->unk40 * bitsPerPixel) + 0x1F) & ~0x1F) >> 3;
            this->unk48 = stride;
            this->unkED0 = stride * this->unk44;
        } else if (biSize == 0x28) {
            // BITMAPINFOHEADER
            this->unk40 = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
            p += 4;
            this->unk44 = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
            p += 8;
            planes = bmp.dibHdr[12] + (bmp.dibHdr[13] << 8);
            bitsPerPixel = bmp.dibHdr[14] + (bmp.dibHdr[15] << 8);
            compression = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
            p += 4;
            this->unkED0 = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
            p += 12;
            u32 paletteCount = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
            this->unk68 = paletteCount;

            if (bitsPerPixel < 9) {
                shift = planes * bitsPerPixel;
                if (paletteCount == 0) {
                    this->unk68 = 1 << shift;
                }
                u32 palSize = this->unk68 * 4;
                palBuf = (palSize <= sizeof(this->unk5F0)) ? this->unk5F0 : new u8[palSize];
                if (palBuf == NULL) {
                    __assert("", NULL, 0, NULL);
                }
                if (this->file.readAt(fileOffset, palBuf, this->unk68 * 4, &dummy) != 0) {
                    __assert("", NULL, 0, NULL);
                }
                p = palBuf;
                for (i = 0; i < this->unk68; i++) {
                    this->palette[i].b = *p++;
                    this->palette[i].g = *p++;
                    this->palette[i].r = *p;
                    p += 2;
                    this->palette[i].a = 0xFF;
                }
                if (palBuf != this->unk5F0 && palBuf != NULL) {
                    delete[] palBuf;
                }
                u32 palCnt = this->unk68;
                if (palCnt < 0x100) {
                    memset(this->palette + palCnt, 0, (0x100 - palCnt) * 4);
                }
            }

            this->unk48 = (((this->unk40 * bitsPerPixel) + 0x1F) & ~0x1F) >> 3;
        } else if (biSize == 0x40) {
            // OS/2 v2 BMP
            this->unk40 = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
            p += 4;
            this->unk44 = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
            p += 8;
            planes = bmp.dibHdr[12] + (bmp.dibHdr[13] << 8);
            bitsPerPixel = bmp.dibHdr[14] + (bmp.dibHdr[15] << 8);
            compression = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
            p += 4;
            this->unkED0 = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
            p += 12;
            u32 paletteCount = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
            this->unk68 = paletteCount;

            if (bitsPerPixel < 9) {
                shift = planes * bitsPerPixel;
                if (paletteCount == 0) {
                    this->unk68 = 1 << shift;
                }
                // NOTE: allocates unk68*3 but reads unk68*4 (original code bug)
                u32 palSize = this->unk68 * 3;
                palBuf = (palSize <= sizeof(this->unk5F0)) ? this->unk5F0 : new u8[palSize];
                if (palBuf == NULL) {
                    __assert("", NULL, 0, NULL);
                }
                if (this->file.readAt(fileOffset, palBuf, this->unk68 * 4, &dummy) != 0) {
                    __assert("", NULL, 0, NULL);
                }
                p = palBuf;
                for (i = 0; i < this->unk68; i++) {
                    this->palette[i].b = *p++;
                    this->palette[i].g = *p++;
                    this->palette[i].r = *p;
                    p += 2;
                    this->palette[i].a = 0xFF;
                }
                if (palBuf != this->unk5F0 && palBuf != NULL) {
                    delete[] palBuf;
                }
                u32 palCnt = this->unk68;
                if (palCnt < 0x100) {
                    memset(this->palette + palCnt, 0, (0x100 - palCnt) * 4);
                }
            }

            this->unk48 = (((this->unk40 * bitsPerPixel) + 0x1F) & ~0x1F) >> 3;
        }

        this->unkED4 = compression;
        if (compression != 0) {
            __assert("", NULL, 0, NULL);
        }
    }
    switch (bitsPerPixel) {
        case 4: {
            PixelFormat* pf = &this->pixelFormat;
            pf->paletteMask = 0xF;
            pf->bitDepth = 4;
            break;
        }
        case 8: {
            PixelFormat* pf = &this->pixelFormat;
            pf->paletteMask = 0xFF;
            pf->bitDepth = 8;
            break;
        }
        case 16: {
            PixelFormat* pf = &this->pixelFormat;
            pf->maskRed = 0x7C00;
            pf->maskGreen = 0x3E0;
            pf->maskBlue = 0x1F;
            pf->bitDepth = 0x10;
            break;
        }
        case 24: {
            PixelFormat* pf = &this->pixelFormat;
            pf->maskRed = 0xFF0000;
            pf->maskGreen = 0xFF00;
            pf->maskBlue = 0xFF;
            pf->bitDepth = 0x18;
            break;
        }
        case 32: {
            PixelFormat* pf = &this->pixelFormat;
            pf->maskRed = 0xFF0000;
            pf->maskGreen = 0xFF00;
            pf->maskBlue = 0xFF;
            pf->maskAlpha = 0xFF000000;
            pf->bitDepth = 0x20;
            break;
        }
        default:
            __assert("", NULL, 0, NULL);
            break;
    }
}

void Albatross::vfunc6(Surface16970* dstSurface, s32 flipFlag, u8* transColor) {
    s32 carryLen;
    PixelFormat localPf;
    s32 dummy;
    s32 stride;
    u8* dstPtr;
    u32 row;
    s32 step;
    s32 scaleX = 1;
    s32 scaleY = 1;

    s32 oob = (dstSurface->unk28 < this->unk44 || dstSurface->unk26 < this->unk40);
    if (oob) {
        __assert("", NULL, 0, NULL);
    }

    s32 needScale = (this->unk44 != dstSurface->unk28 || this->unk40 != dstSurface->unk26);
    if (needScale) {
        scaleX = dstSurface->unk26 / this->unk40;
        scaleY = dstSurface->unk28 / this->unk44;
    }

    s32 totalRows = this->unk44 * scaleY;
    localPf = dstSurface->hdr;

    PixelFormat* pf = &localPf;
    this->func_8001B794(pf, transColor);
    if (pf->paletteMask != 0) {
        if (this->unk68 != 0) {
            this->func_8001B230(dstSurface->vfunc8(), transColor);
        }
    }

    dstSurface->vfunc2(&dstPtr, &stride, 3);
    step = stride;
    s32 fileOff = this->unkECC;

    if (this->unkED4 == 0x80) {
        u8* carryover = NULL;
        if (flipFlag != 0) {
            dstPtr += (totalRows - 1) * stride;
            step = -stride;
        }

        u8* rawBuf = this->unk8F0;
        if (rawBuf == NULL) {
            __assert("", NULL, 0, NULL);
        }

        u32 bufSize = this->unk48 + 2;
        u8* tmpBuf = (bufSize <= sizeof(this->unk5F0)) ? this->unk5F0 : new u8[bufSize];
        if (tmpBuf == NULL) {
            __assert("", NULL, 0, NULL);
        }

        s32 scanSize = this->pixelFormat.bitDepth;
        if (scanSize == 4) {
            scanSize = (this->unk40 + 1) * 4;
        } else {
            scanSize = scanSize * this->unk40;
        }
        scanSize >>= 3;

        carryLen = 0;
        row = 0;

        while (row < this->unk44) {
            if (carryLen < scanSize) {
                s32 filled;
                if (carryLen != 0) {
                    memcpy(tmpBuf, carryover, carryLen);
                    filled = carryLen;
                } else {
                    filled = 0;
                }
                if (filled < scanSize) {
                    do {
                        if (this->file.readAt(fileOff, rawBuf, 4, &dummy) != 0) {
                            dstSurface->vfunc3();
                            __assert("", NULL, 0, NULL);
                        }
                        carryLen = rawBuf[0] + (rawBuf[1] << 8);
                        s32 compSize = rawBuf[2] + (rawBuf[3] << 8);
                        fileOff += 4;
                        if (compSize < carryLen) {
                            carryover = &rawBuf[sizeof(this->unk8F0) - compSize];
                            if (this->file.readAt(fileOff, carryover, compSize, &dummy) != 0) {
                                dstSurface->vfunc3();
                                __assert("", NULL, 0, NULL);
                            }
                            lzss_decompress(carryover, rawBuf);
                        } else {
                            if (this->file.readAt(fileOff, rawBuf, carryLen, &dummy) != 0) {
                                dstSurface->vfunc3();
                                __assert("", NULL, 0, NULL);
                            }
                        }
                        fileOff += compSize;
                        s32 copyLen = scanSize - filled;
                        carryover = rawBuf;
                        if (carryLen < copyLen) {
                            copyLen = carryLen;
                        }
                        memcpy(tmpBuf + filled, carryover, copyLen);
                        carryover += copyLen;
                        filled += copyLen;
                        carryLen -= copyLen;
                    } while (filled < scanSize);
                }
            } else {
                memcpy(tmpBuf, carryover, scanSize);
                carryover += scanSize;
                carryLen -= scanSize;
            }

            this->func_8001BAE0(tmpBuf, dstPtr, &localPf);

            if (scaleX >= 2) {
                this->func_8001D500(dstPtr, scaleX, dstSurface->unk26, localPf.bitDepth);
            }

            if (scaleY >= 2) {
                for (s32 i = 1; i < scaleY; i++) {
                    memcpy(dstPtr + step, dstPtr, stride);
                    dstPtr += step;
                }
            }

            row++;
            dstPtr += step;
        }

        if (tmpBuf != this->unk5F0 && tmpBuf != NULL) {
            delete[] tmpBuf;
        }
    }

    dstSurface->vfunc3();
}

void Albatross::vfunc9(u8* srcBuffer, Surface16970* dstSurface, s32 flipFlag, u8* transColor) {
    s32 stride;
    u8* dstPtr;
    PixelFormat localPf;
    s32 scaleX = 1;
    s32 scaleY = 1;

    s32 oob = (dstSurface->unk28 < this->unk44 || dstSurface->unk26 < this->unk40);
    if (oob) {
        __assert("", NULL, 0, NULL);
    }

    s32 needScale = (this->unk44 != dstSurface->unk28 || this->unk40 != dstSurface->unk26);
    if (needScale) {
        scaleX = dstSurface->unk26 / this->unk40;
        scaleY = dstSurface->unk28 / this->unk44;
    }

    s32 totalRows = this->unk44 * scaleY;

    localPf = dstSurface->hdr;

    PixelFormat* pfPtr = &localPf;

    this->func_8001B794(pfPtr, transColor);

    if (pfPtr->paletteMask != 0) {
        if (this->unk68 != 0) {
            this->func_8001B230(dstSurface->vfunc8(), transColor);
        }
    }

    dstSurface->vfunc2(&dstPtr, &stride, 3);
    s32 step = stride;

    if (flipFlag == 0) {
        dstPtr += (totalRows - 1) * step;
        step = -step;
    }
    u32 row = 0;

    while (row < this->unk44) {
        this->func_8001BAE0(srcBuffer, dstPtr, &localPf);

        if (scaleX >= 2) {
            u32 bpp = localPf.bitDepth;
            switch (bpp) {
                case 8: {
                    u8* dst8 = dstPtr + dstSurface->unk26 - 1;
                    u8* src8 = dstPtr + this->unk40 - 1;
                    s32 srcWidth = this->unk40;
                    while (srcWidth > 0) {
                        for (s32 copies = scaleX; copies > 0; copies--) {
                            *dst8-- = *src8;
                        }
                        src8--;
                        srcWidth--;
                    }
                    break;
                }
                case 15:
                case 16: {
                    u16* dst16 = (u16*)dstPtr + dstSurface->unk26 - 1;
                    u16* src16 = (u16*)dstPtr + this->unk40 - 1;
                    s32 srcWidth = this->unk40;
                    while (srcWidth > 0) {
                        for (s32 copies = scaleX; copies > 0; copies--) {
                            *dst16-- = *src16;
                        }
                        src16--;
                        srcWidth--;
                    }
                    break;
                }
            }
        }

        if (scaleY >= 2) {
            for (s32 y = 1; y < scaleY; y++) {
                memcpy(dstPtr + step, dstPtr, stride);
                dstPtr += step;
            }
        }

        row++;
        srcBuffer += this->unk48;
        dstPtr += step;
    }

    dstSurface->vfunc3();
}

void Albatross::vfunc8(u8* dst) {
    s32 scanSize;
    s32 fileOff = this->unkECC;
    u32 row;
    u8* dstPos;
    s32 dummy;

    if (this->unkED4 != 0x80) {
        return;
    }

    u8* carryover = NULL;
    u8* tmpBuf = this->unk8F0;
    dstPos = dst + this->unk44 * this->unk48;
    if (tmpBuf == NULL) {
        __assert("", NULL, 0, NULL);
    }

    scanSize = this->pixelFormat.bitDepth;
    if (scanSize == 4) {
        scanSize = (this->unk40 + 1) * 4;
    } else {
        scanSize = scanSize * this->unk40;
    }
    scanSize >>= 3;

    s32 carryLen = 0;
    row = 0;
    if (this->unk44 != 0) {
        do {
            dstPos -= this->unk48;
            if (carryLen < scanSize) {
                s32 filled;
                if (carryLen != 0) {
                    memcpy(dstPos, carryover, carryLen);
                    filled = carryLen;
                } else {
                    filled = 0;
                }
                if (filled < scanSize) {
                    do {
                        if (this->file.readAt(fileOff, tmpBuf, 4, &dummy) != 0) {
                            __assert("", NULL, 0, NULL);
                        }
                        carryLen = tmpBuf[0] + (tmpBuf[1] << 8);
                        s32 compSize = tmpBuf[2] + (tmpBuf[3] << 8);
                        fileOff += 4;
                        if (compSize < carryLen) {
                            carryover = &tmpBuf[sizeof(this->unk8F0) - compSize];
                            if (this->file.readAt(fileOff, carryover, compSize, &dummy) != 0) {
                                __assert("", NULL, 0, NULL);
                            }
                            lzss_decompress(carryover, tmpBuf);
                        } else {
                            if (this->file.readAt(fileOff, tmpBuf, carryLen, &dummy) != 0) {
                                __assert("", NULL, 0, NULL);
                            }
                        }
                        fileOff += compSize;
                        s32 copyLen = scanSize - filled;
                        carryover = tmpBuf;
                        if (carryLen < copyLen) {
                            copyLen = carryLen;
                        }
                        memcpy(dstPos + filled, carryover, copyLen);
                        carryover += copyLen;
                        filled += copyLen;
                        carryLen -= copyLen;
                    } while (filled < scanSize);
                }
            } else {
                memcpy(dstPos, carryover, scanSize);
                carryover += scanSize;
                carryLen -= scanSize;
            }
            row++;
        } while (row < this->unk44);
    }
}

void Albatross::vfunc7(TiledSurface* dstSurface, s32 flipFlag, u8* transColor) {
    u8* rawBuf = NULL;

    PixelFormat localPf;
    s32 lockPitch;
    s32 tileW;
    s32 tileH;
    s32 dummy;
    s32 rowWidth;
    s32 colH;
    u32 scanLine;
    s32 fileOff;
    s32* pitchArr;
    u8* tmpBuf;
    u8** ptrArr;
    u8* lineBuf;
    s32 scaleX;
    s32 rowPos;
    u32 colIdx;
    s32 step;
    s32* readOutPtr;
    s32 sentinel;

    u32 r;
    u32 c;

    rowPos = 0;
    colIdx = 0;
    step = 1;

    u32 dstDim;
    u32 srcDim;
    s32 needAssert = ((dstDim = dstSurface->totalHeight), (srcDim = this->unk44), srcDim != dstDim) ||
                     ((dstDim = dstSurface->totalWidth), (srcDim = this->unk40), srcDim != dstDim);
    if (needAssert) {
        __assert("", NULL, 0, NULL);
    }

    PixelFormat* pfPtr = &localPf;
    localPf = dstSurface->vfunc8(0, 0)->hdr;

    this->func_8001B794(pfPtr, transColor);
    scaleX = pfPtr->bitDepth >> 2;

    if (pfPtr->paletteMask != 0) {
        if (this->unk68 != 0) {
            for (r = 0; r < dstSurface->rowCount; r++) {
                for (c = 0; c < dstSurface->colCount; c++) {
                    Surface16970* tile = dstSurface->vfunc8(r, c);
                    this->func_8001B230(tile->vfunc8(), transColor);
                }
            }
        }
    }

    ptrArr = new u8*[dstSurface->rowCount * dstSurface->colCount];
    if (ptrArr == NULL) {
        __assert("", NULL, 0, NULL);
    }
    pitchArr = new s32[dstSurface->rowCount * dstSurface->colCount];
    if (pitchArr == NULL) {
        __assert("", NULL, 0, NULL);
    }

    u32 tileIdx = 0;
    r = 0;
    while (r < dstSurface->rowCount) {
        c = 0;
        s32* pitchPtr = (s32*)(tileIdx * 4 + (u32)pitchArr);
        u8** bufPtr = (u8**)(tileIdx * 4 + (u32)ptrArr);
        while (c < dstSurface->colCount) {
            Surface16970* tile = dstSurface->vfunc8(r, c);
            tileIdx++;
            c++;
            tile->vfunc2(bufPtr, &lockPitch, 3);
            bufPtr++;
            *pitchPtr = lockPitch;
            pitchPtr++;
        }
        r++;
    }

    fileOff = this->unkECC;

    if ((this->unkED4 != 0x80 && flipFlag == 0) ||
        (this->unkED4 == 0x80 && flipFlag != 0)) {
        step = -1;
        s32 totalH = 0;
        c = 0;
        colIdx = dstSurface->colCount - 1;
        for (; c < dstSurface->colCount; c++) {
            tileDims(dstSurface, 0, c, &tileW, &tileH);
            totalH += tileH;
        }
        rowPos = (dstSurface->totalHeight - (totalH - tileH)) - 1;
    }

    lineBuf = new u8[(this->unk48 + 2) * 4];
    if (lineBuf == NULL) {
        __assert("", NULL, 0, NULL);
    }

    if (this->unkED4 == 0x80) {
        rawBuf = this->unk8F0;
        u8* carryover = NULL;

        if (rawBuf == NULL) {
            __assert("", NULL, 0, NULL);
        }

        tmpBuf = new u8[this->unk48 + 2];
        if (tmpBuf == NULL) {
            __assert("", NULL, 0, NULL);
        }

        s32 scanSize = this->pixelFormat.bitDepth;
        if (scanSize == 4) {
            scanSize = (this->unk40 + 1) * 4;
        } else {
            scanSize = scanSize * this->unk40;
        }
        scanSize >>= 3;

        s32 carryLen = 0;
        scanLine = 0;
        readOutPtr = &dummy;
        sentinel = INT_MAX;

        while (scanLine < this->unk44) {
            if (carryLen < scanSize) {
                s32 filled;
                if (carryLen != 0) {
                    memcpy(tmpBuf, carryover, carryLen);
                    filled = carryLen;
                } else {
                    filled = 0;
                }
                while (filled < scanSize) {
                    if (this->file.readAt(fileOff, rawBuf, 4, readOutPtr) != 0) {
                        for (r = 0; r < dstSurface->rowCount; r++) {
                            for (c = 0; c < dstSurface->colCount; c++) {
                                Surface16970* tile = dstSurface->vfunc8(r, c);
                                tile->vfunc3();
                            }
                        }
                        __assert("", NULL, 0, NULL);
                    }
                    fileOff += 4;
                    carryLen = rawBuf[0] + (rawBuf[1] << 8);
                    s32 compSize = rawBuf[2] + (rawBuf[3] << 8);

                    if (compSize < carryLen) {
                        carryover = &rawBuf[sizeof(this->unk8F0) - compSize];
                        if (this->file.readAt(fileOff, carryover, compSize, readOutPtr) != 0) {
                            for (r = 0; r < dstSurface->rowCount; r++) {
                                for (c = 0; c < dstSurface->colCount; c++) {
                                    Surface16970* tile = dstSurface->vfunc8(r, c);
                                    tile->vfunc3();
                                }
                            }
                            __assert("", NULL, 0, NULL);
                        }
                        lzss_decompress(carryover, rawBuf);
                    } else {
                        if (this->file.readAt(fileOff, rawBuf, carryLen, readOutPtr) != 0) {
                            for (r = 0; r < dstSurface->rowCount; r++) {
                                for (c = 0; c < dstSurface->colCount; c++) {
                                    Surface16970* tile = dstSurface->vfunc8(r, c);
                                    tile->vfunc3();
                                }
                            }
                            __assert("", NULL, 0, NULL);
                        }
                    }

                    fileOff += compSize;
                    compSize = scanSize - filled;
                    if (carryLen < compSize) {
                        compSize = carryLen;
                    }

                    memcpy(tmpBuf + filled, rawBuf, compSize);
                    carryover = rawBuf + compSize;
                    carryLen -= compSize;
                    filled += compSize;
                }
            } else {
                memcpy(tmpBuf, carryover, scanSize);
                carryover += scanSize;
                carryLen -= scanSize;
            }

            this->func_8001BAE0(tmpBuf, lineBuf, &localPf);

            u32 xAccum = 0;
            u32 tileRow = 0;
            colH = 0;

            while (tileRow < dstSurface->rowCount) {
                tileDims(dstSurface, tileRow, colIdx, &rowWidth, &colH);

                if (rowPos == sentinel) {
                    rowPos = colH - 1;
                }

                u32 nPix = ((u32)(rowWidth * scaleX) + 1) >> 1;
                u32 nPixEnd = nPix + xAccum;
                u32 maxPix = ((u32)(this->unk40 * scaleX) + 1) >> 1;
                if (maxPix < nPixEnd) {
                    nPix = maxPix - xAccum;
                }

                u32 ti = tileRow * dstSurface->colCount + colIdx;
                u8* pixPtr = ptrArr[ti];
                s32 pitch = pitchArr[ti];
                memcpy(pixPtr + pitch * rowPos, lineBuf + xAccum, nPix);
                xAccum += nPix;
                tileRow++;
            }

            rowPos += step;
            if (rowPos == colH) {
                rowPos = 0;
                colIdx++;
            } else if (rowPos == -1) {
                rowPos = INT_MAX;
                colIdx--;
            }

            scanLine++;
        }

        if (tmpBuf != NULL) {
            delete[] tmpBuf;
        }
    }

    if (rawBuf != NULL && rawBuf != this->unk8F0) {
        delete[] rawBuf;
    }
    if (ptrArr != NULL) {
        delete[] ptrArr;
    }
    if (pitchArr != NULL) {
        delete[] pitchArr;
    }

    if (lineBuf != NULL) {
        delete[] lineBuf;
    }
    for (r = 0; r < dstSurface->rowCount; r++) {
        for (c = 0; c < dstSurface->colCount; c++) {
            Surface16970* tile = dstSurface->vfunc8(r, c);
            tile->vfunc3();
        }
    }

    s32 flags = dstSurface->flags & ~6;
    dstSurface->flags = flags | 8;
    if (!((flags | 8) & 0x30)) {
        dstSurface->flags = flags | 0x18;
    }
}

void Albatross::vfunc3(const char* filename) {
    this->unkECC = 0;
    this->unkED0 = 0;
    this->unkED4 = 0;
    u32 nameLen = strlen(filename);
    u32 extLen = strlen(D_8006F610);
    if ((extLen >= nameLen) || (memcmp(filename + (nameLen - extLen), D_8006F610, extLen) != 0)) {
        u32 totalLen = nameLen + extLen + 1;
        char* buf = (totalLen <= 0x40) ? this->pathBuf : new char[totalLen];
        if (buf == NULL) {
            __assert("", NULL, 0, NULL);
        }
        strcpy(buf, filename);
        strcat(buf, D_8006F610);
        this->Dugong::vfunc3(buf);
        if (buf != this->pathBuf && buf != NULL) {
            delete[] buf;
        }
    } else {
        this->Dugong::vfunc3(filename);
    }
}

const char* Albatross::vfunc5() {
    return D_8006F610;
}

Albatross::Albatross() {
    unkECC = 0;
    unkED0 = 0;
    unkED4 = 0;
}
