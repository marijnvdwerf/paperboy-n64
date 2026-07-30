#include "cormorant.h"
#include "surfaces.h"

extern "C" char* strcpy(char*, const char*);
extern "C" char* strcat(char*, const char*);
extern "C" unsigned strlen(const char*);
extern "C" int memcmp(const void*, const void*, unsigned);

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

#pragma clang diagnostic ignored "-Wc++11-compat-deprecated-writable-strings"
char* D_80070B60 = ".tga";

void Cormorant::vfunc1() {
    s32 dummy;
    u8 hdr[0x12];

    if (this->file.readAt(0, hdr, 0x12, &dummy) != 0) {
        __assert("", NULL, 0, NULL);
    }

    u32 b0 = hdr[0];
    u32 b1 = hdr[1];
    u32 b2 = hdr[2];
    this->idLength = b0;
    this->colorMapType = b1;
    this->imageType = b2;
    this->colorMapStart = hdr[3] + (hdr[4] << 8);
    u32 cmBitDepth = hdr[7];
    u32 cmlHi = hdr[6];
    this->colorMapBitDepth = cmBitDepth;
    this->colorMapLength = hdr[5] + (cmlHi << 8);
    this->xOrigin = hdr[8] + (hdr[9] << 8);
    this->yOrigin = hdr[10] + (hdr[11] << 8);
    this->unk40 = hdr[12] + (hdr[13] << 8);
    u32 id = hdr[17];
    u32 hHi = hdr[15];
    this->imageDescriptor = id;
    u32 bitsPerPixel = hdr[16];
    this->unk68 = 0;
    this->dataOffset = this->idLength + 0x12;
    this->unk44 = hdr[14] + (hHi << 8);
    u32 stride = (((this->unk40 * bitsPerPixel) + 0x1F) & ~0x1F) >> 3;
    this->unk48 = stride;
    this->dataSize = stride * this->unk44;

    if (this->colorMapType != 0) {
        if (bitsPerPixel < 9) {
            s32 palDataSize = ((this->colorMapBitDepth + 7) >> 3) * this->colorMapLength;
            u8* palBuf = new u8[palDataSize];
            if (palBuf == NULL) {
                __assert("", NULL, 0, NULL);
            }
            if (this->file.readAt(this->dataOffset, palBuf, palDataSize, &dummy) != 0) {
                __assert("", NULL, 0, NULL);
            }
            this->unk68 = this->colorMapLength;
            if (this->unk68 >= 0x101) {
                this->unk68 = 0x100;
            }

            u8* p;
            u32 i;
            u32 lo;
            u32 hi;
            switch (this->colorMapBitDepth) {
                case 0x0F: {
                    p = palBuf;
                    i = 0;
                    if (i < this->unk68) {
                        do {
                            lo = *p++;
                            hi = *p++;
                            this->palette[i].a = 0xFF;
                            u32 val = lo | (hi << 8);
                            this->palette[i].b = (val & 0x1F) * 8;
                            this->palette[i].g = (val & 0x3E0) >> 2;
                            this->palette[i].r = (val & 0x7C00) >> 7;
                            i++;
                        } while (i < this->unk68);
                    }
                    break;
                }
                case 0x10: {
                    p = palBuf;
                    i = 0;
                    if (i < this->unk68) {
                        do {
                            lo = *p++;
                            hi = *p++;
                            this->palette[i].a = 0;
                            u32 val = lo | (hi << 8);
                            this->palette[i].b = (val & 0x1F) * 8;
                            this->palette[i].g = (val & 0x3E0) >> 2;
                            this->palette[i].r = (val & 0x7C00) >> 7;
                            i++;
                        } while (i < this->unk68);
                    }
                    break;
                }
                case 0x18: {
                    p = palBuf;
                    i = 0;
                    if (i < this->unk68) {
                        do {
                            this->palette[i].b = *p++;
                            this->palette[i].g = *p++;
                            u32 r = *p++;
                            this->palette[i].a = 0xFF;
                            this->palette[i].r = r;
                            i++;
                        } while (i < this->unk68);
                    }
                    break;
                }
                case 0x20: {
                    p = palBuf;
                    i = 0;
                    if (i < this->unk68) {
                        do {
                            this->palette[i].b = *p++;
                            this->palette[i].g = *p++;
                            this->palette[i].r = *p++;
                            this->palette[i].a = *p++;
                            i++;
                        } while (i < this->unk68);
                    }
                    break;
                }
            }

            if (palBuf != NULL) {
                delete[] palBuf;
            }
            this->dataOffset = this->dataOffset + palDataSize;
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
            this->pixelFormat.maskRed = 0x7C00;
            pf->maskGreen = 0x3E0;
            pf->maskBlue = 0x1F;
            pf->bitDepth = 0x10;
            break;
        }
        case 24: {
            PixelFormat* pf = &this->pixelFormat;
            this->pixelFormat.maskRed = 0xFF0000;
            pf->maskGreen = 0xFF00;
            pf->maskBlue = 0xFF;
            pf->bitDepth = 0x18;
            break;
        }
        case 32: {
            PixelFormat* pf = &this->pixelFormat;
            this->pixelFormat.maskRed = 0xFF0000;
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

void Cormorant::vfunc6(Surface16970* dstSurface, s32 flipFlag, u8* transColor) {
    s32 dummy;
    PixelFormat localPf;
    u8* dstPtr;
    s32 stride;
    s32 scaleX = 1;
    s32 scaleY = 1;
    s32 step;
    s32 fileOff;
    u8* readBuf;
    u8* decodeBuf;
    PixelFormat* pf;

    s32 oob = (dstSurface->unk28 < this->unk44 || dstSurface->unk26 < this->unk40);
    if (oob) {
        __assert("", NULL, 0, NULL);
    }

    s32 needScale = (this->unk44 != dstSurface->unk28 || this->unk40 != dstSurface->unk26);
    if (needScale) {
        scaleX = dstSurface->unk26 / this->unk40;
        scaleY = dstSurface->unk28 / this->unk44;
    }

    localPf = dstSurface->hdr;

    PixelFormat* pfPtr = &localPf;
    this->func_8001B794(pfPtr, transColor);
    if (pfPtr->paletteMask != 0) {
        this->func_8001B230(dstSurface->vfunc8(), transColor);
    }

    dstSurface->vfunc2(&dstPtr, &stride, 3);
    step = stride;
    fileOff = this->dataOffset;

    readBuf = new u8[this->unk48 + 2];
    if (readBuf == NULL) {
        __assert("", NULL, 0, NULL);
    }

    decodeBuf = readBuf;
    if (this->imageType >= 9) {
        decodeBuf = new u8[this->unk48 + 2];
        if (decodeBuf == NULL) {
            __assert("", NULL, 0, NULL);
        }
    }

    if (flipFlag == 0) {
        dstPtr += (dstSurface->unk28 - 1) * step;
        step = -step;
    }

    flipFlag = 0;
    pf = &localPf;

    while ((u32)flipFlag < this->unk44) {
        if (this->file.readAt(fileOff, readBuf, this->unk48, &dummy) != 0) {
            dstSurface->vfunc3();
            __assert("", NULL, 0, NULL);
        }

        if (this->imageType >= 9) {
            this->decompressTgaRle(readBuf, decodeBuf);
        }

        this->func_8001BAE0(decodeBuf, dstPtr, &localPf);

        if (scaleX >= 2) {
            this->func_8001D500(dstPtr, scaleX, dstSurface->unk26, pf->bitDepth);
        }
        if (scaleY >= 2) {
            for (s32 i = 1; i < scaleY; i++) {
                memcpy(dstPtr + step, dstPtr, stride);
                dstPtr += step;
            }
        }

        flipFlag++;
        fileOff += dummy;
        dstPtr += step;
    }

    if (readBuf != NULL) {
        delete[] readBuf;
    }
    if (this->imageType >= 9 && decodeBuf != NULL) {
        delete[] decodeBuf;
    }

    dstSurface->vfunc3();
}

void Cormorant::decompressTgaRle(u8* src, u8* dst) {
    s32 totalWritten = 0;
    u32 pixelPos = 0;
    u32 bytesPerPixel = (u32)(this->pixelFormat.bitDepth + 7) >> 3;
    s32 header;
    s32 count;
    s32 savedPos;
    s32 nextPos;
    u32 j;

    while (pixelPos < this->unk40) {
        header = *src;
        src++;
        if (this->unk40 < (u32)(pixelPos + (header & 0x7F) + 1)) {
            return;
        }
        if (header & 0x80) {
            nextPos = pixelPos + 1;
            count = header & ~0x80;
            pixelPos = nextPos + count;
            j = 0;
            savedPos = totalWritten;
            if (bytesPerPixel != 0) {
                do {
                    dst[totalWritten] = *src;
                    src++;
                    totalWritten++;
                    j++;
                } while (j < bytesPerPixel);
            }
            for (count--; count != -1; count--) {
                j = 0;
                if (bytesPerPixel != 0) {
                    do {
                        dst[totalWritten] = dst[savedPos + j];
                        totalWritten++;
                        j++;
                    } while (j < bytesPerPixel);
                }
            }
        } else {
            nextPos = pixelPos + 1;
            pixelPos = nextPos + header;
            do {
                j = 0;
                if (bytesPerPixel != 0) {
                    do {
                        dst[totalWritten] = *src;
                        src++;
                        totalWritten++;
                        j++;
                    } while (j < bytesPerPixel);
                }
                header--;
            } while (header != -1);
        }
    }
}

void Cormorant::vfunc7(TiledSurface* dstSurface, s32 flipFlag, u8* transColor) {
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
    u8** ptrArr;
    u8* readBuf;
    u8* lineBuf;
    s32 scaleX;
    s32 rowPos;
    u32 colIdx;
    s32 step;
    s32 sentinel;
    u8* decodeBuf;

    u32 r;
    u32 c;

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

    r = 0;
    while (r < dstSurface->rowCount) {
        c = 0;
        while (c < dstSurface->colCount) {
            Surface16970* tile = dstSurface->vfunc8(r, c);
            tile->vfunc2(&ptrArr[r * dstSurface->colCount + c], &lockPitch, 3);
            pitchArr[r * dstSurface->colCount + c] = lockPitch;
            c++;
        }
        r++;
    }

    fileOff = this->dataOffset;

    readBuf = new u8[this->unk48 + 2];
    if (readBuf == NULL) {
        __assert("", NULL, 0, NULL);
    }

    lineBuf = new u8[(this->unk48 + 2) * 4];
    if (lineBuf == NULL) {
        __assert("", NULL, 0, NULL);
    }

    if (this->imageType >= 9) {
        decodeBuf = new u8[this->unk48 + 2];
        if (decodeBuf == NULL) {
            __assert("", NULL, 0, NULL);
        }
    } else {
        decodeBuf = readBuf;
    }

    rowPos = 0;
    colIdx = rowPos;
    scaleX = localPf.bitDepth >> 3;

    step = 1;

    if (flipFlag == 0) {
        step = -1;
        s32 totalH = 0;
        c = 0;
        colIdx = dstSurface->colCount - 1;
        while (c < dstSurface->colCount) {
            tileDims(dstSurface, 0, c, &tileW, &tileH);
            c++;
            totalH += tileH;
        }
        rowPos = (dstSurface->totalHeight - (totalH - tileH)) - 1;
    }

    scanLine = 0;
    sentinel = INT_MAX;

    while (scanLine < this->unk44) {
        if (this->file.readAt(fileOff, readBuf, this->unk48, &dummy) != 0) {
            for (r = 0; r < dstSurface->rowCount; r++) {
                for (c = 0; c < dstSurface->colCount; c++) {
                    Surface16970* tile = dstSurface->vfunc8(r, c);
                    tile->vfunc3();
                }
            }
            __assert("", NULL, 0, NULL);
        }

        if (this->imageType >= 9) {
            this->decompressTgaRle(readBuf, decodeBuf);
        }

        this->func_8001BAE0(decodeBuf, lineBuf, &localPf);

        u32 xAccum = 0;
        u32 tileRow = 0;
        colH = 0;

        while (tileRow < dstSurface->rowCount) {
            tileDims(dstSurface, tileRow, colIdx, &rowWidth, &colH);

            if (rowPos == sentinel) {
                rowPos = colH - 1;
            }

            u32 nPix = rowWidth * scaleX;
            u32 nPixEnd = nPix + xAccum;
            u32 maxPix = this->unk40 * scaleX;
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

        fileOff += dummy;
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

    if (readBuf != NULL) {
        delete[] readBuf;
    }
    if (lineBuf != NULL) {
        delete[] lineBuf;
    }
    if (ptrArr != NULL) {
        delete[] ptrArr;
    }
    if (pitchArr != NULL) {
        delete[] pitchArr;
    }

    if (this->imageType >= 9 && decodeBuf != NULL) {
        delete[] decodeBuf;
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

void Cormorant::vfunc3(const char* filename) {
    u32 nameLen = strlen(filename);
    u32 extLen = strlen(D_80070B60);
    if ((extLen >= nameLen) || (memcmp(filename + (nameLen - extLen), D_80070B60, extLen) != 0)) {
        u32 totalLen = nameLen + extLen + 1;
        char* buf = new char[totalLen];
        if (buf == NULL) {
            __assert("", NULL, 0, NULL);
        }
        strcpy(buf, filename);
        strcat(buf, D_80070B60);
        this->Dugong::vfunc3(buf);
        if (buf != NULL) {
            delete[] buf;
        }
    } else {
        this->Dugong::vfunc3(filename);
    }
}

const char* Cormorant::vfunc5() {
    return D_80070B60;
}

Cormorant::Cormorant() {
    dataSize = 0;
    dataOffset = 0;
}

void Cormorant::vfunc9(u8* srcBuffer, Surface16970* dstSurface, s32 flipFlag, u8* transColor) {
}

void Cormorant::vfunc8(u8* dst) {
}
