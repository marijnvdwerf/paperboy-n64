#ifndef STRUCTS_H
#define STRUCTS_H

#ifdef __cplusplus

#include "common.h"

class LocalIOParent {
  public:
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8* unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ // vtable

    LocalIOParent();
    virtual s32 virt1() = 0;
    virtual s32 virt2() = 0;
    virtual s32 virt3(u32) = 0;
    virtual s32 virt4(void*, s32, s32*) = 0;
    virtual s32 virt5();
    virtual s32 virt6();
    virtual ~LocalIOParent();
    virtual s32 virt8(const char*, s32, s32);   /// "open"
    virtual void virt9();                       /// "close"
    virtual s32 virt10(s32, void*, s32, void*); /// 5-arg read
    virtual s32 virt11(void*, s32);             /// 3-arg read
    virtual void virt12();
    virtual void virt13();
    virtual void virt14();
};

class LocalIOBase : public LocalIOParent {
  public:
    /* 0x2C */ u32 unk2C;

    LocalIOBase();

    virtual s32 virt1();
    virtual s32 virt2();
    virtual s32 virt3(u32);
    virtual s32 virt4(void*, s32, s32*);
    virtual ~LocalIOBase();
    virtual s32 virt15(void*, s32, s32*);
    virtual s32 virt16();

    static s32 func_80048A30();
    s32 func_80048A40(s32, s32, s32, s32);
    static s32 func_80048D58();
};

class LocalIO : public LocalIOBase {};
class LocalIO2 : public LocalIOBase {};

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
    /* 0x00 */ u32 unk0;
    /* 0x04 */ // vtable

    StructVVParent();
    virtual ~StructVVParent();
    virtual void vfunc1(const char*) = 0;
    virtual void vfunc2() = 0;
    virtual s32 vfunc3() = 0;
    virtual StructUU* vfunc4(s32) = 0;
    virtual void vfunc5(StructUU*) = 0;
    virtual class StructWWBase* vfunc6() = 0;

    u32 func_8003CA28();
};

class StructVV : public StructVVParent {
  public:
    /* 0x08 */ class StructWWBase* owner;
    /* 0x0C */ StructVV* next;
    /* 0x10 */ u8* unk10;
    /* 0x14 */ StructUU* unk14;

    StructVV();
    virtual ~StructVV();
    virtual void vfunc1(const char*);
    virtual void vfunc2();
    virtual s32 vfunc3();
    virtual StructUU* vfunc4(s32);
    virtual void vfunc5(StructUU*);
    virtual StructWWBase* vfunc6();

    StructWWBase* getOwner();
    void setOwner(StructWWBase*);
    void setNext(StructVV*);
    StructVV* getNext();

    void func_8003DCE0(LocalIOBase*);
    void func_8003DE48();
    void func_8003DE90();
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
