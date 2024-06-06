#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "lexeme.h"

class Identifier : public Lexeme
{
protected:
    Lexeme& type;
    int pointerNum;
    bool sign;
    bool constant;
    bool reg;
public:
    Identifier(string _name,
               TokenType _tokenType,
               int _pos,
               Lexeme& _type,
               int _pnum,
               bool _sign,
               bool _constant,
               bool _reg);
};

#endif // IDENTIFIER_H
