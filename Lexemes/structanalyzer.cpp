#include "structanalyzer.h"

StructAnalyzer::StructAnalyzer()
{
    name = "struct";
    names =
        {
            {"const", 0},
            {"volatile", 0},
            {"extern", 0},
            {"inline", 0},
            {"auto", 0},
            {"struct", 0}
        };
}

bool StructAnalyzer::checkRules()
{
    if(checkMods() && names.at("struct") == 1)
        return true;
    return false;
}
