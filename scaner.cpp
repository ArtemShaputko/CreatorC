#include "scaner.h"
#include "Lexemes/charanalyzer.h"
#include "Lexemes/doubleanalyzer.h"
#include "Lexemes/enumanalyzer.h"
#include "Lexemes/floatanalyzer.h"
#include "Lexemes/intanalyzer.h"
#include "Lexemes/structanalyzer.h"
#include "Lexemes/unionanalyzer.h"
#include "Lexemes/voidanalyzer.h"
#include <algorithm>
#include <regex>

using namespace std;

vector<Token> Scaner::getTokens() const
{
    return tokens;
}

vector<Error> Scaner::getErrors() const
{
    return errors;
}

Scaner::Scaner()
{
}

void Scaner::clear()
{
    errors.clear();
    tokens.clear();
}

const array<const QString, 33> Scaner::keywords{
    "auto", "break", "case",
    "char", "const", "continue",
    "default", "do", "double",
    "else", "enum", "extern",
    "float", "for", "goto",
    "if", "inline", "int",
    "long", "register", "return",
    "short", "signed", "sizeof",
    "static", "struct", "switch",
    "typedef", "union", "unsigned",
    "void", "volatile", "while"
};

const array<const QString, 14> Scaner::punctuators {".", "(", ")", "[", "]", "{", "}",
                                                 ":", ";", "...", "#"};

const array<const QString,26> Scaner::operators = {"+", "-", "/", "*", "%", "=", ">", ",",
                                                  "<", "&", "|", "^", "?", "~", "!", "++",
                                                  "--", "==", "!=", ">=", "<=", "->",
                                                  "<<", ">>", "&&", "||"};

const array<const QString, 9> Scaner::typeWords {"int", "float", "void", "char", "double",
                                              "short", "long", "signed", "unsigned"};

const array<const QString, 12> Scaner::typeSpecifiers{"int", "float", "void", "char", "double",
                                                     "struct", "union", "enum" };

const array<const QString, 17> Scaner::possibleWordsForType{
    "int", "float", "void", "char", "double",
    "short", "long", "signed", "unsigned",
    "const", "volatile", "extern", "inline", "auto",
    "struct", "enum", "union"
};

const QString Scaner::dividers = " \t\n\0";

const QString Scaner::openBrackets = "{([";
const QString Scaner::closeBrackets = "})]";

const QRegularExpression Scaner::regex8{"^0[0-7]*([uUlL]){0,3}$"};

const QRegularExpression Scaner::regex10{"^\\d+"
                                        "([uUlL]){0,3}$"};

const QRegularExpression Scaner::regex10float{"^\\d*"
                                             "(\\.\\d+)"
                                             "([eE][-+]?\\d+)?"
                                             "[FfLl]?$"};

const QRegularExpression Scaner::regex16{"^0x[0-9A-Fa-f]+"
                                        "([uUlL]){0,3}$"};

const QRegularExpression Scaner::regex16float{"^0x[0-9A-Fa-f]*"
                                             "(.[0-9A-Fa-f]+)"
                                             "([eE][-+]?\\d+)?"
                                             "([pP][-+]?\\d+)?"
                                             "[FfLl]?$"};

const QRegularExpression Scaner::regexStrLit{"'([^'\\\\]|\\\\.)'"};
const QRegularExpression Scaner::regexString{"\"([^\"\\\\]|\\\\.)*\""};

