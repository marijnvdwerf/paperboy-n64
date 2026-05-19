#include "common.h"
#include "render_context.h"

struct ListNodeBase {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ // vptr

    virtual UNK vfunc1();
    virtual UNK vfunc2();
    virtual UNK vfunc3();
    virtual UNK vfunc4();
    virtual UNK vfunc5();
    virtual UNK vfunc6();
    virtual UNK vfunc7();
    virtual UNK vfunc8();
};

struct ListNode : ListNodeBase {
    /* 0x0C */ s32 unkC;
    /* 0x10 */ ListNode* next;
};

struct Inner {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ // vptr

    virtual UNK vfunc1();
    virtual UNK vfunc2();
    virtual UNK vfunc3();
    virtual UNK vfunc4();
    virtual UNK vfunc5();
    virtual UNK vfunc6();
    virtual UNK vfunc7();
    virtual UNK vfunc8();
    virtual void vfunc9(f32* a, f32* b);
    virtual UNK vfunc10();
    virtual UNK vfunc11();
    virtual UNK vfunc12();
    virtual UNK vfunc13();
    virtual UNK vfunc14();
    virtual UNK vfunc15();
    virtual UNK vfunc16();
    virtual UNK vfunc17(UNK);
};

struct Renderer {
    /* 0x00 */ Inner* inner;
    /* 0x04 */ u8 pad4[0x2C];
    /* 0x30 */ u32 data[51]; // 0x30..0xFC
};

extern "C" s32 func_80024C80(ListNode*, void*);
extern "C" void vec2_normalize(f32*, f32*);
extern "C" void vec3_normalize(f32*, f32*);
extern "C" const f32 D_8006C5F0[1024];

struct Vec3 {
    f32 x, y, z;
};

// Geometry result populated by Mover::vfunc6 (sp10..sp30 in the asm)
struct MoverGeom {
    /* 0x00 */ s32 status;
    /* 0x04 */ s32 idx;
    /* 0x08 */ f32 vec_a[3];
    /* 0x14 */ f32 pad14;
    /* 0x18 */ f32 vec_b[3];
};

// Result type of Mover::vfunc23 — vptr at offset 0x8, slot-7 returns an Inner with vptr at 0xC
struct MoverItemBase {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
};

struct MoverItem : MoverItemBase {
    /* 0x08 */ // vptr here
    virtual UNK vfunc1();
    virtual UNK vfunc2();
    virtual UNK vfunc3();
    virtual UNK vfunc4();
    virtual UNK vfunc5();
    virtual UNK vfunc6();
    virtual Inner* vfunc7(s32);
};

// arg1 of func_80025E7C / func_800260AC — has its vtable at offset 0x30
struct MoverBase {
    /* 0x00 */ char pad[0x30];
};

struct Mover : MoverBase {
    /* 0x30 */ // vptr here
    virtual UNK vfunc1();
    virtual UNK vfunc2();
    virtual UNK vfunc3();
    virtual UNK vfunc4();
    virtual UNK vfunc5();
    virtual void vfunc6(void* src, MoverGeom* out);
    virtual UNK vfunc7();
    virtual UNK vfunc8();
    virtual UNK vfunc9();
    virtual UNK vfunc10();
    virtual UNK vfunc11();
    virtual UNK vfunc12();
    virtual UNK vfunc13();
    virtual void vfunc14(f32* in, f32* out);
    virtual UNK vfunc15();
    virtual UNK vfunc16();
    virtual void vfunc17(f32* in, f32* out);
    virtual UNK vfunc18();
    virtual UNK vfunc19();
    virtual UNK vfunc20();
    virtual UNK vfunc21();
    virtual UNK vfunc22();
    virtual MoverItem* vfunc23(s32 idx);
    virtual void vfunc24(s32 idx);
};

void RenderContext::func_80024F30() {
    RenderContext* self = this;
    if (self->unk30 != NULL) {
        delete[] self->unk30;
        self->unk30 = NULL;
    }
    self->unk2C = 0;
    self->unkC = 0;
    self->unk10 = 0;
    self->unk14 = 0;
    self->unk18 = 0;
    self->unk1C = 0;
    self->unk20 = 0;
    self->unk24 = 0;
    {
        ListNode* n = self->unk38;
        while (n != NULL) {
            ListNode* next = n->next;
            n->vfunc3();
            n = next;
        }
    }
    {
        ListNode* n = self->unk34;
        while (n != NULL) {
            ListNode* next = n->next;
            n->vfunc3();
            n = next;
        }
    }
    {
        ListNode* n = self->unk3C;
        while (n != NULL) {
            ListNode* next = n->next;
            n->vfunc3();
            n = next;
        }
    }
    {
        ListNode* n = self->unk40;
        while (n != NULL) {
            ListNode* next = n->next;
            n->vfunc3();
            n = next;
        }
    }
}

