#ifndef ACTION_H
#define ACTION_H

#include <string>
#include "codeeditor.h"

using namespace std;

class CodeEditor;
class Action
{
private:
    CodeEditor * document;
    qsizetype startPos;
    QString symbols;
    bool added;
public:
    Action(CodeEditor * _document, qsizetype _startPos, QString _symbols, bool _added )
        : document(_document), startPos(_startPos),
        symbols(_symbols), added(_added) {};
    void setStartPos(qsizetype newStartPos);
    qsizetype getStartPos() const;
    void setSymbols(const QString &newSymbols);
    QString getSymbols() const;
    bool getAdded() const;
    void setAdded(bool newAdded);
    void undo();
    void redo();

    ~Action() = default;
};

#endif // ACTION_H
