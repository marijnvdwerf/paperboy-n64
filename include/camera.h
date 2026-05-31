#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

// Local node type compatible with Joey layout but with correct signatures
// for Camera's usage. Some vcalls pass different arg counts than scene.h declares.
struct CameraNode {
    /* 0x0 */ CameraNode* parent;
    /* 0x4 */ CameraNode* next;
    /* 0x8 */ CameraNode* child;
    // vptr at 0xC
    virtual UNK vfunc1(UNK) = 0;
    virtual void vfunc2(f32* src, f32* dst) = 0;
    virtual UNK vfunc3(UNK) = 0;
    virtual void vfunc4(f32* src, f32* dst) = 0;
    virtual UNK vfunc5(UNK) = 0;
    virtual UNK vfunc6(UNK) = 0;
    virtual UNK vfunc7(UNK) = 0;
    virtual void vfunc8(f32* a, f32* b) = 0;
    virtual void vfunc9(f32* a, f32* b) = 0;
    virtual void vfunc10(f32* dir, f32* up) = 0;
    virtual UNK vfunc11(UNK) = 0;
    virtual UNK vfunc12(UNK) = 0;
    virtual void vfunc13(f32* vec) = 0;
    virtual void vfunc14(f32* vec) = 0;
    virtual void vfunc15(f32* vec) = 0;
    virtual UNK vfunc16(UNK) = 0;
    virtual void vfunc17(f32* vec) = 0;
    virtual void vfunc18(f32* pos) = 0;
    virtual void vfunc19(CameraNode* src) = 0;
};

// Object returned by SceneTarget::vfunc23(), vptr at 0x8
struct WorldNode {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    // vptr at 0x8
    virtual UNK vfunc1(UNK) = 0;
    virtual UNK vfunc2(UNK) = 0;
    virtual UNK vfunc3(UNK) = 0;
    virtual UNK vfunc4(UNK) = 0;
    virtual UNK vfunc5(UNK) = 0;
    virtual UNK vfunc6(UNK) = 0;
    virtual CameraNode* vfunc7(s32 index) = 0;
};

// Scene target for camera tracking, vptr at 0x30
struct SceneTarget {
    /* 0x00 */ u8 sceneData[0x30];
    // vptr at 0x30
    virtual UNK vfunc1(UNK) = 0;
    virtual UNK vfunc2(UNK) = 0;
    virtual UNK vfunc3(UNK) = 0;
    virtual UNK vfunc4(UNK) = 0;
    virtual UNK vfunc5(UNK) = 0;
    virtual UNK vfunc6(UNK) = 0;
    virtual UNK vfunc7(UNK) = 0;
    virtual UNK vfunc8(UNK) = 0;
    virtual UNK vfunc9(UNK) = 0;
    virtual UNK vfunc10(UNK) = 0;
    virtual UNK vfunc11(UNK) = 0;
    virtual void vfunc12(f32* in, f32* out) = 0;
    virtual UNK vfunc13(UNK) = 0;
    virtual void vfunc14(f32* in, f32* out) = 0;
    virtual UNK vfunc15(UNK) = 0;
    virtual UNK vfunc16(UNK) = 0;
    virtual UNK vfunc17(UNK) = 0;
    virtual UNK vfunc18(UNK) = 0;
    virtual UNK vfunc19(UNK) = 0;
    virtual UNK vfunc20(UNK) = 0;
    virtual UNK vfunc21(UNK) = 0;
    virtual UNK vfunc22(UNK) = 0;
    virtual WorldNode* vfunc23(s32 arg) = 0;
    virtual void vfunc24(s32 arg) = 0;
};

struct Viewport {
    s32 x, y, w, h;
};

struct FrustumNearData {
    /* 0x00 */ f32 header[3];
    /* 0x0C */ f32 nearCorners[4][3];
};

struct FrustumData {
    /* 0x00 */ FrustumNearData near;
    /* 0x3C */ f32 farCorners[4][3];
    /* 0x6C */ f32 planes[6][4];
};

extern "C" s32 func_80029480(FrustumData* frustum, f32* point, f32 radius);

struct Camera {
    /* 0x000 */ CameraNode* node;
    /* 0x004 */ f32 fov;
    /* 0x008 */ f32 aspect;
    /* 0x00C */ f32 nearClip;
    /* 0x010 */ f32 farClip;
    /* 0x014 */ f32 field_14;
    /* 0x018 */ f32 field_18;
    /* 0x01C */ f32 field_1C;
    /* 0x020 */ f32 field_20;
    /* 0x024 */ SceneTarget* sceneTarget;
    /* 0x028 */ s32 field_28;
    /* 0x02C */ u32 flags;
    /* 0x030 */ FrustumData frustum;
    /* 0x0FC */ f32 field_FC;
    /* 0x100 */ f32 field_100;
    /* 0x104 */ f32 field_104;
    /* 0x108 */ f32 field_108;
    /* 0x10C */ Viewport viewport;
    /* 0x11C */ // vptr

    Camera();
    virtual UNK vfunc1(UNK) = 0;
    virtual UNK vfunc2(UNK) = 0;
    virtual ~Camera();
    virtual UNK vfunc4(UNK) = 0;
    virtual UNK vfunc5(UNK) = 0;
    virtual UNK vfunc6(UNK) = 0;
    virtual UNK vfunc7(UNK) = 0;
    virtual UNK vfunc8(UNK) = 0;
    virtual void vfunc9(f32* point, f32* out); // func_80014E58
    virtual s32 vfunc10(f32* pos, f32 dist, f32* outHit); // func_800148C4
    virtual UNK vfunc11(UNK) = 0;

    void func_80013AD0(f32* out);
    void func_800141D8(f32* out);
    void func_80014BF0();
    void func_80014E40();
    void func_80014E4C(SceneTarget* a, s32 b);
    void func_80014E60(f32 value);
    void func_80014EA4(f32 fov, f32 nearClip, f32 farClip, f32 aspect);
    void func_80014ED0(f32* eye, f32* target, f32* up);
    s32 func_80015038();
    SceneTarget* func_80015044();
    s32 func_80015050();
    void func_8001505C(Viewport* out);
    s32 func_80015084();
    s32 func_80015090();
    s32 func_800150A4();
    f32 func_800150B4();
    f32 func_800150C0();
    f32 func_800150CC();
    f32 func_800150D8();
    void func_800150E4(f32* out);
    void func_80015114(f32 value);
    f32 func_80015128();
    void func_80015134(f32 value);
    f32 func_80015148();
    f32 func_80015154();
    f32 func_80015160();
    void func_8001516C(f32 value);
    void func_80015180(f32* pos);
    void func_800151C8(CameraNode** arg);
    void func_80015210(f32* dir, f32* up);
    void func_80015258(f32* a, f32* b);
    void func_80015288(f32* vec);
    void func_800152B8(f32* out);
    void func_80015314(f32* vec);
    void func_80015344(f32* out);
    void func_800153A0(f32* vec);
    void func_800153D0(f32* out);
    void func_8001542C(FrustumNearData* out);
    FrustumData* func_8001547C();
    void func_80015484(FrustumData* out);
    void func_800154D4(f32* src, f32* dst);
};

#endif
