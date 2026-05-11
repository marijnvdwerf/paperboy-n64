#ifndef STRUCTS_H
#define STRUCTS_H

#ifdef __cplusplus

#include "common.h"

class StructUUBase {
  public:
    /* 0x00 */ // vtable

    virtual ~StructUUBase();
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
};

class StructUU : public StructUUBase {
  public:
    /* 0x04 */ class StructVV* owner;
    /* 0x08 */ StructUU* next;
    /* 0x0C */ char rest[0x1C];

    StructUU();
};

class StructVVParent {
  public:
    /* 0x00 */ s32 unk0;
    /* 0x04 */ // vtable

    StructVVParent();
    virtual ~StructVVParent();
};

class StructVV : public StructVVParent {
  public:
    /* 0x08 */ class StructWWBase* owner;
    /* 0x0C */ StructVV* next;
    /* 0x10 */ void* unk10;
    /* 0x14 */ StructUU* unk14;

    StructVV();
    virtual ~StructVV();
    virtual void vfunc1(const char*);
    virtual void vfunc2();
    virtual s32 vfunc3();
    virtual StructUU* vfunc4(s32);
    virtual void vfunc5(StructUU*);
    virtual StructWWBase* vfunc6();
};

class StructWWParent {
  public:
    /* 0x00 */ s32 unk0;
    /* 0x04 */ char pad04[0x4];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ // vtable

    StructWWParent();
    virtual ~StructWWParent();
};

struct musConfig {
    /* 0x00 */ u32 control_flag;
    /* 0x04 */ s32 channels;
    /* 0x08 */ void* sched;
    /* 0x0C */ s32 thread_priority;
    /* 0x10 */ u8* heap;
    /* 0x14 */ s32 heap_length;
    /* 0x18 */ u8* ptr;
    /* 0x1C */ u8* wbk;
    /* 0x20 */ void* default_fxbank;
    /* 0x24 */ s32 fifo_length;
    /* 0x28 */ s32 syn_updates;
    /* 0x2C */ s32 syn_output_rate;
    /* 0x30 */ s32 syn_rsp_cmds;
    /* 0x34 */ s32 syn_retraceCount;
    /* 0x38 */ s32 syn_num_dma_bufs;
    /* 0x3C */ s32 syn_dma_buf_size;
    /* 0x40 */ OSPiHandle* diskrom_handle;
}; // sizeof = 0x44

class StructWWBase : public StructWWParent {
  public:
    /* 0x18 */ musConfig config;
    /* 0x5C */ u8* heap;
    /* 0x60 */ u8* ptrbank;
    /* 0x64 */ u8* wbk;
    /* 0x68 */ StructVV* unk68;
    /* 0x6C */ class Node2* unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ void* sched;

    StructWWBase();
    virtual ~StructWWBase();
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual StructVV* vfunc5();
    virtual void vfunc6(StructVV*);
    virtual class Node2* vfunc7();
    virtual void vfunc8();
    virtual class Node3* vfunc9();
    virtual void vfunc10(void*);
    virtual void vfunc11();
    virtual void vfunc12();
    virtual void vfunc13();
};

class StructWW : public StructWWBase {
  public:
    /* 0x78 */ s32 unk78;
    /* 0x7C */ StructVV* unk7C;
    /* 0x80 */ StructUU* unk80;

    StructWW();
    virtual ~StructWW();

    void func_80009210();
    void func_80009270(s32);
    void func_800092F8();
    void func_80009350(s32);
    static StructWW* func_800094A0();
};

#endif

#endif
