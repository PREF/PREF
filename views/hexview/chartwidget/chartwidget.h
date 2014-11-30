#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/histogrammodel/histogrammodel.h"
#include "views/hexview/bytecolorsdialog/bytecolorsdialog.h"
#include "views/hexview/workertab.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/bytecolors.h"
#include "prefsdk/math.h"
#include "chartworker.h"

using namespace PrefSDK;

namespace Ui {
class ChartWidget;
}

class ChartWidget : public WorkerTab
{
    Q_OBJECT

    public:
        explicit ChartWidget(QWidget *parent = 0);
        void plot(QHexEditData* hexeditdata);
        ~ChartWidget();

    private:
        void updateEntropy(const QList<qint64>& occurrences);
        void updateEntropyText(const QString& text, const QColor& forecolor);

    private slots:
        void onOccurrencesListCompleted();
        void onDataEntropyCompleted();
        void on_tbHelp_clicked();
        void on_tbSwitchChart_clicked();

    private:
        Ui::ChartWidget *ui;
        HistogramModel* _histogrammodel;
        QHexEditData* _hexeditdata;
        ChartWorker _worker;
        QIcon _xycharticon;
        QIcon _histogramicon;
};

#endif // CHARTWIDGET_H
