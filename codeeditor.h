#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "documenthighlighter.h"
#include "stack.h"
#include "action.h"

class Action;
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    friend class MainWindow;
    friend class Action;

public:
    friend class DocumentHighlighter;
    CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    void placeTabs(int);
    void deleteTabs(int tabs);
    void manageNewLine(int pos, int removed, int inseerted);
signals:
    void codeChanged(int& pos, int removed, int inserted);
    void newLine(int l);

private:
    DocumentHighlighter *highlighter;
    QString previousText;
    QWidget *lineNumberArea;
    Stack<Action> undoRedo;
    Stack<Action>::Iterator undoPos = undoRedo.begin();
};

#endif
