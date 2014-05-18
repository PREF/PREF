#ifndef DATATYPESWIDGET_H
#define DATATYPESWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/datatypesmodel/datatypesmodel.h"
#include "datatypesmenu.h"

using namespace PrefSDK;

namespace Ui {
class DataTypesWidget;
}

class DataTypesWidget : public QWidget
{
    Q_OBJECT
    
    public:
        explicit DataTypesWidget(QWidget *parent = 0);
        DataTypesModel* model();
        void setData(QHexEditData *hexeditdata);
        ~DataTypesWidget();

    private slots:
        void resizeColumns(QModelIndex, QModelIndex);
        void on_tvDataTypes_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::DataTypesWidget *ui;
        DataTypesModel* _datatypesmodel;
        DataTypesMenu* _datatypesmenu;
};

#endif // DATATYPESWIDGET_H
