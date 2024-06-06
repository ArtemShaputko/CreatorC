#ifndef ERROR_H
#define ERROR_H

#include <QTextCharFormat>

using namespace std;

class Error
{
private:
    qsizetype pos;
    qsizetype len;
    QString name;
    QTextCharFormat format;
public:
    Error(qsizetype _pos, qsizetype _len, QString _name);
    void setPos(qsizetype newPos);
    qsizetype getPos() const;
    qsizetype getLen() const;
    void setLen(qsizetype newLen);
    QString getName() const;
    void setName(const QString &newName);
    QTextCharFormat getFormat() const;
    ~Error() = default;
};

#endif // ERROR_H
