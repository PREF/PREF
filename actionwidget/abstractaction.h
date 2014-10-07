#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H

#include <QWidget>

class AbstractAction : public QWidget
{
    Q_OBJECT

    public:
        explicit AbstractAction(QWidget *parent = 0);

    signals:
        void closeRequested();
};

#endif // ABSTRACTACTION_H
