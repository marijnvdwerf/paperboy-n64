#include "jabiru.h"

#include "cockatoo.h"
#include "parrot.h"

extern "C" void func_8004B390(void);
extern "C" void func_8004B3BC(s32 heapId);
extern "C" s32 tri_contains_point(Vec3f* point, Vec3f* triData);
extern "C" void vec3_normalize(Vec3f* src, Vec3f* dst);

class OtherBird : public Parrot {
  public:
    /* 0x650 */ u8 pad650[8];
    virtual void selectDriver(const char* path);
    virtual char* getExtension();
    virtual void parseError(s32 code);
    virtual s32 nextToken();
    virtual s32 vfunc21(char* name, s32 len);
};

class JabiruParrot : public OtherBird {};

struct BspNode {
    /* 0x00 */ f32 nx;
    /* 0x04 */ f32 ny;
    /* 0x08 */ f32 nz;
    /* 0x0C */ u16 faceStart;
    /* 0x0E */ u16 faceCount;
    /* 0x10 */ u16 children[2]; // left, right

    void setNormalFromInt(s32 x, s32 y, s32 z);
    void setNormal(Vec3f* in);
    void getNormal(Vec3f* out);
    f32 dot(Vec3f* v);
};

struct BspFace {
    /* 0x00 */ u16 v0;
    /* 0x02 */ u16 v1;
    /* 0x04 */ u16 v2;
    /* 0x06 */ u16 material;
};

struct TriData {
    /* 0x00 */ Vec3f v0;
    /* 0x0C */ Vec3f v1;
    /* 0x18 */ Vec3f v2;
    /* 0x24 */ Vec3f normal;
    /* 0x30 */ f32 d;
};

struct BspStackEntry {
    /* 0x0 */ Vec3f* vert;
    /* 0x4 */ f32 dist;
    /* 0x8 */ u16 nodeIndex;
    /* 0xA */ u16 visit;
};

extern Vec3f D_80129650;
extern Vec3f D_80129660;
extern Vec3f D_80129670;
extern Vec3f D_80129680;
extern s32 D_801295B0;

static inline void vec3_sub(Vec3f* out, Vec3f* a, Vec3f* b) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
}

