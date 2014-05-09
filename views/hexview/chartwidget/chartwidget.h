#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"

namespace Ui {
class ChartWidget;
}

class ChartWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit ChartWidget(QWidget *parent = 0);
        void plot(QHexEditData* hexeditdata);
        ~ChartWidget();

    private:
        static void initNonAsciiChars();
        void createListModel();
        void elaborate(QHexEditData* hexeditdata);

    private:
        static QMap<uchar, QString> _nonasciichars;
        Ui::ChartWidget *ui;
};

#endif // CHARTWIDGET_H
