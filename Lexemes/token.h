#ifndef TOKEN_H
#define TOKEN_H

#include <QTextCharFormat>
#include <string>

enum TokenType
{
    KEYWORD,
    IDENTIFIER,
    CONSTANT,
    STR,
    STR_LIT,
    OPERATOR,
    PUNCTUATOR
};

using namespace std;

class Token
{
private:
    QString name;
    TokenType type;
    qsizetype pos;
    int len;
    QTextCharFormat format;
public:
    Token(QString _name, TokenType _type, qsizetype _pos, int _len);
    QString getName();
    TokenType getType();
    void setType(TokenType value);
    void setName(QString _name);
    void setFormat(QTextCharFormat& _format);
    QTextCharFormat getFormat();
    void setStart(qsizetype value);
    qsizetype getStart();
    int getLen();
    ~Token() = default;
};

#endif // TOKEN_H
