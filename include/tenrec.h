#ifndef TENREC_H
#define TENREC_H

#include "common.h"
#include "surface/gol_attached_surface.h"

class Tenrec : public GolAttachedSurface {
  public:
    Tenrec();
    virtual ~Tenrec();

    void func_8003B2B0();
    void func_8003B2FC(s32 unused, GolSurface* src);
    static void func_8003B3A0(s32 val);
};

#endif
