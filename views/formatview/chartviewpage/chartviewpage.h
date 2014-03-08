#ifndef HISTOGRAMVIEWPAGE_H
#define HISTOGRAMVIEWPAGE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
#include "qhistogram/qhistogram.h"
#include "prefsdk/bytecolors.h"
#include "prefsdk/io/bytebuffer.h"
#include "qhexedit/qhexedit.h"
#include "prefsdk/math.h"
#include "elaboratetoolbar.h"
#include "charthelper.h"

using namespace PrefSDK;

namespace Ui {
class ChartViewPage;
}

class ChartViewPage : public QWidget
{
    Q_OBJECT
    
    public:
        explicit ChartViewPage(ByteBuffer* bytebuffer, QHexEdit* hexedit, QWidget *parent = 0);
        ~ChartViewPage();

    private:
        static void initNonAsciiChars();
        void createListModel();
        void createCharts();
        void updateModel();
    
    private slots:
        void calculate();
        void onChartFinished();

    private:
        static QMap<uchar, QString> _nonasciichars;
        Ui::ChartViewPage *ui;
        ChartHelper* _charthelper;
        ByteBuffer* _bytebuffer;
        ElaborateToolBar* _toolbar;
        QHexEdit* _hexedit;

};

#endif // HISTOGRAMVIEWPAGE_H
