// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#ifndef EASY_BITMAP_PIXEL_EDITOR_HELPCOMMAND_H
#define EASY_BITMAP_PIXEL_EDITOR_HELPCOMMAND_H

#include <map>
#include <string>
#include <vector>

std::vector<std::string> commands = {
    "gen", "open", "save", "exit", "set", "get", "draw", "help"
};

std::map<std::string, std::vector<std::string>> helpCommand = {
    {
        "gen", {
            "gen <filename> <width> <height>",
            "gen <filename> <width> <height> <fillingColor>"
        }
    },
    {
        "open", { "open <filename>" }
    },
    {
        "save", { "save" }
    },
    {
        "exit", { "exit" }
    },
    {
        "set", { "set <item> <value>" }
    },
    {
        "get", { "get <item>" }
    },
    {
        "draw", {
            "draw pixel <x> <y> <color>",
            "draw rect <xFirst> <yFirst> <xLast> <yLast> <color>",
            "draw rect <xFirst> <yFirst> <xLast> <yLast> <color> <borderPixelCount>"
        }
    },
    {
        "help", { "help <command>" }
    }
};

#endif //EASY_BITMAP_PIXEL_EDITOR_HELPCOMMAND_H