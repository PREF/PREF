#include "bytecolors.h"

namespace PrefSDK
{
    const Qt::GlobalColor ByteColors::ZeroColor = Qt::black;
    const Qt::GlobalColor ByteColors::FFColor = Qt::white;
    const Qt::GlobalColor ByteColors::PunctColor = Qt::darkGreen;
    const Qt::GlobalColor ByteColors::DigitsColor = Qt::darkYellow;
    const Qt::GlobalColor ByteColors::AlphaColor = Qt::darkRed;
    const Qt::GlobalColor ByteColors::UncategorizedColor = Qt::gray;

    QVector<QRgb> ByteColors::_byteclasscolortable;

    ByteColors::ByteColors()
    {
    }

    QColor ByteColors::byteClassColor(uchar b)
    {
        if(ByteColors::_byteclasscolortable.isEmpty())
            ByteColors::byteClassColorTable(); /* Generate Byte Class Color Vector */

        return ByteColors::_byteclasscolortable[b];
    }

    QVector<QRgb> ByteColors::byteClassColorTable()
    {
        if(ByteColors::_byteclasscolortable.isEmpty())
        {
            ByteColors::_byteclasscolortable = QVector<QRgb>(0x100);

            for(int b = 0x00; b <= 0xFF; b++)
            {
                if(b == 0x00) /* 0 */
                    ByteColors::_byteclasscolortable[b] = QColor(ByteColors::ZeroColor).rgb();
                else if(b == 0xFF) /* 255 */
                    ByteColors::_byteclasscolortable[b] = QColor(ByteColors::FFColor).rgb();
                else if((b >= 0x21 && b <= 0x2F) || (b >= 0x3A && b <= 0x40) || (b >= 0x5B && b <= 0x60) || (b >= 0x7B && b <= 0x7E))
                    ByteColors::_byteclasscolortable[b] = QColor(ByteColors::PunctColor).rgb();
                else if((b >= 0x41 && b <= 0x5A) || (b >= 0x51 && b <= 0x7A))
                    ByteColors::_byteclasscolortable[b] = QColor(ByteColors::AlphaColor).rgb();
                else if(b >= 0x30 && b <= 0x38)
                    ByteColors::_byteclasscolortable[b] = QColor(ByteColors::DigitsColor).rgb();
                else
                    ByteColors::_byteclasscolortable[b] = QColor(ByteColors::UncategorizedColor).rgb(); /* Everything Else */
            }
        }

        return ByteColors::_byteclasscolortable;
    }

    QColor ByteColors::entropyColor(qreal e)
    {
        qreal f = 0.0;

        if(e > 0.5)
            f = qMax(pow((4*(e - 0.5) - 4 * pow(e - 0.5, 2)), 4), 0.0);

        return QColor::fromRgb(255 * f, 0, 255 * pow(e, 2));
    }
}
