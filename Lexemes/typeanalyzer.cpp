#include "typeanalyzer.h"
#include "../scaner.h"

using namespace std;

QString TypeAnalyzer::getName() const
{
    return name;
}

TypeAnalyzer::TypeAnalyzer()
{
    custom = false;
    expectedTokens = vector<QString>{Scaner::possibleWordsForType.begin(), Scaner::possibleWordsForType.end()};
}

bool TypeAnalyzer::checkMods()
{
    if((names.at("volatile") > 0 && names.at("extern") > 0) ||
        (names.at("volatile") > 0 && names.at("auto") > 0) ||
        (names.at("auto") > 0 && names.at("extern") > 0))
        return false;
    return true;
}

vector<Token>::iterator TypeAnalyzer::checkCreating(const vector<Token>::iterator & start, const vector<Token>::iterator & end)
{
    if(end == start) return end;
    auto it = start;
    for(; it != end && find(expectedTokens.begin(), expectedTokens.end(), it.base()->getName()) != expectedTokens.end(); it++)
    {
        if(names.find(it.base()->getName()) != names.end()) names.at(it->getName())++;
        else return end;
    }
    if(it ==start) return it;
    return checkRules() ? it-1 : end;
}
