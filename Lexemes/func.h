#ifndef FUNC_H
#define FUNC_H

#include "var.h"
#include <vector>

using namespace std;

class Func : public Identifier
{
private:
    vector<Var> parameters;
public:
    Func(string _name,
         TokenType _tokenType,
         int _pos,
         Lexeme & _type,
         int _pnum,
         bool _sign,
         bool _constant,
         bool _reg) :
        Identifier(_name, _tokenType, _pos, _type, _pnum, _sign, _constant, _reg) {}
    void addParameter(Var& parameter);

};

#endif // FUNC_H
