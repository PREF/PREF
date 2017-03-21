#ifndef WORKERTAB_H
#define WORKERTAB_H

#include <QWidget>

class WorkerTab : public QWidget
{
    Q_OBJECT

    public:
        explicit WorkerTab(QWidget *parent = 0);

    signals:
        void workStarted();
        void workFinished();
};

#endif // WORKERTAB_H
