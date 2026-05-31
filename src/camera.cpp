#include "common.h"
#include "vector.h"

extern "C" {
f32 __sinf(f32);
f32 __cosf(f32);
}

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
    virtual void vfunc9(f32* point, f32* out) = 0; // func_80014E58
    virtual void vfunc10(f32*) = 0; // func_800148C4
    virtual UNK vfunc11(UNK) = 0;

    void func_80013AD0(f32* out);
    void func_800141D8(f32* out);
    s32 func_800148C4(f32* pos, f32 dist, f32* outHit);
    void func_80014BF0();
    void func_80014E40();
    void func_80014E4C(SceneTarget* a, s32 b);
    void func_80014E58();
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

void Camera::func_80013AD0(f32* out) {
    f32 dir[3];
    f32 e0[3];
    f32 e1[3];
    f32 norm[3];

    this->node->vfunc17(out);

    f32 angle = this->fov * 0.008726646f;
    f32 tanA = __sinf(angle) / __cosf(angle);

    f32 farH = tanA * this->farClip;
    f32 farW = this->aspect * farH;
    this->field_20 = farH;
    this->field_1C = farW;

    dir[0] = -farW;
    dir[1] = -this->field_20;
    dir[2] = this->farClip;
    this->node->vfunc2(dir, out + 3);

    dir[1] = this->field_20;
    f32* c9 = out + 9;
    this->node->vfunc2(dir, c9);

    dir[0] = this->field_1C;
    dir[1] = -this->field_20;
    this->node->vfunc2(dir, out + 6);

    dir[1] = this->field_20;
    this->node->vfunc2(dir, out + 12);

    f32 nearH = tanA * this->nearClip;
    f32 nearW = this->aspect * nearH;
    this->field_18 = nearH;
    this->field_14 = nearW;

    dir[0] = -nearW;
    dir[1] = -this->field_18;
    dir[2] = this->nearClip;
    f32* c15 = out + 15;
    this->node->vfunc2(dir, c15);

    dir[1] = this->field_18;
    f32* c21 = out + 21;
    this->node->vfunc2(dir, c21);

    dir[0] = this->field_14;
    dir[1] = -this->field_18;
    f32* c18 = out + 18;
    this->node->vfunc2(dir, c18);

    dir[1] = this->field_18;
    this->node->vfunc2(dir, out + 24);

    e0[0] = out[3] - out[15];
    e0[1] = out[4] - out[16];
    e0[2] = out[5] - out[17];
    e1[0] = out[21] - out[15];
    e1[1] = out[22] - out[16];
    e1[2] = out[23] - out[17];
    norm[0] = e1[1] * e0[2] - e1[2] * e0[1];
    norm[1] = e1[2] * e0[0] - e1[0] * e0[2];
    norm[2] = e1[0] * e0[1] - e1[1] * e0[0];
    vec3_normalize((Vec3f*)norm, (Vec3f*)norm);
    out[27] = norm[0];
    out[28] = norm[1];
    out[29] = norm[2];
    {
        f32* p = &out[27];
        out[30] = -(p[0] * c15[0] + p[1] * c15[1] + p[2] * c15[2]);
    }

    e0[0] = out[6] - out[18];
    e0[1] = out[7] - out[19];
    e0[2] = out[8] - out[20];
    e1[0] = out[24] - out[18];
    e1[1] = out[25] - out[19];
    e1[2] = out[26] - out[20];
    norm[0] = e0[1] * e1[2] - e0[2] * e1[1];
    norm[1] = e0[2] * e1[0] - e0[0] * e1[2];
    norm[2] = e0[0] * e1[1] - e0[1] * e1[0];
    vec3_normalize((Vec3f*)norm, (Vec3f*)norm);
    out[31] = norm[0];
    out[32] = norm[1];
    out[33] = norm[2];
    {
        f32* p = &out[31];
        out[34] = -(p[0] * c18[0] + p[1] * c18[1] + p[2] * c18[2]);
    }

    e0[0] = out[9] - out[21];
    e0[1] = out[10] - out[22];
    e0[2] = out[11] - out[23];
    e1[0] = out[24] - out[21];
    e1[1] = out[25] - out[22];
    e1[2] = out[26] - out[23];
    norm[0] = e1[1] * e0[2] - e1[2] * e0[1];
    norm[1] = e1[2] * e0[0] - e1[0] * e0[2];
    norm[2] = e1[0] * e0[1] - e1[1] * e0[0];
    vec3_normalize((Vec3f*)norm, (Vec3f*)norm);
    out[35] = norm[0];
    out[36] = norm[1];
    out[37] = norm[2];
    {
        f32* p = &out[35];
        out[38] = -(p[0] * c21[0] + p[1] * c21[1] + p[2] * c21[2]);
    }

    e0[0] = out[3] - out[15];
    e0[1] = out[4] - out[16];
    e0[2] = out[5] - out[17];
    e1[0] = out[18] - out[15];
    e1[1] = out[19] - out[16];
    e1[2] = out[20] - out[17];
    norm[0] = e0[1] * e1[2] - e0[2] * e1[1];
    norm[1] = e0[2] * e1[0] - e0[0] * e1[2];
    norm[2] = e0[0] * e1[1] - e0[1] * e1[0];
    vec3_normalize((Vec3f*)norm, (Vec3f*)norm);
    out[39] = norm[0];
    out[40] = norm[1];
    out[41] = norm[2];
    {
        f32* p = &out[39];
        out[42] = -(p[0] * c15[0] + p[1] * c15[1] + p[2] * c15[2]);
    }

    this->node->vfunc15(norm);
    out[47] = norm[0];
    out[48] = norm[1];
    out[49] = norm[2];
    {
        f32* p = &out[47];
        out[50] = -(p[0] * c15[0] + p[1] * c15[1] + p[2] * c15[2]);
    }
    out[43] = -norm[0];
    out[44] = -norm[1];
    out[45] = -norm[2];
    out[46] = norm[0] * c9[0] + norm[1] * c9[1] + norm[2] * c9[2];
}

