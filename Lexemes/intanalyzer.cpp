#include "intanalyzer.h"

IntAnalyzer::IntAnalyzer() : TypeAnalyzer()
{
    name = "int";
    names =
        {
            {"const", 0},
            {"volatile", 0},
            {"extern", 0},
            {"inline", 0},
            {"auto", 0},
            {"int", 0},
            {"signed", 0},
            {"unsigned", 0},
            {"short", 0},
            {"long", 0}
        };
}

bool IntAnalyzer::checkRules()
{
    if(!checkMods()) return false;
    if(names.at("int")>1 || names.at("signed")>1 || names.at("unsigned")>1 || names.at("short")>1 || names.at("long")>2)
        return false;
    if((names.at("signed")>0 && names.at("unsigned")>0) || (names.at("short")>0 && names.at("long")>0))
        return false;
    return true;
}
