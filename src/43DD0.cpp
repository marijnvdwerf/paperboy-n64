#include "common.h"
#include "structs.h"

enum OpenFlags {
    OPEN_WRITE = 0x01,
    OPEN_READ = 0x02,
    OPEN_CREATE = 0x04,
    OPEN_SEQUENTIAL = 0x08,
    OPEN_EXTBUFFER = 0x20,
    OPEN_EXCLUSIVE = 0x40,
};

enum IOState {
    IO_OPEN = 0x01,
    IO_ARCHIVE = 0x02,
    IO_BUFFERED = 0x04,
    IO_DIRTY = 0x08,
};

extern "C" JamArchive* D_800763F8;
extern "C" u32 D_800763FC;
extern "C" u32 D_80076400;
extern "C" char D_80076404[256];
extern "C" s32 D_80076504;
extern "C" char D_80076508[256];
extern "C" char* D_80076608[4];
extern "C" s32 D_80076390[28];
extern "C" u8 D_80064360[];

extern "C" unsigned strlen(const char*);
extern "C" char* strcpy(char*, const char*);
extern "C" char* strcat(char*, const char*);
extern "C" char* strrchr(const char*, int);
extern "C" char* strncpy(char*, const char*, unsigned);
extern "C" int toupper(int);
extern "C" void* memcpy(void*, const void*, unsigned);

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390();
extern "C" s32 D_800047F0;
extern "C" void func_800079A8(const void*, s32, s32, s32);

