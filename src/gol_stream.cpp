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

extern "C" GolFileSource* D_800763F8;
extern "C" u32 D_800763FC;
extern "C" u32 D_80076400;
extern "C" char D_80076404[256];
extern "C" s32 D_80076504;
extern "C" char D_80076508[256];
extern "C" char* D_80076608[4];
extern "C" char* D_80076390[28];
extern "C" u8 D_80064360[];

extern "C" unsigned strlen(const char*);
extern "C" char* strcpy(char*, const char*);
extern "C" char* strcat(char*, const char*);
extern "C" char* strrchr(const char*, int);
extern "C" char* strncpy(char*, const char*, unsigned);
extern "C" int toupper(int);

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390();

inline void GolStream::resetBufferRange() {
    this->bufferEnd = 0;
    this->bufferStart = 0;
}

inline s32 GolStream::isOpen() {
    return this->state & 1;
}

inline s32 GolStream::isSequentialMode() {
    return this->openFlags & 8;
}

inline s32 GolStream::isReadMode() {
    return this->openFlags & 2;
}

inline s32 GolStream::isCreateMode() {
    return this->openFlags & 4;
}

inline s32 GolStream::isArchiveMode() {
    return this->state & 2;
}

inline s32 GolStream::isBuffered() {
    return this->state & 4;
}

inline s32 GolStream::isDirty() {
    return this->state & 8;
}

inline s32 GolStream::getFileSize() {
    return this->fileSize;
}

inline void GolStream::setArchives(GolFileSource* arg0, u32 arg1) {
    D_800763F8 = arg0;
    D_800763FC = arg1;
}

inline GolFileSource* GolStream::getArchives() {
    return D_800763F8;
}

inline u32 GolStream::getArchiveCount() {
    return D_800763FC;
}

inline char* GolStream::getSearchPath(s32 index) {
    return D_80076608[index];
}

inline u32 GolStream::getSearchPathCount() {
    return D_80076400;
}

inline char* GolStream::getCurrentDir() {
    return D_80076404;
}

inline void GolStream::setCurrentDir(const char* src) {
    if (src) {
        strncpy(D_80076404, src, 256);
        D_80076404[255] = '\0';
    } else {
        D_80076404[0] = '\0';
    }
}

inline GolStream::GolStream() {
    this->init();
}

inline GolStream::~GolStream() {
    if (this->buffer) {
        delete[] this->buffer;
        this->buffer = NULL;
    }
    this->init();
}

