#include "platypus.h"

#include "cockatoo.h"
#include "parrot.h"

extern "C" void func_8004B390();
extern "C" void func_8004B3BC(s32 heapId);
extern "C" f32 sqrtf(f32 x);

class OtherBird : public Parrot {
  public:
    /* 0x650 */ u8 pad650[8];
    virtual void selectDriver(const char* path);
    virtual char* getExtension();
    virtual void parseError(s32 code);
    virtual s32 nextToken();
    virtual s32 vfunc21(char* name, s32 len);
};

class Corella : public OtherBird {};

void Platypus::vfunc8(RenderContext* ctx, const char* path, Parrot* file) {
    if (this->unk20 != NULL) {
        this->vfunc10();
    }

    if (file != NULL) {
        file = new Cockatoo();
        if (file == NULL) {
            __assert("", NULL, 0, NULL);
        }
        file->setExtension(".gdb");
    } else {
        file = new Corella();
        if (file == NULL) {
            __assert("", NULL, 0, NULL);
        }
    }

    file->selectDriver(path);

    while (1) {
        s32 token = file->nextToken();
        if (token == 0) {
            break;
        }
        switch (token) {
            case 0x27:
                if (this->skink.context != NULL) {
                    file->parseError(0x12);
                }
                this->skink.func_80021380(ctx, file);
                break;
            case 0x28:
                this->vfunc4(file);
                break;
            case 0x2A:
                this->vfunc5(file);
                break;
            case 0x29:
                this->vfunc6(file);
                break;
            case 0x2D:
                this->vfunc2(file);
                break;
            case 0x2E:
                if (this->unk20 != NULL) {
                    file->parseError(0x12);
                }
                this->vfunc1(file);
                break;
            case 0x33:
                this->unk40 = file->readFloat();
                break;
            case 0x34:
                this->vfunc3(file);
                break;
            default:
                file->parseError(0);
                break;
        }
    }

    if (this->unkC != NULL) {
        this->vfunc15(&this->unk24, &this->unk30, this->unk40);
    }

    file->close();
    if (file != NULL) {
        delete file;
    }
}

void Platypus::vfunc10() {
    if (this->unk10 != NULL) {
        this->unk10->vfunc4();
        if (this->unk10 != NULL) {
            delete this->unk10;
        }
        this->unk10 = NULL;
        this->unkC = NULL;
    }
    if (this->unk18 != NULL) {
        delete[] this->unk18;
        this->unk18 = NULL;
    }
    if (this->unk14 != NULL) {
        this->unk14->func_80016C80();
        if (this->unk14 != NULL) {
            delete this->unk14;
        }
        this->unk14 = NULL;
    }
    if (this->unk20 != NULL) {
        delete[] this->unk20;
        this->unk20 = NULL;
    }
    this->skink.func_80021680();
    this->unk1C = 0;
}