static inline void vec3_cross(Vec3f* a, Vec3f* b, Vec3f* out) {
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

static inline f32 vec3_dot(Vec3f* a, Vec3f* b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline void vec3_scale(Vec3f* v, f32 t, Vec3f* out) {
    out->x = v->x * t;
    out->y = v->y * t;
    out->z = v->z * t;
}

static inline void vec3_add(Vec3f* a, Vec3f* b, Vec3f* out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
}

inline Jabiru::Jabiru() {
    func_80120AD0();
}

inline Jabiru::~Jabiru() {
    vfunc3();
}

inline void Jabiru::func_80120AD0() {
    this->numNodes = 0;
    this->nodes = NULL;
    this->mesh = NULL;
    this->numFaces = 0;
    this->faces = NULL;
    this->skinkPtr = &this->skink;
    this->depth = 0;
    this->stack = NULL;
}

void Jabiru::vfunc2(RenderContext* ctx, const char* path, s32 useCockatoo) {
    if (this->nodes != NULL) {
        this->vfunc3();
    }
    Parrot* file;
    if (useCockatoo) {
        file = new Cockatoo();
        if (file == NULL) {
            __assert("", 0, 0, 0);
        }
        file->setExtension(".bvb");
    } else {
        file = new JabiruParrot();
        if (file == NULL) {
            __assert("", 0, 0, 0);
        }
    }
    file->selectDriver(path);
    for (;;) {
        u32 token = file->nextToken();
        if (token == 0) {
            break;
        }
        switch (token) {
            case 0x27: {
                Skink* sk = &this->skink;
                if (sk->context != NULL) {
                    file->parseError(0x12);
                }
                sk->func_80021380(ctx, file);
                break;
            }
            case 0x34:
                this->func_80120954(file);
                break;
            case 0x2D:
                this->func_8012084C(file);
                break;
            case 0x8B:
                this->func_8011F404(file);
                break;
            case 0x8E:
                this->func_8011F5F4(file);
                break;
            default:
                file->parseError(0);
                break;
        }
    }
    if (this->numNodes != 0) {
        this->depth = this->func_8011FF88();
        func_8004B3BC(D_801295B0);
        this->stack = new BspStackEntry[this->depth];
        func_8004B390();
        if (this->stack == NULL) {
            __assert("", 0, 0, 0);
        }
    }
    file->close();
    delete file;
}

inline void Jabiru::vfunc3() {
    if (this->nodes != NULL) {
        Potoroo* m = this->mesh;
        if (m != NULL) {
            m->vfunc4();
            delete this->mesh;
        }
        delete[] this->faces;
        delete[] this->nodes;
        delete[] this->stack;
        this->numNodes = 0;
        this->nodes = NULL;
        this->mesh = NULL;
        this->numFaces = 0;
        this->faces = NULL;
        this->skinkPtr = &this->skink;
        this->depth = 0;
        this->stack = NULL;
    }
}

inline void Jabiru::func_80120954(Parrot* file) {
    if (this->mesh != NULL) {
        file->parseError(0x12);
    }
    func_8004B3BC(D_801295B0);
    this->mesh = new Potoroo();
    func_8004B390();
    if (this->mesh == NULL) {
        __assert("", 0, 0, 0);
    }
    Potoroo* m = this->mesh;
    m->vfunc2(file);
}

inline void Jabiru::func_8012084C(Parrot* file) {
    this->numFaces = file->beginArray();
    func_8004B3BC(D_801295B0);
    this->faces = new BspFace[this->numFaces];
    func_8004B390();
    if (this->faces == NULL) {
        __assert("", 0, 0, 0);
    }
    u32 i = 0;
    if (this->numFaces != 0) {
        do {
            this->faces[i].v0 = file->readInt();
            this->faces[i].v1 = file->readInt();
            this->faces[i].v2 = file->readInt();
            this->faces[i].material = file->readInt();
            i++;
        } while (i < this->numFaces);
    }
    file->expectToken(TOKEN_CLOSE_BRACE);
}

void Jabiru::func_8011F404(Parrot* file) {
    u32 i;

    this->numNodes = file->beginArray();
    func_8004B3BC(D_801295B0);
    this->nodes = new BspNode[this->numNodes];
    func_8004B390();
    if (this->nodes == NULL) {
        __assert("", 0, 0, 0);
        i = 0;
    }
    for (i = 0; i < this->numNodes; i++) {
        u32 j;
        f32 vals[4]; // TODO: Vec4f?

        for (j = 0; j < 2; j++) {
            u32 token = file->nextToken();
            switch (token) {
                case TOKEN_INT: {
                    s32 val = file->intValue;
                    this->nodes[i].children[j] = val;
                    break;
                }
                case 0x8C:
                    this->nodes[i].children[j] = 0xFFFF;
                    break;
                case 0x8D:
                    this->nodes[i].children[j] = 0xFFFE;
                    break;
                default:
                    file->parseError(0);
                    break;
            }
        }
        vals[0] = file->readFloat();
        vals[1] = file->readFloat();
        vals[2] = file->readFloat();
        vals[3] = file->readFloat();
        this->nodes[i].setNormal((Vec3f*)vals);
        this->nodes[i].faceStart = file->readInt();
        this->nodes[i].faceCount = file->readInt();
    }
    file->expectToken(TOKEN_CLOSE_BRACE);
}

void Jabiru::func_8011F5F4(Parrot* file) {
    u32 i;
    u32 j;
    s32 val;
    s32 nx;
    s32 ny;

    this->numNodes = file->beginArray();
    func_8004B3BC(D_801295B0);
    this->nodes = new BspNode[this->numNodes];
    func_8004B390();
    if (this->nodes == NULL) {
        __assert("", 0, 0, 0);
    }
    for (i = 0; i < this->numNodes; i++) {
        for (j = 0; j < 2; j++) {
            val = file->readInt();
            if (val < 0) {
                if (val == -1) {
                    this->nodes[i].children[j] = 0xFFFF;
                } else if (val == -2) {
                    this->nodes[i].children[j] = 0xFFFE;
                } else {
                    file->parseError(0);
                }
            } else {
                this->nodes[i].children[j] = val;
            }
        }
        nx = file->readInt();
        ny = file->readInt();
        this->nodes[i].setNormalFromInt(nx, ny, file->readInt());
        this->nodes[i].faceStart = file->readInt();
        this->nodes[i].faceCount = file->readInt();
    }
    file->expectToken(TOKEN_CLOSE_BRACE);
}

inline s32 Jabiru::func_80120344() {
    Vec3f* endp = &this->end;
    Vec3f* startp = &this->start;
    D_80129650.x = endp->x - startp->x;
    D_80129650.y = endp->y - startp->y;
    D_80129650.z = endp->z - startp->z;
    f32 t = this->distA / (this->distA + this->distB);
    Vec3f* hp = this->hitPoint;
    Vec3f delta;
    delta.x = D_80129650.x * t;
    delta.y = D_80129650.y * t;
    delta.z = D_80129650.z * t;
    hp->x = startp->x + delta.x;
    hp->y = startp->y + delta.y;
    hp->z = startp->z + delta.z;
    BspFace* face = &this->faces[this->hitNode->faceStart];
    BspFace* faceEnd = &face[this->hitNode->faceCount];
    Vec3f* verts = this->mesh->unk4;
    this->hitNode->getNormal(&D_80129680);
    BspFace* bestFace = NULL;
    this->triData->normal.x = D_80129680.x;
    this->triData->normal.y = D_80129680.y;
    this->triData->normal.z = D_80129680.z;
    for (; face < faceEnd; face++) {
        this->triData->v0 = verts[face->v0];
        this->triData->v1 = verts[face->v1];
        this->triData->v2 = verts[face->v2];
        if (tri_contains_point(this->hitPoint, &this->triData->v0)) {
            vec3_sub(&D_80129660, &this->triData->v0, &this->triData->v1);
            vec3_sub(&D_80129670, &this->triData->v2, &this->triData->v1);
            vec3_cross(&D_80129670, &D_80129660, &D_80129680);
            if (vec3_dot(&D_80129680, &D_80129650) > 0.0f) {
                bestFace = face;
            } else {
                vec3_normalize(&D_80129680, &D_80129680);
                this->triData->normal.x = D_80129680.x;
                this->triData->normal.y = D_80129680.y;
                this->triData->normal.z = D_80129680.z;
                this->triData->d = -vec3_dot(&D_80129680, &this->triData->v0);
                if (this->hitIndex != NULL) {
                    *this->hitIndex = (s32)this->skinkPtr->entries[face->material]; // TODO: fix type
                }
                if (this->hitT != NULL) {
                    *this->hitT = t;
                }
                return 1;
            }
        }
    }
    if (bestFace != NULL) {
        this->triData->v0 = verts[bestFace->v0];
        this->triData->v1 = verts[bestFace->v1];
        this->triData->v2 = verts[bestFace->v2];
        vec3_normalize(&D_80129680, &D_80129680);
        this->triData->normal.x = D_80129680.x;
        this->triData->normal.y = D_80129680.y;
        this->triData->normal.z = D_80129680.z;
        this->triData->d = -vec3_dot(&D_80129680, &this->triData->v0);
        if (this->hitIndex != NULL) {
            *this->hitIndex = (s32)this->skinkPtr->entries[bestFace->material]; // TODO: fix type
        }
        if (this->hitT != NULL) {
            *this->hitT = t;
        }
        return 1;
    }
    return 0;
}

s32 Jabiru::func_8011F7B4(Vec3f* start, Vec3f* end, TriData* triData, Vec3f* hitPoint, s32* hitIndex, f32* hitT) {
    this->start = *start;
    this->end = *end;
    this->triData = triData;
    this->hitPoint = hitPoint;
    this->hitIndex = hitIndex;
    this->hitT = hitT;
    BspStackEntry* entry = this->stack;
    entry->nodeIndex = entry->visit = 0;
    BspNode* node = this->nodes;
    Vec3f* vertices = this->mesh->unk4;
    for (;;) {
        Vec3f offset;
        Vec3f* offsetPtr = &offset;
        u32 childIndex;
        if (entry->visit == 0) {
            BspFace* face = &this->faces[node->faceStart];
            Vec3f* vertex = entry->vert = &vertices[face->v0];
            offset.x = start->x - vertex->x;
            offsetPtr->y = start->y - vertex->y;
            offsetPtr->z = start->z - vertex->z;
            entry->dist = node->dot(offsetPtr);
            childIndex = entry->dist <= 0.0f ? node->children[1] : node->children[0];
            ++entry->visit;
            if (childIndex < 0xFFFE) {
                node = &this->nodes[childIndex];
                ++entry;
                entry->nodeIndex = childIndex;
                entry->visit = 0;
            }
            continue;
        } else if (entry->visit == 1) {
            entry->visit = 2;
            Vec3f* vertex = entry->vert;
            offset.x = end->x - vertex->x;
            offsetPtr->y = end->y - vertex->y;
            offsetPtr->z = end->z - vertex->z;
            f32 endDist = node->dot(offsetPtr);
            if (endDist <= 0.0f) {
                if (!(0.0f < entry->dist)) {
                    continue;
                }
                this->distA = entry->dist;
                this->distB = -endDist;
                childIndex = node->children[1];
            } else {
                if (!(entry->dist <= 0.0f)) {
                    continue;
                }
                this->distA = -entry->dist;
                this->distB = endDist;
                childIndex = node->children[0];
            }
            this->hitNode = node;
            if (this->func_80120344()) {
                return 1;
            }
            if (childIndex < 0xFFFE) {
                node = &this->nodes[childIndex];
                ++entry;
                entry->nodeIndex = childIndex;
                entry->visit = 0;
            }
            continue;
        }
        if (--entry < this->stack) {
            return 0;
        }
        node = &this->nodes[entry->nodeIndex];
    }
}

inline s32 Jabiru::func_8012021C(Vec3f* point) {
    Vec3f offset;
    BspNode* node;
    Vec3f* verts;
    Vec3f* v;
    u32 child;
    f32 d;

    verts = this->mesh->unk4;
    node = this->nodes;
    for (;;) {
        BspFace* face = &this->faces[node->faceStart];
        v = &verts[face->v0];
        vec3_sub(&offset, point, v);
        d = node->dot(&offset);
        if (d <= 0.0f) {
            child = node->children[1];
            if (child == 0xFFFF) {
                return 1;
            }
            if (child == 0xFFFE) {
                return 0;
            }
            node = &this->nodes[child];
        } else {
            child = node->children[0];
            if (child == 0xFFFF) {
                return 1;
            }
            if (child == 0xFFFE) {
                return 0;
            }
            node = &this->nodes[child];
        }
    }
}

s32 Jabiru::func_8011FF88() {
    u16* stack;
    u32 maxDepth;
    u32 depth;
    u32 cur;
    u32 prev;
    u32 next;
    BspNode* node;

    func_8004B3BC(D_801295B0);
    stack = new u16[this->numNodes];
    func_8004B390();
    if (stack == NULL) {
        __assert("", 0, 0, 0);
        maxDepth = 0;
    }
    maxDepth = 0;
    depth = 0;
    cur = 0;
    prev = 0;
    stack[0] = 0;
    for (;;) {
        node = &this->nodes[cur];
        if (maxDepth < depth) {
            maxDepth = depth;
        }
        if (prev != node->children[1]) {
            next = node->children[0];
            if (prev != next) {
                prev = cur;
                if (next + 0xFFFF0002 >= 2) {
                    cur = next;
                    depth++;
                    stack[depth] = cur;
                    continue;
                }
            }
            next = node->children[1];
            prev = cur;
            if (next + 0xFFFF0002 >= 2) {
                cur = next;
                depth++;
                stack[depth] = cur;
                continue;
            }
        }
        prev = cur;
        if (depth == 0) {
            break;
        }
        depth--;
        cur = stack[depth];
    }
    delete[] stack;
    return maxDepth + 1;
}

extern "C" void func_801200B0(Jabiru* self) {
    u32 i;
    Vec3f* verts;
    u16 a;
    u16 b;

    for (i = 0; i < self->numNodes; i++) {
        self->nodes[i].ny = -self->nodes[i].ny;
    }
    verts = self->mesh->unk4;
    i = 0;
    while (1) {
        if (i >= self->mesh->unk0) {
            break;
        }
        verts[i].y = -verts[i].y;
        i++;
    }
    for (i = 0; i < self->numFaces; i++) {
        a = self->faces[i].v2;
        b = self->faces[i].v1;
        self->faces[i].v1 = a;
        self->faces[i].v2 = b;
    }
}

extern "C" void func_8012016C(s32 heapId) {
    D_801295B0 = heapId;
}

inline void BspNode::setNormal(Vec3f* in) {
    this->nx = in->x;
    this->ny = in->y;
    this->nz = in->z;
}

inline void BspNode::setNormalFromInt(s32 x, s32 y, s32 z) {
    this->nx = (f32)x * (1.0f / (1 << 30));
    this->ny = (f32)y * (1.0f / (1 << 30));
    this->nz = (f32)z * (1.0f / (1 << 30));
}

inline void BspNode::getNormal(Vec3f* out) {
    out->x = this->nx;
    out->y = this->ny;
    out->z = this->nz;
}

inline f32 BspNode::dot(Vec3f* v) {
    return v->x * this->nx + v->y * this->ny + v->z * this->nz;
}
