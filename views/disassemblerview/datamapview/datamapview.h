#ifndef DATAMAPVIEW_H
#define DATAMAPVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexedit.h"
#include "viewmodels/variablesmodel/variablesmodel.h"
#include "prefsdk/disassembler/disassemblerlisting.h"

using namespace PrefSDK;

namespace Ui {
class DataMapView;
}

class DataMapView : public QWidget
{
    Q_OBJECT
    
    public:
        explicit DataMapView(QWidget *parent = 0);
        void setModel(VariablesModel *model);
        void selectRange(const DataValue& offset, const DataValue& length);
        ~DataMapView();

    private slots:
        void on_dataView_doubleClicked(const QModelIndex &index);

    private:
        void highlightData();

    private:
        Ui::DataMapView *ui;
        DisassemblerListing* _listing;
        VariablesModel* _variablesmodel;
        QColor _addresscolor;
        QColor _addressalternatecolor;
        QColor _stringcolor;
};

#endif // DATAMAPVIEW_H
