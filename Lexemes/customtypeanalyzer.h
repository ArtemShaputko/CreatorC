#ifndef CUSTOMTYPEANALYZER_H
#define CUSTOMTYPEANALYZER_H

#include "typeanalyzer.h"

class CustomTypeAnalyzer : public TypeAnalyzer
{
private:
    QTextCharFormat customTypeFormat;
public:
    CustomTypeAnalyzer();
    vector<Token>::iterator checkCreating(const vector<Token>::iterator & start, const vector<Token>::iterator & end) override;
};

#endif // CUSTOMTYPEANALYZER_H
