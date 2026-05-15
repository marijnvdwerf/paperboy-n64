#ifndef SCENE_H
#define SCENE_H

#include "common.h"

struct Joey {
    Joey* parent; // 0x0
    Joey* next; // 0x4
    Joey* child; // 0x8
    // vptr at 0xC
    Joey();
    void addChild(Joey* child);
    void removeChild(Joey* child);
    virtual void vfunc1() = 0;
    virtual void vfunc2() = 0;
    virtual void vfunc3() = 0;
    virtual void vfunc4() = 0;
    virtual void vfunc5() = 0;
    virtual void vfunc6() = 0;
    virtual void vfunc7() = 0;
    virtual void vfunc8() = 0;
    virtual void vfunc9() = 0;
    virtual void vfunc10() = 0;
    virtual void vfunc11() = 0;
    virtual void vfunc12() = 0;
    virtual void vfunc13() = 0;
    virtual void vfunc14() = 0;
    virtual void vfunc15() = 0;
    virtual void vfunc16() = 0;
    virtual void vfunc17() = 0;
    virtual void vfunc18() = 0;
    virtual void vfunc19() = 0;
};

#endif
