#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
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
        static void initNonAsciiChars();
        void createListModel();
        void updateModel(const QList<qint64>& occurrences);
        void updateEntropy(const QList<qint64>& occurrences);
        void updateEntropyText(const QString& text, const QColor& forecolor);

    private slots:
        void onElaborationCompleted();

    private:
        static QMap<uchar, QString> _nonasciichars;
        Ui::ChartWidget *ui;
        QHexEditData* _hexeditdata;
        ChartWorker _worker;
};

#endif // CHARTWIDGET_H
