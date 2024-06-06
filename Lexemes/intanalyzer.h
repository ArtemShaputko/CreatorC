#ifndef INTANALYZER_H
#define INTANALYZER_H

#include "typeanalyzer.h"

class IntAnalyzer : public TypeAnalyzer
{
public:
    IntAnalyzer();
    bool checkRules();
};
#endif // INTANALYZER_H
