#include "dugong.h"

extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strcat(char* dst, const char* src);

extern "C" const char D_80001620[]; // "\nFile: "
extern "C" const char D_80001628[]; // ""
extern "C" const char D_800016A4[]; // ".img"

class Surface16970Palette {
  public:
    virtual void vfunc1(DugongColor* dst, s32 start, u32 count);
    virtual void vfunc2(DugongColor* colors, s32 start, u32 count);
    virtual void vfunc3(u8* dst, s32 index);
    virtual void vfunc4(Surface16970Palette* src);
    virtual void vfunc5();
    virtual u32 vfunc6();
    virtual u32 vfunc7();
    virtual u32 vfunc8();
};

extern DugongColor D_80070AF0;

void Dugong::func_8001B230(Surface16970Palette* dstPalette, u8* transColor) {
    s32 transparent = -1;
    u32 count = 1 << this->pixelFormat.bitDepth;
    u32 i;
    if (transColor != NULL) {
        for (i = 0; i < count; i++) {
            if (transColor[0] == this->palette[i].r && transColor[1] == this->palette[i].g && transColor[2] == this->palette[i].b) {
                transparent = i;
                if (this->unk94 != 0) {
                    this->palette[i].r = this->unk9C;
                    this->palette[i].g = this->unk9D;
                    this->palette[i].b = this->unk9E;
                    this->palette[i].a = 0;
                }
                break;
            }
        }
    }
    u32 available = dstPalette->vfunc7();
    this->unk8 = dstPalette->vfunc8();
    if (this->unk8 < count) {
        count = this->unk8;
    }
    if (this->unk8 == available && transColor == NULL) {
        for (i = 0; i < count; i++) {
            this->unk4A4[i] = i;
        }
        dstPalette->vfunc2(this->palette, 0, count);
    } else {
        if (this->unkC == NULL) {
            this->unkC = new DugongColor[this->unk8];
            if (this->unkC == NULL) {
                __assert(D_80001628, NULL, 0, NULL);
            }
        }
        u32 first = dstPalette->vfunc6();
        this->unk0 = first;
        if (first != 0) {
            dstPalette->vfunc1(this->unkC, 0, first);
        }
        this->unk4 = this->unk8 - available - first;
        if (this->unk4 != 0 && available < this->unk8) {
            dstPalette->vfunc1(this->unkC + (first + available), first + available, this->unk4);
        }
        if (transparent >= 0) {
            this->unk4A4[transparent] = func_8001B5C8(&this->palette[transparent]);
            for (i = 0; i < count; i++) {
                if (i != transparent) {
                    this->unk4A4[i] = func_8001B5C8(&this->palette[i]);
                }
            }
        } else if (this->unk5A8 != 0) {
            for (i = 0; i < count; i++) {
                if (this->palette[i].r == 0 && this->palette[i].g == 0 && this->palette[i].b == 0) {
                    D_80070AF0.a = this->palette[i].a;
                    this->unk4A4[i] = func_8001B5C8(&D_80070AF0);
                } else {
                    this->unk4A4[i] = func_8001B5C8(&this->palette[i]);
                }
            }
        } else {
            for (i = 0; i < count; i++) {
                this->unk4A4[i] = func_8001B5C8(&this->palette[i]);
            }
        }
        dstPalette->vfunc2(this->unkC + first, first, this->unk0 - first);
        delete[] this->unkC;
        this->unkC = NULL;
    }
}

s32 Dugong::func_8001B5C8(DugongColor* color) {
    u32 i = 0;
    for (; i < this->unk0; i++) {
        if (color->r == this->unkC[i].r && color->g == this->unkC[i].g && color->b == this->unkC[i].b) {
            return i;
        }
    }
    for (i = this->unk8 - this->unk4; i < this->unk8; i++) {
        if (color->r == this->unkC[i].r && color->g == this->unkC[i].g && color->b == this->unkC[i].b) {
            return i;
        }
    }
    s32 idx;
    if ((this->unk0 + this->unk4) >= this->unk8) {
        s32 minDist = 0x7FFFFFFF;
        s32 dist;
        i = 0;
        idx = 0;
        for (; i < this->unk8; i++) {
            dist = (this->unkC[i].r - color->r) * (this->unkC[i].r - color->r) + (this->unkC[i].g - color->g) * (this->unkC[i].g - color->g) + (this->unkC[i].b - color->b) * (this->unkC[i].b - color->b);
            if (dist < minDist) {
                idx = i;
                minDist = dist;
            }
        }
        return idx;
    }
    idx = this->unk0;
    this->unkC[idx].r = color->r;
    this->unkC[idx].g = color->g;
    this->unkC[idx].b = color->b;
    this->unkC[idx].a = color->a;
    this->unk0 = this->unk0 + 1;
    return idx;
}

INCLUDE_RODATA("asm/nonmatchings/dugong", D_80001620);

INCLUDE_RODATA("asm/nonmatchings/dugong", D_80001628);

