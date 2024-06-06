ifndef LEXEME_H
#define LEXEME_H

#include <string>
#include "token.h"

using namespace std;

enum Data
{
    VAR,
    FUNC
};

class Lexeme
{
protected:
    string name;
    Data data;
public:
    Lexeme() = default;
    Lexeme(string _name, TokenType _tokenType, Data _data) : name(_name), data(_data) {}
    string getName()
    {
        return name;
    }
    void setName(string _name)
    {
        name = _name;
    }

    Data getData()
    {
        return data;
    }

    void setData(Data _data)
    {
        data = _data;
    }
};

#endif // LEXEME_H