void Scaner::tokenize(const QString & code, vector<pair<qsizetype,qsizetype>>& comments)
{
    QString buf;
    TokenType tokenType;
    qsizetype pos;
    Token * cur;
    QTextCharFormat* format = nullptr;
    for(qsizetype i = 0; i<code.size();i++)
    {
        buf = "";
        pos = i;
        cur = nullptr;
        format = new QTextCharFormat();
        if(i+1<code.size() && code[i] == '/' && code[i+1] == '/')
        {
            for(;i<code.size() && code[i]!='\n';i++);
            comments.push_back({pos, i-pos+1});
        }
        else if(i+1<code.size() && code[i]=='/' && code[i+1]== '*')
        {
            for(;i+1<code.size() && !(code[i] =='*' && code[i+1] == '/'); i++);
            if(i+1>=code.size()) errors.push_back(Error{pos, i-pos+1, "Missing */ "});
            else i++;
            comments.push_back({pos, i-pos+1});
        }
        else if(dividers.contains(code[i]))
        {
            delete format;
            format = nullptr;
            continue;
        }
        else if(code[i] == '"')
        {
            buf+=code[i];
            i++;
            for(;i<code.size() && code[i] != '\n' && code[i]!='"';i++) buf+=code[i];
            if(code[i]=='"') buf+=code[i];
            tokens.push_back(getString(buf, pos));
        }
        else if(code[i] == '\'')
        {
            buf+=code[i];
            i++;
            for(;i<code.size() && code[i]!='\''  && code[i] != '\n'; i++) buf+=code[i];
            if(code[i]=='\'') buf+=code[i];
            tokens.push_back(getStrLit(buf, pos));
        }
        else if(code[i].isDigit() || (i+1<code.size() && code[i] == '.' && code[i+1].isDigit()))
        {
            for(;i<code.size() &&
                   find(operators.cbegin(),operators.cend(), code[i]) == end(operators) &&
                   !dividers.contains(code[i]); i++)
            {
                if(code[i] !='.' && find(punctuators.cbegin(),punctuators.cend(), code[i]) != cend(punctuators))
                {
                    i--;
                    break;
                }
                buf+=code[i];
            }
            tokens.push_back(getConstant(buf, pos));
        }
        else if(find(punctuators.cbegin(),punctuators.cend(), code[i]) != cend(punctuators))
        {
            buf+=code[i];
            if(code[i]=='.')
            {
                if(i+2< code.size() && code[i+1] == '.' && code[i+2] == '.' )
                {
                    buf = "...";
                    i+=2;
                }
            }
            cur = new Token(buf, PUNCTUATOR, pos, (int)(i-pos+1));
            if(buf == "(" && !tokens.empty() && tokens.back().getType() == IDENTIFIER)
            {
                format->setForeground(QColor::fromRgb(153,50,204));
                tokens.back().setFormat(*format);
            }
            tokens.push_back(*cur);
        }
        else if(find(operators.cbegin(),operators.cend(), code[i]) != cend(operators))
        {
            while(i < code.size() && find(operators.cbegin(),operators.cend(), code[i]) != cend(operators))
            {
                buf+=code[i];
                i++;
            }
            i--;
            if(buf == "<" && !tokens.empty() && tokens.back().getName() == "include")
            {
                i++;
                for(; i < code.size() && code[i] != '\n' && code[i] != '>';i++)
                {
                    buf+=code[i];
                }
                cur = new  Token{buf, IDENTIFIER, pos, (int)(i-pos+1)};
                format->setForeground(Qt::darkGreen);
                cur->setFormat(*format);
                tokens.push_back(*cur);
            }
            else
            {
                cur = new Token{buf, OPERATOR, pos, (int)(i-pos)};
                format->setFontItalic(true);
                cur->setFormat(*format);
                tokens.push_back(*cur);
            }
        }
        else
        {
            tokenType = IDENTIFIER;

            pos = i;
            while(i<code.size() && !dividers.contains(code[i]) &&
                   find(punctuators.cbegin(),punctuators.cend(), code[i]) == cend(punctuators) &&
                   find(operators.cbegin(),operators.cend(), code[i]) == cend(operators))
            {
                buf+=code[i];
                i++;
            }
            cur = new Token(buf, tokenType, pos, (int)(i-pos));
            if(find(keywords.cbegin(), keywords.cend(), buf) != cend(keywords))
            {
                cur->setType(KEYWORD);
                format->setFontWeight(QFont::Bold);

                if(find(typeWords.cbegin(), typeWords.cend(), buf) != cend(typeWords))
                    format->setForeground(QColor::fromRgb(72, 61, 139));
                else format->setForeground(QColor::fromRgb(255, 140, 0));
            }
            else
            {
                if(tokens.size() > 0 && tokens.back().getName()=="#")
                {
                    format->setForeground(QColor::fromRgb(0,0,205));
                    tokens.back().setFormat(*format);
                }
                else if(tokens.size() > 0 && (tokens.back().getName()=="." || tokens.back().getName()=="->"))
                {
                    format->setForeground(QColor::fromRgb(233, 150, 122));
                }
            }
            i--;
            cur->setFormat(*format);
            tokens.push_back(*cur);
        }
        delete cur;
        delete format;
    }
}