void RenderContext::vfunc2() {
    RenderContext* self = this;
    if (self->unk30 != NULL) {
        delete[] self->unk30;
        self->unk30 = NULL;
    }
    self->unk2C = 0;
    {
        ListNode* n = self->unk38;
        while (n != NULL) {
            n->vfunc4();
            n = n->next;
        }
    }
    {
        ListNode* n = self->unk34;
        while (n != NULL) {
            n->vfunc4();
            n = n->next;
        }
    }
    {
        ListNode* n = self->unk3C;
        while (n != NULL) {
            n->vfunc5();
            n = n->next;
        }
    }
    {
        ListNode* n = self->unk40;
        while (n != NULL) {
            n->vfunc7();
            n = n->next;
        }
    }
    self->unkC = 0;
    self->unk10 = 0;
    self->unk14 = 0;
    self->unk18 = 0;
    self->unk1C = 0;
    self->unk20 = 0;
    self->unk24 = 0;
}

s32 RenderContext::vfunc1() {
    RenderContext* self = this;
    {
        ListNode* n = self->unk3C;
        while (n != NULL) {
            n->vfunc6();
            n = n->next;
        }
    }
    {
        ListNode* n = self->unk34;
        while (n != NULL) {
            n->vfunc5();
            n = n->next;
        }
    }
    {
        ListNode* n = self->unk38;
        while (n != NULL) {
            n->vfunc5();
            n = n->next;
        }
    }
    {
        ListNode* n = self->unk40;
        while (n != NULL) {
            n->vfunc8();
            n = n->next;
        }
    }
    return 0;
}

