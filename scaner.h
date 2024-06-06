#ifndef SCANER_H
#define SCANER_H

#include "Lexemes/token.h"
#include "error.h"
#include <map>
#include <stack>
#include <vector>
#include <array>
#include <QRegularExpression>

using namespace std;

class Scaner
{
    friend class Token;
private:
    vector<Token> tokens;
    vector<Error> errors;
    stack<Token> brackets;

    Q_PROPERTY(QRegularExpression regexStrLit READ getRegexStrLit CONSTANT FINAL)

public:
    const static array<const QString, 33> keywords;
    const static array<const QString, 14> punctuators;
    const static array<const QString,26> operators;
    const static array<const QString, 9> typeWords;
    const static array<const QString, 12> typeSpecifiers;
    const static array<const QString, 17> possibleWordsForType;
    const static QString dividers;
    const static QString openBrackets;
    const static QString closeBrackets;
    const static QRegularExpression regex8;
    const static QRegularExpression regex10;
    const static QRegularExpression regex10float;
    const static QRegularExpression regex16;
    const static QRegularExpression regex16float;
    const static QRegularExpression regexStrLit;
    const static QRegularExpression regexString;

public:
    Scaner();
    void clear();
    void tokenize(const QString & code, vector<pair<qsizetype,qsizetype>>& comments);
    void checkDefinitions();
    void checkBrackets();
    Token getConstant(QString buf, qsizetype pos);
    Token getString(QString buf, qsizetype pos);
    Token getStrLit(QString buf, qsizetype pos);
    vector<Token> getTokens() const;
    vector<Error> getErrors() const;
    ~Scaner() = default;
};



#endif // SCANER_H
