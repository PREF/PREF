#ifndef DATAMAPVIEW_H
#define DATAMAPVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexedit.h"
#include "viewmodels/datamapmodel/datamapmodel.h"

namespace Ui {
class DataMapView;
}

class DataMapView : public QWidget
{
    Q_OBJECT
    
    public:
        explicit DataMapView(QWidget *parent = 0);
        void setHexEditData(QHexEditData* hexeditdata);
        ~DataMapView();

    private:
        void highlightData();

    private:
        Ui::DataMapView *ui;
        DataMapModel* _datamapmodel;
};

#endif // DATAMAPVIEW_H
