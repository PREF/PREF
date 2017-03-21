#ifndef FINDACTION_H
#define FINDACTION_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qnumberlineedit.h"
#include "actionwidget/abstractaction.h"

namespace Ui {
class FindAction;
}

class FindAction : public AbstractAction
{
    Q_OBJECT

    public:
        explicit FindAction(QWidget *parent = 0);
        ~FindAction();

    signals:
        void findRequest(QByteArray);

    private slots:
        void on_cbBase_currentIndexChanged(int index);
        void on_pbFind_clicked();
        void on_leText_returnPressed();

    protected:
        virtual void showEvent(QShowEvent* e);

    private:
        Ui::FindAction *ui;
};

#endif // FINDACTION_H
