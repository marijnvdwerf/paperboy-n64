#include "common.h"
#include "structs.h"
#include "PR/sched.h"

struct D_800768F0_Row {
    u8 _pad[0xC];
    u8 fieldC;
    u8 _pad2[0xB];
};

extern "C" {
extern u8 D_800768F0[];
extern s32 D_80074114;
extern s32 D_80074120;
extern s32 D_80074140;
extern s32 D_800740D0;
extern OSThread D_800AE1C0;
extern OSMesgQueue D_800AE140;
extern OSMesg D_800AE160[];
extern OSMesgQueue D_800AE1A0;
extern OSMesg D_800AE1B8[];
extern u8 D_800AE130[];
extern u8 D_800AE370[];
extern const char D_80003D90[];
extern const char D_80003DA0[];
extern const char D_80003EB0[];
extern void* D_80003D9C;
extern void* D_80003DB0;
extern void* D_80003EB8;
extern void* D_80003F00;

void func_8003DBD4(StructWWBase*);
void func_800079A8(void*, s32, s32, s32);
s32 func_80043340(LocalIOBase*, const char*, s32, s32);
s32 func_800436DC(LocalIOBase*, s32, void*, s32, void*);
void func_8004360C(LocalIOBase*);
s32 func_80048D60(LocalIOBase*);
s32 func_8004AA98(void*);
void func_8004B390(void);
void func_8004B3BC(s32);
s32 MusInitialize(musConfig*);
void* func_80064320(s32);
void func_8003D6AC(StructWWBase*);
void MusStop(u32 flags, s32 speed);
s32 MusAsk(u32 flags);
void func_80064340(void*);
void func_8003C8A4(StructWWBase*);
void func_8003F388(class Node2*, s32);
void func_8003DE48(StructVV*);
void func_8003DE90(StructVV*);
void func_8003F4AC(class Node2*);
void func_8003F5E0(class Node2*);
s32 func_8003C804(StructWWBase*, void*);
void func_8003DCE0(StructVV*, LocalIOBase*);
char* strcpy(char*, const char*);
char* strcat(char*, const char*);
u8* strncpy(u8*, u8*, s32);
s32 atoi(const char*);
extern s32 D_800740D4;
extern s32 D_800740F4;
void* memset(void*, s32, s32);
}

#ifdef NON_MATCHING
extern "C" s32 vfunc1__12StructWWBase(StructWWBase* self, s32 arg1) {
    LocalIO sp18;
    s32 sp48;
    s32 size;
    LocalIO* io = &sp18;
    if (self->unk0 & 1) {
        self->vfunc2();
    }
    if (func_80043340(io, D_80003D90, 2, 0)) {
        func_800079A8(&D_80003D9C, 0, 0, 0);
    }
    func_8004B3BC(D_80074114);
    self->ptrbank = (u8*)func_80064320(io->unk10);
    func_8004B390();
    if (self->ptrbank == NULL) {
        func_800079A8(&D_80003D9C, 0, 0, 0);
    }
    if (func_800436DC(io, 0, self->ptrbank, io->unk10, &sp48)) {
        func_800079A8(&D_80003D9C, 0, 0, 0);
    }
    func_8004360C(io);
    if (func_80043340(io, D_80003DA0, 2, 0)) {
        func_800079A8(&D_80003D9C, 0, 0, 0);
    }
    self->wbk = (u8*)func_80048D60(io);
    func_8004360C(io);
    func_8004B3BC(D_80074114);
    size = func_8004AA98(&((D_800768F0_Row*)&D_800768F0)[D_80074114].fieldC) - 0x1000;
    self->heap = (u8*)func_80064320(size);
    func_8004B390();
    if (self->heap == NULL) {
        func_800079A8(&D_80003D9C, 0, 0, 0);
    }
    memset(self->heap, 0, size);
    self->config.control_flag = 0;
    if ((u32)(arg1 - 1) >= 0x14U) {
        arg1 = 0x14;
    }
    self->config.thread_priority = 0x32;
    self->config.syn_output_rate = 0x7080;
    self->config.syn_updates = 0x100;

    self->config.syn_rsp_cmds = 0x1000;
    self->config.syn_num_dma_bufs = 0x24;
    self->config.syn_dma_buf_size = 0x800;
    self->config.channels = arg1;
    self->config.heap_length = size;
    self->config.syn_retraceCount = 1;
    self->config.control_flag = 0;
    self->config.fifo_length = 0x80;
    self->config.default_fxbank = NULL;
    self->config.sched = self->sched;
    self->config.heap = self->heap;
    self->config.ptr = self->ptrbank;
    self->config.wbk = self->wbk;
    if (MusInitialize(&self->config) <= 0) {
        return 0;
    }
    if (D_800740D0 == 0) {
        memset(&D_800AE130, 0, 8);
        osCreateMesgQueue(&D_800AE140, D_800AE160, 0x10);
        osCreateMesgQueue(&D_800AE1A0, D_800AE1B8, 1);
        osScAddClient((OSSched*)self->sched, (OSScClient*)&D_800AE130, &D_800AE140);
        osCreateThread(&D_800AE1C0, 0x3D, (void (*)(void*))&func_8003D6AC, self, &D_800AE370[0x28], 0x28);
        osStartThread(&D_800AE1C0);
        D_800740D0 = 1;
    }
    self->unk0 |= 1;
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc1__12StructWWBase);
#endif

