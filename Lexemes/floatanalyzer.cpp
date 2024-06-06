#include "floatanalyzer.h"

FloatAnalyzer::FloatAnalyzer()
{
    name = "float";
    names =
        {
            {"const", 0},
            {"volatile", 0},
            {"extern", 0},
            {"inline", 0},
            {"auto", 0},
            {"float", 0}
        };
}

bool FloatAnalyzer::checkRules()
{
    if(!checkMods() || names.at("float")!=1) return false;
    return true;
}
