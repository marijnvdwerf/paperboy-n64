#include "dugong.h"

extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strcat(char* dst, const char* src);

extern "C" const char D_80001620[]; // "\nFile: "
extern "C" const char D_80001628[]; // ""
extern "C" const char D_800016A4[]; // ".img"

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001B230__6DugongPUcT1);

#ifdef NON_MATCHING
s32 Dugong::func_8001B5C8(u8* color) {
    u8* c;
    u32 i = 0;
    u32 bound = this->unk0;
    if (bound != 0) {
        c = this->unkC;
        do {
            if (color[0] == c[0] && color[1] == c[1] && color[2] == c[2]) {
                return i;
            }
            i++;
            c += 4;
        } while (i < bound);
    }
    bound = this->unk8;
    i = bound - this->unk4;
    if (i < bound) {
        c = this->unkC + i * 4;
        do {
            if (color[0] == c[0] && color[1] == c[1] && color[2] == c[2]) {
                return i;
            }
            i++;
            c += 4;
        } while (i < bound);
    }
    if ((u32)(this->unk0 + this->unk4) >= (u32)this->unk8) {
        s32 best = 0;
        s32 minDist = 0x7FFFFFFF;
        i = 0;
        bound = this->unk8;
        if (bound != 0) {
            u8 q0 = color[0];
            u8 q1 = color[1];
            u8 q2 = color[2];
            c = this->unkC;
            do {
                s32 dr = c[0] - q0;
                s32 dg = c[1] - q1;
                s32 db = c[2] - q2;
                s32 dist = dr * dr + dg * dg + db * db;
                if (dist < minDist) {
                    best = i;
                    minDist = dist;
                }
                i++;
                c += 4;
            } while (i < bound);
        }
        return best;
    }
    s32 idx = this->unk0;
    this->unkC[idx * 4 + 0] = color[0];
    this->unkC[idx * 4 + 1] = color[1];
    this->unkC[idx * 4 + 2] = color[2];
    this->unkC[idx * 4 + 3] = color[3];
    this->unk0 = this->unk0 + 1;
    return idx;
}
#else
INCLUDE_ASM("asm/nonmatchings/dugong", func_8001B5C8__6DugongPUc);
#endif

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
                    func_8001BF54(src, dst);
                    break;
                case 24:
                    func_8001C158(src, dst);
                    break;
                case 32:
                    func_8001C3AC(src, dst);
                    break;
            }
        }
    }
}

#ifdef NON_MATCHING
void Dugong::func_8001BD54(u8* src, u8* dst) {
    u32 b0, b1, b2, b3, pixel, cfg;
    u32 red, green, blue;
    u32 color;
    u8* end = dst + this->unk40;
    if (this->unk94 != 0) {
        while (dst < end) {
            cfg = this->unk6C;
            b0 = *src;
            src += cfg & 1;
            b1 = *src;
            src += (cfg >> 1) & 1;
            b2 = *src;
            src += (cfg >> 2) & 1;
            b3 = *src;
            src += (cfg >> 3) & 1;
            pixel = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
            red = (pixel & this->pixelFormat.maskRed) >> this->unk74;
            green = (pixel & this->pixelFormat.maskGreen) >> this->unk78;
            blue = (pixel & this->pixelFormat.maskBlue) >> this->unk7C;
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
            cfg = this->unk6C;
            b0 = *src;
            src += cfg & 1;
            b1 = *src;
            src += (cfg >> 1) & 1;
            b2 = *src;
            src += (cfg >> 2) & 1;
            b3 = *src;
            src += (cfg >> 3) & 1;
            pixel = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
            red = (pixel & this->pixelFormat.maskRed) >> this->unk74;
            green = (pixel & this->pixelFormat.maskGreen) >> this->unk78;
            blue = (pixel & this->pixelFormat.maskBlue) >> this->unk7C;
            u32 alpha = (pixel & this->pixelFormat.maskAlpha) >> this->unk80;
            color = red << this->unk84;
            color |= green << this->unk88;
            color |= blue << this->unk8C;
            color |= alpha << this->unk90;
            *dst++ = color;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/dugong", func_8001BD54__6DugongPUcT1);
#endif

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001BF54__6DugongPUcT1);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001C158__6DugongPUcT1);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001C3AC__6DugongPUcT1);

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

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001D738__6DugongPUcN21);

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
            func_8001D738(src, src + this->unk48, dst);
            src += srcStride2;
            dst += dstStride;
        }
    }
}

void Dugong::func_8001DEC0(u8* src, u8* dst, u32 dstWidth, u32 dstHeight, s32 dstStride, PixelFormat* dstPf, u8* palette, s32 flipVertical, u8* transColor) {
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

void Dugong::vfunc9() {
}

void Dugong::vfunc8() {
}

void Dugong::vfunc1() {
}

void Dugong::vfunc7() {
}

void Dugong::vfunc6() {
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