void Dugong::func_8001B794(PixelFormat* dstPf, u8* transColor) {
    this->unk94 = 0;
    if (dstPf->paletteMask != 0) {
        if (transColor != NULL) {
            this->unk94 = 1;
        }
    } else {
        switch (this->pixelFormat.bitDepth) {
            case 4:
                this->unk6C = 0x80;
                break;
            case 8:
                this->unk6C = 0x88;
                break;
            case 16:
                this->unk6C = 0x99;
                break;
            case 24:
                this->unk6C = 0xBB;
                break;
            case 32:
                this->unk6C = 0xFF;
                break;
            default:
                __assert(D_80001628, NULL, 0, NULL);
                break;
        }
        PixelFormat* srcPf = &this->pixelFormat;
        u32 srcRedWidth = 8;
        u32 srcGreenWidth, srcBlueWidth, srcAlphaWidth;
        if (srcPf->paletteMask != 0) {
            srcGreenWidth = 8;
            srcBlueWidth = 8;
            srcAlphaWidth = 0;
            this->unk74 = 0;
            this->unk78 = 0;
            this->unk7C = 0;
            this->unk80 = 0;
        } else {
            this->unk74 = srcPf->ctzMaskRed();
            srcRedWidth = srcPf->bitWidthMaskRed();
            this->unk78 = srcPf->ctzMaskGreen();
            srcGreenWidth = srcPf->bitWidthMaskGreen();
            this->unk7C = srcPf->ctzMaskBlue();
            srcBlueWidth = srcPf->bitWidthMaskBlue();
            this->unk80 = srcPf->ctzMaskAlpha();
            srcAlphaWidth = srcPf->bitWidthMaskAlpha();
        }
        u32 dstRedWidth = dstPf->bitWidthMaskRed();
        if (dstRedWidth < srcRedWidth) {
            this->unk74 += srcRedWidth - dstRedWidth;
        }
        this->unk84 = dstPf->ctzMaskRed();
        if (srcRedWidth < dstRedWidth) {
            this->unk84 += dstRedWidth - srcRedWidth;
        }
        u32 dstBlueWidth;
        u32 dstGreenWidth = dstPf->bitWidthMaskGreen();
        if (dstGreenWidth < srcGreenWidth) {
            this->unk78 += srcGreenWidth - dstGreenWidth;
        }
        this->unk88 = dstPf->ctzMaskGreen();
        if (srcGreenWidth < dstGreenWidth) {
            this->unk88 += dstGreenWidth - srcGreenWidth;
        }
        dstBlueWidth = dstPf->bitWidthMaskBlue();
        if (dstBlueWidth < srcBlueWidth) {
            this->unk7C += srcBlueWidth - dstBlueWidth;
        }
        this->unk8C = dstPf->ctzMaskBlue();
        if (srcBlueWidth < dstBlueWidth) {
            this->unk8C += dstBlueWidth - srcBlueWidth;
        }
        if (transColor != NULL) {
            s32 redShift = 8 - dstRedWidth;
            s32 greenShift = 8 - dstGreenWidth;
            this->unk94 = 1;
            this->unk98 = (s32)transColor[0] >> redShift;
            this->unk99 = (s32)transColor[1] >> greenShift;
            s32 v1 = ((s32)this->unk9C >> redShift) << this->unk84;
            s32 blue = transColor[2];
            s32 blueShift = 8 - dstBlueWidth;
            this->unk80 = 0;
            this->unk90 = 0;
            this->unkA0 = v1;
            v1 = v1 | (((s32)this->unk9D >> greenShift) << this->unk88);
            this->unk9A = (s32)blue >> blueShift;
            this->unkA0 = v1;
            this->unkA0 = v1 | (((s32)this->unk9E >> blueShift) << this->unk8C);
            this->unk70 = dstPf->maskAlpha;
            return;
        }
        u32 dstAlphaWidth = dstPf->bitWidthMaskAlpha();
        if (dstAlphaWidth < srcAlphaWidth) {
            this->unk80 += srcAlphaWidth - dstAlphaWidth;
        }
        this->unk90 = dstPf->ctzMaskAlpha();
        if (srcAlphaWidth < dstAlphaWidth) {
            this->unk90 += dstAlphaWidth - srcAlphaWidth;
        }
    }
}

void Dugong::func_8001BAE0(u8* src, u8* dst, PixelFormat* dstPf) {
    PixelFormat* srcPf = &this->pixelFormat;
    if (srcPf->paletteMask != 0 && dstPf->paletteMask != 0) {
        if (srcPf->bitDepth == 4) {
            if (dstPf->bitDepth == srcPf->bitDepth) {
                if (this->unk5A4 != 0) {
                    func_8001E6C0(src, dst);
                } else {
                    func_8001E65C(src, dst);
                }
            } else if (dstPf->bitDepth == 8) {
                func_8001E5E0(src, dst);
            }
        } else {
            s32 match = 0;
            if (srcPf->bitDepth == 8) {
                match = dstPf->bitDepth == 8;
            }
            if (match) {
                func_8001E5A4(src, dst);
            }
        }
    } else if (srcPf->paletteMask != 0) {
        if (srcPf->bitDepth == 4) {
            switch ((u32)dstPf->bitDepth) {
                case 8:
                    func_8001C5B0(src, dst);
                    break;
                case 16:
                    func_8001C8E4(src, (u16*)dst);
                    break;
                case 24:
                    func_8001CC1C(src, dst);
                    break;
                case 32:
                    func_8001D040(src, (u32*)dst);
                    break;
            }
        } else {
            switch ((u32)dstPf->bitDepth) {
                case 8:
                    func_8001E464(src, dst);
                    break;
                case 16:
                    func_8001E320(src, (u16*)dst);
                    break;
                case 24:
                    func_8001D378(src, dst);
                    break;
                case 32:
                    func_8001E1DC(src, (u32*)dst);
                    break;
            }
        }
    } else {
        if (srcPf->unk10 != 0 || dstPf->paletteMask != 0) {
            __assert(D_80001628, NULL, 0, NULL);
        } else {
            switch ((u32)dstPf->bitDepth) {
                case 8:
                    func_8001BD54(src, dst);
                    break;
                case 16:
                    func_8001BF54(src, (u16*)dst);
                    break;
                case 24:
                    func_8001C158(src, dst);
                    break;
                case 32:
                    func_8001C3AC(src, (u32*)dst);
                    break;
            }
        }
    }
}

