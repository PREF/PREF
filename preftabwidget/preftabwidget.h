#ifndef PREFTABWIDGET_H
#define PREFTABWIDGET_H

#include <QtCore>
#include <QtWidgets>

class PrefTabWidget : public QTabWidget
{
    Q_OBJECT

    public:
        explicit PrefTabWidget(QWidget *parent = 0);

    protected:
        virtual void dragEnterEvent(QDragEnterEvent* event);
        virtual void dragMoveEvent(QDragMoveEvent* event);
        virtual void dropEvent(QDropEvent* event);

    signals:
        void fileDragged(QString);
};

#endif // PREFTABWIDGET_H