void Scaner::checkDefinitions()
{
    TypeAnalyzer * curTypeAnalyzer;

    QString curTypeSpecifier;

    auto start = tokens.begin();
    auto typeWordsBegin = Scaner::possibleWordsForType.cbegin();
    auto typeWordsEnd = Scaner::possibleWordsForType.cend();
    for(auto it = tokens.begin(); it != tokens.end(); it++)
    {
        if(find(typeWordsBegin, typeWordsEnd, it.base()->getName()) != typeWordsEnd)
        {
            start = it;
            // Поиск идентификаторра типа
            for(; it != tokens.end()
                       && (find(typeWordsBegin, typeWordsEnd, it.base()->getName()) != typeWordsEnd
                       || it.base()->getType() == IDENTIFIER); it++)
            {
                if(find(Scaner::typeSpecifiers.cbegin(), Scaner::typeSpecifiers.cend(), it.base()->getName()) != Scaner::typeSpecifiers.cend())
                    curTypeSpecifier = it.base()->getName();
            }
            it--;

            if(it == tokens.end())
                curTypeAnalyzer = new IntAnalyzer();

            else if(curTypeSpecifier == "char")
                curTypeAnalyzer = new CharAnalyzer();

            else if(curTypeSpecifier == "float")
                curTypeAnalyzer = new FloatAnalyzer();

            else if(curTypeSpecifier == "double")
                curTypeAnalyzer = new DoubleAnalyzer();

            else if(curTypeSpecifier == "void")
                curTypeAnalyzer = new VoidAnalyzer();

            else if(curTypeSpecifier == "struct")
                curTypeAnalyzer = new StructAnalyzer();

            else if(curTypeSpecifier == "enum")
                curTypeAnalyzer = new EnumAnalyzer();

            else if(curTypeSpecifier == "union")
                curTypeAnalyzer = new UnionAnalyzer();
            else
                curTypeAnalyzer = new IntAnalyzer();

            auto newIt = curTypeAnalyzer->checkCreating(start, tokens.end());

            if(newIt != tokens.end())
            {
                it = newIt;
            }
            else
            {
                qsizetype startPos = start->getStart();
                qsizetype len = it->getStart()+it->getLen()-startPos;
                errors.push_back(Error{startPos,len, "Wrong " + curTypeAnalyzer->getName() + " using"});
            }
            curTypeSpecifier = "";
            delete curTypeAnalyzer;
        }
    }
}

void Scaner::checkBrackets()
{
    qsizetype open_pos;
    qsizetype close_pos;
    for(auto & it : tokens)
    {
        if(openBrackets.contains(it.getName()))
            brackets.push(it);
        else if(closeBrackets.contains(it.getName()))
        {
            close_pos = closeBrackets.indexOf(it.getName());
            if(brackets.empty()) errors.push_back(Error{it.getStart(), 1, "Unexpected closing bracket"});
            else
            {
                open_pos = openBrackets.indexOf(brackets.top().getName());
                if(open_pos != close_pos)
                    errors.push_back(Error{it.getStart(), 1, "Unexpected closing bracket"});
                else
                    brackets.pop();
            }
        }
    }
    while(!brackets.empty())
    {
        errors.push_back(Error{brackets.top().getStart(), 1, "Unexpected closing bracket"});
        brackets.pop();
    }
}


Token Scaner::getConstant(QString buf, qsizetype pos)
{
    QTextCharFormat format;
    format.setForeground(QColor::fromRgb(255, 20, 147));
    if(buf.size()>2 && buf[0]=='0' && buf[1]=='x')
    {
        if(!regex16.match(buf).hasMatch() &&
            !regex16float.match(buf).hasMatch())
            errors.push_back(Error{pos, buf.size(), "Wrong 16 number format"});
    }
    else if(buf.size()>1 && buf[0]=='0')
    {
        if(!regex8.match(buf).hasMatch() &&
            !regex10float.match(buf).hasMatch())
            errors.push_back(Error{pos, buf.size(), "Wrong 8 number format"});
    }
    else
    {
        if(!regex10.match(buf).hasMatch() &&
            !regex10float.match(buf).hasMatch())
            errors.push_back(Error{pos, buf.size(), "Wrong 10 number format"});
    }

    Token cur{buf, CONSTANT, pos, (int)buf.size()};
    cur.setFormat(format);
    return cur;

}

Token Scaner::getString(QString buf, qsizetype pos)
{
    QTextCharFormat format;
    format.setForeground(Qt::darkGreen);
    if(!regexString.match(buf).hasMatch())
        errors.push_back(Error{pos, buf.size(), "Wrong constant format"});
    Token cur{buf, STR, pos, (int)buf.size()};
    cur.setFormat(format);
    return cur;
}

Token Scaner::getStrLit(QString buf, qsizetype pos)
{
    QTextCharFormat format;
    format.setForeground(Qt::darkGreen);
    if(!regexStrLit.match(buf).hasMatch())
        errors.push_back(Error{pos, buf.size(), "Wrong constant format"});
    Token cur{buf, STR, pos, (int)buf.size()};
    cur.setFormat(format);
    return cur;
}
