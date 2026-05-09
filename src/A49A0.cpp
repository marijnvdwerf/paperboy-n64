#include "common.h"

struct SceneNode;

struct GameObj {
    char pad0[0x12C];
    virtual void vfunc_01();
    virtual void vfunc_02();
    virtual void vfunc_03();
    virtual void vfunc_04();
    virtual void vfunc_05();
    virtual void vfunc_06();
    virtual void vfunc_07();
    virtual void vfunc_08();
    virtual void vfunc_09(void* arg);
    virtual void vfunc_10();
    virtual void vfunc_11();
    virtual void vfunc_12();
    virtual void vfunc_13();
    virtual void vfunc_14();
    virtual void vfunc_15();
    virtual void vfunc_16();
    virtual void vfunc_17();
    virtual void vfunc_18();
    virtual void vfunc_19();
    virtual void vfunc_20();
    virtual void vfunc_21();
    virtual void vfunc_22();
    virtual void vfunc_23();
    virtual void vfunc_24();
    virtual void vfunc_25();
    virtual void vfunc_26();
    virtual void vfunc_27();
    virtual void vfunc_28();
    virtual void vfunc_29();
    virtual void vfunc_30();
    virtual void vfunc_31();
    virtual void vfunc_32();
    virtual void vfunc_33();
    virtual void vfunc_34();
    virtual void vfunc_35();
    virtual void vfunc_36();
    virtual void vfunc_37();
    virtual void vfunc_38();
    virtual void vfunc_39();
    virtual void vfunc_40();
    virtual void vfunc_41();
    virtual void vfunc_42();
    virtual void vfunc_43();
    virtual void vfunc_44();
    virtual void vfunc_45();
    virtual void vfunc_46();
    virtual void vfunc_47();
    virtual void vfunc_48();
    virtual void vfunc_49();
    virtual void vfunc_50();
    virtual void vfunc_51();
    virtual void vfunc_52();
    virtual void vfunc_53();
    virtual void vfunc_54();
    virtual void vfunc_55();
    virtual void vfunc_56();
    virtual void vfunc_57();
    virtual void vfunc_58(s32 arg);
};

struct GameObjChildBase {
    char pad0[0x30];
    virtual void vfunc_01();
    virtual void vfunc_02();
    virtual void vfunc_03();
    virtual void vfunc_04();
    virtual void vfunc_05();
    virtual void vfunc_06();
    virtual void vfunc_07();
    virtual void vfunc_08();
    virtual void vfunc_09();
    virtual void vfunc_10();
    virtual void vfunc_11();
    virtual void vfunc_12();
    virtual void vfunc_13();
    virtual void vfunc_14();
    virtual void vfunc_15();
    virtual void vfunc_16();
    virtual void vfunc_17();
    virtual void vfunc_18();
    virtual void vfunc_19();
    virtual void vfunc_20();
    virtual void vfunc_21();
    virtual void vfunc_22();
    virtual void vfunc_23();
    virtual void vfunc_24();
    virtual void vfunc_25();
    virtual void vfunc_26();
    virtual void vfunc_27();
};

struct GameObjChild : GameObjChildBase {
    char pad34[0x60];
    s32 unk94;
};

struct GameObjChild2 {
    char pad0[0xC];
    virtual void vfunc_01();
    virtual void vfunc_02();
    virtual void vfunc_03();
    virtual void vfunc_04();
    virtual void vfunc_05();
    virtual void vfunc_06();
    virtual s32 vfunc_07();
    virtual void vfunc_08();
    virtual void vfunc_09();
    virtual void vfunc_10(void* a1, void* a2);
    virtual void vfunc_11();
    virtual void vfunc_12();
    virtual void vfunc_13();
    virtual void vfunc_14();
    virtual void vfunc_15();
    virtual void vfunc_16();
    virtual void vfunc_17();
    virtual void vfunc_18(void* a1);
};

struct SceneChild {
    GameObjChild2* child2;
    char pad4[0xC];
    f32 unk10;
    char pad14[0x18];
    s32 unk2C;
    char pad30[0xEC];
    virtual void vfunc_01();
    virtual void vfunc_02();
    virtual void vfunc_03();
    virtual void vfunc_04();
    virtual void vfunc_05(void* arg);
};

struct SceneEntry {
    char pad0[0x8];
    void* unk8;
    SceneChild* unkC;
};

