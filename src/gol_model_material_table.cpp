#include "common.h"
#include "render/gol_render_device.h"
#include "gol_bin_parser.h"
#include "gol_model_material_table.h"
#include "gol_material.h"

// External functions
extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);
extern "C" char* strncpy(char*, const char*, s32);
extern "C" char* strcat(char*, const char*);
extern "C" s32 strncmp(const char*, const char*, s32);
extern "C" s32 D_80070B10;

// OtherBird - Parrot subclass with concrete virtuals
class OtherBird : public GolFileParser {
  public:
    /* 0x650 */ u8 pad650[8];

    void selectDriver(const char* path) CXX_OVERRIDE;
    char* getExtension() CXX_OVERRIDE;
    void parseError(s32 code) CXX_OVERRIDE;
    s32 nextToken() CXX_OVERRIDE;
    s32 vfunc21(char* name, s32 len) CXX_OVERRIDE;
};

// Rosella - concrete OtherBird, auto-generates vtable + dtor
class Rosella : public OtherBird {};

extern "C" const char D_800019E0[];
extern "C" const char D_800019EC[];

INCLUDE_RODATA("asm/nonmatchings/gol_model_material_table", D_800019E0);

void GolModelMaterialTable::func_80021380(GolRenderDevice* ctx, GolFileParser* file) {
    u32 i;
    char nameBuf[8];
    char msgBuf[0x40];

    if (this->context) {
        this->func_80021680();
    }
    this->context = ctx;
    this->count = file->beginArray();
    func_8004B3BC(D_80070B10);
    this->entries = new GolMaterial*[this->count];
    if (this->entries == NULL) {
        __assert(D_800019E0, 0, 0, 0);
    }
    func_8004B390();
    for (i = 0; i < this->count; i++) {
        GolMaterial* entry;
        file->nextToken();
        strncpy(nameBuf, (char*)file->stringValue, 8);
        entry = (GolMaterial*)this->context->func_800264FC(nameBuf);
        this->entries[i] = entry;
        if (this->entries[i] != NULL) {
            continue;
        }
        memset(msgBuf, 0, 9);
        strncpy(msgBuf, nameBuf, 8);
        strcat(msgBuf, D_800019EC);
        __assert(D_800019E0, 0, 0, 0);
    }

    if (file->nextToken() != TOKEN_CLOSE_BRACE) {
        file->parseError(TOKEN_CLOSE_BRACE);
    }
}

void GolModelMaterialTable::func_8002151C(s32 value) {
    D_80070B10 = value;
}

void GolModelMaterialTable::func_80021528(s32 index, GolMaterial* value) {
    this->entries[index] = value;
}

void GolModelMaterialTable::func_8002153C(s32 index, const char* name) {
    char buf[0x40];

    this->entries[index] = (GolMaterial*)this->context->func_800264FC(name);
    if (this->entries[index] == NULL) {
        strncpy(buf, name, 8);
        buf[7] = '\0';
        strcat(buf, D_800019EC);
        __assert(D_800019E0, 0, 0, 0);
    }
}

s32 GolModelMaterialTable::func_800215DC(const char* name) {
    char buf[8];

    for (u32 i = 0; i < this->count; i++) {
        GolMaterial* entry = this->entries[i];
        if (entry == NULL) {
            continue;
        }
        const char* materialName = entry->unk14;
        memcpy(buf, materialName, 8);
        if (strncmp(name, buf, 8) == 0) {
            return i;
        }
    }
    return -1;
}

void GolModelMaterialTable::func_80021680() {
    if (this->entries) {
        delete[] this->entries;
        this->entries = NULL;
    }
    this->context = NULL;
    this->count = 0;
}

void GolModelMaterialTable::func_800216C0(GolRenderDevice* ctx, const char* path, s32 useBinaryParser) {
    GolFileParser* file;

    if (this->context) {
        this->func_80021680();
    }

    if (useBinaryParser) {
        file = new GolBinParser();
        if (file == NULL) {
            __assert(D_800019E0, 0, 0, 0);
        }
        file->setExtension(".gdb");
    } else {
        file = new Rosella();
        if (file == NULL) {
            __assert(D_800019E0, 0, 0, 0);
        }
    }

    file->selectDriver(path);
    file->expectToken(0x27);
    this->func_80021380(ctx, file);
    file->close();

    if (file != NULL) {
        delete file;
    }
}

INCLUDE_RODATA("asm/nonmatchings/gol_model_material_table", D_800019EC);

void GolModelMaterialTable::func_80021808(GolRenderDevice* ctx, u32 count) {
    if (this->context) {
        this->func_80021680();
    }
    this->context = ctx;
    this->count = count;
    func_8004B3BC(D_80070B10);
    this->entries = new GolMaterial*[this->count];
    if (this->entries == NULL) {
        __assert(D_800019E0, 0, 0, 0);
    }
    func_8004B390();
    memset(this->entries, 0, this->count * 4);
}

GolModelMaterialTable::~GolModelMaterialTable() {
    this->func_80021680();
}

GolModelMaterialTable::GolModelMaterialTable() {
    context = NULL;
    count = 0;
    entries = NULL;
}
