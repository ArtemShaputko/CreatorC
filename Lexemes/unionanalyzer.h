#ifndef UNIONANALYZER_H
#define UNIONANALYZER_H


#include "customtypeanalyzer.h"
class UnionAnalyzer : public CustomTypeAnalyzer
{
public:
    UnionAnalyzer();
    bool checkRules() override;

};

#endif // UNIONANALYZER_H