struct GameScene {
    char pad0[0x8];
    GameObjChild2* child;
};

struct GameMiddle {
    char pad0[0x78];
    GameObj* unk78;
};

struct GameTop {
    char pad0[0x48];
    GameMiddle* unk48;
};

struct UnkArgStruct {
    char pad0[0xC];
    SceneNode** children;
    u32 capacity;
    u32 count;
    char pad18[0x8];
    s32 unk20;
    char pad24[0x20];
    s32 unk44;
    char pad48[0x10];
    s32 unk58;
    char pad5C[0x90];
    char unkEC[0xC];
    char unkF8[0xC];
    char unk104[0x24];
    f32 unk128;
    s32 unk12C;
};

struct SoundState {
    char pad0[0x8];
    f32 unk8;
    char padC[0x8];
    s32 unk14;
};

struct GameState {
    char pad0[0xB4];
    s32 unkB4;
};

struct UnkStruct7954 {
    char pad0[0xC1C];
    s32 unkC1C;
};

struct Actor {
    char pad0[0x168];
    s32 unk168;
};

struct ChildDef {
    char pad0[0xC];
    char unk0C[0xC];
    char unk18[0xC];
    char unk24[0xC];
};

struct SceneNodeData {
    char pad0[0x8];
    char name[0x5C];
    s32 type;
    char pad68[0x2F];
    u8 childCount;
    char pad98[0x20];
    ChildDef* children;
};

struct SceneNodeEntry {
    void* node;
    SceneNodeData* data;
    char pad8[0x46];
    u8 unk4E;
    char pad50[0x3C];
    s32 unk8C;
};

struct SceneNodeBase {
    char pad0[0x30];
    virtual void vfunc_01();
    virtual void vfunc_02();
    virtual void vfunc_03();
    virtual void vfunc_04();
    virtual void vfunc_05();
    virtual void vfunc_06();
    virtual void vfunc_07();
    virtual void vfunc_08();
    virtual void vfunc_09();
    virtual void vfunc_10();
    virtual void vfunc_11();
    virtual void vfunc_12();
    virtual void vfunc_13();
    virtual void vfunc_14();
    virtual void vfunc_15();
    virtual void vfunc_16();
    virtual void vfunc_17();
    virtual void vfunc_18();
    virtual void vfunc_19();
    virtual void vfunc_20();
    virtual void vfunc_21();
    virtual void vfunc_22();
    virtual void vfunc_23();
    virtual void vfunc_24();
    virtual void vfunc_25();
    virtual void vfunc_26();
    virtual void vfunc_27();
    virtual void vfunc_28();
    virtual void vfunc_29();
    virtual void vfunc_30();
    virtual void vfunc_31(void* pos, void* rot);
    virtual void vfunc_32(void* scale, s32 arg);
    virtual void vfunc_33();
    virtual void vfunc_34();
    virtual void vfunc_35();
    virtual void vfunc_36();
    virtual void vfunc_37();
    virtual void vfunc_38();
    virtual void vfunc_39();
    virtual void vfunc_40();
    virtual void vfunc_41();
    virtual void vfunc_42();
    virtual void vfunc_43();
    virtual void vfunc_44();
    virtual void vfunc_45();
    virtual void vfunc_46();
    virtual void vfunc_47();
    virtual void vfunc_48();
    virtual void vfunc_49();
    virtual void vfunc_50();
    virtual void vfunc_51();
    virtual void vfunc_52();
    virtual void vfunc_53();
    virtual void vfunc_54();
    virtual void vfunc_55();
    virtual void vfunc_56();
    virtual void vfunc_57();
};

struct SceneNode : SceneNodeBase {
    char pad34[0x60];
    u32 flags;
    char pad98[0x40];
    s32 unkD8;
};

