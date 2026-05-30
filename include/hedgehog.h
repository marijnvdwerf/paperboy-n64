#ifndef HEDGEHOG_H
#define HEDGEHOG_H

#pragma interface

#include "common.h"
#include "surfaces.h"

class Hedgehog : Surface16970 {
  public:
    /* 0x30 */ u32 unk30;

    Hedgehog() : unk30(0) {};
};

#endif
