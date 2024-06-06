#ifndef BLOCKCONTENT_H
#define BLOCKCONTENT_H

#include <QTextObject>

class BlockContent : public QTextBlockUserData
{
private:
    int level = 0;
    bool isEnded = true;
public:
    BlockContent(int _level, bool _isEnded);;
    void setIsEnded(bool _isEnded);
    void setLevel(int _level);
    bool getIsEnded();
    int getLevel();
    ~BlockContent() = default;
};

#endif // BLOCKCONTENT_H