#ifdef NON_MATCHING
void RenderContext::vfunc4(PixelFormat* req, PixelFormat* out) {
    RenderContext* self = this;
    u32 r = req->bitWidthMaskRed();
    u32 g = req->bitWidthMaskGreen();
    u32 b = req->bitWidthMaskBlue();
    u32 a = req->bitWidthMaskAlpha();
    u32 u = req->bitWidthUnk10();
    u32 p = req->bitWidthPaletteMask();
    u32 depth = req->bitDepth;

    if (self->unk10 == r && self->unk14 == g && self->unk18 == b && self->unk1C == a && self->unk20 == u && self->unk24 == p) {
        *out = self->unk30[self->unkC];
        return;
    }
    self->unk10 = r;
    self->unk14 = g;
    self->unk18 = b;
    self->unk1C = a;
    self->unk20 = u;
    self->unk24 = p;

    if (a != 0) {
        if (p != 0) {
            // (M) exact depth + palette + alpha
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (e->bitDepth == depth && e->bitWidthPaletteMask() == p && e->bitWidthMaskAlpha() == a) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        } else if (u != 0) {
            // (L) exact depth + unk10 + alpha
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (e->bitDepth == depth && e->bitWidthUnk10() == u && e->bitWidthMaskAlpha() == a) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        } else {
            // (K) exact depth + RGB widths + alpha
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (e->bitDepth == depth && e->bitWidthMaskRed() == r && e->bitWidthMaskGreen() == g && e->bitWidthMaskBlue() == b && e->bitWidthMaskAlpha() == a) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        }
    } else {
        if (p != 0) {
            // (G) exact depth + palette
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (e->bitDepth == depth && e->bitWidthPaletteMask() == p) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        } else if (u != 0) {
            // (D) exact depth + unk10
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (e->bitDepth == depth && e->bitWidthUnk10() == u) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        } else {
            // (A) exact RGB mask values + depth
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (e->bitDepth == depth && e->maskRed == req->maskRed && e->maskGreen == req->maskGreen && e->maskBlue == req->maskBlue) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
            // (B) depth + RGB widths
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (e->bitDepth == depth && e->bitWidthMaskRed() == r && e->bitWidthMaskGreen() == g && e->bitWidthMaskBlue() == b) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        }
    }

    // Fuzzy level (block_67)
    if (a != 0) {
        if (p != 0) {
            // (Q) depth > + palette >= + alpha >=
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (depth < e->bitDepth && e->bitWidthPaletteMask() >= p && e->bitWidthMaskAlpha() >= a) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
            // (R) depth >= 16 + RGB >= 5 + alpha >=
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (e->bitDepth >= 0x10 && e->bitWidthMaskRed() >= 5 && e->bitWidthMaskGreen() >= 5 && e->bitWidthMaskBlue() >= 5 && e->bitWidthMaskAlpha() >= a) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        } else if (u != 0) {
            // (O) depth > + unk10 >= + alpha >=
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (depth < e->bitDepth && e->bitWidthUnk10() >= u && e->bitWidthMaskAlpha() >= a) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
            // (P) depth > + RGB >= u + alpha >=
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (depth < e->bitDepth && e->bitWidthMaskRed() >= u && e->bitWidthMaskGreen() >= u && e->bitWidthMaskBlue() >= u && e->bitWidthMaskAlpha() >= a) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        } else {
            // (N) depth > + RGB >= + alpha >=
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (depth < e->bitDepth && e->bitWidthMaskRed() >= r && e->bitWidthMaskGreen() >= g && e->bitWidthMaskBlue() >= b && e->bitWidthMaskAlpha() >= a) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        }
    } else {
        if (p != 0) {
            // (H) p<8 special: depth < 16 + palette >=
            if (p < 8)
                for (u32 i = 0; i < self->unk2C; i++) {
                    PixelFormat* e = &self->unk30[i];
                    if (e->bitDepth < 0x10 && e->bitWidthPaletteMask() >= p) {
                        *out = self->unk30[i];
                        self->unkC = i;
                        return;
                    }
                }
            // (I) depth == 16 + RGB >= 5
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (e->bitDepth == 0x10 && e->bitWidthMaskRed() >= 5 && e->bitWidthMaskGreen() >= 5 && e->bitWidthMaskBlue() >= 5) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        } else if (u != 0) {
            // (E) depth > + unk10 >=
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (depth < e->bitDepth && e->bitWidthUnk10() >= u) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
            // (F) depth > + RGB >= u
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (depth < e->bitDepth && e->bitWidthMaskRed() >= u && e->bitWidthMaskGreen() >= u && e->bitWidthMaskBlue() >= u) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        } else {
            // (C) depth > + RGB >=
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (depth < e->bitDepth && e->bitWidthMaskRed() >= r && e->bitWidthMaskGreen() >= g && e->bitWidthMaskBlue() >= b) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        }
    }

    // block_159
    if (a != 0) {
        // (S) alpha >= 4
        if (a >= 4)
            for (u32 i = 0; i < self->unk2C; i++) {
                PixelFormat* e = &self->unk30[i];
                if (e->bitWidthMaskAlpha() >= 4) {
                    *out = self->unk30[i];
                    self->unkC = i;
                    return;
                }
            }
        // (T) alpha != 0
        for (u32 i = 0; i < self->unk2C; i++) {
            PixelFormat* e = &self->unk30[i];
            if (e->bitWidthMaskAlpha() != 0) {
                *out = self->unk30[i];
                self->unkC = i;
                return;
            }
        }
    } else {
        // (J) depth >= 15 + RGB >= 5
        for (u32 i = 0; i < self->unk2C; i++) {
            PixelFormat* e = &self->unk30[i];
            if (e->bitDepth >= 0xF && e->bitWidthMaskRed() >= 5 && e->bitWidthMaskGreen() >= 5 && e->bitWidthMaskBlue() >= 5) {
                *out = self->unk30[i];
                self->unkC = i;
                return;
            }
        }
    }

    // (U) same depth
    for (u32 i = 0; i < self->unk2C; i++) {
        PixelFormat* e = &self->unk30[i];
        if (e->bitDepth == depth) {
            *out = self->unk30[i];
            self->unkC = i;
            return;
        }
    }
    // (V) larger depth
    for (u32 i = 0; i < self->unk2C; i++) {
        PixelFormat* e = &self->unk30[i];
        if (depth < e->bitDepth) {
            *out = self->unk30[i];
            self->unkC = i;
            return;
        }
    }

    // (W) entry[0] + cache reset
    self->unk10 = 0;
    self->unk14 = 0;
    self->unk18 = 0;
    self->unk1C = 0;
    self->unk20 = 0;
    self->unk24 = 0;
    *out = self->unk30[0];
    self->unkC = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/render_context", vfunc4__13RenderContextP11PixelFormatT1);
#endif

void RenderContext::vfunc41(Mover* arg1) {
    f32 fv1, fv0;
    MoverGeom geom;
    f32 sp38[3];
    f32 sp48[3];

    arg1->vfunc6(this->unk48, &geom);
    if (geom.status != 0) {
        MoverItem* item = arg1->vfunc23(geom.idx);
        if (item == NULL) {
            arg1->vfunc17(geom.vec_a, geom.vec_b);
        } else {
            arg1->vfunc24(geom.idx);
            Inner* inner = item->vfunc7(0);
            inner->vfunc9(sp38, sp48);
            arg1->vfunc14(sp38, geom.vec_a);
            arg1->vfunc14(sp48, geom.vec_b);
        }
        if (geom.vec_a[0] != 0.0f || geom.vec_a[1] != 0.0f) {
            f32 buf[2];
            buf[0] = geom.vec_a[0];
            buf[1] = geom.vec_a[1];
            vec2_normalize(buf, buf);
            s32 idx_x = ((s32)((buf[0] + 1.0f) * 511.5f)) & 0x3FF;
            fv1 = D_8006C5F0[idx_x] * 0.31830987f * 0.5f;
            if (buf[1] < 0.0f) {
                fv1 = 1.0f - fv1;
            }
            s32 idx_z = ((s32)((geom.vec_b[2] + 1.0f) * 511.5f)) & 0x3FF;
            fv0 = D_8006C5F0[idx_z] * 0.31830987f;
        } else {
            fv1 = 0.0f;
            fv0 = 0.5f;
        }
        this->vfunc43(arg1, fv1, fv0);
    }
}

#ifdef NON_MATCHING
extern "C" void func_800260AC(RenderContext* arg0, Mover* arg1, Vec3* arg2, Vec3* arg3, Vec3* arg4) {
    MoverGeom geom;
    Vec3 sp38, sp48;
    f32 fv1, fv0;

    arg1->vfunc6(arg0->unk48, &geom);
    if (geom.status != 0) {
        arg1->vfunc17(geom.vec_a, geom.vec_b);
        f32 dot = geom.vec_a[0] * arg4->x + geom.vec_a[1] * arg4->y + geom.vec_a[2] * arg4->z;
        sp48.x = arg4->x * dot;
        sp48.y = arg4->y * dot;
        sp48.z = arg4->z * dot;
        sp38.x = geom.vec_a[0] - sp48.x;
        sp38.y = geom.vec_a[1] - sp48.y;
        sp38.z = geom.vec_a[2] - sp48.z;
        f32 zero = 0.0f;
        if (sp38.x != zero || sp38.y != zero || sp38.z != zero) {
            vec3_normalize((f32*)&sp38, (f32*)&sp38);
            f32 dot_a2 = sp38.x * arg2->x + sp38.y * arg2->y + sp38.z * arg2->z;
            s32 idx_a = ((s32)((dot_a2 + 1.0f) * 511.5f)) & 0x3FF;
            f32 lut_a = D_8006C5F0[idx_a] * 0.31830987f;
            f32 dot_a3 = sp38.x * arg3->x + sp38.y * arg3->y + sp38.z * arg3->z;
            fv1 = lut_a * 0.5f;
            if (dot_a3 < zero) {
                fv1 = 1.0f - fv1;
            }
            f32 dot_b4 = geom.vec_b[0] * arg4->x + geom.vec_b[1] * arg4->y + geom.vec_b[2] * arg4->z;
            s32 idx_b = ((s32)((dot_b4 + 1.0f) * 511.5f)) & 0x3FF;
            fv0 = D_8006C5F0[idx_b] * 0.31830987f;
        } else {
            fv1 = zero;
            fv0 = 0.5f;
        }
        arg0->vfunc43(arg1, fv1, fv0);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/render_context", vfunc42__13RenderContextP5MoverP4Vec3N22);
#endif

void RenderContext::vfunc10() {
    this->unk8 = 0;
}

void RenderContext::vfunc22(UNK, UNK arg) {
    this->vfunc23(arg);
}

void RenderContext::vfunc21() {
}

s32 RenderContext::vfunc20() {
    return 0;
}

void RenderContext::vfunc19() {
    this->flags &= ~0x40000;
}

void RenderContext::vfunc18() {
    this->flags |= 0x40000;
}

void RenderContext::vfunc17() {
}

void RenderContext::vfunc16() {
}

void RenderContext::vfunc59() {
}

void RenderContext::vfunc15() {
}

void RenderContext::vfunc14() {
}

void RenderContext::vfunc51() {
}

void RenderContext::vfunc50() {
}

void RenderContext::vfunc25() {
}

void RenderContext::vfunc13(s32 val) {
    if (this->unk118 < 3) {
        this->flags |= 0x8000;
        this->unk120[this->unk118++] = val;
    }
}

void RenderContext::vfunc12(s32 val) {
    this->unk11C = val;
    this->flags |= 0x8000;
}

void RenderContext::vfunc11() {
    this->unk118 = 0;
    this->unk11C = 0;
    this->unk120[0] = 0;
    this->flags &= ~0x8000;
}

void RenderContext::vfunc49() {
    this->flags &= ~0x80000;
}

void RenderContext::vfunc48(const char* src) {
    memcpy(&this->unk114, src, 4);
    this->flags |= 0x80000;
}

void RenderContext::vfunc47() {
    this->flags &= ~0x4000;
}

void RenderContext::vfunc46(s16 a, s16 b) {
    this->unk6 = a;
    this->unk4 = b;
    this->flags |= 0x4000;
}

void RenderContext::vfunc43(Mover* arg, f32 f1, f32 f2) {
    this->vfunc35((UNK)arg);
}

void RenderContext::vfunc40() {
}

void RenderContext::vfunc39() {
}

void RenderContext::vfunc38() {
}

void RenderContext::vfunc37() {
}

void RenderContext::vfunc61() {
}

s32 RenderContext::vfunc68() {
    return 0;
}

s32 RenderContext::func_800264FC(void* arg) {
    RenderContext* self = this;
    ListNode* n = self->unk38;
    s32 result;
    while (1) {
        if (n == NULL) {
            result = 0;
            break;
        }
        if (n->unk0 == 0)
            result = 0;
        else
            result = func_80024C80(n, arg);
        if (result != 0)
            break;
        n = n->next;
    }
    return result;
}

void RenderContext::func_80026560(ListNode* node) {
    RenderContext* self = this;
    ListNode* head = self->unk38;
    if (head == NULL)
        return;
    if (node == head) {
        self->unk38 = node->next;
        return;
    }
    ListNode* prev = head;
    ListNode* curr = head->next;
    while (curr != NULL) {
        if (curr == node) {
            prev->next = curr->next;
            curr->next = NULL;
            return;
        }
        ListNode* next = curr->next;
        prev = curr;
        curr = next;
    }
}

void RenderContext::func_800265BC(ListNode* node) {
    node->next = this->unk38;
    this->unk38 = node;
}

s32 RenderContext::func_800265CC(void* arg) {
    RenderContext* self = this;
    ListNode* n = self->unk34;
    s32 result;
    while (1) {
        if (n == NULL) {
            result = 0;
            break;
        }
        if (n->unk0 == 0)
            result = 0;
        else
            result = func_80024C80(n, arg);
        if (result != 0)
            break;
        n = n->next;
    }
    return result;
}

void RenderContext::func_80026630(ListNode* node) {
    RenderContext* self = this;
    ListNode* head = self->unk34;
    if (head == NULL)
        return;
    if (node == head) {
        self->unk34 = node->next;
        return;
    }
    ListNode* prev = head;
    ListNode* curr = head->next;
    while (curr != NULL) {
        if (curr == node) {
            prev->next = curr->next;
            curr->next = NULL;
            return;
        }
        ListNode* next = curr->next;
        prev = curr;
        curr = next;
    }
}

void RenderContext::func_8002668C(ListNode* node) {
    node->next = this->unk34;
    this->unk34 = node;
}

s32 RenderContext::func_8002669C(void* arg) {
    RenderContext* self = this;
    ListNode* n = self->unk3C;
    s32 result;
    while (1) {
        if (n == NULL) {
            result = 0;
            break;
        }
        if (n->unk0 == 0)
            result = 0;
        else
            result = func_80024C80(n, arg);
        if (result != 0)
            break;
        n = n->next;
    }
    return result;
}

void RenderContext::func_80026700(ListNode* node) {
    RenderContext* self = this;
    ListNode* head = self->unk3C;
    if (head == NULL)
        return;
    if (node == head) {
        self->unk3C = node->next;
        return;
    }
    ListNode* prev = head;
    ListNode* curr = head->next;
    while (curr != NULL) {
        if (curr == node) {
            prev->next = curr->next;
            curr->next = NULL;
            return;
        }
        ListNode* next = curr->next;
        prev = curr;
        curr = next;
    }
}

void RenderContext::func_8002675C(ListNode* node) {
    node->next = this->unk3C;
    this->unk3C = node;
}

s32 RenderContext::func_8002676C(void* arg) {
    RenderContext* self = this;
    ListNode* n = self->unk40;
    s32 result;
    while (1) {
        if (n == NULL) {
            result = 0;
            break;
        }
        if (n->unk0 == 0)
            result = 0;
        else
            result = func_80024C80(n, arg);
        if (result != 0)
            break;
        n = n->next;
    }
    return result;
}

void RenderContext::func_800267D0(ListNode* node) {
    RenderContext* self = this;
    ListNode* head = self->unk40;
    if (head == NULL)
        return;
    if (node == head) {
        self->unk40 = node->next;
        return;
    }
    ListNode* prev = head;
    ListNode* curr = head->next;
    while (curr != NULL) {
        if (curr == node) {
            prev->next = curr->next;
            curr->next = NULL;
            return;
        }
        ListNode* next = curr->next;
        prev = curr;
        curr = next;
    }
}

void RenderContext::func_8002682C(ListNode* node) {
    node->next = this->unk40;
    this->unk40 = node;
}

RenderContext::~RenderContext() {
    this->func_80024F30();
}

RenderContext::RenderContext() {
    unkC = 0;
    unk10 = 0;
    unk14 = 0;
    unk18 = 0;
    unk1C = 0;
    unk20 = 0;
    unk24 = 0;
    unk2C = 0;
    unk30 = 0;
    unk34 = 0;
    unk38 = 0;
    unk3C = 0;
    unk40 = 0;
    unk44 = 0;
    unk8 = 0;
    flags = 0;
    unk6 = 0;
    memset(unk48, 0, 0xCC);
    unk118 = 0;
    unk11C = 0;
    memset(unk120, 0, 0xC);
    unk120[0] = 0;
}

s32 RenderContext::func_8002692C() {
    return this->flags & 0x80000;
}

s32 RenderContext::func_8002693C() {
    return this->flags & 0x40000;
}

s32 RenderContext::func_8002694C() {
    return this->flags & 0x20000;
}

s32 RenderContext::func_8002695C() {
    return this->flags & 0x10000;
}

s32 RenderContext::func_8002696C() {
    return this->flags & 0x8000;
}

u16 RenderContext::func_80026978() {
    return this->unk6;
}

u16 RenderContext::func_80026984() {
    return this->unk4;
}

s32 RenderContext::func_80026990() {
    return this->flags & 0x4000;
}

void RenderContext::func_8002699C(UNK arg) {
    this->unk8->inner->vfunc17(arg);
}

void RenderContext::func_800269D0(u32* dst) {
    memcpy(dst, this->unk8->data, 60);
}

void* RenderContext::func_80026A24() {
    return this->unk48;
}

s32 RenderContext::func_80026A2C(s32 i) {
    return this->unk120[i];
}

s32 RenderContext::func_80026A40() {
    return this->unk118;
}

s32 RenderContext::func_80026A4C() {
    return this->unk11C;
}

Renderer* RenderContext::func_80026A58() {
    return this->unk8;
}

void RenderContext::func_80026A64(u32* dst) {
    memcpy(dst, this->unk8->data, 0xCC);
}

s32 RenderContext::func_80026AB8() {
    return this->flags & 0x1000;
}

s32 RenderContext::func_80026AC4() {
    return this->flags & 0x800;
}

s32 RenderContext::func_80026AD0() {
    return this->flags & 0x200;
}

s32 RenderContext::func_80026ADC() {
    return this->flags & 0x100;
}

s32 RenderContext::func_80026AE8() {
    return this->flags & 0x80;
}

s32 RenderContext::func_80026AF4() {
    return this->flags & 0x180;
}

s32 RenderContext::func_80026B00() {
    return this->flags & 0x40;
}

s32 RenderContext::func_80026B0C() {
    return this->flags & 0x20;
}

s32 RenderContext::func_80026B18() {
    return this->flags & 0x2;
}

s32 RenderContext::func_80026B24() {
    return this->flags & 0x1;
}

s32 RenderContext::func_80026B30() {
    return this->unk44;
}

void RenderContext::func_80026B3C(s32 v) {
    this->unk44 = v;
}
