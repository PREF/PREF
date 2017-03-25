#ifndef BINARYVIEW_H
#define BINARYVIEW_H

#include "abstractview.h"
#include "platform/loadeddata.h"
#include "platform/chartworker.h"
#include "../../models/datainspectormodel.h"

namespace Ui {
class BinaryView;
}

class BinaryView : public AbstractView
{
    Q_OBJECT

    public:
        explicit BinaryView(QHexEditData *hexeditdata, QLabel* lblstatus, const QString& loadedfile, QWidget *parent = 0);
        ~BinaryView();
        virtual void updateToolBar(QToolBar *toolbar) const;

    private:
        void analyze();

    private slots:
        void updateStatus() const;

    private:
        Ui::BinaryView *ui;
        LoadedData* _loadeddata;
        ChartWorker* _chartworker;
        DataInspectorModel* _datainspectormodel;
};

#endif // BINARYVIEW_H
