#include "error.h"

void Error::setPos(qsizetype newPos)
{
    pos = newPos;
}

qsizetype Error::getPos() const
{
    return pos;
}

qsizetype Error::getLen() const
{
    return len;
}

void Error::setLen(qsizetype newLen)
{
    len = newLen;
}

QString Error::getName() const
{
    return name;
}

void Error::setName(const QString &newName)
{
    name = newName;
}

QTextCharFormat Error::getFormat() const
{
    return format;
}

Error::Error(qsizetype _pos, qsizetype _len, QString _name) :
    pos(_pos), len(_len), name(_name)
{
    format.setFontUnderline(true);
    format.setUnderlineColor(Qt::red);
    format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
}
