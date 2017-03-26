#ifndef BINARYVIEW_H
#define BINARYVIEW_H

#include "abstractview.h"
#include "platform/loadeddata.h"
#include "../../models/datainspectormodel.h"

namespace Ui {
class BinaryView;
}

class BinaryView : public AbstractView
{
    Q_OBJECT

    public:
        explicit BinaryView(QHexEditData *hexeditdata, QLabel* lblstatus, const QString& loadedfile, QWidget *parent = 0);
        virtual void updateToolBar(QToolBar *toolbar) const;
        ~BinaryView();

    private:
        void analyze();

    private slots:
        void updateStatus() const;

    private:
        Ui::BinaryView *ui;
        LoadedData* _loadeddata;
        DataInspectorModel* _datainspectormodel;
};

#endif // BINARYVIEW_H
