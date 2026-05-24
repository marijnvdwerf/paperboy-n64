#ifndef N64_RENDER_CONTEXT_H
#define N64_RENDER_CONTEXT_H

#include "common.h"
#include "pixel_format.h"

class StructYYSubA8;
class StructYYSubA8Inner1;

class RenderContext {
  public:
    /* 0x000 */ s32 flags;
    /* 0x004 */ u16 unk4;
    /* 0x006 */ u16 unk6;
    /* 0x008 */ struct Renderer* unk8;
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ s32 unk18;
    /* 0x01C */ s32 unk1C;
    /* 0x020 */ s32 unk20;
    /* 0x024 */ s32 unk24;
    /* 0x028 */ s32 unk28;
    /* 0x02C */ u32 unk2C;
    /* 0x030 */ PixelFormat* unk30;
    /* 0x034 */ struct ListNode* unk34;
    /* 0x038 */ struct ListNode* unk38;
    /* 0x03C */ struct ListNode* unk3C;
    /* 0x040 */ struct ListNode* unk40;
    /* 0x044 */ s32 unk44;
    /* 0x048 */ u8 unk48[0xCC];
    /* 0x114 */ s32 unk114;
    /* 0x118 */ u32 unk118;
    /* 0x11C */ s32 unk11C;
    /* 0x120 */ s32 unk120[3];
    /* vtable at 0x12C */

    RenderContext();
    virtual s32 vfunc1();
    virtual void vfunc2();
    virtual ~RenderContext();
    virtual void vfunc4(struct PixelFormat* req, struct PixelFormat* out);
    virtual UNK vfunc5(UNK) = 0;
    virtual UNK vfunc6(UNK) = 0;
    virtual UNK vfunc7(UNK) = 0;
    virtual UNK vfunc8(UNK) = 0;
    virtual UNK vfunc9(UNK) = 0;
    virtual void vfunc10();
    virtual void vfunc11();
    virtual void vfunc12(s32 val);
    virtual void vfunc13(s32 val);
    virtual void vfunc14();
    virtual void vfunc15();
    virtual void vfunc16();
    virtual void vfunc17();
    virtual void vfunc18();
    virtual void vfunc19();
    virtual s32 vfunc20();
    virtual void vfunc21();
    virtual void vfunc22(UNK, UNK arg);
    virtual UNK vfunc23(s32 idx) = 0;
    virtual void vfunc24(UNK) = 0;
    virtual void vfunc25();
    virtual UNK vfunc26(UNK) = 0;
    virtual UNK vfunc27(UNK) = 0;
    virtual UNK vfunc28(UNK) = 0;
    virtual UNK vfunc29(UNK) = 0;
    virtual UNK vfunc30(UNK) = 0;
    virtual UNK vfunc31(UNK) = 0;
    virtual UNK vfunc32(UNK) = 0;
    virtual UNK vfunc33(UNK) = 0;
    virtual UNK vfunc34(UNK) = 0;
    virtual void vfunc35(UNK arg) = 0;
    virtual UNK vfunc36(UNK) = 0;
    virtual void vfunc37();
    virtual void vfunc38();
    virtual void vfunc39();
    virtual void vfunc40();
    virtual void vfunc41(struct Mover* arg1);
    virtual void vfunc42(struct Mover* arg1, struct Vec3* arg2, struct Vec3* arg3, struct Vec3* arg4);
    virtual void vfunc43(struct Mover* arg, f32 f1, f32 f2);
    virtual UNK vfunc44(UNK) = 0;
    virtual UNK vfunc45(UNK) = 0;
    virtual void vfunc46(s16 a, s16 b);
    virtual void vfunc47();
    virtual void vfunc48(const char* src);
    virtual void vfunc49();
    virtual void vfunc50();
    virtual void vfunc51();
    virtual UNK vfunc52(UNK) = 0;
    virtual UNK vfunc53(UNK) = 0;
    virtual UNK vfunc54(UNK) = 0;
    virtual UNK vfunc55(UNK) = 0;
    virtual UNK vfunc56(UNK) = 0;
    virtual UNK vfunc57(UNK) = 0;
    virtual UNK vfunc58(UNK) = 0;
    virtual void vfunc59();
    virtual UNK vfunc60(UNK) = 0;
    virtual void vfunc61();
    virtual UNK vfunc62(UNK) = 0;
    virtual UNK vfunc63(UNK) = 0;
    virtual UNK vfunc64(UNK) = 0;
    virtual UNK vfunc65(UNK) = 0;
    virtual UNK vfunc66(UNK) = 0;
    virtual UNK vfunc67(UNK) = 0;
    virtual s32 vfunc68();

