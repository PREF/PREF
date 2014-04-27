#include "tablewidget.h"
#include "ui_tablewidget.h"

namespace PrefSDK
{
    TableWidget::TableWidget(QWidget *parent): QDialog(parent), ui(new Ui::TableWidget)
    {
        ui->setupUi(this);
    }

    QTableWidget *TableWidget::table()
    {
        return ui->tableWidget;
    }

    TableWidget::~TableWidget()
    {
        delete ui;
    }
}
