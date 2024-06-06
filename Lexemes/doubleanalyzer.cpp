#include "doubleanalyzer.h"

DoubleAnalyzer::DoubleAnalyzer()
{
    name = "double";
    names =
        {
            {"const", 0},
            {"volatile", 0},
            {"extern", 0},
            {"inline", 0},
            {"auto", 0},
            {"double", 0},
            {"long", 0}
        };
}

bool DoubleAnalyzer::checkRules()
{
    if(!checkMods() || names.at("double")!=1 || names.at("long")>1) return false;
    return true;
}
