#ifndef EASY_BITMAP_PIXEL_EDITOR_CMDOPR_H
#define EASY_BITMAP_PIXEL_EDITOR_CMDOPR_H

bmpFile curFile;
string curFileName;

namespace cmdOpr{
    // utils:
    int32_t hex_to_dec(char hex);
    int32_t hex_2_to_dec(string hex);
    color analyseColor(string str);
    // analysers:
    int32_t analyseGen(uint32_t wordCount, vector<string> cmds);
    int32_t analyseOpen(uint32_t wordCount, const vector<string>& cmds);
    int32_t analyseSave(uint32_t wordCount, const vector<string>& cmds);
    int32_t analyseSet(uint32_t wordCount, const vector<string>& cmds);
    int32_t analyseGet(uint32_t wordCount, const vector<string>& cmds);
    int32_t analyseDraw(uint32_t wordCount, const vector<string>& cmds);
    int32_t analyseExit(uint32_t wordCount, const vector<string>& cmds);
    void outPutError(int32_t errCode);
    // looptime:
    void loopTime();
}

#endif //EASY_BITMAP_PIXEL_EDITOR_CMDOPR_H