extern "C" {

s32 func_80008C74(Actor* arg);
f32 __sinf(f32 arg);
void func_8000D2FC(void*, void*);
void func_8000D5CC(void*, void*, void*);
s32 func_800431D0(void*);
void func_800C5610(s32, void*);
void func_800CB780(s32);
void func_800CB804(s32, void*);
s32 func_800CC3B8(s32 arg0, void* arg1);
void func_800DA890(s32 arg);
SceneNode* func_800DCA68(UnkArgStruct*, SceneNodeEntry*, s32);
void func_800DC944(UnkArgStruct* arg);
SceneNode* func_800DF190(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF25C(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF2E4(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF36C(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF3F4(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF47C(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF504(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF58C(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF614(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF69C(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF724(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF7AC(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF834(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF8BC(UnkArgStruct*, SceneNodeEntry*);
SceneNode* func_800DF944(UnkArgStruct*, SceneNodeEntry*);
void func_800DF9C8(GameState* arg);
void func_800EB4E0(UnkArgStruct*);
void func_800F1390(SceneNodeEntry*, void*);
void func_800F13B0(SceneNodeEntry*, void*);
void func_800F1488(SceneNodeEntry*, void*, void*);
void func_800F14C0(SceneNodeEntry*, void*, void*);
SceneNodeEntry* func_800F1C40(s32, s32);
void func_800F1E1C(s32, SceneNodeEntry*);
void func_800F48CC(SceneNodeData*, SceneNode*);
s32 func_800F5120(s32, ChildDef*);
void func_800F8484(SceneNode*, SceneNodeEntry*);
void func_800F9EE4(SceneNode*, SceneNodeEntry*);
void func_800FA5F4(SceneNode*, u32, SceneNode*);
void func_800FA608(SceneNode*, u8);
void func_800FAFEC(SceneNode*);
void func_80101C44(SceneNode*);
void func_80114BB0(SoundState* a0, void* a1, s32 a2, s32 a3, f32 a4, f32 a5, s32 a6);

extern GameTop* D_8006AB04;
extern Actor* D_8006AB10;
extern GameScene* D_801258C0;
extern SceneEntry* D_80127670;
extern UnkStruct7954* D_80127954;
extern s32 D_801272F0;
extern s32 D_801286D0;
extern s32 D_80128710;
extern GameState* D_80128010;
extern u8 D_8012802C;
extern SoundState* D_80129060;

void func_800DC5C0(UnkArgStruct* self) {
    SceneEntry* sceneEntry;
    GameObj* gameObj;
    void* savedState;
    s32 var_s0;
    s32 state;
    s32 i;
    Actor* actor_s4;

    gameObj = D_8006AB04->unk48->unk78;
    sceneEntry = D_80127670;
    func_800DF9C8(D_80128010);
    savedState = sceneEntry->unk8;
    gameObj->vfunc_09(sceneEntry->unkC);
    gameObj->vfunc_24();

    var_s0 = 0;
    {
        GameScene* scene = D_801258C0;
        GameObjChild2* child2 = scene->child;
        if (child2->vfunc_07() != 0) {
            var_s0 = D_80128010->unkB4 == 0;
        }
    }
    if (var_s0 != 0) {
        gameObj->vfunc_58(0);
    }

    for (i = 0; (u32)i < self->count; i++) {
        SceneNode* child = self->children[i];
        if (!(child->flags & 1)) {
            continue;
        }
        child->vfunc_27();
    }

    gameObj->vfunc_58(0);
    var_s0 = 0;
    if (func_80008C74(D_8006AB10) == 5) {
        var_s0 = D_80128010->unkB4 == 0;
    }
    if (var_s0 != 0) {
        func_800DC944(self);
        func_800DA890(self->unk58);
    }

    gameObj->vfunc_57();
    gameObj->vfunc_58(0);
    actor_s4 = D_8006AB10;
    state = func_80008C74(actor_s4);
    if (state == 1 && D_80127954->unkC1C != 0) {
        char* dumy = "DUMY";
        func_800CC3B8(self->unk44, dumy);
        SoundState* snd = D_80129060;
        snd->unk14 = state;
        snd->unk8 = 0.9f;
        func_80114BB0(snd, "A NEW CONTROLLER PAK%CHAS BEEN DETECTED", func_800CC3B8(self->unk44, dumy), 0, -8.25f, 1.0f,
                      state);
    }
#ifdef PAL
    if (state == 1 && self->unk12C != 0) {
        char* dumy = "DUMY";
        func_800CC3B8(self->unk44, dumy);
        SoundState* snd = D_80129060;
        snd->unk14 = state;
        snd->unk8 = 0.9f;
        func_80114BB0(snd, "INSERT CONTROLLER%CPAK NOW", func_800CC3B8(self->unk44, dumy), 0, -8.25f, 1.0f, state);
    }
#endif
    if (func_80008C74(actor_s4) == 5 && actor_s4->unk168 != 0) {
        char* dumy = "DUMY";
        func_800CC3B8(self->unk44, dumy);
        SoundState* snd = D_80129060;
        snd->unk14 = 4;
        snd->unk8 = __sinf((f32)D_8012802C * 0.20943952f) * 0.25f + 1.25f;
        func_80114BB0(snd, "DEMO", func_800CC3B8(self->unk44, dumy), 0, -9.0f, 1.0f, 1);
        u8 val = D_8012802C + 1;
        D_8012802C = val;
        if ((u32)(val & 0xFF) >= 0x1E) {
            D_8012802C = 0;
        }
    }

    gameObj->vfunc_57();
    gameObj->vfunc_09(savedState);
    gameObj->vfunc_24();
}

void func_800DC944(UnkArgStruct* self) {
    SceneChild* sceneChild = D_80127670->unkC;
    GameObj* gameObj = D_8006AB04->unk48->unk78;

    sceneChild->child2->vfunc_18(self->unk104);

    sceneChild->unk2C |= 1;

    sceneChild->child2->vfunc_10(self->unkEC, self->unkF8);

    sceneChild->unk2C |= 1;
    sceneChild->unk10 = self->unk128;
    sceneChild->unk2C |= 2;

    f32 floats[4];
    floats[0] = -2.0f;
    floats[1] = -1.5f;
    floats[2] = 2.0f;
    floats[3] = 1.5f;

    sceneChild->vfunc_05(floats);

    gameObj->vfunc_09(sceneChild);
    gameObj->vfunc_24();
}

#ifdef NON_MATCHING
SceneNode* func_800DCA68(UnkArgStruct* arg0, SceneNodeEntry* entry, s32 arg2) {
    char sp10[0x10];
    char sp20[0x8];
    u8 sp28[0x8];
    char sp30[0x10];
    char sp40[0x10];

    arg0->unk20 = arg2;
    SceneNodeData* data = entry->data;
    SceneNode* node = NULL;
    if (arg0->count >= arg0->capacity) {
        func_800EB4E0(arg0);
    }
    switch (data->type) {
        case 2:
            node = func_800DF944(arg0, entry);
            break;
        case 3:
            node = func_800DF25C(arg0, entry);
            break;
        case 17:
            node = func_800DF190(arg0, entry);
            break;
        case 21:
            node = func_800DF2E4(arg0, entry);
            break;
        case 5:
            node = func_800DF36C(arg0, entry);
            break;
        case 22:
            node = func_800DF3F4(arg0, entry);
            break;
        case 8:
            node = func_800DF8BC(arg0, entry);
            break;
        case 7:
            node = func_800DF47C(arg0, entry);
            break;
        case 25:
            node = func_800DF834(arg0, entry);
            break;
        case 27:
            node = func_800DF7AC(arg0, entry);
            break;
        case 28:
            node = func_800DF724(arg0, entry);
            break;
        case 29:
            node = func_800DF614(arg0, entry);
            break;
        case 30:
            node = func_800DF58C(arg0, entry);
            break;
        case 31:
            node = func_800DF504(arg0, entry);
            break;
        case 32:
            node = func_800DF69C(arg0, entry);
            break;
    }
    func_800F48CC(data, node);
    func_800F8484(node, entry);
    entry->node = node;
    func_800F1488(entry, sp30, sp20);
    func_800F1390(entry, sp10);
    node->vfunc_31(sp30, sp20);
    node->vfunc_32(sp10, 1);
    if ((node->flags >> 5) & 1) {
        SceneNode* obj = node;
        obj->vfunc_57();
        if (entry->unk4E != 0) {
            func_8000D2FC(sp28, &entry->unk4E);
        } else {
            func_8000D2FC(sp28, entry->data->name);
        }
        if (sp28[0] != 0) {
            u8* temp_s1 = sp28;

            func_8000D5CC(sp10, temp_s1, "OBB");
            s32 temp_s3 = D_801272F0;
            func_800CB804(temp_s3, "OBF");
            if (func_800431D0(sp10) != 8) {
                func_80101C44(obj);
                func_800C5610(obj->unkD8, temp_s1);
            }
            func_800FAFEC(obj);
            func_800CB780(temp_s3);
        } else {
            func_800FAFEC(obj);
        }
    }
    arg0->children[arg0->count] = node;
    arg0->count = arg0->count + 1;
    func_800FA608(node, data->childCount);
    for (u32 var_s3 = 0; var_s3 < data->childCount; var_s3++) {
        SceneNodeEntry* childEntry = func_800F1C40(D_801286D0, func_800F5120(D_80128710, &data->children[var_s3]));
        func_800F13B0(childEntry, data->children[var_s3].unk0C);
        func_800F14C0(childEntry, data->children[var_s3].unk18, data->children[var_s3].unk24);
        func_800FA5F4(node, var_s3, func_800DCA68(arg0, childEntry, arg0->unk20));
    }
    func_800F1390(entry, sp10);
    func_800F1488(entry, sp40, sp20);
    node->vfunc_31(sp40, sp20);
    node->vfunc_32(sp10, 1);
    func_800F9EE4(node, entry);
    entry->unk8C = 1;
    func_800F1E1C(D_801286D0, entry);
    return node;
}
#else
INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DCA68);
#endif

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DCED4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DCFF0);

// Possible boundary

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DD2C0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DD6BC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DD7BC);

#ifndef PAL
INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DD8D8);
#else
__asm__(".globl func_800DD8D8\nfunc_800DD8D8:\n");
TEXT_PAD(0x3F0);
#endif

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DDCA4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DDE08);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DDF90);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE3FC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE498);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE4F4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE530);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE5AC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE67C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE6AC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE6E8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE778);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE7A0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE7C0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE7E0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE81C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE878);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE8F4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DE930);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DEA00);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DEA98);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DEB30);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DEBC4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DEC5C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DECF0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DED84);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DEE3C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DEE5C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DEEC8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DEF34);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DEF50);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF020);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF0D8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF134);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF190);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF21C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF25C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF2E4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF36C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF3F4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF47C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF504);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF58C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF614);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF69C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF724);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF7AC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF834);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF8BC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF944);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DF9C8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFA40);

