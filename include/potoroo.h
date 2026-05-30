#ifndef POTOROO_H
#define POTOROO_H

#include "common.h"
#include "vector.h"

class Parrot;

struct PotorooTruffle {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
};

class Potoroo {
  public:
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ Vec3f* unk4;
    /* 0x08 */ // vtable

    Potoroo();
    virtual ~Potoroo();
    virtual void vfunc2(Parrot* parrot);
    virtual void vfunc3(s32 newCount);
    virtual void vfunc4(void);
    virtual void vfunc5(void);
    virtual void vfunc6(s32 index, Vec3f* out);
    virtual void vfunc7(UNK, s32* out);
    virtual void vfunc8(UNK, Vec3f* out);
    virtual void vfunc9(UNK, u8* out);
    virtual void vfunc10(s32 index, Vec3f* src);
    virtual void vfunc11(void);
    virtual void vfunc12(void);
    virtual void vfunc13(void);
    virtual void vfunc14(PotorooTruffle* op);
    virtual void vfunc15(void);

    u16 func_8002986C();
    u16 func_80029878();
    s32 func_80029884();
    Vec3f* func_80029890();

    static void func_80029654(s32 arg0);
};

#endif // POTOROO_H