// reg-alloc diffs: s2 init from s3 vs zero, instruction reordering in loop and search path section
#ifdef NON_MATCHING
s32 AbstractFile::findFile(const char* filename) {
    s32 result = 8;
    s32 isAbsolute = isAbsolutePath(filename);
    if (isAbsolute == 0) {
        buildPath(NULL, filename);
        u32 i = 0;
        do {
            if (i >= D_800763FC) {
                break;
            }
            result = D_800763F8[i].findFile(D_80076508);
            i++;
        } while (result == 8);
        if (result == 8) {
            goto check_search_paths;
        }
    } else {
    check_search_paths:
        if (D_80076400 != 0 && isAbsolute == 0) {
            result = 8;
            u32 j = 0;
            char** searchPath = D_80076608;
            do {
                if (j >= D_80076400) {
                    goto done_search;
                }
                buildPath(*searchPath, filename);
                searchPath++;
                j++;
                result = RomFile::func_80048D58(D_80076508);
                if (result != 8) {
                    goto done_search;
                }
            } while (1);
        } else {
            buildPath(NULL, filename);
            result = RomFile::func_80048D58(D_80076508);
        done_search:;
        }
    }
    return result;
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", findFile__12AbstractFilePCc);
#endif

// complex: archive search, search paths, buffer allocation
#ifdef NON_MATCHING
s32 AbstractFile::open(const char* filename, s32 flags, s32 bufferSize) {
    if (this->state & 1) {
        this->close();
    }
    this->state = 0;
    this->readCursor = 0;
    this->bufferStart = 0;
    this->bufferEnd = 0;
    this->fileOffset = 0;
    this->openFlags = flags;

    s32 isAbsolute = isAbsolutePath(filename);
    s32 result;
    if (isAbsolute == 0) {
        buildPath(NULL, filename);
        s32 v1 = 8;
        if (D_800763F8 != NULL) {
            result = 8;
            if (this->openFlags & 0x45) {
            } else if (isAbsolute < D_800763FC) {
                u32 i = 0;
                s32 byteOff = 0;
                do {
                    JamArchive* archive = &D_800763F8[i];
                    v1 = archive->locate(D_80076508, &this->fileOffset, &this->fileSize);
                    if (v1 == 0) {
                        this->archiveIndex = i;
                        this->state = 3;
                        break;
                    }
                    i++;
                    byteOff += 0x34;
                } while (i < D_800763FC);
            }
            result = v1;
        } else {
            result = v1;
        }
        if (result != 8) {
            this->state |= (result == 0);
            return result;
        }
    }

    if (D_80076400 != 0 && isAbsolute == 0) {
        result = 8;
        u32 j = 0;
        char** searchPath = D_80076608;
        do {
            if (j >= D_80076400) {
                break;
            }
            buildPath(*searchPath, filename);
            searchPath++;
            j++;
            result = this->rawOpen();
            if (result != 8) {
                break;
            }
        } while (1);
    } else {
        buildPath(NULL, filename);
        result = this->rawOpen();
    }

    if (result != 0) {
        return result;
    }

    this->state = 1;
    if ((flags & 0x20) && this->buffer != 0) {
        return result;
    }

    delete[] this->buffer;
    this->buffer = NULL;

    if (bufferSize != 0) {
        bufferSize += bufferSize & 1;
        func_8004B3BC(D_80076504);
        this->buffer = new u8[bufferSize];
        func_8004B390();
        if (this->buffer == NULL) {
            this->close();
            return 4;
        }
    }
    this->bufferCapacity = bufferSize;
    return result;
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", open__12AbstractFilePCcll);
#endif

// beqzl vs beqz and reg-alloc diffs in multiply and delete paths
#ifdef NON_MATCHING
void AbstractFile::close() {
    if (this->state & 2) {
        D_800763F8[this->archiveIndex].close();
        this->archiveIndex = -1;
        this->openFlags = 0;
        this->state = 0;
        return;
    }
    this->rawClose();
    if (this->openFlags & 0x20) {
    } else {
        u8* buf = this->buffer;
        if (buf) {
            delete[] buf;
            this->buffer = NULL;
        }
    }
    this->bufferStart = 0;
    this->bufferEnd = 0;
    this->openFlags = 0;
    this->state = 0;
    this->archiveIndex = -1;
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", close__12AbstractFile);
#endif

// complex: large-read chunking, archive/buffer/unbuffered paths
#ifdef NON_MATCHING
s32 AbstractFile::readAt(s32 pos, void* buf, s32 len, void* outActual) {
    s32* actual = (s32*)outActual;
    *actual = 0;
    if (!(this->state & 1)) {
        return 7;
    }
    if (len == 0) {
        return 3;
    }

    while ((u32)len >= 0x1001) {
        s32 chunkActual;
        s32 result = this->readAt(pos, buf, 0x1000, &chunkActual);
        if (result != 0) {
            return result;
        }
        *actual += chunkActual;
        buf = (u8*)buf + chunkActual;
        len -= chunkActual;
        pos += chunkActual;
    }

    if (this->state & 2) {
        if ((u32)pos >= (u32)this->fileSize) {
            return 0x10;
        }
        if ((u32)this->fileSize < (u32)(pos + len)) {
            len = this->fileSize - pos;
        }
        JamArchive* archive = &D_800763F8[this->archiveIndex];
        s32 saved = *actual;
        s32 result = archive->read(pos + this->fileOffset, buf, len, actual);
        *actual += saved;
        return result;
    }

    if (this->state & 4) {
        s32 bufStart = this->bufferStart;
        if ((u32)pos >= (u32)bufStart && (u32)pos < (u32)this->bufferEnd) {
            u8* src = this->buffer + (pos - bufStart);
            u32 avail = this->bufferEnd - pos;
            if ((u32)this->bufferEnd >= (u32)(pos + len)) {
                memcpy(buf, src, len);
                *actual += len;
                return 0;
            }
            memcpy(buf, src, avail);
            buf = (u8*)buf + avail;
            pos += avail;
            *actual += avail;
            len -= avail;
        }
    }

    if (this->fileOffset != pos) {
        s32 result = this->seek(pos);
        if (result != 0) {
            return result;
        }
    }

    if (this->buffer) {
        while (1) {
            s32 chunkActual;
            this->state &= ~4;
            s32 result = this->rawRead(this->buffer, this->bufferCapacity, &chunkActual);
            if (result != 0) {
                if (result == 0x10 && *actual != 0) {
                    return 0;
                }
                return result;
            }
            s32 readStart = this->fileOffset;
            s32 readEnd = readStart + chunkActual;
            this->state |= 4;
            this->bufferStart = readStart;
            this->bufferEnd = readEnd;
            u8* src = this->buffer + (pos - readStart);
            if ((u32)readEnd >= (u32)(pos + len)) {
                memcpy(buf, src, len);
                *actual += len;
                len = 0;
            } else {
                u32 avail = readEnd - pos;
                memcpy(buf, src, avail);
                buf = (u8*)buf + avail;
                pos += avail;
                len -= avail;
                *actual += avail;
            }
            this->fileOffset += chunkActual;
            if (len == 0) {
                return result;
            }
        }
    }

    s32 chunkActual;
    s32 result = this->rawRead(buf, len, &chunkActual);
    if (result != 0) {
        if (result == 0x10 && *actual != 0) {
            return 0;
        }
        return result;
    }
    *actual += chunkActual;
    this->fileOffset += chunkActual;
    return result;
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", readAt__12AbstractFilelPvlT2);
#endif

// s0↔s1 reg-alloc, beqz/bnez inversions, beqzl in clamping
#ifdef NON_MATCHING
s32 AbstractFile::read(void* buf, s32 len) {
    u32 remaining = len;
    s32 flags = this->state;
    if (!(flags & 1)) {
        return 7;
    }
    if (remaining == 0) {
        return 3;
    }
    u32 bytesRead = 0;
    if (flags & 2) {
        u32 pos = this->readCursor;
        u32 size = this->fileSize;
        if (pos >= size) {
            return 0x10;
        }
        u32 origLen = remaining;
        if (size < pos + remaining) {
            remaining = size - pos;
        }
        JamArchive* archive = &D_800763F8[this->archiveIndex];
        s32 actual;
        s32 result = archive->readLine(pos + this->fileOffset, (u8*)buf, remaining, origLen, (u32*)&actual);
        if (result != 0) {
            return result;
        }
        this->readCursor += actual;
        return 0;
    }

    while (1) {
        s32 filePos = this->fileOffset;
        if (this->state & 4) {
            s32 bufStart = this->bufferStart;
            if ((u32)filePos >= (u32)bufStart && (u32)filePos < (u32)this->bufferEnd) {
                s32 offset = filePos - bufStart;
                u8 newline = '\n';
                u8 cr = '\r';
                u8* dst = (u8*)buf + bytesRead;
                while (1) {
                    u8 ch = this->buffer[offset];
                    if (ch == newline) {
                        *dst = 0;
                        if (bytesRead != 0 && *(dst - 1) == cr) {
                            *(dst - 1) = 0;
                        }
                        this->fileOffset = bufStart + offset + 1;
                        return 0;
                    }
                    if (bytesRead >= remaining) {
                        *((u8*)buf + remaining - 1) = 0;
                        this->fileOffset = bufStart + offset;
                        return 0;
                    }
                    filePos++;
                    offset++;
                    *dst = ch;
                    dst++;
                    if ((u32)filePos >= (u32)this->bufferEnd) {
                        this->fileOffset = filePos;
                        break;
                    }
                    bytesRead++;
                }
            }
        }
        this->state &= ~4;
        s32 actual;
        s32 result = this->rawRead(this->buffer, this->bufferCapacity, &actual);
        if (result != 0) {
            if (result != 0x10) {
                return result;
            }
            if (bytesRead != 0) {
                *((u8*)buf + bytesRead) = 0;
                return 0;
            }
            return result;
        }
        s32 pos = this->fileOffset;
        this->state |= 4;
        this->bufferStart = pos;
        this->bufferEnd = pos + actual;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", read__12AbstractFilePvl);
#endif

INCLUDE_RODATA("asm/nonmatchings/43DD0", D_800047F0);

// reg-alloc: s4↔s5 swap (dir vs nameLen) + one missing move s0,zero
#ifdef NON_MATCHING
void AbstractFile::buildPath(const char* dir, const char* filename) {
    u32 dirLen = 0;
    D_80076508[0] = '\0';
    u32 nameLen = strlen(filename);
    if (nameLen >= 256) {
        func_800079A8(&D_800047F0, 0, 0, 0);
        dirLen = 0;
    }
    s32 isAbsolute = 0;
    if (filename[0] == '\\' && filename[1] == '\\') {
        filename++;
        isAbsolute = 1;
    } else if ((D_80064360[(u8)filename[0]] & 6) && filename[1] == ':') {
        isAbsolute = 1;
    } else if (dir) {
        dirLen = strlen(dir);
        if (dirLen + nameLen + 1 >= 256) {
            func_800079A8(&D_800047F0, 0, 0, 0);
        }
        char* p = D_80076508;
        strcpy(p, dir);
        if (p[dirLen - 1] != '\\') {
            dirLen++;
            p[dirLen - 1] = '\\';
            p[dirLen] = '\0';
        }
    }
    char* hasBackslash = strrchr(filename, '\\');
    if (isAbsolute) {
        strcpy(D_80076508, filename);
    } else if (hasBackslash) {
        char* cwd = D_80076404;
        if (filename[0] == '\\' || cwd == NULL) {
            if (filename[0] == '\\') {
                filename++;
            }
        } else {
            if (strlen(cwd) + dirLen + nameLen + 1 >= 256) {
                func_800079A8(&D_800047F0, 0, 0, 0);
            }
            if (D_80076404[0] == '\\') {
                cwd++;
            }
            strcat(D_80076508, cwd);
            u32 len = strlen(D_80076508);
            if (D_80076508[len - 1] != '\\') {
                D_80076508[len] = '\\';
                D_80076508[len + 1] = '\0';
            }
        }
        strcat(D_80076508, filename);
    } else {
        char* cwd = D_80076404;
        if (cwd) {
            if (D_80076404[0]) {
                strcat(D_80076508, cwd);
                strcat(D_80076508, "\\");
            }
        }
        strcat(D_80076508, filename);
    }
    toUpperCase(D_80076508);
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", buildPath__12AbstractFilePCcPCc);
#endif

// delay slot filling (li v0,0xf) and beqzl in dirty-buffer check
#ifdef NON_MATCHING
s32 AbstractFile::writeAt(s32 pos, void* buf, s32 len) {
    if (!(this->state & 1)) {
        return 7;
    }
    if (this->buffer == NULL || this->bufferCapacity < len) {
        if (this->fileOffset != pos) {
            s32 err = this->seek(pos);
            if (err) {
                return err;
            }
            this->fileOffset = pos;
        }
        s32 err = this->rawWrite(buf, len);
        if (err) {
            return err;
        }
        this->fileOffset += len;
        return 0;
    }
    if (this->state & 8) {
        s32 buffered = this->bufferEnd - this->bufferStart;
        if (len + buffered <= this->bufferCapacity && pos == this->bufferEnd) {
            memcpy(this->buffer + buffered, buf, len);
            this->bufferEnd += len;
            return 0;
        }
        if (this->fileOffset != this->bufferStart) {
            s32 err = this->seek(this->bufferStart);
            if (err) {
                return 0xF;
            }
            this->fileOffset = this->bufferStart;
        }
        s32 err = this->rawWrite(this->buffer, buffered);
        if (err) {
            return err;
        }
        this->fileOffset += buffered;
    }
    this->bufferStart = pos;
    this->bufferEnd = pos + len;
    this->state |= 0xC;
    memcpy(this->buffer, buf, len);
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", writeAt__12AbstractFilelPvl);
#endif

// control flow structure: NULL/size checks share dirty-check path in target
#ifdef NON_MATCHING
s32 AbstractFile::writeLine(void* buf, s32 len) {
    char newline = '\n';
    if (!(this->state & 1)) {
        return 7;
    }
    if (this->buffer && this->bufferCapacity < len + 1) {
        if (this->state & 8) {
            s32 buffered = this->bufferEnd - this->bufferStart;
            if (this->bufferCapacity - buffered < len + 1) {
                this->flush();
            }
        }
        s32 err = this->rawWrite(buf, len);
        if (err) {
            return err;
        }
        err = this->rawWrite(&newline, 1);
        return err;
    }
    if (this->state & 8) {
        s32 buffered = this->bufferEnd - this->bufferStart;
        if (this->bufferCapacity - buffered < len + 1) {
            this->flush();
        }
    }
    s32 offset = this->bufferEnd - this->bufferStart;
    memcpy(this->buffer + offset, buf, len);
    s32 newEnd = this->bufferEnd + len;
    this->bufferEnd = newEnd;
    offset = newEnd - this->bufferStart;
    u8* p = this->buffer + offset;
    *p = newline;
    this->bufferEnd++;
    this->state |= 0xC;
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", writeLine__12AbstractFilePvl);
#endif

s32 AbstractFile::flush() {
    if (!(this->state & 1)) {
        return 7;
    }
    s32 buffered = this->bufferEnd - this->bufferStart;
    if (this->buffer == NULL || !(this->state & 8)) {
        return 0;
    }
    if (buffered == 0) {
        return 0;
    }
    if (!(this->openFlags & 8)) {
        if (this->fileOffset != this->bufferStart) {
            s32 err = this->seek(this->bufferStart);
            if (err) {
                return 0xF;
            }
            this->fileOffset = this->bufferStart;
        }
    }
    s32 err = this->rawWrite(this->buffer, buffered);
    if (err) {
        return err;
    }
    if (this->openFlags & 8) {
        this->bufferEnd = this->bufferStart;
    } else {
        this->fileOffset += buffered;
    }
    this->state &= ~8;
    return this->sync();
}

void AbstractFile::setBuffer(s32 size, s32 allocate, u8* buffer) {
    func_8004B3BC(D_80076504);
    if (this->state & 1) {
        if (allocate) {
            this->buffer = new u8[size];
        } else {
            delete[] this->buffer;
            this->buffer = buffer;
        }
        this->bufferEnd = 0;
        this->bufferStart = 0;
        this->bufferCapacity = size;
        this->state &= ~8;
    }
    func_8004B390();
}

void AbstractFile::setSectorSize(s32 val) {
    D_80076504 = val;
}

s32 AbstractFile::sync() {
    return 0;
}

s32 AbstractFile::rawWrite(void* buf, s32 len) {
    return 0;
}

void AbstractFile::resetAllBuffers() {
    s32 count = D_800763FC;
    JamArchive* archives = D_800763F8;
    while (count > 0) {
        count--;
        AbstractFile* io = archives->io;
        archives++;
        io->bufferEnd = 0;
        io->bufferStart = 0;
    }
}

void AbstractFile::addSearchPath(const char* path) {
    if (D_80076400 >= 4) {
        return;
    }
    func_8004B3BC(D_80076504);
    char* copy = new char[strlen(path) + 1];
    D_80076608[D_80076400] = copy;
    func_8004B390();
    if (D_80076608[D_80076400] == NULL) {
        func_800079A8(&D_800047F0, 0, 0, 0);
    }
    strcpy(D_80076608[D_80076400], path);
    D_80076400++;
}

// scheduling: move s2,v1 placed after lui/addiu D_80076608 instead of before
#ifdef NON_MATCHING
void AbstractFile::clearSearchPaths() {
    u32 i = 0;
    if (D_80076400 == 0) {
        return;
    }
    char** p = D_80076608;
    do {
        char* entry = *p;
        if (entry) {
            delete[] entry;
            *p = NULL;
        }
        p++;
        i++;
    } while (i < D_80076400);
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", clearSearchPaths__12AbstractFile);
#endif

s32 AbstractFile::isAbsolutePath(const char* path) {
    if (path[0] == '\\' && path[1] == '\\') {
        return 1;
    }
    if ((D_80064360[(u8)path[0]] & 6) && path[1] == ':') {
        return 1;
    }
    return 0;
}

void AbstractFile::toUpperCase(char* str) {
    u32 i = 0;
    while (*str && i < 256) {
        *str = toupper(*str);
        str++;
        i++;
    }
}

s32 AbstractFile::getDeviceParam(s32 index) {
    return D_80076390[index];
}

// reg-alloc and control flow structure diffs
#ifdef NON_MATCHING
s32 AbstractFile::locateInArchives() {
    s32 v1 = 8;
    if (D_800763F8 == NULL) {
        return v1;
    }
    if (this->openFlags & 0x45) {
        return 8;
    }
    for (u32 i = 0; i < D_800763FC; i++) {
        v1 = D_800763F8[i].locate(D_80076508, &this->fileOffset, &this->fileSize);
        if (v1 == 0) {
            this->archiveIndex = i;
            this->state = 3;
            break;
        }
    }
    return v1;
}
#else
INCLUDE_ASM("asm/nonmatchings/43DD0", locateInArchives__12AbstractFile);
#endif

void AbstractFile::reset() {
    delete[] this->buffer;
    this->openFlags = 0;
    this->state = 0;
    this->fileOffset = 0;
    this->fileSize = 0;
    this->readCursor = 0;
    this->bufferCapacity = 0;
    this->bufferStart = 0;
    this->bufferEnd = 0;
    this->buffer = NULL;
    this->archiveIndex = -1;
}

void AbstractFile::init() {
    this->openFlags = 0;
    this->state = 0;
    this->fileOffset = 0;
    this->fileSize = 0;
    this->readCursor = 0;
    this->bufferCapacity = 0;
    this->bufferStart = 0;
    this->bufferEnd = 0;
    this->buffer = NULL;
    this->archiveIndex = -1;
}

AbstractFile::~AbstractFile() {
    if (this->buffer) {
        delete[] this->buffer;
        this->buffer = NULL;
    }
    this->init();
}

AbstractFile::AbstractFile() {
    this->init();
}

void AbstractFile::setCurrentDir(const char* src) {
    if (src) {
        strncpy(D_80076404, src, 256);
        D_80076404[255] = '\0';
    } else {
        D_80076404[0] = '\0';
    }
}

char* AbstractFile::getCurrentDir() {
    return D_80076404;
}

u32 AbstractFile::getSearchPathCount() {
    return D_80076400;
}

char* AbstractFile::getSearchPath(s32 index) {
    return D_80076608[index];
}

u32 AbstractFile::getArchiveCount() {
    return D_800763FC;
}

JamArchive* AbstractFile::getArchives() {
    return D_800763F8;
}

void AbstractFile::setArchives(JamArchive* arg0, u32 arg1) {
    D_800763F8 = arg0;
    D_800763FC = arg1;
}

s32 AbstractFile::getFileSize() {
    return this->fileSize;
}

s32 AbstractFile::isDirty() {
    return this->state & 8;
}

s32 AbstractFile::isBuffered() {
    return this->state & 4;
}

s32 AbstractFile::isArchiveMode() {
    return this->state & 2;
}

s32 AbstractFile::isCreateMode() {
    return this->openFlags & 4;
}

s32 AbstractFile::isReadMode() {
    return this->openFlags & 2;
}

s32 AbstractFile::isSequentialMode() {
    return this->openFlags & 8;
}

s32 AbstractFile::isOpen() {
    return this->state & 1;
}

void AbstractFile::resetBufferRange() {
    this->bufferEnd = 0;
    this->bufferStart = 0;
}