void Camera::func_800141D8(f32* out) {
    f32 dir[3];
    f32 e0[3];
    f32 e1[3];
    f32 norm[3];
    f32 farExt[4];

    this->node->vfunc17(out);

    f32 ratio = this->farClip / this->nearClip;
    farExt[0] = this->field_FC * ratio;
    farExt[2] = this->field_104 * ratio;
    farExt[1] = this->field_100 * ratio;
    farExt[3] = this->field_108 * ratio;
    this->field_20 = farExt[3] - farExt[1];
    this->field_1C = farExt[2] - farExt[0];

    dir[0] = farExt[0];
    dir[1] = farExt[1];
    dir[2] = this->farClip;
    this->node->vfunc2(dir, out + 3);

    dir[1] = farExt[3];
    f32* c9 = out + 9;
    this->node->vfunc2(dir, c9);

    dir[0] = farExt[2];
    dir[1] = farExt[1];
    this->node->vfunc2(dir, out + 6);

    dir[1] = farExt[3];
    this->node->vfunc2(dir, out + 12);

    dir[0] = this->field_FC;
    dir[1] = this->field_100;
    dir[2] = this->nearClip;
    f32* c15 = out + 15;
    this->node->vfunc2(dir, c15);

    dir[1] = this->field_108;
    f32* c21 = out + 21;
    this->node->vfunc2(dir, c21);

    dir[0] = this->field_104;
    dir[1] = this->field_100;
    f32* c18 = out + 18;
    this->node->vfunc2(dir, c18);

    dir[1] = this->field_108;
    this->node->vfunc2(dir, out + 24);

    e0[0] = out[3] - out[15];
    e0[1] = out[4] - out[16];
    e0[2] = out[5] - out[17];
    e1[0] = out[21] - out[15];
    e1[1] = out[22] - out[16];
    e1[2] = out[23] - out[17];
    norm[0] = e1[1] * e0[2] - e1[2] * e0[1];
    norm[1] = e1[2] * e0[0] - e1[0] * e0[2];
    norm[2] = e1[0] * e0[1] - e1[1] * e0[0];
    vec3_normalize((Vec3f*)norm, (Vec3f*)norm);
    out[27] = norm[0];
    out[28] = norm[1];
    out[29] = norm[2];
    {
        f32* p = &out[27];
        out[30] = -(p[0] * c15[0] + p[1] * c15[1] + p[2] * c15[2]);
    }

    e0[0] = out[6] - out[18];
    e0[1] = out[7] - out[19];
    e0[2] = out[8] - out[20];
    e1[0] = out[24] - out[18];
    e1[1] = out[25] - out[19];
    e1[2] = out[26] - out[20];
    norm[0] = e0[1] * e1[2] - e0[2] * e1[1];
    norm[1] = e0[2] * e1[0] - e0[0] * e1[2];
    norm[2] = e0[0] * e1[1] - e0[1] * e1[0];
    vec3_normalize((Vec3f*)norm, (Vec3f*)norm);
    out[31] = norm[0];
    out[32] = norm[1];
    out[33] = norm[2];
    {
        f32* p = &out[31];
        out[34] = -(p[0] * c18[0] + p[1] * c18[1] + p[2] * c18[2]);
    }

    e0[0] = out[9] - out[21];
    e0[1] = out[10] - out[22];
    e0[2] = out[11] - out[23];
    e1[0] = out[24] - out[21];
    e1[1] = out[25] - out[22];
    e1[2] = out[26] - out[23];
    norm[0] = e1[1] * e0[2] - e1[2] * e0[1];
    norm[1] = e1[2] * e0[0] - e1[0] * e0[2];
    norm[2] = e1[0] * e0[1] - e1[1] * e0[0];
    vec3_normalize((Vec3f*)norm, (Vec3f*)norm);
    out[35] = norm[0];
    out[36] = norm[1];
    out[37] = norm[2];
    {
        f32* p = &out[35];
        out[38] = -(p[0] * c21[0] + p[1] * c21[1] + p[2] * c21[2]);
    }

    e0[0] = out[3] - out[15];
    e0[1] = out[4] - out[16];
    e0[2] = out[5] - out[17];
    e1[0] = out[18] - out[15];
    e1[1] = out[19] - out[16];
    e1[2] = out[20] - out[17];
    norm[0] = e0[1] * e1[2] - e0[2] * e1[1];
    norm[1] = e0[2] * e1[0] - e0[0] * e1[2];
    norm[2] = e0[0] * e1[1] - e0[1] * e1[0];
    vec3_normalize((Vec3f*)norm, (Vec3f*)norm);
    out[39] = norm[0];
    out[40] = norm[1];
    out[41] = norm[2];
    {
        f32* p = &out[39];
        out[42] = -(p[0] * c15[0] + p[1] * c15[1] + p[2] * c15[2]);
    }

    this->node->vfunc15(norm);
    out[47] = norm[0];
    out[48] = norm[1];
    out[49] = norm[2];
    {
        f32* p = &out[47];
        out[50] = -(p[0] * c15[0] + p[1] * c15[1] + p[2] * c15[2]);
    }
    out[43] = -norm[0];
    out[44] = -norm[1];
    out[45] = -norm[2];
    out[46] = norm[0] * c9[0] + norm[1] * c9[1] + norm[2] * c9[2];
}

