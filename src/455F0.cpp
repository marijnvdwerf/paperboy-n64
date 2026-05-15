#include "common.h"
#include "structs.h"

extern "C" {
void func_800079A8(const char* msg, int a, int b, int c);
int strncmp(const char* a, const char* b, unsigned n);
void func_8004B390(void);
void func_8004B3BC(int);
}

extern "C" int D_80076624;
extern "C" u8 D_800AE780[0x14];
extern "C" char D_800AE770[12];
extern const char D_80004888[];

// ---------------------------------------------------------------------------

#if 0
int JamArchive::readLine(s32 offset, u8* buffer, u32 bufferSize,
                       u32 maxLength, u32* processedLength)
{
    *processedLength = 0;
    if (bufferSize == 0) {
        return 1;
    }

    u32 currentOffset = 0;
    u32 totalProcessed = 0;
    u32 remainingSize = bufferSize;
    s32 hasCR = 0;
    u32 chunkSize;

    while (1) {
        chunkSize = remainingSize > 0x40? 0x40:remainingSize  ;

        s32 bytesProcessed;
        s32 result = this->io->readAt(offset + currentOffset,
                                    buffer + currentOffset, chunkSize, &bytesProcessed);
        if (result != 0) {
            return result;
        }
        totalProcessed += bytesProcessed;

        if (currentOffset < totalProcessed) {
            do {
                u8 ch = buffer[currentOffset];
                if (ch == 0x0D) {
                    hasCR = 1;
                } else if (ch == 0x0A) {
                    *processedLength = currentOffset + 1;
                    if (hasCR) {
                        buffer[currentOffset - 1] = 0;
                    } else {
                        buffer[currentOffset] = 0;
                    }
                    return 0;
                } else if (ch == 0x1A || ch == 0x00) {
                    *processedLength = currentOffset + 1;
                    buffer[currentOffset] = 0;
                    return 0;
                } else {
                    hasCR = 0;
                }
                currentOffset++;
            } while (currentOffset < totalProcessed);
        }

        remainingSize -= chunkSize;
        if (remainingSize == 0 && (u32)bytesProcessed >= chunkSize) {
            break;
        }
    }

    if (totalProcessed == 0) {
        *processedLength = 0;
        *buffer = 0;
        return 0x10;
    }
    if (totalProcessed < maxLength) {
        *processedLength = totalProcessed;
        buffer[totalProcessed] = 0;
    } else {
        *processedLength = bufferSize;
        buffer[bufferSize - 1] = 0;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/455F0", readLine__10JamArchivelPUcUlUlPUl);
#endif

#if 0
// Structurally matches, but the SN compiler's register-allocation choices for
// `this` (s2 vs s3) propagate diffs throughout — would need more reshaping to
// fully match.
void FolderNode::load(AbstractFile* io)
{
    s32 bytesRead;
    if (this->initialized) {
        return;
    }
    s32 offset = this->offset;

    if (io->readAt(offset, D_800AE780, 4, &bytesRead) != 0) {
        func_800079A8(D_80004888, 0, 0, 0);
    }
    offset += 4;
    this->fileCount = D_800AE780[0] + (D_800AE780[1] << 8)
                    + (D_800AE780[2] << 16) + (D_800AE780[3] << 24);
    if (this->fileCount > 0x186A0) {
        func_800079A8(D_80004888, 0, 0, 0);
    }

    if (this->fileCount != 0) {
        func_8004B3BC(D_80076624);
        this->files = (FileNode*)new u8[this->fileCount * sizeof(FileNode)];
        func_8004B390();
        if (this->files == NULL) {
            func_800079A8(D_80004888, 0, 0, 0);
        }
        for (u32 i = 0; i < this->fileCount; i++) {
            if (io->readAt(offset, D_800AE780, 0x14, &bytesRead) != 0) {
                func_800079A8(D_80004888, 0, 0, 0);
            }
            offset += bytesRead;
            memcpy(this->files[i].name, D_800AE780, 12);
            this->files[i].offset = D_800AE780[0xC] + (D_800AE780[0xD] << 8)
                                  + (D_800AE780[0xE] << 16) + (D_800AE780[0xF] << 24);
            this->files[i].size   = D_800AE780[0x10] + (D_800AE780[0x11] << 8)
                                  + (D_800AE780[0x12] << 16) + (D_800AE780[0x13] << 24);
        }
    }

    if (io->readAt(offset, D_800AE780, 4, &bytesRead) != 0) {
        func_800079A8(D_80004888, 0, 0, 0);
    }
    offset += 4;
    this->folderCount = D_800AE780[0] + (D_800AE780[1] << 8)
                      + (D_800AE780[2] << 16) + (D_800AE780[3] << 24);
    if (this->folderCount > 0x186A0) {
        func_800079A8(D_80004888, 0, 0, 0);
    }

    if (this->folderCount != 0) {
        func_8004B3BC(D_80076624);
        this->folders = new FolderNode[this->folderCount];
        for (s32 j = (s32)this->folderCount - 1; j >= 0; j--) {
            this->folders[j].name[0] = 0;
            this->folders[j].initialized = 0;
            this->folders[j].offset = 4;
            this->folders[j].fileCount = 0;
            this->folders[j].files = NULL;
            this->folders[j].folderCount = 0;
            this->folders[j].folders = NULL;
        }
        func_8004B390();
        if (this->folders == NULL) {
            func_800079A8(D_80004888, 0, 0, 0);
        }
        for (u32 i = 0; i < this->folderCount; i++) {
            if (io->readAt(offset, D_800AE780, 0x10, &bytesRead) != 0) {
                func_800079A8(D_80004888, 0, 0, 0);
            }
            offset += bytesRead;
            memcpy(this->folders[i].name, D_800AE780, 12);
            this->folders[i].initialized = 0;
            this->folders[i].offset = D_800AE780[0xC] + (D_800AE780[0xD] << 8)
                                    + (D_800AE780[0xE] << 16) + (D_800AE780[0xF] << 24);
        }
    }
    this->initialized = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/455F0", load__10FolderNodeP12AbstractFile);
#endif

FileNode* FolderNode::findFile(const char* name, AbstractFile* io) {
    if (!this->initialized) {
        this->load(io);
    }
    for (u32 i = 0; i < this->fileCount; i++) {
        if (strncmp(this->files[i].name, name, 12) == 0) {
            return &this->files[i];
        }
    }
    return NULL;
}

FolderNode* FolderNode::findFolder(const char* name, AbstractFile* io) {
    if (!this->initialized) {
        this->load(io);
    }
    for (u32 i = 0; i < this->folderCount; i++) {
        if (strncmp(this->folders[i].name, name, 12) == 0) {
            return &this->folders[i];
        }
    }
    return NULL;
}

void FolderNode::reset() {
    this->initialized = 0;
    if (this->folders) {
        for (u32 i = 0; i < this->folderCount; i++) {
            this->folders[i].reset();
        }
        if (this->folders) {
            delete[] this->folders;
        }
        this->folders = NULL;
        this->folderCount = 0;
    }
    if (this->files) {
        delete[] this->files;
        this->files = NULL;
        this->fileCount = 0;
    }
}

FolderNode* FolderNode::init() {
    this->name[0] = 0;
    this->initialized = 0;
    this->offset = 4;
    this->fileCount = 0;
    this->files = NULL;
    this->folderCount = 0;
    this->folders = NULL;
    return this;
}

extern "C" void func_8004527C(int h) {
    D_80076624 = h;
}

void JamArchive::func_80045288() {
    this->root.reset();
}

s32 JamArchive::read(s32 offset, void* buf, s32 size, s32* outRead) {
    return this->io->readAt(offset, buf, size, outRead);
}

s32 JamArchive::close() {
    this->unk4 = 0;
    return 0;
}

s32 JamArchive::findFile(const char* path) {
    FolderNode* node = &this->root;
    int i = -1;

    while (1) {
        u32 bufferPos = 0;
        while (path[++i] && path[i] != '\\') {
            if (bufferPos >= 12) {
                return 6;
            }
            D_800AE770[bufferPos++] = path[i];
        }
        if (bufferPos < 12) {
            D_800AE770[bufferPos] = 0;
        }
        if (path[i] == '\\') {
            node = node->findFolder(D_800AE770, this->io);
            if (node == NULL) {
                return 8;
            }
        } else {
            break;
        }
    }
    FileNode* res = node->findFile(D_800AE770, this->io);
    if (res == NULL) {
        return 8;
    }
    this->curFile = res;
    return 0;
}

s32 JamArchive::locate(const char* path, s32* outOffset, s32* outSize) {
    FolderNode* node = &this->root;
    int i = -1;

    while (1) {
        u32 bufferPos = 0;
        while (path[++i] && path[i] != '\\') {
            if (bufferPos >= 12) {
                return 6;
            }
            D_800AE770[bufferPos++] = path[i];
        }
        if (bufferPos < 12) {
            D_800AE770[bufferPos] = 0;
        }
        if (path[i] == '\\') {
            node = node->findFolder(D_800AE770, this->io);
            if (node == NULL) {
                return 8;
            }
        } else {
            break;
        }
    }
    FileNode* res = node->findFile(D_800AE770, this->io);
    if (res == NULL) {
        return 8;
    }
    *outOffset = res->offset;
    *outSize = res->size;
    this->unk4 = 1;
    return 0;
}

void JamArchive::reset() {
    this->root.reset();
    this->io = NULL;
    this->unk4 = 0;
}

INCLUDE_RODATA("asm/nonmatchings/455F0", D_80004870);
INCLUDE_RODATA("asm/nonmatchings/455F0", D_80004888);

void JamArchive::open(AbstractFile* io) {
    s32 bytesRead;
    this->io = io;
    this->unk4 = 0;
    if (io->readAt(0, D_800AE780, 4, &bytesRead) != 0) {
        func_800079A8(D_80004888, 0, 0, 0);
    }
    if (memcmp(D_800AE780, "LJAM", 4) != 0) {
        func_800079A8(D_80004888, 0, 0, 0);
    }
}

JamArchive::~JamArchive() {
    reset();
}

JamArchive::JamArchive() {
    this->root.init();
    this->io = NULL;
    this->unk4 = 0;
    this->curFile = NULL;
}

FileNode* JamArchive::getCurFile() {
    return this->curFile;
}

AbstractFile* JamArchive::getIO() {
    return this->io;
}

bool JamArchive::hasIO() {
    return this->io != NULL;
}
