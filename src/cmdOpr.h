// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025-2026 Haohao123coding

#ifndef EASY_BITMAP_PIXEL_EDITOR_CMDOPR_H
#define EASY_BITMAP_PIXEL_EDITOR_CMDOPR_H

#include <string>
#include "bmpFile.h"
#include "utils/rnd.h"

extern bmpFile curFile;
extern rnd rndGen;

namespace cmdOpr{
    // analysers:
    int32_t analyseGen(uint32_t wordCount, const std::vector<std::string>& cmds);
    int32_t analyseOpen(uint32_t wordCount, const std::vector<std::string>& cmds, bool isScriptFileMode);
    int32_t analyseSave(uint32_t wordCount, bool isScriptFileMode);
    int32_t analyseSet(uint32_t wordCount, const std::vector<std::string>& cmds);
    int32_t analyseGet(uint32_t wordCount, const std::vector<std::string>& cmds, bool isScriptFileMode);
    int32_t analyseDraw(uint32_t wordCount, const std::vector<std::string>& cmds);
    int32_t analyseExit(uint32_t wordCount, bool isScriptFileMode);
    int32_t analyseHelp(uint32_t wordCount, const std::vector<std::string>& cmds, bool isScriptFileMode);
    int32_t analyseEcho(uint32_t wordCount, const std::vector<std::string>& cmds);
    void outPutError(int32_t errCode);
    // looptime:
    void loopTime(bool isScriptFileMode);
}

#endif //EASY_BITMAP_PIXEL_EDITOR_CMDOPR_H
