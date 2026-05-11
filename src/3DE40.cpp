#include "common.h"
#include "structs.h"
#include "PR/sched.h"

struct D_800768F0_Row {
    u8 _pad[0xC];
    u8 fieldC;
    u8 _pad2[0xB];
};

class LocalIOBase {
  public:
    /* 0x00 */ char pad00[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ char pad14[0x14];
    /* 0x28 */ // vtable

    LocalIOBase();
    ~LocalIOBase();
    virtual void virt1();
};

class LocalIO : public LocalIOBase {
  public:
    virtual void virt1();
};

extern "C" {
extern u8 D_800768F0[];
extern s32 D_80074114;
extern s32 D_80074120;
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
extern void* D_80003D9C;
extern void* D_80003DB0;

void func_8003DBD4(StructWWBase*);
void func_800079A8(void*, s32, s32, s32);
s32 func_80043340(LocalIO*, const char*, s32, s32);
s32 func_800436DC(LocalIO*, s32, void*, s32, void*);
void func_8004360C(LocalIO*);
s32 func_80048D60(LocalIO*);
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
extern s32 D_800740D4;
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

// Stand-in node type for the linked list at unk6C.
struct Node2Base {
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
struct Node2 : public Node2Base {
    /* 0x04 */ char pad4[8];
    /* 0x0C */ Node2* next;
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

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003D76C);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003D778);

INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc13__12StructWWBase);

INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc4__12StructWWBase);

INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc3__12StructWWBase);

INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc10__12StructWWBase);

INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc9__12StructWWBase);

INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc8__12StructWWBase);

INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc7__12StructWWBase);

INCLUDE_ASM("asm/nonmatchings/3DE40", vfunc6__12StructWWBaseP8StructVV);

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

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DCC0);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DCCC);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DCD8);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DCE0);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DE30);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DE3C);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DE48);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DE90);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DED8);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DEE4);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003DF70);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003E010);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003E084);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003E178);

INCLUDE_ASM("asm/nonmatchings/3DE40", __8StructVV);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003E204);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003E210);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003E218);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003E220);

INCLUDE_ASM("asm/nonmatchings/3DE40", func_8003E22C);
