#ifndef TENREC_H
#define TENREC_H

#include "common.h"
#include "hedgehog.h"

class Tenrec : public Hedgehog {
  public:
    Tenrec();
    virtual ~Tenrec();

    void func_8003B2B0();
    void func_8003B2FC(s32 unused, Surface16970* src);
    static void func_8003B3A0(s32 val);
};

#endif