void Platypus::vfunc1(Parrot* file) {
    s32 refStack[10];
    u32 loopIdx;
    s32 cmdIdx;
    s32 dirty;
    u32 refDepth;
    s32 startIdx;
    u32 count;
    u32 numVerts;

    s32 arrayCount = file->beginArray();
    this->unk1C = arrayCount;

    func_8004B3BC(D_80070B20);
    this->unk20 = new u32[this->unk1C];
    func_8004B390();

    if (this->unk20 == NULL) {
        __assert("", NULL, 0, NULL);
    }
    dirty = 0;
    refDepth = dirty;
    loopIdx = 0;
    cmdIdx = dirty;

    if (this->unk1C != 0) {
        do {
            s32 token = file->nextToken();
            switch (token - 0x27) {
                case 10:
                    if (dirty != 0) {
                        cmdIdx++;
                        dirty = 0;
                    }
                    numVerts = file->readInt();
                    if (numVerts >= 0x20) {
                        file->parseError(0x11);
                    }
                    startIdx = file->readInt();
                    count = file->readInt();
                    if (numVerts + count >= 0x21) {
                        file->parseError(0x11);
                    }
                    this->unk20[cmdIdx] = 0;
                    this->unk20[cmdIdx] |= (numVerts & 0x3F) << 22;
                    this->unk20[cmdIdx] |= ((count - 1) & 0x3F) << 16;
                    this->unk20[cmdIdx] |= startIdx & 0xFFFF;
                    cmdIdx++;
                    break;
                case 6:
                    if (dirty != 0) {
                        cmdIdx++;
                        dirty = 0;
                    }
                    startIdx = file->readInt();
                    count = file->readInt();
                    if (count >= 0x100) {
                        file->parseError(0x11);
                    }
                    this->unk20[cmdIdx] = 0x10000000;
                    this->unk20[cmdIdx] |= (count & 0xFFF) << 16;
                    this->unk20[cmdIdx] |= startIdx & 0xFFFF;
                    cmdIdx++;
                    break;
                case 8:
                    startIdx = file->readInt();
                    if (refDepth >= 10) {
                        file->parseError(0x12);
                    }
                    refStack[refDepth] = startIdx;
                    refDepth++;
                    this->unk20[cmdIdx] = 0x50000000;
                    this->unk20[cmdIdx] |= startIdx & 0xFFFFFF;
                    dirty = 1;
                    break;
                case 9:
                    if (refDepth == 0) {
                        file->parseError(0x12);
                    }
                    refDepth--;
                    this->unk20[cmdIdx] = 0x50000000;
                    this->unk20[cmdIdx] |= refStack[refDepth - 1] & 0xFFFFFF;
                    dirty = 1;
                    break;
                case 11:
                    startIdx = file->readInt();
                    this->unk20[cmdIdx] = 0x50000000;
                    this->unk20[cmdIdx] |= startIdx & 0xFFFFFF;
                    dirty = 1;
                    break;
                case 0:
                    if (dirty != 0) {
                        cmdIdx++;
                        dirty = 0;
                    }
                    startIdx = file->readInt();
                    this->unk20[cmdIdx] = 0x40000000;
                    this->unk20[cmdIdx] |= startIdx & 0xFFFFFF;
                    cmdIdx++;
                    break;
                default:
                    file->parseError(0);
                    break;
            }
            loopIdx++;
        } while (loopIdx < this->unk1C);
    }

    // TODO: The duplicated assignment is required for matching register allocation.
    if (file->nextToken() != 6) {
        file->parseError(6);
        this->unk1C = cmdIdx;
    } else {
        this->unk1C = cmdIdx;
    }
}

extern "C" void func_80023000(Platypus* self) {
    Vec3f edge1;
    Vec3f edge2;
    Vec3f normal;
    Vec3f* sp40;
    Vec3f* vertBase;
    u32 cmdIdx;

    vertBase = NULL;
    if (self->unk18 != NULL) {
        delete[] self->unk18;
    }

    func_8004B3BC(D_80070B20);
    self->unk18 = new PlatypusPlane[self->unk14->unk0];
    func_8004B390();

    if (self->unk18 == NULL) {
        __assert("", NULL, 0, NULL);
    }

    cmdIdx = 0;
    sp40 = self->unk10->unk4;

    u32 startVertex;
    for (; cmdIdx < self->unk1C; cmdIdx++) {
        u32 cmd = self->unk20[cmdIdx];
        u32 masked = cmd & 0x70000000;
        // TODO: This dead reference keeps 0x10000000 in a register for matching.
        u32 unused = masked + 0x10000000;
        (void)unused;

        switch (masked) {
            case 0: {
                startVertex = cmd & 0xFFFF;
                vertBase = &sp40[startVertex];
                continue;
            }
            case 0x10000000: {
                u32 vertexCount = (cmd >> 16) & 0xFFF;
                startVertex = cmd & 0xFFFF;
                u32 i = 0;
                Vec3f* e1 = &edge1;

                while (i < vertexCount) {
                    s32 faceIdx = startVertex + i;
                    BoodieEntry* entry = &self->unk14->unk8[faceIdx];

                    u8 idxA = entry->unk1;
                    u8 idxB = entry->unk2;
                    u8 idxC = entry->unk3;

                    Vec3f* vertA = &vertBase[idxA];
                    Vec3f* vertB = &vertBase[idxB];
                    Vec3f* vertC = &vertBase[idxC];

                    edge1.x = vertA->x - vertB->x;
                    edge1.y = vertA->y - vertB->y;
                    edge1.z = vertA->z - vertB->z;

                    edge2.x = vertC->x - vertB->x;
                    edge2.y = vertC->y - vertB->y;
                    edge2.z = vertC->z - vertB->z;

                    f32 nx = edge2.y * e1->z - edge2.z * e1->y;
                    f32 ny = edge2.z * edge1.x - edge2.x * e1->z;
                    f32 nz = edge2.x * e1->y - edge2.y * edge1.x;
                    normal.x = nx;
                    normal.y = ny;
                    normal.z = nz;

                    vec3_normalize(&normal, &normal);

                    PlatypusPlane* plane = &self->unk18[faceIdx];
                    plane->normal.x = normal.x;
                    plane->normal.y = normal.y;
                    plane->normal.z = normal.z;
                    plane->distance = -(normal.x * vertB->x + normal.y * vertB->y + normal.z * vertB->z);

                    i++;
                }
                break;
            }
            case 0x60000000: {
                cmdIdx = self->unk1C;
                continue;
            }
        }
    }
}

