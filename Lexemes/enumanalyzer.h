#ifndef ENUMANALYZER_H
#define ENUMANALYZER_H


#include "customtypeanalyzer.h"

class EnumAnalyzer : public CustomTypeAnalyzer
{
public:
    EnumAnalyzer();
    bool checkRules() override;
};

#endif // ENUMANALYZER_H
