#ifndef TYPEANALYZER_H
#define TYPEANALYZER_H

#include "token.h"
#include <map>
#include <vector>

using namespace std;

class CustomType;

class TypeAnalyzer
{
protected:
    QString name;
    map<QString, int> names;
    vector<QString> expectedTokens;
    bool custom;
public:
    TypeAnalyzer();
    bool checkMods();
    virtual bool checkRules() = 0;
    virtual vector<Token>::iterator checkCreating(const vector<Token>::iterator & start, const vector<Token>::iterator & end);
    QString getName() const;
    virtual ~TypeAnalyzer() = default;
};
#endif // TYPEANALYZER_H
