#include "charanalyzer.h"

CharAnalyzer::CharAnalyzer()
{
    name = "char";
    names =
        {
            {"const", 0},
            {"volatile", 0},
            {"extern", 0},
            {"inline", 0},
            {"auto", 0},
            {"char", 0},
            {"signed", 0},
            {"unsigned", 0}
        };
}

bool CharAnalyzer::checkRules()
{
    if(!checkMods()) return false;
    if(names.at("char")!=1 || names.at("signed")>1 || names.at("unsigned")>1 )
        return false;
    if((names.at("signed")>0 && names.at("unsigned")>0))
        return false;
    return true;
}
