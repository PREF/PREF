#ifndef GOTOACTION_H
#define GOTOACTION_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui {
class GotoAction;
}

class GotoAction : public QWidget
{
    Q_OBJECT

    public:
        explicit GotoAction(QWidget *parent = 0);
        ~GotoAction();
        void setMinimum(qint64 v);
        void setMaximum(qint64 v);

    signals:
        void addressChanged(qint64 offset);

    protected:
        virtual void showEvent(QShowEvent* e);

    private:
        Ui::GotoAction *ui;
};

#endif // GOTOACTION_H
