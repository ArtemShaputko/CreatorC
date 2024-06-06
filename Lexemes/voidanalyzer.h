#ifndef VOIDANALYZER_H
#define VOIDANALYZER_H

#include "typeanalyzer.h"

class VoidAnalyzer : public TypeAnalyzer
{
public:
    VoidAnalyzer();
    bool checkRules() override;
};

#endif // VOIDANALYZER_H
