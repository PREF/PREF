#ifndef PIXELVIEWMODE_H
#define PIXELVIEWMODE_H

#include "abstractviewmode.h"

class PixelViewMode : public AbstractViewMode
{
    Q_OBJECT

    public:
        explicit PixelViewMode(QHexEdit *hexedit, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual qint64 size() const;
        virtual qint64 offset(const QPoint &p) const;
        virtual void render(QPainter* painter, qint64 width);

    private:
        QByteArray _bits;
};

#endif // PIXELVIEWMODE_H
