#include "unionanalyzer.h"

UnionAnalyzer::UnionAnalyzer()
{
    name = "void";
    names =
        {
            {"const", 0},
            {"volatile", 0},
            {"extern", 0},
            {"inline", 0},
            {"auto", 0},
            {"union", 0}
    };
}

bool UnionAnalyzer::checkRules()
{
    if(checkMods() && names.at("union") == 1)
        return true;
    return false;
}
