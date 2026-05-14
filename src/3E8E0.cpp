#include "common.h"
#include "structs.h"

extern "C" {
extern s32 D_80074120;
extern void* D_80003F00;

extern char D_80003EB0[];
extern char D_80003EB8[];

void func_800079A8(const void*, s32, s32, s32);
void func_8004B390(void);
void func_8004B3BC(s32);
void func_80064340(void*);
void* func_80064320(s32);
void func_8003E250(StructUU*, void*);
void func_8003E830(StructUU*);
char* strcpy(char*, const char*);
char* strcat(char*, const char*);
u8* strncpy(u8*, u8*, s32);
s32 atoi(const u8*);
}

void StructVV::func_8003DCE0(LocalIOBase* arg1) {
    u8 sp10[0x10];
    u32 i;

    if (arg1->read(sp10, 0x10) != 0) {
        func_800079A8(D_80003EB8, 0, 0, 0);
    }
    this->unk0 = atoi(sp10);
    if (this->unk0 == 0) {
        func_800079A8(D_80003EB8, 0, 0, 0);
    }

    func_8004B3BC(D_80074120);
    this->unk10 = new u8[this->unk0 * 8];
    func_8004B390();
    if (this->unk10 == NULL) {
        func_800079A8(D_80003EB8, 0, 0, 0);
    }
    i = 0;
    while (i < this->unk0) {
        if (arg1->read(sp10, 0x10) != 0) {
            func_800079A8(D_80003EB8, 0, 0, 0);
        }
        strncpy(this->unk10 + i * 8, sp10, 8);
        i += 1;
    }
}

// vv vfunc6
StructWWBase* StructVV::vfunc6() {
    return this->owner;
}

extern "C" void func_8003DE3C(s32 arg0) {
    D_80074120 = arg0;
}

void StructVV::func_8003DE48() {
    StructUU* node = this->unk14;
    if (node != NULL) {
        do {
            node->vfunc4();
            node = node->next;
        } while (node != NULL);
    }
}

void StructVV::func_8003DE90() {
    StructUU* node = this->unk14;
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
        func_800079A8(D_80003EB8, 0, 0, 0);
    }
    node->owner = this;
    node->next = this->unk14;
    this->unk14 = node;
    func_8003E250(node, this->unk10 + arg * 8);
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
        delete[] this->unk10;
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
    if (sp10.open(sp40, 0xA, 0x1000)) {
        func_800079A8(D_80003EB8, 0, 0, 0);
    }
    this->func_8003DCE0(&sp10);
    sp10.close();
}

StructVV::~StructVV() {
    this->vfunc2();
}

StructVV::StructVV() {
    this->owner = NULL;
    this->next = NULL;
    this->unk10 = 0;
    this->unk14 = 0;
}

StructWWBase* StructVV::getOwner() {
    return this->owner;
}

void StructVV::setOwner(StructWWBase* arg) {
    this->owner = arg;
}

void StructVV::setNext(StructVV* arg) {
    this->next = arg;
}

StructVV* StructVV::getNext() {
    return this->next;
}

// Auto generated virtual dtor of LocalIO2
// INCLUDE_ASM("asm/nonmatchings/3E8E0", func_8003E22C);
