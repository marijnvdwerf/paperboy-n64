#ifndef N64_RENDER_CONTEXT_H
#define N64_RENDER_CONTEXT_H

#include "common.h"

typedef s32 UNK;

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
    /* 0x030 */ void* unk30;
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
    virtual UNK vfunc1(UNK); // func_8002515C
    virtual UNK vfunc2(UNK); // func_80025050
    virtual ~RenderContext(); // func_8002683C
    virtual UNK vfunc4(UNK); // func_80025238
    virtual UNK vfunc5(UNK) = 0;
    virtual UNK vfunc6(UNK) = 0;
    virtual UNK vfunc7(UNK) = 0;
    virtual UNK vfunc8(UNK) = 0;
    virtual UNK vfunc9(UNK) = 0;
    virtual UNK vfunc10(UNK); // func_80026310
    virtual UNK vfunc11(UNK); // func_80026414
    virtual UNK vfunc12(UNK); // func_80026400
    virtual UNK vfunc13(UNK); // func_800263C0
    virtual UNK vfunc14(UNK); // func_800263A0
    virtual UNK vfunc15(UNK); // func_80026398
    virtual UNK vfunc16(UNK); // func_80026388
    virtual UNK vfunc17(UNK); // func_80026380
    virtual UNK vfunc18(UNK); // func_8002636C
    virtual UNK vfunc19(UNK); // func_80026354
    virtual UNK vfunc20(UNK); // func_8002634C
    virtual UNK vfunc21(UNK); // func_80026344
    virtual UNK vfunc22(UNK); // func_80026318
    virtual UNK vfunc23(UNK) = 0;
    virtual UNK vfunc24(UNK) = 0;
    virtual UNK vfunc25(UNK); // func_800263B8
    virtual UNK vfunc26(UNK) = 0;
    virtual UNK vfunc27(UNK) = 0;
    virtual UNK vfunc28(UNK) = 0;
    virtual UNK vfunc29(UNK) = 0;
    virtual UNK vfunc30(UNK) = 0;
    virtual UNK vfunc31(UNK) = 0;
    virtual UNK vfunc32(UNK) = 0;
    virtual UNK vfunc33(UNK) = 0;
    virtual UNK vfunc34(UNK) = 0;
    virtual UNK vfunc35(UNK) = 0;
    virtual UNK vfunc36(UNK) = 0;
    virtual UNK vfunc37(UNK); // func_800264E4
    virtual UNK vfunc38(UNK); // func_800264DC
    virtual UNK vfunc39(UNK); // func_800264D4
    virtual UNK vfunc40(UNK); // func_800264CC
    virtual UNK vfunc41(UNK); // func_80025E7C
    virtual UNK vfunc42(UNK); // func_800260AC
    virtual UNK vfunc43(void* arg, f32 f1, f32 f2); // func_800264A0
    virtual UNK vfunc44(UNK) = 0;
    virtual UNK vfunc45(UNK) = 0;
    virtual UNK vfunc46(UNK); // func_80026488
    virtual UNK vfunc47(UNK); // func_80026474
    virtual UNK vfunc48(UNK); // func_80026450
    virtual UNK vfunc49(UNK); // func_80026438
    virtual UNK vfunc50(UNK); // func_800263B0
    virtual UNK vfunc51(UNK); // func_800263A8
    virtual UNK vfunc52(UNK) = 0;
    virtual UNK vfunc53(UNK) = 0;
    virtual UNK vfunc54(UNK) = 0;
    virtual UNK vfunc55(UNK) = 0;
    virtual UNK vfunc56(UNK) = 0;
    virtual UNK vfunc57(UNK) = 0;
    virtual UNK vfunc58(UNK) = 0;
    virtual UNK vfunc59(UNK); // func_80026390
    virtual UNK vfunc60(UNK) = 0;
    virtual UNK vfunc61(UNK); // func_800264EC
    virtual UNK vfunc62(UNK) = 0;
    virtual UNK vfunc63(UNK) = 0;
    virtual UNK vfunc64(UNK) = 0;
    virtual UNK vfunc65(UNK) = 0;
    virtual UNK vfunc66(UNK) = 0;
    virtual UNK vfunc67(UNK) = 0;
    virtual UNK vfunc68(UNK); // func_800264F4
};

class N64RenderContext : public RenderContext {
  public:
    /* 0x00 */ s32 unk0;
    /* 0x04 */ char pad4[0xCD4C];

    N64RenderContext();
    virtual s32 vfunc1(UNK); // func_80036A68
    virtual UNK vfunc2(UNK); // func_80036AA0
    virtual ~N64RenderContext(); // _._16N64RenderContext
    virtual UNK vfunc5(UNK); // func_8003758C
    virtual UNK vfunc6(UNK); // func_80036A54
    virtual UNK vfunc7(UNK); // func_80037320
    virtual UNK vfunc8(UNK); // func_80036C00
    virtual UNK vfunc9(UNK); // func_80037540
    virtual UNK vfunc12(UNK); // func_80036280
    virtual UNK vfunc13(UNK); // func_800349E4
    virtual UNK vfunc23(UNK); // func_80036C18
    virtual UNK vfunc24(UNK); // func_80037104
    virtual UNK vfunc26(UNK); // func_80036878
    virtual UNK vfunc27(UNK); // func_800367C4
    virtual UNK vfunc28(UNK); // func_80036710
    virtual UNK vfunc29(UNK); // func_80036674
    virtual UNK vfunc30(UNK); // func_800365E8
    virtual UNK vfunc31(UNK); // func_800364E0
    virtual UNK vfunc32(UNK); // func_80036420
    virtual UNK vfunc33(UNK); // func_80036418
    virtual UNK vfunc34(UNK); // func_80033098
    virtual UNK vfunc35(UNK); // func_8003747C
    virtual UNK vfunc36(UNK); // func_800316E4
    virtual UNK vfunc37(UNK); // func_80033FD8
    virtual UNK vfunc39(UNK); // func_8003636C
    virtual UNK vfunc43(void* arg, f32 f1, f32 f2); // func_80030E88
    virtual UNK vfunc44(UNK); // func_800373C8
    virtual UNK vfunc45(UNK); // func_80031A0C
    virtual UNK vfunc46(UNK); // func_800362C8
    virtual UNK vfunc47(UNK); // func_800362C0
    virtual UNK vfunc52(UNK); // func_80036A60
    virtual UNK vfunc53(UNK); // func_80036EC4
    virtual UNK vfunc54(UNK); // func_80036ED4
    virtual UNK vfunc55(UNK); // func_80036F74
    virtual UNK vfunc56(UNK); // func_80036EE8
    virtual UNK vfunc57(UNK); // func_80032FC0
    virtual UNK vfunc58(UNK); // func_8003697C
    virtual UNK vfunc60(UNK); // func_8003284C
    virtual UNK vfunc61(UNK); // func_800363A0
    virtual UNK vfunc62(UNK); // func_80036974
    virtual UNK vfunc63(UNK); // func_8003695C
    virtual UNK vfunc64(UNK); // func_80036954
    virtual UNK vfunc65(UNK); // func_8003693C
    virtual UNK vfunc66(UNK); // func_80036934
    virtual UNK vfunc67(UNK); // func_8003692C

    s32 func_80031B58(StructYYSubA8*, StructYYSubA8Inner1*, s32, s32);
};

#endif
