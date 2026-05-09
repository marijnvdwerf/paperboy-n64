#include "common.h"

struct UnkResult;

struct Obj {
    char pad0[0x8];
    s32 state;
    virtual void vfunc1();
    virtual void vfunc2(s32 arg);
    virtual void vfunc3();
    virtual void vfunc4();
};

struct SceneNode {
    char pad0[0x8];
    Obj* child;
};

struct UnkStruct48 {
    char pad0[0x9C];
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9();
    virtual void vfunc10();
    virtual void vfunc11();
    virtual void vfunc12();
    virtual void vfunc13();
    virtual UnkResult* vfunc14();
};

struct UnkStructTop {
    char pad0[0x48];
    UnkStruct48* unk48;
};

struct UnkResult {
    char pad0[0x34];
    s32 unk34;
};

struct Actor {
    char pad0[0x8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    char pad14[0x108];
    s32 unk11C;
    char pad120[0x494];
    s32 unk5B4;
};

extern "C" {

void func_800081A0(s32 arg);
void func_80008C58(Actor* actor);
s32 func_80008C68(Actor* actor);
s32 func_80008C74(Actor* actor);
void func_80008C80(Actor* actor, s32 arg);
void func_800090BC(s32 arg);
void func_8004B0D4(void* a, s32 b);
void* func_8004B414(s32 size);
SceneNode* func_800BFF2C(void* mem);
void func_800BFDC0(SceneNode* node);
void func_800BFE30(SceneNode* node);
void func_800BFE5C(SceneNode* node, Actor* actor);
void func_800BFF00(SceneNode* node, s32 arg);
s32 func_800C02F4();
Obj* func_800C1094(void* mem);
Obj* func_800C15F8(void* mem);
Obj* func_800C1944(void* mem);
Obj* func_800C1E60(void* mem);
Obj* func_800C22EC(void* mem);
Obj* func_800C26D4(void* mem);

#ifndef PAL
extern s32 D_80000300;
extern s32 D_8006AAE4;
extern UnkStructTop* D_8006AB04;
extern s32 D_8006AB10;
extern s32 D_800768F0;
extern s32 D_80125894;
extern SceneNode* D_801295E0;

void func_800BFF50(Actor* self) {
    s32 state;
    s32 var_s0;
    UnkResult* result;
    Obj* child;
    UnkStruct48* unk48;

    D_801295E0 = func_800BFF2C(func_8004B414(0xC));
    if (func_80008C68(self) == 7) {
        unk48 = D_8006AB04->unk48;
        result = unk48->vfunc14();
        if (self->unkC == 0) {
            var_s0 = 0;
            if (func_800C02F4() == 0 && self->unk10 == 0) {
                var_s0 = (u32)result->unk34 > 0U;
            }
            if (var_s0 != 0) {
                if (D_80000300 != 0) {
                    func_80008C80(self, 1);
                    if (self->unk5B4 == 0) {
                        func_80008C80(self, 6);
                    }
                    if (self->unk11C == 0) {
                        func_80008C80(self, 5);
                    }
                } else {
                    self->unk8 = 1;
                }
            } else {
                self->unk8 = 1;
            }
        }
    }
    func_80008C58(self);
    func_8004B0D4(&D_800768F0, D_8006AAE4);
    child = NULL;
    state = func_80008C74(self);
    switch (state) {
        case 5:
            child = func_800C15F8(func_8004B414(0x28));
            break;
        case 4:
            child = func_800C1944(func_8004B414(0x24));
            break;
        case 3:
            child = func_800C26D4(func_8004B414(0x28));
            break;
        case 1:
            child = func_800C1E60(func_8004B414(0x28));
            break;
        case 7:
            child = func_800C22EC(func_8004B414(0x24));
            break;
        case 6:
            child = func_800C1094(func_8004B414(0x238));
            break;
    }
    func_800090BC(D_8006AB10);
    if (child != NULL) {
        child->state = state;
    }
    D_801295E0->child = child;
    func_800BFE5C(D_801295E0, self);
    if (child != NULL) {
        child->vfunc3();
    }
    func_800081A0(D_80125894);
    func_800BFE30(D_801295E0);
    if (child != NULL) {
        child->vfunc4();
        child->vfunc2(3);
    }
    func_800BFDC0(D_801295E0);
    if (D_801295E0 != NULL) {
        func_800BFF00(D_801295E0, 3);
    }
}

INCLUDE_ASM("asm/nonmatchings/88330", func_800C01EC);
#else
TEXT_PAD(0x2D0);
#endif

} // extern "C"