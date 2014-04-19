#ifndef DATATYPESVIEW_H
#define DATATYPESVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/datatypesmodel/datatypesmodel.h"
#include "datatypesview/datatypesmenu.h"

using namespace PrefSDK;

namespace Ui {
class DataTypesView;
}

class DataTypesView : public QWidget
{
    Q_OBJECT
    
    public:
        explicit DataTypesView(QWidget *parent = 0);
        DataTypesModel* model();
        void setData(QHexEditData *hexeditdata);
        ~DataTypesView();

    private slots:
        void resizeColumns(QModelIndex, QModelIndex);
        void on_tvDataTypes_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::DataTypesView *ui;
        DataTypesModel* _datatypesmodel;
        DataTypesMenu* _datatypesmenu;
};

#endif // DATATYPESVIEW_H
