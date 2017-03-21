#ifndef DATATYPESWIDGET_H
#define DATATYPESWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/datatypesmodel/datatypesdelegate.h"
#include "viewmodels/datatypesmodel/datatypesmodel.h"
#include "datatypesmenu.h"

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
        void on_dataTypesTable_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::DataTypesWidget *ui;
        DataTypesModel* _datatypesmodel;
        DataTypesDelegate* _datatypesdelegate;
        DataTypesMenu* _datatypesmenu;
};

#endif // DATATYPESWIDGET_H
