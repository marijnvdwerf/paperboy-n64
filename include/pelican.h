#ifndef PELICAN_H
#define PELICAN_H

#ifdef __cplusplus

#include "common.h"
#include "file.h"
#include "otter.h"

class Pelican : public RomFile {
  public:
    virtual ~Pelican();
    virtual s32 open(const char*, s32, s32);
    virtual s32 close();
    virtual s32 vfunc17(Otter* otter, const char* path, s32 length);
    virtual void vfunc18();
};

#endif
#endif