void Dugong::func_8001BD54(u8* src, u8* dst) {
    u32 pixel, red, green, blue;
    u8* end = dst + this->unk40;
    if (this->unk94 != 0) {
        while (dst < end) {
            u32 b3;
            pixel = *src;
            src += this->unk6C & 1;
            pixel |= *src << 8;
            src += (this->unk6C >> 1) & 1;
            pixel |= *src << 16;
            src += (this->unk6C >> 2) & 1;
            b3 = *src;
            src += (this->unk6C >> 3) & 1;
            pixel |= b3 << 24;
            PixelFormat* pf = &this->pixelFormat;
            red = pixel & pf->maskRed;
            green = pixel & pf->maskGreen;
            blue = pixel & pf->maskBlue;
            red >>= this->unk74;
            green >>= this->unk78;
            blue >>= this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0;
            } else {
                pixel = red << this->unk84;
                pixel |= green << this->unk88;
                pixel |= blue << this->unk8C;
                pixel |= this->unk70;
                *dst++ = pixel;
            }
        }
    } else {
        while (dst < end) {
            u32 b3;
            pixel = *src;
            src += this->unk6C & 1;
            pixel |= *src << 8;
            src += (this->unk6C >> 1) & 1;
            pixel |= *src << 16;
            src += (this->unk6C >> 2) & 1;
            b3 = *src;
            src += (this->unk6C >> 3) & 1;
            pixel |= b3 << 24;
            PixelFormat* pf = &this->pixelFormat;
            red = pixel & pf->maskRed;
            green = pixel & pf->maskGreen;
            blue = pixel & pf->maskBlue;
            u32 alpha = pixel & pf->maskAlpha;
            red >>= this->unk74;
            green >>= this->unk78;
            blue >>= this->unk7C;
            alpha >>= this->unk80;
            pixel = red << this->unk84;
            pixel |= green << this->unk88;
            pixel |= blue << this->unk8C;
            pixel |= alpha << this->unk90;
            *dst++ = pixel;
        }
    }
}

void Dugong::func_8001BF54(u8* src, u16* dst) {
    u32 pixel, red, green, blue;
    u16* end = dst + this->unk40;
    if (this->unk94 != 0) {
        while (dst < end) {
            u32 b3;
            pixel = *src;
            src += this->unk6C & 1;
            pixel |= *src << 8;
            src += (this->unk6C >> 1) & 1;
            pixel |= *src << 16;
            src += (this->unk6C >> 2) & 1;
            b3 = *src;
            src += (this->unk6C >> 3) & 1;
            pixel |= b3 << 24;
            PixelFormat* pf = &this->pixelFormat;
            red = pixel & pf->maskRed;
            green = pixel & pf->maskGreen;
            blue = pixel & pf->maskBlue;
            red >>= this->unk74;
            green >>= this->unk78;
            blue >>= this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst = this->unkA0;
                dst++;
            } else {
                pixel = red << this->unk84;
                pixel |= green << this->unk88;
                pixel |= blue << this->unk8C;
                pixel |= this->unk70;
                *dst = pixel;
                dst++;
            }
        }
    } else {
        while (dst < end) {
            u32 b3;
            pixel = *src;
            src += this->unk6C & 1;
            pixel |= *src << 8;
            src += (this->unk6C >> 1) & 1;
            pixel |= *src << 16;
            src += (this->unk6C >> 2) & 1;
            b3 = *src;
            src += (this->unk6C >> 3) & 1;
            pixel |= b3 << 24;
            PixelFormat* pf = &this->pixelFormat;
            red = pixel & pf->maskRed;
            green = pixel & pf->maskGreen;
            blue = pixel & pf->maskBlue;
            u32 alpha = pixel & pf->maskAlpha;
            red >>= this->unk74;
            green >>= this->unk78;
            blue >>= this->unk7C;
            alpha >>= this->unk80;
            pixel = red << this->unk84;
            pixel |= green << this->unk88;
            pixel |= blue << this->unk8C;
            pixel |= alpha << this->unk90;
            *dst = pixel;
            dst++;
        }
    }
}

void Dugong::func_8001C158(u8* src, u8* dst) {
    u32 pixel, red, green, blue;
    u32 i;
    if (this->unk94 != 0) {
        for (i = 0; i < this->unk40; i++) {
            u32 b3;
            pixel = *src;
            src += this->unk6C & 1;
            pixel |= *src << 8;
            src += (this->unk6C >> 1) & 1;
            pixel |= *src << 16;
            src += (this->unk6C >> 2) & 1;
            b3 = *src;
            src += (this->unk6C >> 3) & 1;
            pixel |= b3 << 24;
            PixelFormat* pf = &this->pixelFormat;
            red = pixel & pf->maskRed;
            green = pixel & pf->maskGreen;
            blue = pixel & pf->maskBlue;
            red >>= this->unk74;
            green >>= this->unk78;
            blue >>= this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0 >> 16;
                *dst++ = this->unkA0 >> 8;
                *dst++ = this->unkA0;
            } else {
                pixel = red << this->unk84;
                pixel |= green << this->unk88;
                pixel |= blue << this->unk8C;
                pixel |= this->unk70;
                *dst++ = pixel >> 16;
                *dst++ = pixel >> 8;
                *dst++ = pixel;
            }
        }
    } else {
        for (i = 0; i < this->unk40; i++) {
            u32 b3;
            pixel = *src;
            src += this->unk6C & 1;
            pixel |= *src << 8;
            src += (this->unk6C >> 1) & 1;
            pixel |= *src << 16;
            src += (this->unk6C >> 2) & 1;
            b3 = *src;
            src += (this->unk6C >> 3) & 1;
            pixel |= b3 << 24;
            PixelFormat* pf = &this->pixelFormat;
            red = pixel & pf->maskRed;
            green = pixel & pf->maskGreen;
            blue = pixel & pf->maskBlue;
            u32 alpha = pixel & pf->maskAlpha;
            red >>= this->unk74;
            green >>= this->unk78;
            blue >>= this->unk7C;
            alpha >>= this->unk80;
            pixel = red << this->unk84;
            pixel |= green << this->unk88;
            pixel |= blue << this->unk8C;
            pixel |= alpha << this->unk90;
            *dst++ = pixel >> 16;
            *dst++ = pixel >> 8;
            *dst++ = pixel;
        }
    }
}