s32 Camera::func_800148C4(f32* pos, f32 dist, f32* outHit) {
    f32 hitPoint[3];
    f32 slot1[4];
    f32 slot2[4];
    f32 slot0[4];
    f32 slot3[4];
    f32 dir[3];
    f32 upDir[3];
    f32 hitDir[3];

    s32 result = func_80029480(&this->frustum, pos, dist);
    if (result == 0) {
        return 0;
    }

    this->node->vfunc13(dir);

    f32* hp = hitPoint;
    f32 nd;

    f32* plane = this->frustum.planes[1];
    f32 denom0 = dir[0] * plane[0] + dir[1] * plane[1];
    denom0 += dir[2] * plane[2];
    nd = -denom0;
    f32 t0 = dist / nd;
    hitDir[0] = dir[0] * t0;
    hitDir[1] = dir[1] * t0;
    hitDir[2] = dir[2] * t0;
    hitPoint[0] = pos[0] + hitDir[0];
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    this->vfunc9(hp, slot0);

    plane = this->frustum.planes[0];
    f32 denom1 = dir[0] * plane[0] + dir[1] * plane[1];
    denom1 += dir[2] * plane[2];
    nd = dist / denom1;
    f32 t1 = -nd;
    hitDir[0] = dir[0] * t1;
    hitDir[1] = dir[1] * t1;
    hitDir[2] = dir[2] * t1;
    hitPoint[0] = pos[0] + hitDir[0];
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    this->vfunc9(hp, slot1);

    this->node->vfunc14(upDir);

    plane = this->frustum.planes[3];
    f32 denom2 = upDir[0] * plane[0] + upDir[1] * plane[1];
    denom2 += upDir[2] * plane[2];
    nd = dist / denom2;
    f32 t2 = -nd;
    hitDir[0] = upDir[0] * t2;
    hitDir[1] = upDir[1] * t2;
    hitDir[2] = upDir[2] * t2;
    hitPoint[0] = pos[0] + hitDir[0];
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    this->vfunc9(hp, slot2);

    plane = this->frustum.planes[2];
    f32 denom3 = upDir[0] * plane[0] + upDir[1] * plane[1];
    denom3 += upDir[2] * plane[2];
    nd = -denom3;
    f32 t3 = dist / nd;
    hitDir[0] = upDir[0] * t3;
    hitDir[1] = upDir[1] * t3;
    hitDir[2] = upDir[2] * t3;
    hitPoint[0] = pos[0] + hitDir[0];
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    this->vfunc9(hp, slot3);

    outHit[0] = slot1[0];
    outHit[2] = slot0[0];
    outHit[1] = slot2[1];
    outHit[3] = slot3[1];

    return result;
}

