#ifndef PIXELVIEWMODE_H
#define PIXELVIEWMODE_H

#include "abstractviewmode.h"
#include "qhexedit/qhexeditdatareader.h"
#include "prefsdk/bytecolors.h"

using namespace PrefSDK;

class PixelViewMode : public AbstractViewMode
{
    Q_OBJECT

    public:
        explicit PixelViewMode(QHexEditData* hexeditdata, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual qint64 length() const;
        virtual qint64 offset(const QVector2D &v) const;
        virtual void render(QPainter&painter, qint64 start, qint64 end, qint64 width);

    private:
        QByteArray _bits;
};

#endif // PIXELVIEWMODE_H
