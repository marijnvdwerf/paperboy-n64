#ifndef GOL_BIN_PARSER_H
#define GOL_BIN_PARSER_H

#include "gol_file_parser.h"

struct GolBinParser : public GolFileParser {
    /* 0x650 */ void* unk650;

    GolBinParser();
    virtual s32 close();
    virtual void selectDriver(const char* path);
    virtual char* getExtension();
    virtual void parseError(s32 code);
    virtual s32 nextToken();
    virtual s32 vfunc21(char* name, s32 len);

    void resetStream();
    s32 readBytes(s32 nbytes);
    void readStructDef();
};

#endif
