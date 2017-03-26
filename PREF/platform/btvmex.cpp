#include "btvmex.h"
#include <QColor>

BTVMEX::BTVMEX(QHexEdit *hexedit, BTVMIO *btvmio): BTVM(btvmio), _hexedit(hexedit)
{
    this->_hexedit->clearHighlight();
}

QRgb BTVMEX::bgrToRgb(uint32_t bgr) const
{
    return (bgr & 0X000000FF) << 16 | (bgr & 0x0000FF00) | (bgr & 0x00FF0000) >> 16;
}

void BTVMEX::entryCreated(const BTEntryPtr &btentry)
{
    QRgb rgb = 0;

    if(btentry->value->value_fgcolor != ColorInvalid)
    {
        rgb = this->bgrToRgb(btentry->value->value_fgcolor);
        this->_hexedit->highlightForeground(btentry->location.offset, btentry->location.end(), QColor::fromRgb(rgb));

        if(btentry->value->is_readable() && !btentry->value->value_id.empty())
            this->_hexedit->commentRange(btentry->location.offset, btentry->location.end(), QString::fromStdString(btentry->value->value_id));
    }

    if(btentry->value->value_bgcolor != ColorInvalid)
    {
        rgb = this->bgrToRgb(btentry->value->value_bgcolor);
        this->_hexedit->highlightBackground(btentry->location.offset, btentry->location.end(), QColor::fromRgb(rgb));
    }
}