void Camera::func_80014BF0() {
    SceneTarget* scene = this->sceneTarget;
    if (scene == NULL) {
        return;
    }

    scene->vfunc24(0);

    scene = this->sceneTarget;
    WorldNode* world = scene->vfunc23(0);

    CameraNode* child = world->vfunc7(this->field_28);

    Vec3f pos;
    Vec3f newPos;
    Vec3f dir1;
    Vec3f dir2;
    Vec3f newDir1;
    Vec3f newDir2;

    child->vfunc17(&pos.x);
    child->vfunc9(&dir1.x, &dir2.x);

    while (1) {
        child = child->parent;
        if (child == NULL) {
            break;
        }
        child->vfunc2(&pos.x, &newPos.x);
        child->vfunc4(&dir1.x, &newDir1.x);
        child->vfunc4(&dir2.x, &newDir2.x);
        pos = newPos;
        dir1 = newDir1;
        dir2 = newDir2;
    }

    scene = this->sceneTarget;
    scene->vfunc12(&pos.x, &newPos.x);

    scene = this->sceneTarget;
    scene->vfunc14(&dir1.x, &newDir1.x);

    scene = this->sceneTarget;
    scene->vfunc14(&dir2.x, &newDir2.x);

    this->node->vfunc18(&newPos.x);
    this->flags |= 1;

    newDir2.x = -newDir2.x;
    newDir2.y = -newDir2.y;
    newDir2.z = -newDir2.z;

    this->node->vfunc10(&newDir1.x, &newDir2.x);
    this->flags |= 1;
}

void Camera::func_80014E40() {
    this->sceneTarget = NULL;
    this->field_28 = 0;
}

void Camera::func_80014E4C(SceneTarget* a, s32 b) {
    this->sceneTarget = a;
    this->field_28 = b;
}

void Camera::func_80014E58() {
}

void Camera::func_80014E60(f32 value) {
    if (value > 0.0f) {
        this->aspect = value;
        this->flags |= 8;
    } else {
        this->flags &= ~8;
    }
    this->flags |= 3;
}

void Camera::func_80014EA4(f32 fov, f32 nearClip, f32 farClip, f32 aspect) {
    this->fov = fov;
    this->nearClip = nearClip;
    this->farClip = farClip;
    func_80014E60(aspect);
}

void Camera::func_80014ED0(f32* eye, f32* target, f32* up) {
    f32 dir[3];
    f32 negUp[3];

    dir[0] = target[0] - eye[0];
    dir[1] = target[1] - eye[1];
    dir[2] = target[2] - eye[2];

    negUp[0] = -up[0];
    negUp[1] = -up[1];
    negUp[2] = -up[2];

    this->node->vfunc10(dir, negUp);
    this->node->vfunc18(eye);
}

