#ifndef SURFACE_GOL_ATTACHED_SURFACE_H
#define SURFACE_GOL_ATTACHED_SURFACE_H

#pragma interface

#include "common.h"
#include "gol_surface.h"

class GolAttachedSurface : public GolSurface {
  public:
    /* 0x30 */ void* unk30;

    GolAttachedSurface();
};

#endif
