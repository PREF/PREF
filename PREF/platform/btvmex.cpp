#include "btvmex.h"
#include <QColor>

BTVMEX::BTVMEX(QHexEdit *hexedit, BTVMIO *btvmio): BTVM(btvmio), _hexedit(hexedit)
{
    this->_hexedit->clearHighlight();
}

void BTVMEX::entryCreated(const BTEntryPtr &btentry)
{
    if(btentry->forecolor != ColorInvalid)
        this->_hexedit->highlightForeground(btentry->location.offset, btentry->location.end(), QColor::fromRgb(btentry->forecolor));

    if(btentry->backcolor != ColorInvalid)
        this->_hexedit->highlightBackground(btentry->location.offset, btentry->location.end(), QColor::fromRgb(btentry->backcolor));
}