void Platypus::vfunc15(Vec3f* center, f32* radius, f32 scale) {
    Vec3f bmin;
    Vec3f bmax;
    Vec3f tmpVec;
    s32 vertRangeCount;
    u32* cmdPtr;
    u32* cmdEnd;
    u32 cmd;
    u32 startIdx, endIdx;
    s32 n;
    f32 maxDistSq;
    u32 kind;

    bmin.x = 1e37f;
    bmin.y = 1e37f;
    bmin.z = 1e37f;
    bmax.x = -1e37f;
    bmax.y = -1e37f;
    bmax.z = -1e37f;

    cmdPtr = this->unk20;
    cmdEnd = &cmdPtr[this->unk1C];
    vertRangeCount = 0;

    for (; cmdPtr < cmdEnd; cmdPtr++) {
        cmd = *cmdPtr;
        if ((cmd & 0x70000000) == 0) {
            n = ((cmd >> 16) & 0x3F) + 1;
            startIdx = cmd & 0xFFFF;
            endIdx = startIdx + n;
            for (; startIdx < endIdx; startIdx++) {
                this->unkC->vfunc6(startIdx, &tmpVec);
                if (tmpVec.x < bmin.x)
                    bmin.x = tmpVec.x;
                if (tmpVec.x > bmax.x)
                    bmax.x = tmpVec.x;
                if (tmpVec.y < bmin.y)
                    bmin.y = tmpVec.y;
                if (tmpVec.y > bmax.y)
                    bmax.y = tmpVec.y;
                if (tmpVec.z < bmin.z)
                    bmin.z = tmpVec.z;
                if (tmpVec.z > bmax.z)
                    bmax.z = tmpVec.z;
            }
            vertRangeCount++;
        } else if ((cmd & 0x70000000) == 0x60000000) {
            break;
        }
    }

    if (vertRangeCount != 0) {
        center->x = (bmax.x + bmin.x) * 0.5f;
        center->y = (bmax.y + bmin.y) * 0.5f;
        center->z = (bmax.z + bmin.z) * 0.5f;

        maxDistSq = -1e37f;

        cmdPtr = this->unk20;
        cmdEnd = &cmdPtr[this->unk1C];

        for (; cmdPtr < cmdEnd; cmdPtr++) {
            cmd = *cmdPtr;
            // TODO: Separate temporaries preserve matching CSE and register allocation.
            kind = cmd & 0x70000000;
            n = cmd >> 16;
            if (kind == 0) {
                n = (n & 0x3F) + 1;
                startIdx = cmd & 0xFFFF;
                endIdx = startIdx + n;
                for (; startIdx < endIdx; startIdx++) {
                    f32 dx, dy, dz, distSq;
                    this->unkC->vfunc6(startIdx, &tmpVec);
                    dx = tmpVec.x - center->x;
                    dy = tmpVec.y - center->y;
                    dz = tmpVec.z - center->z;
                    distSq = dx * dx + dy * dy + dz * dz;
                    if (distSq > maxDistSq)
                        maxDistSq = distSq;
                }
            } else if (kind == 0x60000000) {
                break;
            }
        }

        this->unk34.x = bmax.x - center->x;
        this->unk34.y = bmax.y - center->y;
        this->unk34.z = bmax.z - center->z;

        center->x *= scale;
        center->y *= scale;
        center->z *= scale;

        *radius = sqrtf(maxDistSq) * scale;
    }
}

