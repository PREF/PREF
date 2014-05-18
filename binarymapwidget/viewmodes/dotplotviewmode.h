#ifndef DOTPLOTVIEWMODE_H
#define DOTPLOTVIEWMODE_H

#include "abstractviewmode.h"
#include "qhexedit/qhexeditdatareader.h"
#include "prefsdk/bytecolors.h"

using namespace PrefSDK;

class DotPlotViewMode : public AbstractViewMode
{
    Q_OBJECT

    public:
        explicit DotPlotViewMode(QHexEditData* hexeditdata, QObject *parent = 0);

    public:
        explicit DotPlotViewMode(QObject *parent = 0);
        virtual qint64 length() const;
        virtual qint64 offset(const QVector2D &v) const;
        virtual void render(QPainter& painter, qint64 start, qint64 end, qint64 width);
};

#endif // DOTPLOTVIEWMODE_H
