#include "common.h"
#include "structs.h"

extern "C" {
extern s32 D_80074120;
extern const char D_80003EB0[];
extern void* D_80003EB8;
extern void* D_80003F00;

void func_800079A8(void*, s32, s32, s32);
s32 func_80043340(LocalIOBase*, const char*, s32, s32);
void func_8004360C(LocalIOBase*);
void func_8004B390(void);
void func_8004B3BC(s32);
void func_80064340(void*);
void func_8003E250(StructUU*, void*);
void func_8003E830(StructUU*);
void func_8003DCE0(StructVV*, LocalIOBase*);
char* strcpy(char*, const char*);
char* strcat(char*, const char*);
}

INCLUDE_ASM("asm/nonmatchings/3E8E0", func_8003DCE0);

// vv vfunc6
StructWWBase* StructVV::vfunc6() {
    return this->owner;
}

INCLUDE_ASM("asm/nonmatchings/3E8E0", func_8003DE3C);

extern "C" void func_8003DE48(StructVV* self) {
    StructUU* node = self->unk14;
    if (node != NULL) {
        do {
            node->vfunc4();
            node = node->next;
        } while (node != NULL);
    }
}

extern "C" void func_8003DE90(StructVV* self) {
    StructUU* node = self->unk14;
    if (node != NULL) {
        do {
            node->vfunc3();
            node = node->next;
        } while (node != NULL);
    }
}

// vv vfunc3
s32 StructVV::vfunc3() {
    return this->unk10 != NULL;
}

// vv vfunc5
void StructVV::vfunc5(StructUU* arg) {
    StructUU* prev;
    StructUU* cur;
    if (arg != NULL) {
        prev = NULL;
        cur = this->unk14;
        while (TRUE) {
            if (cur == NULL) {
                break;
            }
            if (cur == arg) {
                if (prev == NULL) {
                    this->unk14 = cur->next;
                } else {
                    prev->next = cur->next;
                }
                func_8003E830(cur);
                delete cur;
                return;
            }
            prev = cur;
            cur = cur->next;
        }
    }
}

// vv vfunc4
StructUU* StructVV::vfunc4(s32 arg) {
    StructUU* node;
    func_8004B3BC(D_80074120);
    node = new StructUU();
    func_8004B390();
    if (node == NULL) {
        func_800079A8(&D_80003EB8, 0, 0, 0);
    }
    node->owner = this;
    node->next = this->unk14;
    this->unk14 = node;
    func_8003E250(node, (u8*)this->unk10 + arg * 8);
    return node;
}

// vv vfunc2
void StructVV::vfunc2() {
    StructUU* p = this->unk14;
    while (p != NULL) {
        StructUU* next = p->next;
        delete p;
        p = next;
    }
    this->unk14 = NULL;
    if (this->unk10 != NULL) {
        func_80064340(this->unk10);
        this->unk10 = NULL;
    }
    this->unk0 = 0;
}

// vv vfunc1
void StructVV::vfunc1(const char* arg1) {
    LocalIO2 sp10;
    char sp40[0x40];

    if (this->vfunc3()) {
        this->vfunc2();
    }
    strcpy(sp40, arg1);
    strcat(sp40, D_80003EB0);
    if (func_80043340(&sp10, sp40, 0xA, 0x1000)) {
        func_800079A8(&D_80003EB8, 0, 0, 0);
    }
    func_8003DCE0(this, &sp10);
    func_8004360C(&sp10);
}

INCLUDE_ASM("asm/nonmatchings/3E8E0", func_8003E178); /// vv ~dtor

StructVV::StructVV() {
    this->owner = NULL;
    this->next = NULL;
    this->unk10 = 0;
    this->unk14 = 0;
}

INCLUDE_ASM("asm/nonmatchings/3E8E0", func_8003E204);

INCLUDE_ASM("asm/nonmatchings/3E8E0", func_8003E210);

INCLUDE_ASM("asm/nonmatchings/3E8E0", func_8003E218);

INCLUDE_ASM("asm/nonmatchings/3E8E0", func_8003E220);

INCLUDE_ASM("asm/nonmatchings/3E8E0", func_8003E22C);

INCLUDE_ASM("asm/nonmatchings/3E8E0", func_8003E248);
