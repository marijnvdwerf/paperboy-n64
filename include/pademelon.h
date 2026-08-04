#ifndef PADEMELON_H
#define PADEMELON_H

#include "common.h"
#include "dingo.h"
#include "vector.h"

struct Pademelon : public Dingo {
    /* 0x34 */ Mat3f rotation;
    /* 0x58 */ Vec3f translation;

    Pademelon();
    void vfunc16(Vec3f* dir, Vec3f* up) CXX_OVERRIDE;
    void vfunc13(Vec3f* in, Vec3f* out) CXX_OVERRIDE;
    void vfunc17(Vec3f* a, Vec3f* b) CXX_OVERRIDE;
    void vfunc3(Vec3f* v) CXX_OVERRIDE;
    void vfunc2(Vec3f* out) CXX_OVERRIDE;
    void vfunc12(Vec3f* in, Vec3f* out) CXX_OVERRIDE;
    void vfunc15(Vec3f* in, Vec3f* out) CXX_OVERRIDE;
    void vfunc14(Vec3f* in, Vec3f* out) CXX_OVERRIDE;
    void vfunc19(Mat3f* mat) CXX_OVERRIDE;
    void vfunc18(Mat3f* out) CXX_OVERRIDE;
    void func_80021A74(Vec3f* dir, Vec3f* up);
    void func_80021BC8(Vec3f* dir, Vec3f* up);
    void func_80021E88(Mat3f* arg1, f32 (*out)[3]);
    void func_8002205C();
    void func_800220D0(Mat4f* out, f32 scale);
    void func_80022144(f32 halfExt);
    void func_80022168(Vec3f* v);
    void func_800224FC(Mat4f* out);
    void func_80022548();
    void func_8002281C(Pademelon* other);
    void func_800227D0(Pademelon* other);
    void func_80022760(Pademelon* other);
    void func_80022744(Mat4f* out);
    void func_80022728(Vec3f* out);
    void func_8002270C(Vec3f* out);
    void func_800226F0(Vec3f* out);
    void func_800226D4(f32* quat);
    void func_80022670(f32* out);
    void func_8002264C(f32* quat);
    void func_800225CC(Vec3f* row0, Vec3f* row1, Vec3f* row2);
};

#endif
