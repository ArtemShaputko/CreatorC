#include "enumanalyzer.h"

EnumAnalyzer::EnumAnalyzer()
{
    name = "enum";
    names =
        {
            {"const", 0},
            {"volatile", 0},
            {"extern", 0},
            {"inline", 0},
            {"auto", 0},
            {"enum", 0}
        };
}

bool EnumAnalyzer::checkRules()
{
    if(checkMods() && names.at("enum") == 1)
        return true;
    return false;
}
