#include "token.h"


Token::Token(QString _name, TokenType _type, qsizetype _pos, int _len) :
    name(_name),
    type{_type},
    pos(_pos),
    len(_len){}

QString Token::getName()
{
    return name;
}

TokenType Token::getType()
{
    return type;
}

void Token::setType(TokenType value)
{
    type = value;
}

void Token::setName(QString _name)
{
    name = _name;
}

void Token::setFormat(QTextCharFormat &_format)
{
    format = _format;
}

QTextCharFormat Token::getFormat()
{
    return format;
}

void Token::setStart(qsizetype value)
{
    pos = value;
}

qsizetype Token::getStart()
{
    return pos;
}

int Token::getLen()
{
    return len;
}