    // non-virtual members
    void func_80024F30();
    void* func_800264FC(const char* arg);
    void func_80026560(struct ListNode* node);
    void func_800265BC(struct ListNode* node);
    void* func_800265CC(const char* arg);
    void func_80026630(struct ListNode* node);
    void func_8002668C(struct ListNode* node);
    void* func_8002669C(const char* arg);
    void func_80026700(struct ListNode* node);
    void func_8002675C(struct ListNode* node);
    void* func_8002676C(const char* arg);
    void func_800267D0(struct ListNode* node);
    void func_8002682C(struct ListNode* node);
    s32 func_8002692C();
    s32 func_8002693C();
    s32 func_8002694C();
    s32 func_8002695C();
    s32 func_8002696C();
    u16 func_80026978();
    u16 func_80026984();
    s32 func_80026990();
    void func_8002699C(UNK arg);
    void func_800269D0(u32* dst);
    void* func_80026A24();
    s32 func_80026A2C(s32 i);
    s32 func_80026A40();
    s32 func_80026A4C();
    struct Renderer* func_80026A58();
    void func_80026A64(u32* dst);
    s32 func_80026AB8();
    s32 func_80026AC4();
    s32 func_80026AD0();
    s32 func_80026ADC();
    s32 func_80026AE8();
    s32 func_80026AF4();
    s32 func_80026B00();
    s32 func_80026B0C();
    s32 func_80026B18();
    s32 func_80026B24();
    s32 func_80026B30();
    void func_80026B3C(s32 v);
};

class N64RenderContext : public RenderContext {
  public:
    /* 0x00 */ s32 unk0;
    /* 0x04 */ char pad4[0xCD4C];

    N64RenderContext();
    virtual s32 vfunc1() CXX_OVERRIDE; // func_80036A68
    virtual void vfunc2() CXX_OVERRIDE; // func_80036AA0
    virtual ~N64RenderContext() CXX_OVERRIDE; // _._16N64RenderContext
    virtual UNK vfunc5(UNK) CXX_OVERRIDE; // func_8003758C
    virtual UNK vfunc6(UNK) CXX_OVERRIDE; // func_80036A54
    virtual UNK vfunc7(UNK) CXX_OVERRIDE; // func_80037320
    virtual UNK vfunc8(UNK) CXX_OVERRIDE; // func_80036C00
    virtual UNK vfunc9(UNK) CXX_OVERRIDE; // func_80037540
    virtual void vfunc12(s32 val) CXX_OVERRIDE; // func_80036280
    virtual void vfunc13(s32 val) CXX_OVERRIDE; // func_800349E4
    virtual UNK vfunc23(s32 idx) CXX_OVERRIDE; // func_80036C18
    virtual void vfunc24(UNK) CXX_OVERRIDE; // func_80037104
    virtual UNK vfunc26(UNK) CXX_OVERRIDE; // func_80036878
    virtual UNK vfunc27(UNK) CXX_OVERRIDE; // func_800367C4
    virtual UNK vfunc28(UNK) CXX_OVERRIDE; // func_80036710
    virtual UNK vfunc29(UNK) CXX_OVERRIDE; // func_80036674
    virtual UNK vfunc30(UNK) CXX_OVERRIDE; // func_800365E8
    virtual UNK vfunc31(UNK) CXX_OVERRIDE; // func_800364E0
    virtual UNK vfunc32(UNK) CXX_OVERRIDE; // func_80036420
    virtual UNK vfunc33(UNK) CXX_OVERRIDE; // func_80036418
    virtual UNK vfunc34(UNK) CXX_OVERRIDE; // func_80033098
    virtual void vfunc35(UNK arg) CXX_OVERRIDE; // func_8003747C
    virtual UNK vfunc36(UNK) CXX_OVERRIDE; // func_800316E4
    virtual void vfunc37() CXX_OVERRIDE; // func_80033FD8
    virtual void vfunc39() CXX_OVERRIDE; // func_8003636C
    virtual void vfunc43(struct Mover* arg, f32 f1, f32 f2) CXX_OVERRIDE; // func_80030E88
    virtual UNK vfunc44(UNK) CXX_OVERRIDE; // func_800373C8
    virtual UNK vfunc45(UNK) CXX_OVERRIDE; // func_80031A0C
    virtual void vfunc46(s16 a, s16 b) CXX_OVERRIDE; // func_800362C8
    virtual void vfunc47() CXX_OVERRIDE; // func_800362C0
    virtual UNK vfunc52(UNK) CXX_OVERRIDE; // func_80036A60
    virtual UNK vfunc53(UNK) CXX_OVERRIDE; // func_80036EC4
    virtual UNK vfunc54(UNK) CXX_OVERRIDE; // func_80036ED4
    virtual UNK vfunc55(UNK) CXX_OVERRIDE; // func_80036F74
    virtual UNK vfunc56(UNK) CXX_OVERRIDE; // func_80036EE8
    virtual UNK vfunc57(UNK) CXX_OVERRIDE; // func_80032FC0
    virtual UNK vfunc58(UNK) CXX_OVERRIDE; // func_8003697C
    virtual UNK vfunc60(UNK) CXX_OVERRIDE; // func_8003284C
    virtual void vfunc61() CXX_OVERRIDE; // func_800363A0
    virtual UNK vfunc62(UNK) CXX_OVERRIDE; // func_80036974
    virtual UNK vfunc63(UNK) CXX_OVERRIDE; // func_8003695C
    virtual UNK vfunc64(UNK) CXX_OVERRIDE; // func_80036954
    virtual UNK vfunc65(UNK) CXX_OVERRIDE; // func_8003693C
    virtual UNK vfunc66(UNK) CXX_OVERRIDE; // func_80036934
    virtual UNK vfunc67(UNK) CXX_OVERRIDE; // func_8003692C

    s32 func_80031B58(StructYYSubA8*, StructYYSubA8Inner1*, s32, s32);
};

#endif
