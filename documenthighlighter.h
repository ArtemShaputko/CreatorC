#ifndef DOCUMENTHIGHLIGHTER_H
#define DOCUMENTHIGHLIGHTER_H
#include <QTextCursor>
#include <QTextDocument>
#include <QPlainTextEdit>
#include "scaner.h"

#include <QObject>
#include <stack>

class DocumentHighlighter : public QWidget
{
    Q_OBJECT
public:
    DocumentHighlighter(QPlainTextEdit * _parent): parent(_parent)
    {
        CommentFormat.setFontItalic(true);
        CommentFormat.setForeground(Qt::darkGray);

        WrongFormat.setFontUnderline(true);
        WrongFormat.setUnderlineColor(Qt::red);
        WrongFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    };
    ~DocumentHighlighter() = default;

    void setComments();
    void setTokensFormat();
    void setErrors();
    void setTabs(int& pos, int removed, int inserted);
    void setFormat(qsizetype pos, int size, QTextCharFormat format);
    void mergeFormat(qsizetype pos, int size, QTextCharFormat format);

    QTextBlock getBlock(qsizetype pos);
    QTextDocument* getDoc(){
        return parent->document();
    }

signals:
    void insertSymbols(string str);
    void tabsTodelete(int num);
public slots:
    void highlight(int& pos, int removed, int inserted);

private:
    Scaner scaner;
    QString text;
    QPlainTextEdit * parent;
    QTextCharFormat CommentFormat;
    QTextCharFormat WrongFormat;
    QTextCharFormat defaultFormat;
    QString closingLine = ";{}";

    vector<Error> errors;
    vector<pair<qsizetype, qsizetype>> comments;
    vector<Token> tokens;
};

#endif // DOCUMENTHIGHLIGHTER_H
