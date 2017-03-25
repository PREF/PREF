#ifndef DOTPLOTVIEWMODE_H
#define DOTPLOTVIEWMODE_H

#include "abstractviewmode.h"

class DotPlotViewMode : public AbstractViewMode
{
    Q_OBJECT

    public:
        explicit DotPlotViewMode(QHexEdit* hexedit, QObject *parent = 0);

    public:
        explicit DotPlotViewMode(QObject *parent = 0);
        virtual qint64 size() const;
        virtual qint64 offset(const QPoint &p) const;
        virtual void render(QPainter* painter, qint64 width);
};

#endif // DOTPLOTVIEWMODE_H
