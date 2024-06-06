#ifndef STRUCTANALYZER_H
#define STRUCTANALYZER_H


#include "customtypeanalyzer.h"

class StructAnalyzer : public CustomTypeAnalyzer
{
public:
    StructAnalyzer();
    bool checkRules() override;
};

#endif // STRUCTANALYZER_H
