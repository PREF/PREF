#ifndef CHARTTAB_H
#define CHARTTAB_H

#include <QWidget>
#include <qhexedit/qhexeditdata.h>
#include "../chart/qhistogram.h"
#include "../chart/qxychart.h"

namespace Ui {
class ChartTab;
}

class ChartTab : public QWidget
{
    Q_OBJECT

    public:
        explicit ChartTab(QWidget *parent = 0);
        void initialize(QHexEditData* hexeditdata);
        ~ChartTab();

    private slots:
        void on_tbSwitchChart_clicked();
        void updateEntropy(double e, quint64 size);

    private:
        Ui::ChartTab *ui;
};

#endif // CHARTTAB_H