extern "C" void func_80023638(Platypus* self) {
    Boodie* faces = NULL;
    Potoroo* verts = NULL;
    u32 count;
    u32 i;
    self->vfunc11(&verts);
    count = verts->unk0;
    i = 0;
    if (count != 0) {
        do {
            Vec3f tmp;
            verts->vfunc6(i, &tmp);
            tmp.y = -tmp.y;
            verts->vfunc10(i, &tmp);
            i++;
        } while (i < count);
    }
    self->vfunc12(1, 1);
    self->vfunc13(&faces);
    count = faces->unk0;
    i = 0;
    while (1) {
        if (i >= count) {
            break;
        }
        BoodieEntry* entry = &faces->unk8[i];
        u8 a = entry->unk2;
        u8 b = entry->unk3;
        entry->unk2 = b;
        entry->unk3 = a;
        i++;
    }
    self->vfunc14(1);
}

extern "C" void func_8002377C(s32 arg0) {
    D_80070B20 = arg0;
}

void Platypus::vfunc17() {
    this->unkC->vfunc15();
}

void Platypus::vfunc16(PotorooTruffle* op) {
    this->unkC->vfunc14(op);
}

void Platypus::vfunc3(Parrot* file) {
    if (this->unk10 != NULL) {
        file->parseError(0x12);
    }
    func_8004B3BC(D_80070B20);
    Potoroo* p = new Potoroo();
    this->unk10 = p;
    this->unkC = p;
    func_8004B390();
    if (this->unk10 == NULL) {
        __assert("", NULL, 0, NULL);
    }
    this->unk10->vfunc2(file);
}

void Platypus::vfunc14(s32 flags) {
    this->unk44 |= flags;
}

void Platypus::vfunc13(Boodie** out) {
    *out = this->unk14;
}

void Platypus::vfunc12(s32 flags, s32 update) {
    this->unk44 |= flags;
    if (update != 0) {
        this->vfunc15(&this->unk24, &this->unk30, this->unk40);
    }
}

void Platypus::vfunc11(Potoroo** out) {
    *out = this->unkC;
}

void Platypus::vfunc6(Parrot*) {
}

void Platypus::vfunc2(Parrot* file) {
    if (this->unk14 != NULL) {
        file->parseError(0x12);
    }
    func_8004B3BC(D_80070B20);
    this->unk14 = new Boodie();
    func_8004B390();
    if (this->unk14 == NULL) {
        __assert("", NULL, 0, NULL);
    }
    this->unk14->vfunc2(file);
}

extern "C" void func_800239C0(Platypus* self, s32 faceCount, u32 commandCount) {
    self->unk1C = commandCount;
    func_8004B3BC(D_80070B20);
    self->unk14 = new Boodie();
    self->unk20 = new u32[self->unk1C];
    func_8004B390();
    if (self->unk14 == NULL) {
        __assert("", NULL, 0, NULL);
    }
    self->unk14->func_8002851C(faceCount);
    if (self->unk20 == NULL) {
        __assert("", NULL, 0, NULL);
    }
    self->unk20[0] = 0x60000000;
}

Platypus::~Platypus() {
    this->vfunc10();
}

inline Platypus::Platypus() {
    unkC = NULL;
    unk10 = NULL;
    unk14 = NULL;
    unk18 = NULL;
    unk1C = 0;
    unk20 = NULL;
    unk24.x = 0;
    unk24.y = 0;
    unk24.z = 0;
    unk44 = 0;
    unk34.x = 0;
    unk34.y = 0;
    unk34.z = 0;
    unk30 = 5e36f;
    unk40 = 1.0f;
}
