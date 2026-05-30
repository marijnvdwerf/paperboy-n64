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

// Sphere-in-frustum cull, implemented (and matched) in src/29ED0.cpp.
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
    virtual void vfunc10(f32*) = 0;  // func_800148C4
    virtual UNK vfunc11(UNK) = 0;
};

// Frustum plane computation from FOV/aspect/near/far. Same plane builder as func_800141D8,
// but the corner extents come from FOV trig: angle = fov * (pi/180/2), tanA = sin/cos held
// in fs0 across __sinf/__cosf and across the far-corner calls so it also scales the near
// extents. field_1C/20 (far) and field_14/18 (near) are reloaded for each corner.
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

    // Far corners via vfunc2
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

    // Plane 0: cross(e1, e0)
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

    // Plane 1: cross(e0, e1)
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

    // Plane 2: cross(e1, e0)
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

    // Plane 3: cross(e0, e1)
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

    // Plane 5 (+norm) and Plane 4 (-norm) from vfunc15
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

// Frustum plane computation variant: like func_80013AD0 but the corner extents come
// from precomputed fields (field_FC/100/104/108) scaled by the far/near ratio rather
// than from FOV trig. The extents live in a stack array so they reload across the
// vfunc2 calls (matching the target's spill/reload rather than callee-saved registers).
extern "C" void func_800141D8(Camera* self, f32* out) {
    f32 dir[3];
    f32 e0[3];
    f32 e1[3];
    f32 norm[3];
    f32 farExt[4];

    self->node->vfunc17(out);

    // Precomputed frustum extents from field_FC-108 scaled by the far/near ratio.
    // Held in a stack array (not registers) so they reload across the vfunc2 calls.
    f32 ratio = self->farClip / self->nearClip;
    farExt[0] = self->field_FC * ratio;
    farExt[2] = self->field_104 * ratio;
    farExt[1] = self->field_100 * ratio;
    farExt[3] = self->field_108 * ratio;
    self->field_20 = farExt[3] - farExt[1];
    self->field_1C = farExt[2] - farExt[0];

    // 8 vfunc2 calls to transform frustum corners
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

    // Plane 0: cross(e1, e0)
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

    // Plane 1: cross(e0, e1)
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

    // Plane 2: cross(e1, e0)
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

    // Plane 3: cross(e0, e1)
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

    // Plane 5 (+norm) and Plane 4 (-norm) from vfunc15
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

// vfunc10: Frustum ray-plane intersection.
// Score 120: structure/signature/stack/scheduling all match; the ENTIRE diff is one
// register-naming difference repeated across the 4 plane blocks -- the divide/negate
// result `t` lands in the dying dot-accumulator reg (fv1) instead of the target's freed
// fv0. Mechanism (confirmed this session): two interacting GCC local-alloc combine_regs
// ties. (1) At the dot's final `add`, the dot ties to the accumulator operand (op0) and
// stays in fv1 -- correct. (2) At the neg/div, `t` ties to the dying dot and coalesces
// into fv1 -- this is the diff. The target keeps {neg,div,t} as a SEPARATE quantity in
// fv0 (P2's freed reg). The deeper tension: the target's accumulator RIDES ON n0's
// transient reg (`mul fv1,ft2,fv1` reuses the just-loaded normal -> dot=fv1), which only
// happens with a transient/unnamed dot; but SPLITTING t off requires dot to be a named
// multi-def quantity, whose register is pre-fixed and can NOT ride on n0. These two are
// mutually exclusive in this allocator. Solution space mapped this session:
//   - unnamed dot (this draft): dot=fv1 (perfect block), t=fv1   -> score 120 (best)
//   - named `f32 dot`: t coalesces with dot at fv0, tie (1) flips to op1 so the dot
//     accumulator's first product shifts fv1->ft0           -> score 240 (worse)
//   - function-scoped running accumulator (one `dot` reused, dot=P0; dot+=P1; dot+=P2):
//     SPLITS t cleanly to fv0 and dot becomes a stable single-reg accumulator (target's
//     reg STORY) -- but as ONE cross-block quantity it can't ride n0 per-block, so the
//     whole block permutes (dot->ft3, dir comps shift)     -> score 600 (worse)
//   - per-block accumulators (distinct d1/d0/d3/d2, multi-def): identical to unnamed --
//     each per-block dot coalesces into t's reg chain (one mega-quantity through fv1),
//     perfect block but t=fv1                              -> score 120 (ties best)
//   - inline helper called 4x: breaks the stack (target caches the hitPoint ptr in s0
//     and uses 5 saved regs) AND forces uniform neg-first, losing the div-first
//     ordering on planes 0/3                                -> score 2533 (much worse)
// Why target works where no C form does: its per-block dots each ride n0 (->fv1) AND stay
// SEPARATE from the single cross-block `t` (->fv0). Any C that keeps dots transient (to
// ride n0) lets combine_regs coalesce t into the first block's dot at its neg, chaining
// all dots+t into one fv1 quantity; any C that separates them (named/reused dot) loses the
// per-block n0 ride. The split that the target has is not reachable from source here.
// Documented SN in-place neg/div reuse (see sn-codegen skill). Parked on INCLUDE_ASM
// (byte-exact). Also tried earlier: split neg/div, block-scoped t, negDot, quotient,
// hoist, associativity.
#ifdef NON_MATCHING
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

    // Get forward direction
    self->node->vfunc13(dir);

    // Intersect with plane[1]
    f32* plane = self->frustum.planes[1];
    f32 t = dist / -(dir[0] * plane[0] + dir[1] * plane[1] + dir[2] * plane[2]);
    hitDir[0] = dir[0] * t;
    hitDir[1] = dir[1] * t;
    hitDir[2] = dir[2] * t;
    hitPoint[0] = pos[0] + hitDir[0];
    f32* hp = hitPoint;
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    self->vfunc9(hp, slot0);

    // Intersect with plane[0]
    plane = self->frustum.planes[0];
    t = -(dist / (dir[0] * plane[0] + dir[1] * plane[1] + dir[2] * plane[2]));
    hitDir[0] = dir[0] * t;
    hitDir[1] = dir[1] * t;
    hitDir[2] = dir[2] * t;
    hitPoint[0] = pos[0] + hitDir[0];
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    self->vfunc9(hp, slot1);

    // Get up direction
    self->node->vfunc14(upDir);

    // Intersect with plane[3]
    plane = self->frustum.planes[3];
    t = -(dist / (upDir[0] * plane[0] + upDir[1] * plane[1] + upDir[2] * plane[2]));
    hitDir[0] = upDir[0] * t;
    hitDir[1] = upDir[1] * t;
    hitDir[2] = upDir[2] * t;
    hitPoint[0] = pos[0] + hitDir[0];
    hp[1] = pos[1] + hitDir[1];
    hp[2] = pos[2] + hitDir[2];
    self->vfunc9(hp, slot2);

    // Intersect with plane[2]
    plane = self->frustum.planes[2];
    t = dist / -(upDir[0] * plane[0] + upDir[1] * plane[1] + upDir[2] * plane[2]);
    hitDir[0] = upDir[0] * t;
    hitDir[1] = upDir[1] * t;
    hitDir[2] = upDir[2] * t;
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
#else
INCLUDE_ASM("asm/nonmatchings/camera", func_800148C4);
#endif

// Scene graph traversal: walks parent chain, accumulates transforms, sets lookAt
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

    // Get position and orientation from child
    child->vfunc17(&pos.x);
    child->vfunc9(&dir1.x, &dir2.x);

    // Walk parent chain, accumulating transforms
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

    // Final transforms through the scene target
    scene = self->sceneTarget;
    scene->vfunc12(&pos.x, &newPos.x);

    scene = self->sceneTarget;
    scene->vfunc14(&dir1.x, &newDir1.x);

    scene = self->sceneTarget;
    scene->vfunc14(&dir2.x, &newDir2.x);

    // Set camera position
    self->node->vfunc18(&newPos.x);
    self->flags |= 1;

    // Negate up vector
    newDir2.x = -newDir2.x;
    newDir2.y = -newDir2.y;
    newDir2.z = -newDir2.z;

    // Set camera look direction
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

// vfunc9 - empty virtual override
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

// Calls node->vfunc17(out) - out is pass-through from caller in a1
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

// Calls node->vfunc18(pos) - pos is pass-through from caller in a1
extern "C" void func_80015180(Camera* self, f32* pos) {
    self->node->vfunc18(pos);
    self->flags |= 1;
}

// Calls node->vfunc19(arg), then flags |= 1
extern "C" void func_800151C8(Camera* self, CameraNode** arg) {
    self->node->vfunc19(*arg);
    self->flags |= 1;
}

// Calls node->vfunc10(dir, up) - pass-through from caller
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

// Copies header + near corners (0x3C bytes) to output
extern "C" void func_8001542C(Camera* self, FrustumNearData* out) {
    *out = self->frustum.near;
}

extern "C" FrustumData* func_8001547C(Camera* self) {
    return &self->frustum;
}

// Copies all frustum data (0xCC bytes) to output
extern "C" void func_80015484(Camera* self, FrustumData* out) {
    *out = self->frustum;
}

extern "C" void func_800154D4(Camera* self, f32* src, f32* dst) {
    self->node->vfunc2(src, dst);
}
