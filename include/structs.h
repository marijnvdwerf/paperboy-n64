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

class StructZZ {
  public:
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ // vtable

    StructZZ();
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual ~StructZZ();
    virtual void vfunc5();
    void func_80045288();
};

class StructYYHandler {
  public:
    /* 0x00 */ char pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ char pad10[0x4];
    /* 0x14 */ // vtable

    virtual ~StructYYHandler();
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
};

class StructYYUnk88 {
  public:
    /* 0x00 */ // vtable

    virtual void vfunc1();
};

class StructYYBase {
  public:
    /* 0x00 */ s32 unk0;
    /* 0x04 */ LocalIO2 io[1];
    /* 0x34 */ StructZZ zz[1];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ StructYYHandler* unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s32 unk84;
    /* 0x88 */ StructYYUnk88* unk88;
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ s32 unk90;
    /* 0x94 */ s32 unk94;
    /* 0x98 */ s32 unk98;
    /* 0x9C */ // vtable

    StructYYBase();
    void func_8000C6C0();
    void func_8000C74C();
    void func_8000C7AC();
    void func_8000C9B8();
    s32 func_8000C9F8();
    StructZZ* func_8000CA04(s32 idx);
    void func_8000CA24();
    void func_8000CA38();
    s32 func_8000CA48();
    s32 func_8000CA54();
    s32 func_8000CA60();
    s32 func_8000CA6C();
    s32 func_8000CA78();
    void func_8000CA84(s32 v);
    s32 func_8000CA8C();
    s32 func_8000CA98();
    s32 func_8000CAA4();
    s32 func_8000CAB0();
    s32 func_8000CABC();
    StructYYHandler* func_8000CAC8();
    s32 func_8000CAD4();
    s32 func_8000CAE0();
    s32 func_8000CAF0();
    s32 func_8000CAFC();
    s32 func_8000CB08();
    virtual ~StructYYBase();
    virtual void vfunc1();
    virtual void vfunc2(const char*, s32) = 0;
    virtual void vfunc3() = 0;
    virtual void vfunc4() = 0;
    virtual void vfunc5() = 0;
    virtual void vfunc6() = 0;
    virtual void vfunc7() = 0;
    virtual void vfunc8(s32, s32, s32, s32);
    virtual void vfunc9(s32, s32, s32, s32) = 0;
    virtual void vfunc10() = 0;
    virtual void vfunc11() = 0;
    virtual void vfunc12() = 0;
    virtual void vfunc13() = 0;
    virtual void vfunc14() = 0;
};

class StructYYInnerBase {
  public:
    /* 0x00 */ // vtable

    virtual void vfunc1() = 0;
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
    virtual void vfunc14();
    virtual void vfunc15();
    virtual void vfunc16();
    virtual void vfunc17();
    virtual void vfunc18();
    virtual void vfunc19();
    virtual void vfunc20();
    virtual void vfunc21();
    virtual void vfunc22();
    virtual void vfunc23();
    virtual void vfunc24();
    virtual void vfunc25();
    virtual void vfunc26();
    virtual void vfunc27();
    virtual void vfunc28();
};

class StructYYInner : public StructYYInnerBase {
  public:
    /* 0x04 */ s32 unk4;

    virtual void vfunc1();
};

class StructYYSubA8 {
  public:
    char pad[0xD040];

    StructYYSubA8();
    ~StructYYSubA8();
};

class StructYYSubD0E8 {
  public:
    char pad[0x47B0];

    StructYYSubD0E8();
    ~StructYYSubD0E8();
};

class StructYYSub11898 {
  public:
    char pad[0x210];

    StructYYSub11898();
};

class StructYY : public StructYYBase {
  public:
    /* 0x00A0 */ StructYYInner inner;
    /* 0x00A8 */ StructYYSubA8 subA8;
    /* 0x0D0E8 */ StructYYSubD0E8 subD0E8;
    /* 0x11898 */ StructYYSub11898 sub11898;
    /* 0x11AA8 */ s32 unk11AA8;
    /* 0x11AAC */ s32 unk11AAC;

    StructYY();
    virtual ~StructYY();
    virtual void vfunc2(const char*, s32);
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc9(s32, s32, s32, s32);
    virtual void vfunc10();
    virtual void vfunc11();
    virtual void vfunc12();
    virtual void vfunc13();
    virtual void vfunc14();
};

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
