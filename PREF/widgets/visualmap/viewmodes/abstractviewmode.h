#ifndef ABSTRACTVIEWMODE_H
#define ABSTRACTVIEWMODE_H

#include <QObject>
#include <qhexedit/qhexedit.h>

class AbstractViewMode : public QObject
{
    Q_OBJECT

    public:
        explicit AbstractViewMode(QHexEdit* hexedit, QObject *parent = 0);
        virtual integer_t size() const = 0;
        virtual integer_t offset(const QPoint &p) const = 0;
        virtual void render(QPainter*, qint64 width);

    protected:
        virtual qint64 preferredHeight(const QPainter *painter);

    protected:
        QHexEdit* _hexedit;
        qint64 _width;
};

#endif // ABSTRACTVIEWMODE_H