// Stand-in node type for the linked list at unk6C. 0x1B24 bytes.
class Node2Base {
  public:
    /* 0x00 */ // vtable

    virtual void n2_v1();
    virtual void n2_v2();
    virtual void n2_v3();
    virtual void n2_v4();
    virtual void n2_v5();
    virtual void n2_v6();
    virtual void n2_v7();
    virtual void n2_v8();
    virtual void n2_v9();
    virtual void n2_v10();
    virtual void n2_v11();
    virtual ~Node2Base();
};

class Node2 : public Node2Base {
  public:
    /* 0x04 */ char pad4[4];
    /* 0x08 */ StructWWBase* owner;
    /* 0x0C */ Node2* next;
    /* 0x10 */ char rest[0x1B14];

    Node2();
};

class Node3 {
  public:
    /* 0x00 */ char data[0x50];

    Node3();
};

void StructWWBase::vfunc2() {
    MusStop(3, 0);
    while (MusAsk(3) != 0) {
    }
    {
        StructVV* node = this->unk68;
        while (node != NULL) {
            StructVV* next = node->next;
            delete node;
            node = next;
        }
    }
    {
        Node2* node = (Node2*)this->unk6C;
        while (node != NULL) {
            Node2* next = node->next;
            delete node;
            node = next;
        }
    }
    if (this->heap != NULL) {
        func_80064340(this->heap);
    }
    if (this->ptrbank != NULL) {
        func_80064340(this->ptrbank);
    }
    memset(&this->config, 0, sizeof(this->config));
    this->heap = NULL;
    this->ptrbank = NULL;
    this->wbk = NULL;
    this->unk6C = 0;
    this->unk68 = 0;
    this->unk70 = 0;
    this->sched = NULL;
    func_8003C8A4(this);
}

extern "C" void func_8003D6AC(StructWWBase* arg) {
    OSScMsg* msg;
    s32 running = 0;
    while (1) {
        osRecvMesg(&D_800AE140, (OSMesg*)&msg, OS_MESG_BLOCK);
        switch (msg->type) {
            case OS_SC_RETRACE_MSG:
                if (running) {
                    osSendMesg(&D_800AE1A0, (OSMesg)&D_800740D4, OS_MESG_BLOCK);
                    running = 0;
                }
                break;
            case OS_SC_PRE_NMI_MSG:
                MusStop(3, 0);
                break;
            case 0x63:
                running = 1;
                break;
        }
    }
}

extern "C" void func_8003D76C(s32 arg0) {
    D_80074114 = arg0;
}

