#include "fennec.h"
#include "render_context.h"

extern "C" s32 func_80029480(void* frustum, Vec3f* point, f32 radius);
extern "C" f32 sqrtf(f32);

s32 Fennec::vfunc9() {
    if (unk54 & 4) {
        unk34 = unk38->entries[unk56];
    }
    return unk34->flags & 0x1100;
}

void Fennec::vfunc8(SceneRenderer* scene) {
    ((RenderContext*)scene)->vfunc45((UNK)this);
}

FennecEntry* Fennec::func_801251EC() {
    if (unk54 & 4) {
        unk34 = unk38->entries[unk56];
    }
    return unk34;
}

void Fennec::func_80125224(Vec3f* point, DingoFrustumResult* out) {
    Vec3f myPos;
    out->pad = 0;
    func_80122414(&myPos);
    Vec3f* p = &myPos;
    f32 dx = point->x - p->x;
    f32 dy = point->y - p->y;
    f32 dz = point->z - p->z;
    f32 distSq = dx * dx + dy * dy + dz * dz;
    if (distSq == 0.0f) {
        out->result = 0;
        return;
    }
    if (unk50 < distSq) {
        out->result = 0;
        return;
    }
    out->result = 1;
}

inline u16 Fennec::func_80125600() {
    return unk54;
}

inline s32 Fennec::func_801255F4() {
    return unk54 & 2;
}

inline s32 Fennec::func_801255E8() {
    return unk54 & 4;
}

inline void Fennec::func_801255E0(FennecEntry* val) {
    unk34 = val;
}

inline f32 Fennec::func_801255D4() {
    return unk48;
}

inline f32 Fennec::func_801255C8() {
    return unk4C;
}

inline void Fennec::func_801255AC(Vec3f* out) {
    func_80122414(out);
}

inline void Fennec::func_801255A4(f32 val) {
    unk48 = val;
}

inline void Fennec::func_8012559C(f32 val) {
    unk4C = val;
}

inline void Fennec::vfunc3(Vec3f* v) {
    func_80122354(v);
}

inline void Fennec::func_80125554(Vec3f* v) {
    unk54 |= 2;
    unk3C = *v;
}

inline void Fennec::func_80125544() {
    unk54 &= ~2;
}

inline f32 Fennec::func_80125538() {
    return unk50;
}

inline void Fennec::func_80125530(f32 val) {
    unk50 = val;
}

inline void* Fennec::func_80125524() {
    return unk38;
}

inline Fennec::Fennec() {
    unk34 = NULL;
    unk38 = NULL;
    unk3C.x = 0;
    unk3C.y = 0;
    unk54 = 0;
    unk56 = 0;
    unk48 = 0;
    unk4C = 0;
    unk3C.z = 1.0f;
    unk50 = 9.999999934e+36f;
}

inline void Fennec::vfunc20(void* obj, f32 width, f32 height, f32 radius) {
    f32 hw = width * 0.5f;
    f32 hh = height * 0.5f;
    unk34 = (FennecEntry*)obj;
    unk54 = 1;
    unk48 = width;
    unk4C = height;
    unk50 = radius;
    func_80122330(sqrtf(hw * hw + hh * hh));
}

inline void Fennec::func_801253CC(FennecAnimData* data, s32 index, f32 width, f32 height, f32 radius) {
    unk56 = index;
    unk38 = data;
    FennecEntry* entry = data->entries[index];
    vfunc20(entry, width, height, radius);
    unk54 |= 4;
}

inline void Fennec::vfunc21() {
    unk34 = NULL;
    unk54 = 0;
    unk48 = 0;
    unk4C = 0;
}

inline void Fennec::vfunc6(Vec3f* frustum, DingoFrustumResult* out) {
    Vec3f myPos;
    func_80122414(&myPos);
    out->pad = 0;
    f32 radius = unk50;
    if (radius != 9.999999934e+36f) {
        Vec3f* p = &myPos;
        f32 dx = frustum->x - p->x;
        f32 dy = frustum->y - p->y;
        f32 dz = frustum->z - p->z;
        f32 distSq = dx * dx + dy * dy + dz * dz;
        if (distSq == 0.0f) {
            out->result = 0;
            return;
        }
        if (radius < distSq) {
            out->result = 0;
            return;
        }
    }
    out->result = func_80029480(frustum, &myPos, func_801223AC());
}
