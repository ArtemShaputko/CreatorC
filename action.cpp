#include "action.h"

qsizetype Action::getStartPos() const
{
    return startPos;
}

void Action::setSymbols(const QString &newSymbols)
{
    symbols = newSymbols;
}

QString Action::getSymbols() const
{
    return symbols;
}

bool Action::getAdded() const
{
    return added;
}

void Action::setAdded(bool newAdded)
{
    added = newAdded;
}

void Action::undo()
{
    document->document()->blockSignals(true);
    QTextCursor cursor(document->document());
    cursor.setPosition(startPos);

    if(!added) cursor.insertText(symbols);
    else
    {
        cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,symbols.size());
        cursor.removeSelectedText();
    }
    int pos = 0;
    document->highlighter->highlight(pos,0,0);
    document->document()->blockSignals(false);
}

void Action::redo()
{
    document->document()->blockSignals(true);
    QTextCursor cursor(document->document());
    cursor.setPosition(startPos);

    if(added) cursor.insertText(symbols);
    else
    {
        cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,symbols.size());
        cursor.removeSelectedText();
    }
    int pos = 0;
    document->highlighter->highlight(pos, 0,0);
    document->document()->blockSignals(false);
}

void Action::setStartPos(qsizetype newStartPos)
{
    startPos = newStartPos;
}
