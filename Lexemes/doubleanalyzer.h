#ifndef DOUBLEANALYZER_H
#define DOUBLEANALYZER_H


#include "typeanalyzer.h"

class DoubleAnalyzer : public TypeAnalyzer
{
public:
    DoubleAnalyzer();
    bool checkRules() override;
};

#endif // DOUBLEANALYZER_H