void Dugong::func_8001C3AC(u8* src, u32* dst) {
    u32 pixel, red, green, blue;
    u32* end = dst + this->unk40;
    if (this->unk94 != 0) {
        while (dst < end) {
            u32 b3;
            pixel = *src;
            src += this->unk6C & 1;
            pixel |= *src << 8;
            src += (this->unk6C >> 1) & 1;
            pixel |= *src << 16;
            src += (this->unk6C >> 2) & 1;
            b3 = *src;
            src += (this->unk6C >> 3) & 1;
            pixel |= b3 << 24;
            PixelFormat* pf = &this->pixelFormat;
            red = pixel & pf->maskRed;
            green = pixel & pf->maskGreen;
            blue = pixel & pf->maskBlue;
            red >>= this->unk74;
            green >>= this->unk78;
            blue >>= this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst = this->unkA0;
                dst++;
            } else {
                pixel = red << this->unk84;
                pixel |= green << this->unk88;
                pixel |= blue << this->unk8C;
                pixel |= this->unk70;
                *dst = pixel;
                dst++;
            }
        }
    } else {
        while (dst < end) {
            u32 b3;
            pixel = *src;
            src += this->unk6C & 1;
            pixel |= *src << 8;
            src += (this->unk6C >> 1) & 1;
            pixel |= *src << 16;
            src += (this->unk6C >> 2) & 1;
            b3 = *src;
            src += (this->unk6C >> 3) & 1;
            pixel |= b3 << 24;
            PixelFormat* pf = &this->pixelFormat;
            red = pixel & pf->maskRed;
            green = pixel & pf->maskGreen;
            blue = pixel & pf->maskBlue;
            u32 alpha = pixel & pf->maskAlpha;
            red >>= this->unk74;
            green >>= this->unk78;
            blue >>= this->unk7C;
            alpha >>= this->unk80;
            pixel = red << this->unk84;
            pixel |= green << this->unk88;
            pixel |= blue << this->unk8C;
            pixel |= alpha << this->unk90;
            *dst = pixel;
            dst++;
        }
    }
}

void Dugong::func_8001C5B0(u8* src, u8* dst) {
    DugongColor* e;
    s32 red, green, blue;
    u32 color;
    u8* end = dst + this->unk40;
    if (this->unk94 != 0) {
        while (dst + 1 < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color;
            }
            e = &this->palette[*src++ & 0xf];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color;
            }
        }
        if (dst < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst = color;
            }
        }
    } else {
        while (dst + 1 < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color;
            e = &this->palette[*src++ & 0xf];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color;
        }
        if (dst < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst = color;
        }
    }
}

void Dugong::func_8001C8E4(u8* src, u16* dst) {
    DugongColor* e;
    s32 red, green, blue;
    u32 color;
    u16* end = dst + this->unk40;
    if (this->unk94 != 0) {
        while (dst + 1 < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color;
            }
            e = &this->palette[*src++ & 0xf];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color;
            }
        }
        if (dst < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst = color;
            }
        }
    } else {
        while (dst + 1 < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color;
            e = &this->palette[*src++ & 0xf];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color;
        }
        if (dst < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst = color;
        }
    }
}

void Dugong::func_8001CC1C(u8* src, u8* dst) {
    DugongColor* e;
    s32 red, green, blue;
    u32 color;
    s32 i;
    if (this->unk94 != 0) {
        for (i = 0; i + 1 < this->unk40; i += 2) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0 >> 16;
                *dst++ = this->unkA0 >> 8;
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color >> 16;
                *dst++ = color >> 8;
                *dst++ = color;
            }
            e = &this->palette[*src++ & 0xf];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0 >> 16;
                *dst++ = this->unkA0 >> 8;
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color >> 16;
                *dst++ = color >> 8;
                *dst++ = color;
            }
        }
        if (i < this->unk40) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0 >> 16;
                *dst++ = this->unkA0 >> 8;
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color >> 16;
                *dst++ = color >> 8;
                *dst++ = color;
            }
        }
    } else {
        for (i = 0; i + 1 < this->unk40; i += 2) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color >> 16;
            *dst++ = color >> 8;
            *dst++ = color;
            e = &this->palette[*src++ & 0xf];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color >> 16;
            *dst++ = color >> 8;
            *dst++ = color;
        }
        if (i < this->unk40) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color >> 16;
            *dst++ = color >> 8;
            *dst++ = color;
        }
    }
}

