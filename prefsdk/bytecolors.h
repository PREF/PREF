#ifndef BYTECOLORS_H
#define BYTECOLORS_H

#include <QtCore>
#include <QtGui>
#include <math.h>

namespace PrefSDK
{
    class ByteColors
    {
        public:
            static const Qt::GlobalColor ZeroColor;
            static const Qt::GlobalColor FFColor;
            static const Qt::GlobalColor PunctColor;
            static const Qt::GlobalColor DigitsColor;
            static const Qt::GlobalColor AlphaColor;
            static const Qt::GlobalColor UncategorizedColor;

        private:
            ByteColors();

        public:
            static QColor byteClassColor(uchar b);
            static QVector<QRgb> byteClassColorTable();
            static QColor entropyColor(qreal e);

        private:
            static QVector<QRgb> _byteclasscolortable;
    };
}
#endif // BYTECOLORS_H
