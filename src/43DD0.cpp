#include "common.h"
#include "structs.h"

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800431D0);

INCLUDE_ASM("asm/nonmatchings/43DD0", open__13LocalIOParentPCcll);

INCLUDE_ASM("asm/nonmatchings/43DD0", close__13LocalIOParent);

INCLUDE_ASM("asm/nonmatchings/43DD0", readAt__13LocalIOParentlPvlT2);

INCLUDE_ASM("asm/nonmatchings/43DD0", read__13LocalIOParentPvl);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_80043CC0);

INCLUDE_ASM("asm/nonmatchings/43DD0", virt12__13LocalIOParent);

INCLUDE_ASM("asm/nonmatchings/43DD0", virt13__13LocalIOParent);

INCLUDE_ASM("asm/nonmatchings/43DD0", virt14__13LocalIOParent);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800443A4);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_80044458);

#if 0
s32 LocalIOParent::virt6() {
    return 0;
}

s32 LocalIOParent::virt5() {
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", virt6__13LocalIOParent);
INCLUDE_ASM("asm/nonmatchings/43DD0", virt5__13LocalIOParent);
#endif

INCLUDE_ASM("asm/nonmatchings/43DD0", func_80044474);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800444A8);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_80044574);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800445E8);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_8004463C);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_8004469C);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800446B8);

extern "C" void func_800447A4(LocalIOParent* self) {
    delete[] self->unk20;
    self->unk0 = 0;
    self->unk4 = 0;
    self->unk8 = 0;
    self->unk10 = 0;
    self->unkC = 0;
    self->unk14 = 0;
    self->unk18 = 0;
    self->unk1C = 0;
    self->unk20 = NULL;
    self->unk24 = -1;
}

extern "C" void func_80044804(LocalIOParent* self) {
    self->unk0 = 0;
    self->unk4 = 0;
    self->unk8 = 0;
    self->unk10 = 0;
    self->unkC = 0;
    self->unk14 = 0;
    self->unk18 = 0;
    self->unk1C = 0;
    self->unk20 = NULL;
    self->unk24 = -1;
}

INCLUDE_ASM("asm/nonmatchings/43DD0", _._13LocalIOParent);

#if 0
LocalIOParent::LocalIOParent() {
    func_80044804(this);
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", __13LocalIOParent);
#endif

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800448D0);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_80044918);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_80044924);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_80044934);

extern "C" u8* D_800763F8;
extern "C" u32 D_800763FC;

extern "C" u32 func_80044950() {
    return D_800763FC;
}

extern "C" u8* func_80044960() {
    return D_800763F8;
}

extern "C" void func_80044970(u8* arg0, u32 arg1) {
    D_800763F8 = arg0;
    D_800763FC = arg1;
}

INCLUDE_ASM("asm/nonmatchings/43DD0", func_80044984);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_80044990);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_8004499C);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800449A8);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800449B4);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800449C0);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800449CC);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800449D8);

INCLUDE_ASM("asm/nonmatchings/43DD0", func_800449E4);
