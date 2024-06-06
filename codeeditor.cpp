#include "codeeditor.h"
#include "blockcontent.h"
#include "linenumberarea.h"
#include <QPainter>
#include <QTextBlock>
#include <QEventLoop>
#include <iostream>
#include <QMessageBox>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    QEventLoop loop;

    connect(this->document(), &QTextDocument::contentsChange, this, &CodeEditor::manageNewLine);
    connect(this, &CodeEditor::newLine, this, &CodeEditor::placeTabs);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    highlighter = new DocumentHighlighter(this);

    connect(this, &CodeEditor::codeChanged, highlighter, &DocumentHighlighter::highlight);
    connect(highlighter, &DocumentHighlighter::tabsTodelete, this, &CodeEditor::deleteTabs);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    loop.processEvents();

    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(12);

    this->setFont(font);

    const int tabStop = 4;
    QString spaces;
    for (int i = 0; i < tabStop; ++i) {
        spaces += " ";
    }

    QFontMetrics metrics(this->font(), nullptr);
    this->setTabStopDistance(metrics.horizontalAdvance(spaces));
}

CodeEditor::~CodeEditor()
{
    delete highlighter;
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 5 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::placeTabs(int)
{
    this->blockSignals(true);
    int tabs;
    bool isEnded = true;
    QTextCursor cursor = this->textCursor();

    int pos = cursor.position();
    QTextBlock prev = cursor.block().previous();
    BlockContent* previousBlock = dynamic_cast<BlockContent*>(prev.userData());
    if(!previousBlock) tabs = 0;
    else
    {
        isEnded = previousBlock->getIsEnded();
        tabs = previousBlock->getLevel();
        if(!isEnded) tabs++;
    }
    QString buffer;
    for(int i = 0; i<tabs;i++)
        buffer+="    ";
    insertPlainText(buffer);
    undoRedo.push(Action(this, pos, buffer, true));
    undoPos = undoRedo.begin();
    this->blockSignals(false);
}

void CodeEditor::deleteTabs(int tabs)
{
    this->blockSignals(true);
    QTextBlock current = this->textCursor().block();
    QTextCursor workingCursor(current);
    int pos = workingCursor.position();
    QString buffer;
    for(int i = 0; i<tabs;i++)
    {
        workingCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 4);
        if(workingCursor.selectedText()== "    ")
            workingCursor.deleteChar();
        else break;
        buffer+="    ";
    }
    undoRedo.push(Action(this, pos, buffer, false));
    undoPos = undoRedo.begin();
    this->blockSignals(false);
}

void CodeEditor::manageNewLine(int pos, int removed, int inserted)
{
    QTextCursor cursor{this->textCursor()};
    QString text = this->document()->toPlainText();
    BlockContent *previousData = dynamic_cast<BlockContent*>(cursor.block().previous().userData());
    int i;
    QString buffer;

    emit codeChanged(pos, removed, inserted);

    if(previousData) cursor.block().setUserData(new BlockContent(previousData->getLevel(), previousData->getIsEnded()));
    else cursor.block().setUserData(new BlockContent(0, true));

    QString currentText = this->toPlainText();
    if(currentText.isEmpty()) return;
    try {
        undoRedo.remove(undoPos);
        if(removed == 0 && inserted > 0)
        {
            for(i = 0; (pos+i)<currentText.size() && i < inserted; i++)
                buffer+=currentText[pos+i];
            undoRedo.push(Action(this, pos, buffer, true));
        }
        else if(inserted == 0 && removed > 0)
        {
            for(i = 0;(pos+i)<previousText.size() && i< removed; i++)
                buffer+=previousText[pos+i];
            undoRedo.push(Action(this, pos, buffer, false));
        }
        undoPos = undoRedo.begin();
    }
    catch(StackException)
    {
        QMessageBox::warning(this, "Oops","Something went wrong, check file for errors");
    }

    if(!text.isEmpty() &&pos>=0 && inserted > 0 && removed == 0 && text.at(pos) == '\n')
        emit newLine(1);

    previousText = this->toPlainText();
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();

    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(180);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
