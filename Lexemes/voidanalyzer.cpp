#include "voidanalyzer.h"

VoidAnalyzer::VoidAnalyzer()
{
    name = "void";
    names =
        {
            {"const", 0},
            {"volatile", 0},
            {"extern", 0},
            {"inline", 0},
            {"auto", 0},
            {"void", 0}
    };
}

bool VoidAnalyzer::checkRules()
{
    if(!checkMods() || names.at("void")!=1) return false;
    return true;
}