INCLUDE_ASM("asm/nonmatchings/camera", _._6Camera);

// The vtable is 8-byte aligned. The compiler-emitted float constant from
// func_80013AD0 sits first in .rdata and would otherwise leave the vtable at a
// 4-byte boundary, so force the alignment (the gap is the 0.0f pad at 0x800010C4).
__asm__(".section .rdata\n.align 3\n.section .text\n");
INCLUDE_RODATA("asm/nonmatchings/camera", _vt.6Camera);

Camera::Camera() {
    node = NULL;
    flags = 3;
    viewport.x = 0;
    viewport.w = 0;
    viewport.y = 0;
    viewport.h = 0;
    sceneTarget = NULL;
    field_28 = 0;
    fov = 40.0f;
    aspect = 1.333333373f;
    nearClip = 2.0f;
    farClip = 200.0f;
    f32 z = 0.0f;
    field_14 = z;
    field_18 = z;
    field_20 = z;
    field_1C = z;
}

s32 Camera::func_80015038() {
    return this->field_28;
}

SceneTarget* Camera::func_80015044() {
    return this->sceneTarget;
}

s32 Camera::func_80015050() {
    return this->sceneTarget != NULL;
}

void Camera::func_8001505C(Viewport* out) {
    *out = this->viewport;
}

s32 Camera::func_80015084() {
    return this->flags & 4;
}

s32 Camera::func_80015090() {
    return ((this->flags >> 1) ^ 1) & 1;
}

s32 Camera::func_800150A4() {
    return (this->flags ^ 1) & 1;
}

f32 Camera::func_800150B4() {
    return this->field_20;
}

f32 Camera::func_800150C0() {
    return this->field_1C;
}

f32 Camera::func_800150CC() {
    return this->field_18;
}

f32 Camera::func_800150D8() {
    return this->field_14;
}

void Camera::func_800150E4(f32* out) {
    this->node->vfunc17(out);
}

void Camera::func_80015114(f32 value) {
    this->farClip = value;
    this->flags |= 2;
}

f32 Camera::func_80015128() {
    return this->farClip;
}

void Camera::func_80015134(f32 value) {
    this->nearClip = value;
    this->flags |= 2;
}

f32 Camera::func_80015148() {
    return this->nearClip;
}

f32 Camera::func_80015154() {
    return this->aspect;
}

f32 Camera::func_80015160() {
    return this->fov;
}

void Camera::func_8001516C(f32 value) {
    this->fov = value;
    this->flags |= 2;
}

void Camera::func_80015180(f32* pos) {
    this->node->vfunc18(pos);
    this->flags |= 1;
}

void Camera::func_800151C8(CameraNode** arg) {
    this->node->vfunc19(*arg);
    this->flags |= 1;
}

void Camera::func_80015210(f32* dir, f32* up) {
    this->node->vfunc10(dir, up);
    this->flags |= 1;
}

void Camera::func_80015258(f32* a, f32* b) {
    this->node->vfunc8(a, b);
}

void Camera::func_80015288(f32* vec) {
    this->node->vfunc13(vec);
}

void Camera::func_800152B8(f32* out) {
    this->node->vfunc13(out);
    out[0] = -out[0];
    out[1] = -out[1];
    out[2] = -out[2];
}

void Camera::func_80015314(f32* vec) {
    this->node->vfunc15(vec);
}

void Camera::func_80015344(f32* out) {
    this->node->vfunc15(out);
    out[0] = -out[0];
    out[1] = -out[1];
    out[2] = -out[2];
}

void Camera::func_800153A0(f32* vec) {
    this->node->vfunc14(vec);
}

void Camera::func_800153D0(f32* out) {
    this->node->vfunc14(out);
    out[0] = -out[0];
    out[1] = -out[1];
    out[2] = -out[2];
}

void Camera::func_8001542C(FrustumNearData* out) {
    *out = this->frustum.near;
}

FrustumData* Camera::func_8001547C() {
    return &this->frustum;
}

void Camera::func_80015484(FrustumData* out) {
    *out = this->frustum;
}

void Camera::func_800154D4(f32* src, f32* dst) {
    this->node->vfunc2(src, dst);
}
