#include "documenthighlighter.h"

#include "blockcontent.h"


#include <chrono>

#include <iostream>

using namespace std;
using namespace std::chrono;

void DocumentHighlighter::highlight(int& pos, int removed, int inserted)
{
    text = getDoc()->toPlainText();
    setFormat(0, text.size(), defaultFormat);

    comments.clear();
    scaner.clear();
    scaner.tokenize(text, comments);
    scaner.checkDefinitions();
    scaner.checkBrackets();

    tokens = scaner.getTokens();
    errors = scaner.getErrors();
    setTokensFormat();
    setComments();
    setErrors();
    setTabs(pos, removed, inserted);
}

void DocumentHighlighter::setComments()
{
    if(!text.isEmpty())
    {
        for(auto it = comments.begin(); it != comments.end(); it++)
        {
            setFormat(it->first,it->second, CommentFormat);
        }

    }
}
void DocumentHighlighter::setTabs(int& pos, int removed, int inserted)
{
    int tabs = 0;
    bool isEnded = true;
    bool isPreproc = false;
    bool textDeleted;
    int tabLine;
    size_t cursorPos = parent->textCursor().position();

    QTextBlock previousTokenBlock; // блок предыдущего токена
    QTextBlock currentTokenBlock; // блок текущего токена
    QTextBlock nextTokenBlock; //блок следующего токена

    for(auto it = tokens.begin();it!=tokens.end();it++)
    {
        textDeleted = false;
        it->setStart(it->getStart());
        currentTokenBlock = getBlock(it.base()->getStart());

        if(it->getName()=="{")
            tabs++;
        if(it->getName()=="}")
            if(tabs > 0)
                tabs--;

        if(it+1!=tokens.end())
            nextTokenBlock = getBlock((it+1)->getStart());
        else
            nextTokenBlock = getDoc()->lastBlock();

        // Если это первый токен в строке
        if(it == tokens.begin() || currentTokenBlock.blockNumber() > previousTokenBlock.blockNumber())
        {
            // Если он сейчас редактируется
            if(it != tokens.end() &&
                (it->getStart() == cursorPos ||
                 it->getStart() == cursorPos-1)
                && inserted > 0 && removed == 0)
            {
                if(pos>=4 &&((!isEnded && it->getName() == "{") ||
                                 ((isEnded && it->getName() == "}"))))
                {
                    emit tabsTodelete(1);
                    pos-=4;
                    textDeleted = true;
                }
            }
            if(it->getName()=="#") isPreproc = true;
            else isPreproc = false;
        }

        // Если это последний токен в строке
        if(it == tokens.end()-1 ||  currentTokenBlock.blockNumber() < nextTokenBlock.blockNumber())
        {
            if(closingLine.contains(it.base()->getName()) || isPreproc)
                isEnded = true;
            else
                isEnded = false;

            currentTokenBlock.setUserData(new BlockContent(tabs, isEnded));
            if(textDeleted)
            {
                return;
            }
        }
        previousTokenBlock = currentTokenBlock;
    }
}

void DocumentHighlighter::setTokensFormat()
{
    for(auto it = tokens.begin(); it != tokens.end(); it++)
    {
        setFormat(it->getStart(),it->getLen(), it->getFormat());
    }
}

void DocumentHighlighter::setErrors()
{
    for(const auto & it : errors)
    {
        mergeFormat(it.getPos(),it.getLen(), it.getFormat());
    }
}

void DocumentHighlighter::setFormat(qsizetype pos, int size, QTextCharFormat format)
{
    QTextCursor cursor(getDoc());
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor, size);
    parent->blockSignals(true);
    cursor.setCharFormat(format);
    parent->blockSignals(false);
}

void DocumentHighlighter::mergeFormat(qsizetype pos, int size, QTextCharFormat format)
{
    QTextCursor cursor{getDoc()};
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor, size);
    parent->blockSignals(true);
    cursor.mergeCharFormat(format);
    parent->blockSignals(false);
}

QTextBlock DocumentHighlighter::getBlock(qsizetype pos)
{
    QTextCursor cursor{getDoc()};
    cursor.setPosition(pos);
    return cursor.block();
}

