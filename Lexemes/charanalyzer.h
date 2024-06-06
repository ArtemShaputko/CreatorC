#ifndef CHARANALYZER_H
#define CHARANALYZER_H

#include "typeanalyzer.h"


class CharAnalyzer : public TypeAnalyzer
{
public:
    CharAnalyzer();
    bool checkRules() override;
};

#endif // CHARACTER_H
