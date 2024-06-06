#include "customtypeanalyzer.h"

using namespace std;


CustomTypeAnalyzer::CustomTypeAnalyzer()
{
    customTypeFormat.setForeground(QColor::fromRgb(0, 139, 139));
}

vector<Token>::iterator CustomTypeAnalyzer::checkCreating(const vector<Token>::iterator & start, const vector<Token>::iterator & end)
{
    auto it = TypeAnalyzer::checkCreating(start,end);
    if(it == end) return end;
    if(it+1 != end &&
        ((it+1).base()->getType() == IDENTIFIER || (it+1).base()->getName() == "{"))
    {
        if(checkRules())
        {
            if((it+1).base()->getType() == IDENTIFIER) (it+1)->setFormat(customTypeFormat);
            return it+1;
        }
    }
    return end;
}

