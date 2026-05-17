#ifndef FILE_H
#define FILE_H

#ifdef __cplusplus

#include "common.h"

class JamArchive;

class AbstractFile {
  public:
    /* 0x00 */ s32 openFlags;
    /* 0x04 */ s32 state;
    /* 0x08 */ s32 fileOffset;
    /* 0x0C */ s32 readCursor;
    /* 0x10 */ s32 fileSize;
    /* 0x14 */ u32 bufferCapacity;
    /* 0x18 */ s32 bufferStart;
    /* 0x1C */ s32 bufferEnd;
    /* 0x20 */ u8* buffer;
    /* 0x24 */ s32 archiveIndex;
    /* 0x28 */ // vtable

    AbstractFile();
    virtual s32 rawOpen() = 0;
    virtual s32 rawClose() = 0;
    virtual s32 seek(u32) = 0;
    virtual s32 rawRead(void*, s32, s32*) = 0;
    virtual s32 rawWrite(void* buf, s32 len);
    virtual s32 sync();
    virtual ~AbstractFile();
    virtual s32 open(const char*, s32, s32);
    virtual s32 close();
    virtual s32 readAt(s32, void*, s32, void*);
    virtual s32 read(void*, s32);
    virtual s32 writeAt(s32 pos, void* buf, s32 len);
    virtual s32 writeLine(void* buf, s32 len);
    virtual s32 flush();

    static s32 findFile(const char*);
    static void buildPath(const char*, const char*);
    void setBuffer(s32, s32, u8*);
    static void setSectorSize(s32);
    static void resetAllBuffers();
    static void addSearchPath(const char*);
    static void clearSearchPaths();
    static s32 isAbsolutePath(const char*);
    static void toUpperCase(char*);
    static char* errorMessage(s32);
    static void setCurrentDir(const char*);
    static char* getCurrentDir();
    static u32 getSearchPathCount();
    static char* getSearchPath(s32);
    static u32 getArchiveCount();
    static JamArchive* getArchives();
    static void setArchives(JamArchive*, u32);
    void reset();
    void init();
    s32 locateInArchives();
    s32 getFileSize();
    s32 isDirty();
    s32 isBuffered();
    s32 isArchiveMode();
    s32 isCreateMode();
    s32 isReadMode();
    s32 isSequentialMode();
    s32 isOpen();
    void resetBufferRange();
};

class RomFile : public AbstractFile {
  public:
    /* 0x2C */ u32 unk2C;

    RomFile();

    virtual s32 rawOpen();
    virtual s32 rawClose();
    virtual s32 seek(u32);
    virtual s32 rawRead(void*, s32, s32*);
    virtual ~RomFile();
    virtual s32 rawReadAsync(void*, s32, s32*);
    virtual s32 rawReadWait();

    static s32 func_80048A30();
    s32 func_80048A40(s32, s32, s32, s32);
    static s32 func_80048D58(char* path);
};

class File : public RomFile {};

#endif

#endif
