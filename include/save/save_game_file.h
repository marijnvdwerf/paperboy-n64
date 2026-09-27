#ifndef SAVE_SAVE_GAME_FILE_H
#define SAVE_SAVE_GAME_FILE_H

#ifdef __cplusplus

#include "common.h"
#include "gol_stream.h"
#include "otter.h"

class SaveGameFile : public RomFile {
  public:
    virtual ~SaveGameFile();
    virtual s32 open(const char*, s32, s32);
    virtual s32 close();
    virtual s32 vfunc17(Otter* otter, const char* path, s32 length) = 0;
    virtual s32 vfunc18(Otter* otter, const char* path, s32 openFlags, s32 capacity, s32 length);

    s32 func_80045934();
};

#endif
#endif
