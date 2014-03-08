#ifndef SPINBOXBASESELECTOR_H
#define SPINBOXBASESELECTOR_H

#include <QWidget>
#include "spinboxbaseselector/qnumberspinbox.h"

namespace Ui {
class SpinBoxBaseSelector;
}

class SpinBoxBaseSelector : public QWidget
{
    Q_OBJECT
    
    public:
        explicit SpinBoxBaseSelector(QWidget *parent = 0);
        void setMinimum(qint64 val);
        void setMaximum(qint64 val);
        void setBase(int b);
        int base();
        void setValue(qint64 val);
        qint64 value();
        void clear();
        ~SpinBoxBaseSelector();

    signals:
        void valueChanged(qint64);
        void baseChanged(int);
    
    private slots:
        void on_sbValue_valueChanged(int value);
        void on_cbBase_currentIndexChanged(int index);

    private:
        Ui::SpinBoxBaseSelector *ui;
};

#endif // SPINBOXBASESELECTOR_H