void Dugong::func_8001D040(u8* src, u32* dst) {
    DugongColor* e;
    s32 red, green, blue;
    u32 color;
    u32* end = dst + this->unk40;
    if (this->unk94 != 0) {
        while (dst + 1 < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color;
            }
            e = &this->palette[*src++ & 0xf];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color;
            }
        }
        if (dst < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst = color;
            }
        }
    } else {
        while (dst + 1 < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color;
            e = &this->palette[*src++ & 0xf];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color;
        }
        if (dst < end) {
            e = &this->palette[*src >> 4];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst = color;
        }
    }
}

void Dugong::func_8001D378(u8* src, u8* dst) {
    DugongColor* e;
    s32 red, green, blue;
    u32 color;
    u8* end = src + this->unk40;
    if (this->unk94 != 0) {
        while (src < end) {
            e = &this->palette[*src++];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0 >> 16;
                *dst++ = this->unkA0 >> 8;
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color >> 16;
                *dst++ = color >> 8;
                *dst++ = color;
            }
        }
    } else {
        while (src < end) {
            e = &this->palette[*src++];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color >> 16;
            *dst++ = color >> 8;
            *dst++ = color;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/dugong", D_800016A4);

#ifdef NON_MATCHING
void Dugong::func_8001D500(u8* buf, s32 scaleX, s32 dstWidth, s32 bitDepth) {
    s32 srcWidth, count;
    u8 *src, *out;
    switch (bitDepth) {
        case 4: {
            s32 halfDst = (u32)dstWidth >> 1;
            out = buf + halfDst - 1;
            srcWidth = this->unk40;
            s32 halfSrc = (u32)srcWidth >> 1;
            src = buf + halfSrc - 1;
            s32 t0 = 1;
            src = buf + ((u32)srcWidth >> t0) - 1;
            srcWidth -= 1;
            if (srcWidth < 0)
                break;
            do {
                u8 pixel;
                s32 odd = srcWidth & 1;
                if (odd != 0) {
                    pixel = *src >> 4;
                } else {
                    pixel = *src & 0xF;
                    src--;
                }
                count = scaleX;
                if (count > 0) {
                    u8 hi = pixel << 4;
                    do {
                        if (t0 & 1) {
                            u8 val = *out;
                            *out = val | pixel;
                        } else {
                            *out = hi;
                            out--;
                        }
                        count--;
                        t0++;
                    } while (count > 0);
                }
                srcWidth--;
            } while (srcWidth >= 0);
            break;
        }
        case 8: {
            out = buf + dstWidth - 1;
            srcWidth = this->unk40;
            src = buf + srcWidth - 1;
            if (srcWidth <= 0)
                break;
            do {
                count = scaleX;
                if (count > 0) {
                    do {
                        *out = *src;
                        count--;
                        out--;
                    } while (count > 0);
                }
                srcWidth--;
                src--;
            } while (srcWidth > 0);
            break;
        }
        case 15:
        case 16: {
            out = buf + (dstWidth << 1) - 2;
            srcWidth = this->unk40;
            src = buf + (srcWidth << 1) - 2;
            if (srcWidth <= 0)
                break;
            do {
                count = scaleX;
                if (count > 0) {
                    do {
                        *(u16*)out = *(u16*)src;
                        count--;
                        out -= 2;
                    } while (count > 0);
                }
                srcWidth--;
                src -= 2;
            } while (srcWidth > 0);
            break;
        }
        case 24: {
            s32 dstBytes = (dstWidth << 1) + dstWidth;
            u8* t0p = buf + dstBytes - 3;
            srcWidth = this->unk40;
            s32 srcBytes = (srcWidth << 1) + srcWidth;
            u8* t1p = buf + srcBytes - 3;
            if (srcWidth <= 0)
                break;
            src = buf + srcBytes - 1;
            do {
                count = scaleX;
                if (count > 0) {
                    out = t0p + 2;
                    do {
                        *t0p = *t1p;
                        out[-1] = src[-1];
                        *out = *src;
                        count--;
                        t0p -= 3;
                        out -= 3;
                    } while (count > 0);
                }
                src -= 3;
                srcWidth--;
                t1p -= 3;
            } while (srcWidth > 0);
            break;
        }
        case 32: {
            out = buf + (dstWidth << 2) - 4;
            srcWidth = this->unk40;
            src = buf + (srcWidth << 2) - 4;
            if (srcWidth <= 0)
                break;
            do {
                count = scaleX;
                if (count > 0) {
                    do {
                        *(s32*)out = *(s32*)src;
                        count--;
                        out -= 4;
                    } while (count > 0);
                }
                srcWidth--;
                src -= 4;
            } while (srcWidth > 0);
            break;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/dugong", func_8001D500__6DugongPUclll);
#endif

void Dugong::func_8001D738(u8* src1, u8* src2, u16* dst) {
    u32 pixels[4];
    u32 red, green, blue;
    u32 sumRed, sumGreen, sumBlue, sumAlpha;
    u32 i;
    u16* end = dst + (this->unk40 >> 1);
    if (this->unk94 != 0) {
        while (dst < end) {
            u32 byte;
            pixels[0] = *src1;
            src1 += this->unk6C & 1;
            byte = *src1;
            pixels[0] |= byte << 8;
            src1 += (this->unk6C >> 1) & 1;
            byte = *src1;
            pixels[0] |= byte << 16;
            src1 += (this->unk6C >> 2) & 1;
            byte = *src1;
            pixels[0] |= byte << 24;
            src1 += (this->unk6C >> 3) & 1;
            pixels[1] = *src1;
            src1 += this->unk6C & 1;
            byte = *src1;
            pixels[1] |= byte << 8;
            src1 += (this->unk6C >> 1) & 1;
            byte = *src1;
            pixels[1] |= byte << 16;
            src1 += (this->unk6C >> 2) & 1;
            byte = *src1;
            pixels[1] |= byte << 24;
            src1 += (this->unk6C >> 3) & 1;
            pixels[2] = *src2;
            src2 += this->unk6C & 1;
            byte = *src2;
            pixels[2] |= byte << 8;
            src2 += (this->unk6C >> 1) & 1;
            byte = *src2;
            pixels[2] |= byte << 16;
            src2 += (this->unk6C >> 2) & 1;
            byte = *src2;
            pixels[2] |= byte << 24;
            src2 += (this->unk6C >> 3) & 1;
            pixels[3] = *src2;
            src2 += this->unk6C & 1;
            byte = *src2;
            pixels[3] |= byte << 8;
            src2 += (this->unk6C >> 1) & 1;
            byte = *src2;
            pixels[3] |= byte << 16;
            src2 += (this->unk6C >> 2) & 1;
            byte = *src2;
            pixels[3] |= byte << 24;
            src2 += (this->unk6C >> 3) & 1;
            sumRed = 0;
            sumGreen = 0;
            sumBlue = 0;
            for (i = 0; i < 4; i++) {
                PixelFormat* pf = &this->pixelFormat;
                u32 maskRed = pf->maskRed;
                red = pixels[i] & maskRed;
                green = pixels[i] & pf->maskGreen;
                blue = pixels[i] & pf->maskBlue;
                red >>= this->unk74;
                green >>= this->unk78;
                blue >>= this->unk7C;
                if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                    break;
                }
                sumRed += red;
                sumGreen += green;
                sumBlue += blue;
            }
            if (i < 4) {
                *dst = this->unkA0;
            } else {
                pixels[0] = (sumRed >>= 2) << this->unk84;
                pixels[0] |= (sumGreen >>= 2) << this->unk88;
                pixels[0] |= (sumBlue >>= 2) << this->unk8C;
                pixels[0] |= this->unk70;
                *dst = pixels[0];
            }
            dst++;
        }
    } else {
        while (dst < end) {
            u32 byte;
            pixels[0] = *src1;
            src1 += this->unk6C & 1;
            byte = *src1;
            pixels[0] |= byte << 8;
            src1 += (this->unk6C >> 1) & 1;
            byte = *src1;
            pixels[0] |= byte << 16;
            src1 += (this->unk6C >> 2) & 1;
            byte = *src1;
            pixels[0] |= byte << 24;
            src1 += (this->unk6C >> 3) & 1;
            pixels[1] = *src1;
            src1 += this->unk6C & 1;
            byte = *src1;
            pixels[1] |= byte << 8;
            src1 += (this->unk6C >> 1) & 1;
            byte = *src1;
            pixels[1] |= byte << 16;
            src1 += (this->unk6C >> 2) & 1;
            byte = *src1;
            pixels[1] |= byte << 24;
            src1 += (this->unk6C >> 3) & 1;
            pixels[2] = *src2;
            src2 += this->unk6C & 1;
            byte = *src2;
            pixels[2] |= byte << 8;
            src2 += (this->unk6C >> 1) & 1;
            byte = *src2;
            pixels[2] |= byte << 16;
            src2 += (this->unk6C >> 2) & 1;
            byte = *src2;
            pixels[2] |= byte << 24;
            src2 += (this->unk6C >> 3) & 1;
            pixels[3] = *src2;
            src2 += this->unk6C & 1;
            byte = *src2;
            pixels[3] |= byte << 8;
            src2 += (this->unk6C >> 1) & 1;
            byte = *src2;
            pixels[3] |= byte << 16;
            src2 += (this->unk6C >> 2) & 1;
            byte = *src2;
            pixels[3] |= byte << 24;
            src2 += (this->unk6C >> 3) & 1;
            sumRed = 0;
            sumGreen = 0;
            sumBlue = 0;
            sumAlpha = 0;
            for (i = 0; i < 4; i++) {
                PixelFormat* pf = &this->pixelFormat;
                red = pixels[i] & pf->maskRed;
                green = pixels[i] & pf->maskGreen;
                blue = pixels[i] & pf->maskBlue;
                u32 alpha = pixels[i] & pf->maskAlpha;
                red >>= this->unk74;
                green >>= this->unk78;
                blue >>= this->unk7C;
                alpha >>= this->unk80;
                sumRed += red;
                sumGreen += green;
                sumBlue += blue;
                sumAlpha += alpha;
            }
            pixels[0] = (sumRed >>= 2) << this->unk84;
            pixels[0] |= (sumGreen >>= 2) << this->unk88;
            pixels[0] |= (sumBlue >>= 2) << this->unk8C;
            pixels[0] |= (sumAlpha >>= 2) << this->unk90;
            *dst = pixels[0];
            dst++;
        }
    }
}

#ifdef NON_MATCHING
void Dugong::func_8001DCFC(u8* src1, u8* src2, u8* dst) {
    s32 pixels[4];
    u32 halfWidth = this->unk40 >> 1;
    u8* end = dst + halfWidth;
    u8* counter = dst;
    if (dst < end) {
        src2--;
        src1--;
        dst--;
        do {
            dst++;
            src1 += 2;
            pixels[0] = src1[-1];
            counter++;
            pixels[1] = src1[0];
            src2 += 2;
            if (pixels[1] == pixels[0]) {
                *dst = (u8)pixels[1];
            } else {
                pixels[2] = src2[-1];
                if (pixels[2] == pixels[0] || pixels[2] == pixels[1]) {
                    *dst = (u8)pixels[2];
                } else {
                    *dst = src2[0];
                }
            }
        } while (counter < end);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/dugong", func_8001DCFC__6DugongPUcN21);
#endif

void Dugong::func_8001DD90(u8* src, u8* dst, u32 dstWidth, u32 dstHeight, s32 dstStride, PixelFormat* dstPf, s32 unused, u8* transColor) {
    if ((this->unk44 >> 1) != dstHeight || (this->unk40 >> 1) != dstWidth) {
        __assert(D_80001628, NULL, 0, NULL);
    }
    func_8001B794(dstPf, transColor);
    u32 halfHeight = this->unk44 >> 1;
    s32 srcStride2 = this->unk48 << 1;
    if (dstPf->paletteMask != 0) {
        for (u32 i = 0; i < halfHeight; i++) {
            func_8001DCFC(src, src + this->unk48, dst);
            src += srcStride2;
            dst += dstStride;
        }
    } else {
        for (u32 i = 0; i < halfHeight; i++) {
            func_8001D738(src, src + this->unk48, (u16*)dst);
            src += srcStride2;
            dst += dstStride;
        }
    }
}

void Dugong::func_8001DEC0(u8* src, u8* dst, u32 dstWidth, u32 dstHeight, s32 dstStride, PixelFormat* dstPf, Surface16970Palette* palette, s32 flipVertical, u8* transColor) {
    u32 scaleX = 1;
    u32 scaleY = 1;
    if (this->unk44 != dstHeight || this->unk40 != dstWidth) {
        scaleY = dstHeight / this->unk44;
        scaleX = dstWidth / this->unk40;
    }
    if (dstHeight < this->unk44 || dstWidth < this->unk40) {
        __assert(D_80001628, NULL, 0, NULL);
    }
    func_8001B794(dstPf, transColor);
    if (dstPf->paletteMask != 0 && palette != NULL) {
        func_8001B230(palette, transColor);
    }
    s32 stride;
    if (flipVertical != 0) {
        stride = -dstStride;
        dst += (dstHeight - 1) * dstStride;
    } else {
        stride = dstStride;
    }
    u8* srcPtr = src;
    u32 row = 0;
    s32 skipScale = scaleX < 2;
    while (row < this->unk44) {
        func_8001BAE0(srcPtr, dst, dstPf);
        u32 copy;
        if (skipScale == 0) {
            func_8001D500(dst, scaleX, dstWidth, dstPf->bitDepth);
        }
        copy = 1;
        if (copy < scaleY) {
            do {
                u8* next = dst + stride;
                memcpy(next, dst, stride);
                copy++;
                dst = next;
            } while (copy < scaleY);
        }
        dst += stride;
        row++;
        srcPtr += this->unk48;
    }
}

void Dugong::func_8001E0BC(PixelFormat* pf, u32 width, u32 height, u32 arg4, u8* palette, s32 paletteCount) {
    if (this->file.state & 1) {
        this->vfunc4();
    }
    this->pixelFormat = *pf;
    this->unk40 = width;
    this->unk44 = height;
    this->unk48 = arg4;
    this->unk5A4 = 0;
    this->unk5A8 = 0;
    this->unk68 = 1 << this->pixelFormat.bitDepth;
    if (palette != NULL && paletteCount != 0) {
        memcpy(this->palette, palette, paletteCount * 4);
    }
}

void Dugong::vfunc9(u8*, Surface16970*, s32, u8*) {
}

void Dugong::vfunc8(u8* dst) {
}

void Dugong::vfunc1() {
}

void Dugong::vfunc7(TiledSurface*, s32, u8*) {
}

void Dugong::vfunc6(Surface16970*, s32, u8*) {
}

const char* Dugong::vfunc5() {
    return D_800016A4;
}

void Dugong::func_8001E1DC(u8* src, u32* dst) {
    DugongColor* e;
    s32 red, green, blue;
    u32 color;
    u32* end = dst + this->unk40;
    if (this->unk94 != 0) {
        while (dst < end) {
            e = &this->palette[*src++];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color;
            }
        }
    } else {
        while (dst < end) {
            e = &this->palette[*src++];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color;
        }
    }
}

void Dugong::func_8001E320(u8* src, u16* dst) {
    DugongColor* e;
    s32 red, green, blue;
    u32 color;
    u16* end = dst + this->unk40;
    if (this->unk94 != 0) {
        while (dst < end) {
            e = &this->palette[*src++];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color;
            }
        }
    } else {
        while (dst < end) {
            e = &this->palette[*src++];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color;
        }
    }
}

void Dugong::func_8001E464(u8* src, u8* dst) {
    DugongColor* e;
    s32 red, green, blue;
    u32 color;
    u8* end = dst + this->unk40;
    if (this->unk94 != 0) {
        while (dst < end) {
            e = &this->palette[*src++];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            if (red == this->unk98 && green == this->unk99 && blue == this->unk9A) {
                *dst++ = this->unkA0;
            } else {
                color = red << this->unk84;
                color |= green << this->unk88;
                color |= blue << this->unk8C;
                color |= this->unk70;
                *dst++ = color;
            }
        }
    } else {
        while (dst < end) {
            e = &this->palette[*src++];
            red = e->r >> this->unk74;
            green = e->g >> this->unk78;
            blue = e->b >> this->unk7C;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            *dst++ = color;
        }
    }
}

void Dugong::func_8001E5A4(u8* src, u8* dst) {
    u8* end = dst + this->unk40;
    while (dst < end) {
        *dst = this->unk4A4[*src];
        dst++;
        src++;
    }
}

void Dugong::func_8001E5E0(u8* src, u8* dst) {
    u8* end = dst + this->unk40;
    u8* dst2 = dst + 1;
    while (dst2 < end) {
        *dst = this->unk4A4[*src >> 4];
        *dst2 = this->unk4A4[*src & 0xF];
        dst += 2;
        dst2 += 2;
        src++;
    }
    if (dst < end) {
        *dst = this->unk4A4[*src & 0xF];
    }
}

void Dugong::func_8001E65C(u8* src, u8* dst) {
    u8* end = dst + ((u32)(this->unk40 + 1) >> 1);
    if (dst < end) {
        do {
            u8 lo = this->unk4A4[*src & 0xF];
            *dst = lo;
            u8 hi = this->unk4A4[*src >> 4];
            *dst = lo | (hi << 4);
            dst++;
            src++;
        } while (dst < end);
    }
}

void Dugong::func_8001E6C0(u8* src, u8* dst) {
    u8* end = dst + ((u32)(this->unk40 + 1) >> 1);
    if (dst < end) {
        do {
            u8 lo = this->unk4A4[*src & 0xF];
            *dst = lo;
            u8 hi = this->unk4A4[*src >> 4];
            *dst = lo | (hi << 4);
            dst++;
            src++;
        } while (dst < end);
    }
}

void Dugong::vfunc4() {
    file.close();
    if (unkC != NULL) {
        delete[] unkC;
        unkC = NULL;
    }
}

void Dugong::vfunc3(const char* filename) {
    char buf[0x100];
    s32 err = file.open(filename, 2, 0x2000);
    if (err != 0) {
        strcpy(buf, AbstractFile::errorMessage(err));
        strcat(buf, D_80001620);
        strcat(buf, filename);
        __assert(D_80001628, NULL, 0, NULL);
    }
    PixelFormat* pf = &pixelFormat;
    pf->maskRed = 0;
    pf->maskGreen = 0;
    pf->maskBlue = 0;
    pf->maskAlpha = 0;
    pf->unk10 = 0;
    pf->paletteMask = 0;
    unk5A4 = 0;
    unk5A8 = 0;
    vfunc1();
}

Dugong::~Dugong() {
    vfunc4();
}

Dugong::Dugong() {
    unk40 = 0;
    unk44 = 0;
    unk48 = 0;
    unk94 = 0;
    unk68 = 0;
    unk98 = 0;
    unk99 = 0;
    unk9A = 0;
    unk9B = 0;
    unk70 = 0;
    unk64 = 0;
    unk9C = 0;
    unk9D = 0;
    unk9E = 0;
    unk9F = 0;
    unkA0 = 0;
    unk5A4 = 0;
    unk5A8 = 0;
    unk0 = 0;
    unk4 = 0;
    unk8 = 0;
    unkC = NULL;
}

void Dugong::func_8001E94C() {
    this->unk5A8 = 0;
}

void Dugong::func_8001E954() {
    this->unk5A8 = 1;
}

void Dugong::func_8001E960() {
    this->unk5A4 = 0;
}

void Dugong::func_8001E968() {
    this->unk5A4 = 1;
}

s32 Dugong::func_8001E974() {
    return this->unk48 * this->unk44;
}

void Dugong::func_8001E98C(u8* src) {
    memcpy(&this->unk9C, src, 4);
}

void Dugong::func_8001E9A4(u8* src) {
    memcpy(this->palette, src, 0x400);
}

u8* Dugong::func_8001EA40() {
    return (u8*)this->palette;
}

u16 Dugong::func_8001EA48() {
    return this->pixelFormat.paletteMask;
}

u32 Dugong::func_8001EA54() {
    return this->pixelFormat.unk10;
}

u32 Dugong::func_8001EA60() {
    return this->pixelFormat.maskAlpha;
}

u32 Dugong::func_8001EA6C() {
    return this->pixelFormat.maskBlue;
}

u32 Dugong::func_8001EA78() {
    return this->pixelFormat.maskGreen;
}

u32 Dugong::func_8001EA84() {
    return this->pixelFormat.maskRed;
}

s32 Dugong::func_8001EA90() {
    return this->pixelFormat.bitWidthPaletteMask();
}

s32 Dugong::func_8001EAAC() {
    return this->pixelFormat.bitWidthUnk10();
}

s32 Dugong::func_8001EAC8() {
    return this->pixelFormat.bitWidthMaskAlpha();
}

s32 Dugong::func_8001EAE4() {
    return this->pixelFormat.bitWidthMaskBlue();
}

s32 Dugong::func_8001EB00() {
    return this->pixelFormat.bitWidthMaskGreen();
}

s32 Dugong::func_8001EB1C() {
    return this->pixelFormat.bitWidthMaskRed();
}

s32 Dugong::func_8001EB38() {
    return this->pixelFormat.unk10 != 0;
}

s32 Dugong::func_8001EB44() {
    return this->pixelFormat.maskAlpha != 0;
}

s32 Dugong::func_8001EB50() {
    return this->pixelFormat.paletteMask != 0;
}

u16 Dugong::func_8001EB5C() {
    return this->pixelFormat.bitDepth;
}

void Dugong::func_8001EB68(PixelFormat* out) {
    *out = this->pixelFormat;
}

s32 Dugong::func_8001EBA0() {
    return this->unk44;
}

s32 Dugong::func_8001EBAC() {
    return this->unk48;
}

s32 Dugong::func_8001EBB8() {
    return this->unk40;
}

s32 Dugong::func_8001EBC4() {
    return this->file.state & 1;
}