inline void GolStream::init() {
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

inline void GolStream::reset() {
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

inline s32 GolStream::locateInArchives() {
    s32 result = 8;
    u32 i = 0;
    if (D_800763F8 == NULL) {
        return result;
    }
    if (this->openFlags & 0x45) {
        return 8;
    }
    for (i = 0; i < D_800763FC; i++) {
        result = D_800763F8[i].locate(D_80076508, &this->fileOffset, &this->fileSize);
        if (result == 0) {
            this->archiveIndex = i;
            this->state = 3;
            break;
        }
    }
    return result;
}

s32 GolStream::findFile(const char* filename) {
    s32 result = 8;
    s32 isAbsolute = isAbsolutePath(filename);
    if (isAbsolute == 0) {
        buildPath(NULL, filename);
        u32 i = 0;
        for (i = 0; result == 8; i++) {
            if (i >= D_800763FC) {
                break;
            }
            result = D_800763F8[i].findFile(D_80076508);
        }
        if (result != 8) {
            return result;
        }
    }
    if (D_80076400 != 0 && isAbsolute == 0) {
        result = 8;
        u32 i = 0;
        while (1) {
            if (i >= D_80076400) {
                break;
            }
            buildPath(D_80076608[i], filename);
            s32 err = RomFile::func_80048D58(D_80076508);
            i++;
            if (err != 8) {
                result = err;
                break;
            }
            result = err;
        }
    } else {
        buildPath(NULL, filename);
        result = RomFile::func_80048D58(D_80076508);
    }
    return result;
}

s32 GolStream::open(const char* filename, s32 flags, s32 bufferSize) {
    if (this->state & 1) {
        this->close();
    }
    this->openFlags = flags;
    this->state = 0;
    this->readCursor = 0;
    this->bufferStart = 0;
    this->bufferEnd = 0;
    this->fileOffset = 0;

    s32 isAbsolute = isAbsolutePath(filename);
    s32 result;
    if (isAbsolute == 0) {
        buildPath(NULL, filename);
        result = locateInArchives();
        if (result != 8) {
            this->state |= (result == 0);
            return result;
        }
    }

    if (D_80076400 != 0 && isAbsolute == 0) {
        result = 8;
        u32 j = 0;
        while (1) {
            if (j >= D_80076400) {
                break;
            }
            buildPath(D_80076608[j], filename);
            s32 err = this->rawOpen(D_80076508);
            j++;
            if (err != 8) {
                result = err;
                break;
            }
            result = err;
        }
    } else {
        buildPath(NULL, filename);
        result = this->rawOpen(D_80076508);
    }

    if (result != 0) {
        return result;
    }

    this->state = 1;
    if (!(flags & 0x20) || this->buffer == 0) {
        if (this->buffer != NULL) {
            delete[] this->buffer;
            this->buffer = NULL;
        }

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
    }
    return result;
}

s32 GolStream::close() {
    if (this->state & 2) {
        s32 result = D_800763F8[this->archiveIndex].close();
        this->archiveIndex = -1;
        this->openFlags = 0;
        this->state = 0;
        return result;
    }
    this->rawClose();
    if (!(this->openFlags & 0x20)) {
        if (this->buffer) {
            delete[] this->buffer;
            this->buffer = NULL;
        }
    }
    this->bufferStart = 0;
    this->bufferEnd = 0;
    this->openFlags = 0;
    this->state = 0;
    this->archiveIndex = -1;
    return 0;
}

s32 GolStream::readAt(u32 pos, void* buf, u32 len, s32* actual) {
    s32 chunkActual;
    s32 rawActual;
    s32 result;
    void* src;
    u32 avail;
    *actual = 0;
    if (!(this->state & 1)) {
        return 7;
    }
    if (len == 0) {
        return 3;
    }

    while (len >= 0x1001) {
        result = this->readAt(pos, buf, 0x1000, &chunkActual);
        if (result != 0) {
            return result;
        }
        *actual += chunkActual;
        buf = buf + chunkActual;
        len -= chunkActual;
        pos += chunkActual;
    }

    if (this->state & 2) {
        if (pos >= this->fileSize) {
            return 0x10;
        }
        if (this->fileSize < (pos + len)) {
            len = this->fileSize - pos;
        }
        chunkActual = *actual;
        result = D_800763F8[this->archiveIndex].read(pos + this->fileOffset, buf, len, actual);
        *actual += chunkActual;
        return result;
    }

    if (this->state & 4) {
        if (pos >= this->bufferStart && pos < this->bufferEnd) {
            src = this->buffer + (pos - this->bufferStart);
            if (this->bufferEnd >= (pos + len)) {
                memcpy(buf, src, len);
                *actual += len;
                return 0;
            }
            avail = this->bufferEnd - pos;
            memcpy(buf, src, avail);
            buf = buf + avail;
            pos += avail;
            *actual += avail;
            len -= avail;
        }
    }

    if (this->fileOffset != pos) {
        result = this->seek(pos);
        if (result != 0) {
            return result;
        }
    }

    if (this->buffer) {
        while (1) {
            this->state &= ~4;
            result = this->rawRead(this->buffer, this->bufferCapacity, &rawActual);
            if (result != 0) {
                if (result == 0x10 && *actual != 0) {
                    return 0;
                }
                return result;
            }
            this->state |= 4;
            this->bufferStart = this->fileOffset;
            this->bufferEnd = this->fileOffset + rawActual;
            src = this->buffer + (pos - this->bufferStart);
            if (this->bufferEnd >= (pos + len)) {
                memcpy(buf, src, len);
                *actual += len;
                len = 0;
            } else {
                avail = this->bufferEnd - pos;
                memcpy(buf, src, avail);
                buf = buf + avail;
                pos += avail;
                len -= avail;
                *actual += avail;
            }
            this->fileOffset += rawActual;
            if (len == 0) {
                break;
            }
        }
    } else {
        result = this->rawRead(buf, len, &rawActual);
        if (result != 0) {
            if (result == 0x10 && *actual != 0) {
                return 0;
            }
            return result;
        }
        *actual += rawActual;
        this->fileOffset += rawActual;
    }
    return result;
}

s32 GolStream::read(u8* buf, s32 len) {
    s32 result;
    if (!isOpen()) {
        return 7;
    }
    if (len == 0) {
        return 3;
    }
    if (isArchiveMode()) {
        if (this->readCursor >= this->fileSize) {
            return 0x10;
        }
        u32 origLen = len;
        if (this->fileSize < this->readCursor + len) {
            len = this->fileSize - this->readCursor;
        }
        u32 actual;
        result = D_800763F8[this->archiveIndex].readLine(this->readCursor + this->fileOffset, buf, len, origLen, &actual);
        if (result != 0) {
            return result;
        }
        this->readCursor += actual;
        return 0;
    }
    u32 bytesRead = 0;
    while (1) {
        u32 filePos = this->fileOffset;
        if (this->state & 4) {
            if (filePos >= this->bufferStart && filePos < this->bufferEnd) {
                s32 offset = filePos - this->bufferStart;
                while (1) {
                    result = this->buffer[offset];
                    if (result == '\n') {
                        buf[bytesRead] = 0;
                        if (bytesRead != 0 && buf[bytesRead - 1] == '\r') {
                            buf[bytesRead - 1] = 0;
                        }
                        this->fileOffset = this->bufferStart + offset + 1;
                        return 0;
                    }
                    if (bytesRead >= len) {
                        buf[len - 1] = 0;
                        this->fileOffset = this->bufferStart + offset;
                        return 0;
                    }
                    filePos++;
                    offset++;
                    buf[bytesRead++] = result;
                    if (filePos >= this->bufferEnd) {
                        this->fileOffset = filePos;
                        break;
                    }
                }
            }
        }
        this->state &= ~4;
        s32 actual;
        result = this->rawRead(this->buffer, this->bufferCapacity, &actual);
        if (result != 0) {
            if (result == 0x10 && bytesRead != 0) {
                *(buf + bytesRead) = 0;
                return 0;
            }
            return result;
        }
        this->state |= 4;
        this->bufferStart = this->fileOffset;
        this->bufferEnd = this->fileOffset + actual;
    }
}

void GolStream::buildPath(const char* dir, const char* filename) {
    D_80076508[0] = '\0';
    u32 nameLen = strlen(filename);
    if (nameLen >= 256) {
        __assert("", 0, 0, 0);
    }
    u32 dirLen = 0;
    s32 isAbsolute = 0;
    if (filename[0] == '\\' && filename[1] == '\\') {
        filename++;
        isAbsolute = 1;
    } else if ((D_80064360[filename[0]] & 6) && filename[1] == ':') {
        isAbsolute = 1;
    } else if (dir) {
        dirLen = strlen(dir);
        if (dirLen + nameLen + 1 >= 256) {
            __assert("", 0, 0, 0);
        }
        strcpy(D_80076508, dir);
        if (D_80076508[dirLen - 1] != '\\') {
            D_80076508[dirLen] = '\\';
            D_80076508[dirLen + 1] = '\0';
            dirLen++;
        }
    }
    char* hasBackslash = strrchr(filename, '\\');
    if (isAbsolute) {
        strcpy(D_80076508, filename);
        toUpperCase(D_80076508);
        return;
    }
    char* cwd = getCurrentDir();
    if (hasBackslash) {
        u32 len;
        if (filename[0] == '\\' || cwd == NULL) {
            len = dirLen;
            if (filename[0] == '\\') {
                filename++;
            }
            strcat(D_80076508, filename);
        } else {
            if (strlen(cwd) + dirLen + nameLen + 1 >= 256) {
                __assert("", 0, 0, 0);
            }
            if (cwd[0] == '\\') {
                cwd++;
            }
            strcat(D_80076508, cwd);
            len = strlen(D_80076508);
            if (D_80076508[len - 1] != '\\') {
                D_80076508[len] = '\\';
                D_80076508[len + 1] = '\0';
            }
            strcat(D_80076508, filename);
        }
    } else {
        if (cwd) {
            if (cwd[0]) {
                strcat(D_80076508, cwd);
                strcat(D_80076508, "\\");
            }
        }
        strcat(D_80076508, filename);
        toUpperCase(D_80076508);
        return;
    }
    toUpperCase(D_80076508);
}

s32 GolStream::writeAt(s32 pos, void* buf, s32 len) {
    if (!(this->state & 1)) {
        return 7;
    }
    if (this->buffer == NULL || this->bufferCapacity < len) {
        if (this->fileOffset != pos) {
            if (this->seek(pos)) {
                return 0xF;
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
        if (len + buffered > this->bufferCapacity || pos != this->bufferEnd) {
            if (this->fileOffset != this->bufferStart) {
                if (this->seek(this->bufferStart)) {
                    return 0xF;
                }
                this->fileOffset = this->bufferStart;
            }
            s32 err = this->rawWrite(this->buffer, buffered);
            if (err) {
                return err;
            }
            this->fileOffset += buffered;
        } else {
            memcpy(this->buffer + buffered, buf, len);
            this->bufferEnd += len;
            return 0;
        }
    }
    this->bufferStart = pos;
    this->bufferEnd = pos + len;
    this->state |= 0xC;
    memcpy(this->buffer, buf, len);
    return 0;
}

s32 GolStream::writeLine(void* buf, s32 len) {
    char newline[1] = { '\n' };
    if (!(this->state & 1)) {
        return 7;
    }
    if (this->buffer == NULL || this->bufferCapacity < len + 1) {
        if (this->state & 8) {
            this->flush();
        }
        s32 err = this->rawWrite(buf, len);
        if (err) {
            return err;
        }
        return this->rawWrite(newline, 1);
    }
    if (this->state & 8) {
        if (this->bufferCapacity - (this->bufferEnd - this->bufferStart) < len + 1) {
            this->flush();
        }
    }
    memcpy(this->buffer + (this->bufferEnd - this->bufferStart), buf, len);
    this->bufferEnd += len;
    this->buffer[this->bufferEnd - this->bufferStart] = newline[0];
    this->bufferEnd++;
    this->state |= 0xC;
    return 0;
}

s32 GolStream::flush() {
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
            if (this->seek(this->bufferStart)) {
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

void GolStream::setBuffer(s32 size, s32 allocate, u8* buffer) {
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

void GolStream::setSectorSize(s32 val) {
    D_80076504 = val;
}

s32 GolStream::sync() {
    return 0;
}

s32 GolStream::rawWrite(void* buf, s32 len) {
    return 0;
}

void GolStream::resetAllBuffers() {
    s32 count = D_800763FC;
    GolFileSource* archives = D_800763F8;
    while (count > 0) {
        count--;
        GolStream* io = archives->io;
        archives++;
        io->bufferEnd = 0;
        io->bufferStart = 0;
    }
}

void GolStream::addSearchPath(const char* path) {
    if (D_80076400 >= 4) {
        return;
    }
    func_8004B3BC(D_80076504);
    D_80076608[D_80076400] = new char[strlen(path) + 1];
    func_8004B390();
    if (D_80076608[D_80076400] == NULL) {
        __assert("", 0, 0, 0);
    }
    strcpy(D_80076608[D_80076400], path);
    D_80076400++;
}

void GolStream::clearSearchPaths() {
    for (u32 i = 0; i < D_80076400; i++) {
        if (D_80076608[i]) {
            delete[] D_80076608[i];
            D_80076608[i] = NULL;
        }
    }
}

s32 GolStream::isAbsolutePath(const char* path) {
    if (path[0] == '\\' && path[1] == '\\') {
        return 1;
    }
    if ((D_80064360[path[0]] & 6) && path[1] == ':') {
        return 1;
    }
    return 0;
}

void GolStream::toUpperCase(char* str) {
    u32 i = 0;
    while (*str && i < 256) {
        *str = toupper(*str);
        str++;
        i++;
    }
}

char* GolStream::errorMessage(s32 index) {
    return D_80076390[index];
}
