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
};

extern "C" void func_80013AD0(Camera* self, f32* out) {
    f32 dir[3];
    f32 e0[3];
    f32 e1[3];
    f32 norm[3];

    self->node->vfunc17(out);

    f32 angle = self->fov * 0.008726646f;
    f32 tanA = __sinf(angle) / __cosf(angle);

    f32 farH = tanA * self->farClip;
    f32 farW = self->aspect * farH;
    self->field_20 = farH;
    self->field_1C = farW;

    dir[0] = -farW;
    dir[1] = -self->field_20;
    dir[2] = self->farClip;
    self->node->vfunc2(dir, out + 3);

    dir[1] = self->field_20;
    f32* c9 = out + 9;
    self->node->vfunc2(dir, c9);

    dir[0] = self->field_1C;
    dir[1] = -self->field_20;
    self->node->vfunc2(dir, out + 6);

    dir[1] = self->field_20;
    self->node->vfunc2(dir, out + 12);

    f32 nearH = tanA * self->nearClip;
    f32 nearW = self->aspect * nearH;
    self->field_18 = nearH;
    self->field_14 = nearW;

    dir[0] = -nearW;
    dir[1] = -self->field_18;
    dir[2] = self->nearClip;
    f32* c15 = out + 15;
    self->node->vfunc2(dir, c15);

    dir[1] = self->field_18;
    f32* c21 = out + 21;
    self->node->vfunc2(dir, c21);

    dir[0] = self->field_14;
    dir[1] = -self->field_18;
    f32* c18 = out + 18;
    self->node->vfunc2(dir, c18);

    dir[1] = self->field_18;
    self->node->vfunc2(dir, out + 24);

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

    self->node->vfunc15(norm);
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

extern "C" void func_800141D8(Camera* self, f32* out) {
    f32 dir[3];
    f32 e0[3];
    f32 e1[3];
    f32 norm[3];
    f32 farExt[4];

    self->node->vfunc17(out);

    f32 ratio = self->farClip / self->nearClip;
    farExt[0] = self->field_FC * ratio;
    farExt[2] = self->field_104 * ratio;
    farExt[1] = self->field_100 * ratio;
    farExt[3] = self->field_108 * ratio;
    self->field_20 = farExt[3] - farExt[1];
    self->field_1C = farExt[2] - farExt[0];

    dir[0] = farExt[0];
    dir[1] = farExt[1];
    dir[2] = self->farClip;
    self->node->vfunc2(dir, out + 3);

    dir[1] = farExt[3];
    f32* c9 = out + 9;
    self->node->vfunc2(dir, c9);

    dir[0] = farExt[2];
    dir[1] = farExt[1];
    self->node->vfunc2(dir, out + 6);

    dir[1] = farExt[3];
    self->node->vfunc2(dir, out + 12);

    dir[0] = self->field_FC;
    dir[1] = self->field_100;
    dir[2] = self->nearClip;
    f32* c15 = out + 15;
    self->node->vfunc2(dir, c15);

    dir[1] = self->field_108;
    f32* c21 = out + 21;
    self->node->vfunc2(dir, c21);

    dir[0] = self->field_104;
    dir[1] = self->field_100;
    f32* c18 = out + 18;
    self->node->vfunc2(dir, c18);

    dir[1] = self->field_108;
    self->node->vfunc2(dir, out + 24);

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

    self->node->vfunc15(norm);
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

extern "C" s32 func_800148C4(Camera* self, f32* pos, f32 dist, f32* outHit) {
    f32 hitPoint[3];
    f32 slot1[4];
    f32 slot2[4];
    f32 slot0[4];
    f32 slot3[4];
    f32 dir[3];
    f32 upDir[3];
    f32 hitDir[3];

    s32 result = func_80029480(&self->frustum, pos, dist);
    if (result == 0) {
        return 0;
    }

    self->node->vfunc13(dir);

    f32* hp = hitPoint;
    f32 nd;

    f32* plane = self->frustum.planes[1];
    {
        f32 denom = dir[0] * plane[0] + dir[1] * plane[1];
        denom += dir[2] * plane[2];
        nd = -denom;
        f32 t = dist / nd;
        hitDir[0] = dir[0] * t;
        hitDir[1] = dir[1] * t;
        hitDir[2] = dir[2] * t;
    }
    hitPoint[0] = pos[0] + hitDir[0];
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    self->vfunc9(hp, slot0);

    plane = self->frustum.planes[0];
    {
        f32 denom = dir[0] * plane[0] + dir[1] * plane[1];
        denom += dir[2] * plane[2];
        nd = dist / denom;
        f32 t = -nd;
        hitDir[0] = dir[0] * t;
        hitDir[1] = dir[1] * t;
        hitDir[2] = dir[2] * t;
    }
    hitPoint[0] = pos[0] + hitDir[0];
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    self->vfunc9(hp, slot1);

    self->node->vfunc14(upDir);

    plane = self->frustum.planes[3];
    {
        f32 denom = upDir[0] * plane[0] + upDir[1] * plane[1];
        denom += upDir[2] * plane[2];
        nd = dist / denom;
        f32 t = -nd;
        hitDir[0] = upDir[0] * t;
        hitDir[1] = upDir[1] * t;
        hitDir[2] = upDir[2] * t;
    }
    hitPoint[0] = pos[0] + hitDir[0];
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    self->vfunc9(hp, slot2);

    plane = self->frustum.planes[2];
    {
        f32 denom = upDir[0] * plane[0] + upDir[1] * plane[1];
        denom += upDir[2] * plane[2];
        nd = -denom;
        f32 t = dist / nd;
        hitDir[0] = upDir[0] * t;
        hitDir[1] = upDir[1] * t;
        hitDir[2] = upDir[2] * t;
    }
    hitPoint[0] = pos[0] + hitDir[0];
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    self->vfunc9(hp, slot3);

    outHit[0] = slot1[0];
    outHit[2] = slot0[0];
    outHit[1] = slot2[1];
    outHit[3] = slot3[1];

    return result;
}

extern "C" void func_80014BF0(Camera* self) {
    SceneTarget* scene = self->sceneTarget;
    if (scene == NULL) {
        return;
    }

    scene->vfunc24(0);

    scene = self->sceneTarget;
    WorldNode* world = scene->vfunc23(0);

    CameraNode* child = world->vfunc7(self->field_28);

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

    scene = self->sceneTarget;
    scene->vfunc12(&pos.x, &newPos.x);

    scene = self->sceneTarget;
    scene->vfunc14(&dir1.x, &newDir1.x);

    scene = self->sceneTarget;
    scene->vfunc14(&dir2.x, &newDir2.x);

    self->node->vfunc18(&newPos.x);
    self->flags |= 1;

    newDir2.x = -newDir2.x;
    newDir2.y = -newDir2.y;
    newDir2.z = -newDir2.z;

    self->node->vfunc10(&newDir1.x, &newDir2.x);
    self->flags |= 1;
}

extern "C" void func_80014E40(Camera* self) {
    self->sceneTarget = NULL;
    self->field_28 = 0;
}

extern "C" void func_80014E4C(Camera* self, SceneTarget* a, s32 b) {
    self->sceneTarget = a;
    self->field_28 = b;
}

extern "C" void func_80014E58(void) {
}

extern "C" void func_80014E60(Camera* self, f32 value) {
    if (value > 0.0f) {
        self->aspect = value;
        self->flags |= 8;
    } else {
        self->flags &= ~8;
    }
    self->flags |= 3;
}

extern "C" void func_80014EA4(Camera* self, f32 fov, f32 nearClip, f32 farClip, f32 aspect) {
    self->fov = fov;
    self->nearClip = nearClip;
    self->farClip = farClip;
    func_80014E60(self, aspect);
}

extern "C" void func_80014ED0(Camera* self, f32* eye, f32* target, f32* up) {
    f32 dir[3];
    f32 negUp[3];

    dir[0] = target[0] - eye[0];
    dir[1] = target[1] - eye[1];
    dir[2] = target[2] - eye[2];

    negUp[0] = -up[0];
    negUp[1] = -up[1];
    negUp[2] = -up[2];

    self->node->vfunc10(dir, negUp);
    self->node->vfunc18(eye);
}

INCLUDE_ASM("asm/nonmatchings/camera", _._6Camera);

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

extern "C" s32 func_80015038(Camera* self) {
    return self->field_28;
}

extern "C" SceneTarget* func_80015044(Camera* self) {
    return self->sceneTarget;
}

extern "C" s32 func_80015050(Camera* self) {
    return self->sceneTarget != NULL;
}

extern "C" void func_8001505C(Camera* self, Viewport* out) {
    *out = self->viewport;
}

extern "C" s32 func_80015084(Camera* self) {
    return self->flags & 4;
}

extern "C" s32 func_80015090(Camera* self) {
    return ((self->flags >> 1) ^ 1) & 1;
}

extern "C" s32 func_800150A4(Camera* self) {
    return (self->flags ^ 1) & 1;
}

extern "C" f32 func_800150B4(Camera* self) {
    return self->field_20;
}

extern "C" f32 func_800150C0(Camera* self) {
    return self->field_1C;
}

extern "C" f32 func_800150CC(Camera* self) {
    return self->field_18;
}

extern "C" f32 func_800150D8(Camera* self) {
    return self->field_14;
}

extern "C" void func_800150E4(Camera* self, f32* out) {
    self->node->vfunc17(out);
}

extern "C" void func_80015114(Camera* self, f32 value) {
    self->farClip = value;
    self->flags |= 2;
}

extern "C" f32 func_80015128(Camera* self) {
    return self->farClip;
}

extern "C" void func_80015134(Camera* self, f32 value) {
    self->nearClip = value;
    self->flags |= 2;
}

extern "C" f32 func_80015148(Camera* self) {
    return self->nearClip;
}

extern "C" f32 func_80015154(Camera* self) {
    return self->aspect;
}

extern "C" f32 func_80015160(Camera* self) {
    return self->fov;
}

extern "C" void func_8001516C(Camera* self, f32 value) {
    self->fov = value;
    self->flags |= 2;
}

extern "C" void func_80015180(Camera* self, f32* pos) {
    self->node->vfunc18(pos);
    self->flags |= 1;
}

extern "C" void func_800151C8(Camera* self, CameraNode** arg) {
    self->node->vfunc19(*arg);
    self->flags |= 1;
}

extern "C" void func_80015210(Camera* self, f32* dir, f32* up) {
    self->node->vfunc10(dir, up);
    self->flags |= 1;
}

extern "C" void func_80015258(Camera* self, f32* a, f32* b) {
    self->node->vfunc8(a, b);
}

extern "C" void func_80015288(Camera* self, f32* vec) {
    self->node->vfunc13(vec);
}

extern "C" void func_800152B8(Camera* self, f32* out) {
    self->node->vfunc13(out);
    out[0] = -out[0];
    out[1] = -out[1];
    out[2] = -out[2];
}

extern "C" void func_80015314(Camera* self, f32* vec) {
    self->node->vfunc15(vec);
}

extern "C" void func_80015344(Camera* self, f32* out) {
    self->node->vfunc15(out);
    out[0] = -out[0];
    out[1] = -out[1];
    out[2] = -out[2];
}

extern "C" void func_800153A0(Camera* self, f32* vec) {
    self->node->vfunc14(vec);
}

extern "C" void func_800153D0(Camera* self, f32* out) {
    self->node->vfunc14(out);
    out[0] = -out[0];
    out[1] = -out[1];
    out[2] = -out[2];
}

extern "C" void func_8001542C(Camera* self, FrustumNearData* out) {
    *out = self->frustum.near;
}

extern "C" FrustumData* func_8001547C(Camera* self) {
    return &self->frustum;
}

extern "C" void func_80015484(Camera* self, FrustumData* out) {
    *out = self->frustum;
}

extern "C" void func_800154D4(Camera* self, f32* src, f32* dst) {
    self->node->vfunc2(src, dst);
}
