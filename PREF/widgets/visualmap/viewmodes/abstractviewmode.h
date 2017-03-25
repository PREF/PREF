#ifndef ABSTRACTVIEWMODE_H
#define ABSTRACTVIEWMODE_H

#include <QObject>
#include <qhexedit/qhexedit.h>

class AbstractViewMode : public QObject
{
    Q_OBJECT

    public:
        explicit AbstractViewMode(QHexEdit* hexedit, QObject *parent = 0);
        virtual qint64 size() const = 0;
        virtual qint64 offset(const QPoint &p) const = 0;
        virtual void render(QPainter*, qint64 width);

    protected:
        virtual qint64 preferredHeight(const QPainter *painter);

    protected:
        QHexEdit* _hexedit;
        qint64 _width;
};

#endif // ABSTRACTVIEWMODE_H
