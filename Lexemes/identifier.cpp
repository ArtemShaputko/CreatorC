#include "identifier.h"

Identifier::Identifier(string _name, TokenType _tokenType, int _pos, Lexeme& _type, int _pnum, bool _sign, bool _constant, bool _reg) :
    Lexeme(_name, _tokenType, _pos), type(_type), pointerNum(_pnum), sign(_sign), constant(_constant), reg(_reg)
{}
