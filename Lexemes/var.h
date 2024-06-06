#ifndef VAR_H
#define VAR_H

#include "identifier.h"

class Var : public Identifier
{
private:
    bool isParameter;
public:
    Var(string _name,
        TokenType _tokenType,
        int _pos,
        Lexeme& _type,
        int _pNum,
        bool _sign,
        bool _constant,
        bool _reg,
        bool _isparam) :
        Identifier(_name, _tokenType, _pos, _type, _pNum, _constant, _reg, _sign), isParameter(_isparam){}
};

#endif // VAR_H
