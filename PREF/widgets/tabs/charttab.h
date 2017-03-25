#ifndef CHARTTAB_H
#define CHARTTAB_H

#include <QWidget>
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
        QHistogram* histogram() const;
        QXYChart* xyChart() const;
        ~ChartTab();

    public slots:
        void updateEntropy(double e, quint64 size);

    private slots:
        void on_tbSwitchChart_clicked();

    private:
        Ui::ChartTab *ui;
};

#endif // CHARTTAB_H