extern "C" void func_8003D778() {
    osSendMesg(&D_800AE140, (OSMesg)&D_800740F4, OS_MESG_BLOCK);
    osRecvMesg(&D_800AE1A0, NULL, OS_MESG_BLOCK);
}

void StructWWBase::vfunc13() {
    Node2* node = this->unk6C;
    while (node != NULL) {
        func_8003F388(node, this->unk8);
        node = node->next;
    }
}

void StructWWBase::vfunc4() {
    StructVV* sv = this->unk68;
    this->unk70 = 0;
    while (sv != NULL) {
        func_8003DE48(sv);
        sv = sv->next;
    }
    Node2* n2 = this->unk6C;
    while (n2 != NULL) {
        func_8003F5E0(n2);
        n2 = n2->next;
    }
}

void StructWWBase::vfunc3() {
    StructVV* sv = this->unk68;
    this->unk70 = 1;
    while (sv != NULL) {
        func_8003DE90(sv);
        sv = sv->next;
    }
    Node2* n2 = this->unk6C;
    while (n2 != NULL) {
        func_8003F4AC(n2);
        n2 = n2->next;
    }
}

void StructWWBase::vfunc10(void* arg) {
    if (arg != NULL) {
        this->vfunc12();
        if (func_8003C804(this, arg)) {
            delete arg;
        }
    }
}

Node3* StructWWBase::vfunc9() {
    Node3* node;
    func_8004B3BC(D_80074140);
    node = new Node3();
    func_8004B390();
    if (node == NULL) {
        func_800079A8(&D_80003D9C, 0, 0, 0);
    }
    return node;
}

INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc8__12StructWWBase);

Node2* StructWWBase::vfunc7() {
    Node2* node;
    func_8004B3BC(D_80074140);
    node = new Node2();
    func_8004B390();
    if (node == NULL) {
        func_800079A8(&D_80003D9C, 0, 0, 0);
    }
    node->owner = this;
    node->next = this->unk6C;
    this->unk6C = node;
    return node;
}

#if 0
// Best draft so far (score 1265): structurally correct but the SN
// compiler chooses to inline the "found-in-walk" delete inside the
// loop with a bnel, where the target outlines it before the loop with
// a beq. Functional logic and operands match; register allocation and
// block ordering differ.
void StructWWBase::vfunc6(StructVV* node) {
    if (node == NULL) {
        return;
    }
    if (this->unk68 == NULL) {
        return;
    }
    if (this->unk68 == node) {
        this->unk68 = node->next;
        delete node;
    } else {
        StructVV* prev = this->unk68;
        StructVV* cur = prev->next;
        while (cur != NULL) {
            if (cur == node) {
                prev->next = node->next;
                delete node;
                return;
            }
            prev = cur;
            cur = cur->next;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc6__12StructWWBaseP8StructVV);
#endif

StructVV* StructWWBase::vfunc5() {
    StructVV* node;
    func_8004B3BC(D_80074120);
    node = new StructVV();
    func_8004B390();
    if (node == NULL) {
        func_800079A8(&D_80003D9C, 0, 0, 0);
    }
    node->owner = this;
    node->next = this->unk68;
    this->unk68 = node;
    return node;
}

extern "C" void func_8003DBD4(StructWWBase* self) {
    memset(&self->config, 0, sizeof(self->config));
    self->heap = NULL;
    self->ptrbank = NULL;
    self->wbk = NULL;
    self->unk6C = 0;
    self->unk68 = 0;
    self->unk70 = 0;
    self->sched = NULL;
}

INCLUDE_ASM("asm/nonmatchings/3DE40", _._12StructWWBase);

StructWWBase::StructWWBase() {
    func_8003DBD4(this);
}

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DCA4);

extern "C" s32 func_8003DCC0(StructWWBase* self) {
    return self->unk70;
}

extern "C" s32 func_8003DCCC(StructWWBase* self) {
    return self->config.channels;
}

extern "C" void func_8003DCD8(StructWWBase* self, void* arg) {
    self->sched = arg;
}
