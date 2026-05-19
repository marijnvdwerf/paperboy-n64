#include "common.h"
#include "render_context.h"

struct ListNodeBase {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ // vptr inserted here by cfront (vptr at end of polymorphic base)

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

struct InnerBase {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
};

struct Inner : InnerBase {
    /* 0x0C */ // vptr here (12 bytes of base, then virtuals)
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
extern "C" void func_80024F30(RenderContext*);
extern "C" void vec2_normalize(f32*, f32*);
extern "C" void vec3_normalize(f32*, f32*);
extern "C" const f32 D_8006C5F0[1024];

struct Vec3 { f32 x, y, z; };

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

extern "C" void func_80024F30(RenderContext* self) {
    if (self->unk30 != NULL) {
        delete[] (s32*)self->unk30;
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

extern "C" void func_80025050(RenderContext* self) {
    if (self->unk30 != NULL) {
        delete[] (s32*)self->unk30;
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

extern "C" s32 func_8002515C(RenderContext* self) {
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

// Large pixel-format dispatch (~250 instructions, nested loops with goto-heavy m2c output).
// Iterates self->unk30 (PixelFormatCache[]) to find a matching entry against arg1 (PixelFormat*)
// using bitWidthMaskRed/Green/Blue/Alpha/Unk10/PaletteMask helpers. Multi-branch fallback chain.
// Needs proper PixelFormat/PixelFormatCache types defined before attempting.
INCLUDE_ASM("asm/nonmatchings/render_context", func_80025238);

// Algorithm matches Lego Racers Windows equivalent (entity OrientToCameraBasis vtable@4dbb78 idx 0xE):
// fetch reference vec3 + idx via Mover::vfunc6, conditionally transform via Mover::vfunc17 or
// via MoverItem::vfunc7 -> Inner::vfunc9 -> Mover::vfunc14 pair, then compute 2-axis angles via
// atan2 LUT and dispatch back via RenderContext::vfunc43.
extern "C" void func_80025E7C(RenderContext* arg0, Mover* arg1) {
    f32 fv1, fv0;
    MoverGeom geom;
    f32 sp38[3];
    f32 sp48[3];

    arg1->vfunc6(arg0->unk48, &geom);
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
        arg0->vfunc43(arg1, fv1, fv0);
    }
}

// score 1035; arg3 dot product hoisted before LUT compute (cfront schedules arg3 loads to fill LUT
// prep latency, target keeps them after the LUT lookup). Same instructions, scheduling differs.
#ifdef NON_MATCHING
extern "C" void func_800260AC(RenderContext* arg0, Mover* arg1, Vec3* arg2, Vec3* arg3, Vec3* arg4) {
    MoverGeom geom;
    Vec3 sp38, sp48;
    f32 fv1, fv0;

    arg1->vfunc6(arg0->unk48, &geom);
    if (geom.status != 0) {
        arg1->vfunc17(geom.vec_a, geom.vec_b);
        f32 dot = geom.vec_a[0]*arg4->x + geom.vec_a[1]*arg4->y + geom.vec_a[2]*arg4->z;
        sp48.x = arg4->x * dot;
        sp48.y = arg4->y * dot;
        sp48.z = arg4->z * dot;
        sp38.x = geom.vec_a[0] - sp48.x;
        sp38.y = geom.vec_a[1] - sp48.y;
        sp38.z = geom.vec_a[2] - sp48.z;
        f32 zero = 0.0f;
        if (sp38.x != zero || sp38.y != zero || sp38.z != zero) {
            vec3_normalize((f32*)&sp38, (f32*)&sp38);
            f32 dot_a2 = sp38.x*arg2->x + sp38.y*arg2->y + sp38.z*arg2->z;
            s32 idx_a = ((s32)((dot_a2 + 1.0f) * 511.5f)) & 0x3FF;
            f32 lut_a = D_8006C5F0[idx_a] * 0.31830987f;
            f32 dot_a3 = sp38.x*arg3->x + sp38.y*arg3->y + sp38.z*arg3->z;
            fv1 = lut_a * 0.5f;
            if (dot_a3 < zero) {
                fv1 = 1.0f - fv1;
            }
            f32 dot_b4 = geom.vec_b[0]*arg4->x + geom.vec_b[1]*arg4->y + geom.vec_b[2]*arg4->z;
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
INCLUDE_ASM("asm/nonmatchings/render_context", func_800260AC);
#endif

extern "C" void func_80026310(RenderContext* self) {
    self->unk8 = 0;
}

// calls vfunc23
extern "C" void func_80026318(RenderContext* self, UNK, UNK arg) {
    self->vfunc23(arg);
}

extern "C" void func_80026344(void) {
}

// vfunc20
extern "C" s32 func_8002634C(RenderContext*) {
    return 0;
}

extern "C" void func_80026354(RenderContext* self) {
    self->flags &= ~0x40000;
}

extern "C" void func_8002636C(RenderContext* self) {
    self->flags |= 0x40000;
}

extern "C" void func_80026380(void) {
}

extern "C" void func_80026388(void) {
}

extern "C" void func_80026390(void) {
}

extern "C" void func_80026398(void) {
}

extern "C" void func_800263A0(void) {
}

extern "C" void func_800263A8(void) {
}

extern "C" void func_800263B0(void) {
}

extern "C" void func_800263B8(void) {
}

extern "C" void func_800263C0(RenderContext* self, s32 val) {
    if (self->unk118 < 3) {
        self->flags |= 0x8000;
        self->unk120[self->unk118++] = val;
    }
}

extern "C" void func_80026400(RenderContext* self, s32 val) {
    self->unk11C = val;
    self->flags |= 0x8000;
}

extern "C" void func_80026414(RenderContext* self) {
    self->unk118 = 0;
    self->unk11C = 0;
    self->unk120[0] = 0;
    self->flags &= ~0x8000;
}

extern "C" void func_80026438(RenderContext* self) {
    self->flags &= ~0x80000;
}

extern "C" void func_80026450(RenderContext* self, const char* src) {
    memcpy(&self->unk114, src, 4);
    self->flags |= 0x80000;
}

extern "C" void func_80026474(RenderContext* self) {
    self->flags &= ~0x4000;
}

extern "C" void func_80026488(RenderContext* self, s16 a, s16 b) {
    self->unk6 = a;
    self->unk4 = b;
    self->flags |= 0x4000;
}

// calls vfunc35
extern "C" void func_800264A0(RenderContext* self, UNK arg) {
    self->vfunc35(arg);
}

extern "C" void func_800264CC(void) {
}

extern "C" void func_800264D4(void) {
}

extern "C" void func_800264DC(void) {
}

extern "C" void func_800264E4(void) {
}

extern "C" void func_800264EC(void) {
}

// vfunc68
extern "C" s32 func_800264F4(RenderContext*) {
    return 0;
}

extern "C" s32 func_800264FC(RenderContext* self, void* arg) {
    ListNode* n = self->unk38;
    s32 result;
    while (1) {
        if (n == NULL) { result = 0; break; }
        if (n->unk0 == 0) result = 0;
        else result = func_80024C80(n, arg);
        if (result != 0) break;
        n = n->next;
    }
    return result;
}

extern "C" void func_80026560(RenderContext* self, ListNode* node) {
    ListNode* head = self->unk38;
    if (head == NULL) return;
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

extern "C" void func_800265BC(RenderContext* self, ListNode* node) {
    node->next = self->unk38;
    self->unk38 = node;
}

extern "C" s32 func_800265CC(RenderContext* self, void* arg) {
    ListNode* n = self->unk34;
    s32 result;
    while (1) {
        if (n == NULL) { result = 0; break; }
        if (n->unk0 == 0) result = 0;
        else result = func_80024C80(n, arg);
        if (result != 0) break;
        n = n->next;
    }
    return result;
}

extern "C" void func_80026630(RenderContext* self, ListNode* node) {
    ListNode* head = self->unk34;
    if (head == NULL) return;
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

extern "C" void func_8002668C(RenderContext* self, ListNode* node) {
    node->next = self->unk34;
    self->unk34 = node;
}

extern "C" s32 func_8002669C(RenderContext* self, void* arg) {
    ListNode* n = self->unk3C;
    s32 result;
    while (1) {
        if (n == NULL) { result = 0; break; }
        if (n->unk0 == 0) result = 0;
        else result = func_80024C80(n, arg);
        if (result != 0) break;
        n = n->next;
    }
    return result;
}

extern "C" void func_80026700(RenderContext* self, ListNode* node) {
    ListNode* head = self->unk3C;
    if (head == NULL) return;
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

extern "C" void func_8002675C(RenderContext* self, ListNode* node) {
    node->next = self->unk3C;
    self->unk3C = node;
}

extern "C" s32 func_8002676C(RenderContext* self, void* arg) {
    ListNode* n = self->unk40;
    s32 result;
    while (1) {
        if (n == NULL) { result = 0; break; }
        if (n->unk0 == 0) result = 0;
        else result = func_80024C80(n, arg);
        if (result != 0) break;
        n = n->next;
    }
    return result;
}

extern "C" void func_800267D0(RenderContext* self, ListNode* node) {
    ListNode* head = self->unk40;
    if (head == NULL) return;
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

extern "C" void func_8002682C(RenderContext* self, ListNode* node) {
    node->next = self->unk40;
    self->unk40 = node;
}

RenderContext::~RenderContext() {
    func_80024F30(this);
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

extern "C" s32 func_8002692C(RenderContext* self) { return self->flags & 0x80000; }

extern "C" s32 func_8002693C(RenderContext* self) { return self->flags & 0x40000; }

extern "C" s32 func_8002694C(RenderContext* self) { return self->flags & 0x20000; }

extern "C" s32 func_8002695C(RenderContext* self) { return self->flags & 0x10000; }

extern "C" s32 func_8002696C(RenderContext* self) { return self->flags & 0x8000; }

extern "C" u16 func_80026978(RenderContext* self) { return self->unk6; }

extern "C" u16 func_80026984(RenderContext* self) { return self->unk4; }

extern "C" s32 func_80026990(RenderContext* self) { return self->flags & 0x4000; }

extern "C" void func_8002699C(RenderContext* self, UNK arg) {
    self->unk8->inner->vfunc17(arg);
}

extern "C" void func_800269D0(RenderContext* self, u32* dst) {
    memcpy(dst, self->unk8->data, 60);
}

extern "C" void* func_80026A24(RenderContext* self) { return self->unk48; }

extern "C" s32 func_80026A2C(RenderContext* self, s32 i) { return self->unk120[i]; }

extern "C" s32 func_80026A40(RenderContext* self) { return self->unk118; }

extern "C" s32 func_80026A4C(RenderContext* self) { return self->unk11C; }

extern "C" Renderer* func_80026A58(RenderContext* self) { return self->unk8; }

extern "C" void func_80026A64(RenderContext* self, u32* dst) {
    memcpy(dst, self->unk8->data, 0xCC);
}

extern "C" s32 func_80026AB8(RenderContext* self) { return self->flags & 0x1000; }

extern "C" s32 func_80026AC4(RenderContext* self) { return self->flags & 0x800; }

extern "C" s32 func_80026AD0(RenderContext* self) { return self->flags & 0x200; }

extern "C" s32 func_80026ADC(RenderContext* self) { return self->flags & 0x100; }

extern "C" s32 func_80026AE8(RenderContext* self) { return self->flags & 0x80; }

extern "C" s32 func_80026AF4(RenderContext* self) { return self->flags & 0x180; }

extern "C" s32 func_80026B00(RenderContext* self) { return self->flags & 0x40; }

extern "C" s32 func_80026B0C(RenderContext* self) { return self->flags & 0x20; }

extern "C" s32 func_80026B18(RenderContext* self) { return self->flags & 0x2; }

extern "C" s32 func_80026B24(RenderContext* self) { return self->flags & 0x1; }

extern "C" s32 func_80026B30(RenderContext* self) { return self->unk44; }

extern "C" void func_80026B3C(RenderContext* self, s32 v) { self->unk44 = v; }

INCLUDE_RODATA("asm/nonmatchings/render_context", _vt.13RenderContext);

INCLUDE_RODATA("asm/nonmatchings/render_context", func_80001FB4);

INCLUDE_RODATA("asm/nonmatchings/render_context", func_80001FC8);

INCLUDE_RODATA("asm/nonmatchings/render_context", func_80001FD8);
