#ifndef ELABORATETOOLBAR_H
#define ELABORATETOOLBAR_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class ElaborateToolBar : public QToolBar
{
    Q_OBJECT

    public:
        ElaborateToolBar(QWidget* parent = 0);
        void setElaborateEnabled(bool e);

    private:
        void morphToStart();
        void morphToStop();
        bool isStart();

    public slots:
        void elaborationCompleted();

    private slots:
        void onElaborateTriggered();

    signals:
        void startTriggered();
        void stopTriggered();

    private:
        QAction* _actelaborate;

    private:
        static QIcon _icostart;
        static QIcon _icostop;
        static QString _starttext;
        static QString _stoptext;
};

#endif // ELABORATETOOLBAR_H
