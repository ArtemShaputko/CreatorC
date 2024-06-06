#ifndef FLOATANALYZER_H
#define FLOATANALYZER_H

#include "typeanalyzer.h"

class FloatAnalyzer : public TypeAnalyzer
{
public:
    FloatAnalyzer();
    bool checkRules() override;
};

#endif // FLOATANALYZER_H
