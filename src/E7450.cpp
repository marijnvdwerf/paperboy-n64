#include "common.h"
#include "jabiru.h"
#include "pademelon.h"

struct E7450Class : public Pademelon {
    /* 0x64 */ Jabiru* jabiru;
    /* 0x68 */ char pad68[4];
    /* 0x6C */ s32 unk6C;

    E7450Class();
    void func_8011F070(Vec3f* a, Vec3f* b, TriData* triData, Vec3f* hitPoint, s32* hitIndex);
    void func_8011F118();
    void func_8011F124(Jabiru* j);
};

void E7450Class::func_8011F070(Vec3f* a, Vec3f* b, TriData* triData, Vec3f* hitPoint, s32* hitIndex) {
    Vec3f localA;
    Vec3f localB;
    vfunc13(a, &localA);
    vfunc13(b, &localB);
    jabiru->func_8011F7B4(&localA, &localB, triData, hitPoint, hitIndex, NULL);
}

void E7450Class::func_8011F118() {
    jabiru = NULL;
    unk6C = 0;
}

void E7450Class::func_8011F124(Jabiru* j) {
    jabiru = j;
}

E7450Class::E7450Class() {
    jabiru = NULL;
    unk6C = 0;
}