#ifndef PAL
INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFA6C); // Constructor
#else
__asm__(".globl func_800DFA6C\nfunc_800DFA6C:\n");
TEXT_PAD(0x88);
#endif

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFADC); // -> A80A0

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFAE4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFAF0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFAF8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB00);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB0C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB18);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB24);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB2C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB34);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB3C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB48);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB54);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB60);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB6C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB78);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB84);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB90);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFB9C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFBA8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFBB4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFBBC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFBC4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFBF4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC24);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC2C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC34);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC3C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC44);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC4C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC54);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC5C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC64);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC6C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC74);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC7C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC84);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC8C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC94);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFC9C); // -> A8260

#ifndef PAL
INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFCA8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFCB4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFCC4);
#else
__asm__(".globl func_800DFCC4\nfunc_800DFCC4:\n");
TEXT_PAD(0x24);
#endif

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFCD0); // -> A8290

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFE00);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800DFF24);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E0028);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E01D4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E0580);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E0BC4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E0E44);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E12AC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E1534);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E1868);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E1A08);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E1BDC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E207C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E2748);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E29B8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E2AC0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E2DC8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E2F54);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E30F8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3260);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E32B0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E334C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E33F8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E34E0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E35A4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3620);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E369C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3774);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3824);

void func_800E38A0(void) {
}

void func_800E38A8(void) {
}

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E38B0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E39A4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3A2C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3A58);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3AD0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3BCC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3C90);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3CBC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3D1C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3E14);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3E6C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3F14);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3F40);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3F8C);

void func_800E3F90(void) {
}

void func_800E3F98(void) {
}

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3FA0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E3FD4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E4018);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E4044);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E4058);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E4060);

#ifndef PAL
INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E4184); // mismatch
#else
__asm__(".globl func_800E4184\nfunc_800E4184:\n");
TEXT_PAD(0x450);
#endif

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E45B0); // -> ACB94

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E46D0);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E47AC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E4898);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E4A34);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E4BD4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E4DF8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E5044);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E5684);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E5B98);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E5BB8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E5CE8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E5F1C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E605C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E623C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E6330);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E641C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E64A8);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E6510);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E6520);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E65C4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E6750);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E67E4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E6830);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E686C);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E68C4);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E6964);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E6980);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E69AC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E69BC);

INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E69D8);

#ifndef PAL
INCLUDE_ASM("asm/nonmatchings/A49A0", func_800E69F4);
#endif

} // extern "C"
