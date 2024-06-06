#include "blockcontent.h"

BlockContent::BlockContent(int _level, bool _isEnded) :
    level(_level),
    isEnded(_isEnded){}

void BlockContent::setIsEnded(bool _isEnded)
{
    isEnded = _isEnded;
}

void BlockContent::setLevel(int _level)
{
    level = _level;
}

bool BlockContent::getIsEnded()
{
    return isEnded;
}


int BlockContent::getLevel()
{
    return level;
}
