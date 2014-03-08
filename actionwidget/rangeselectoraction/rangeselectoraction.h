#ifndef RANGESELECTORACTION_H
#define RANGESELECTORACTION_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui {
class RangeSelectorAction;
}

class RangeSelectorAction : public QWidget
{
    Q_OBJECT

    public:
        explicit RangeSelectorAction(QWidget *parent = 0);
        ~RangeSelectorAction();
        void setMaximum(qint64 val);

    signals:
        void rangeSelected(qint64 start, qint64 end);

    private slots:
        void on_pbSelect_clicked();
        void onValueChanged(qint64);

    protected:
        virtual void showEvent(QShowEvent* e);

    private:
        Ui::RangeSelectorAction *ui;
};

#endif // RANGESELECTORACTION_H
