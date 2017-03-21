#include "datatypeswidget.h"
#include "ui_datatypeswidget.h"

DataTypesWidget::DataTypesWidget(QWidget *parent): QWidget(parent), ui(new Ui::DataTypesWidget)
{
    ui->setupUi(this);

    this->_datatypesmodel = new DataTypesModel(this);
    this->_datatypesdelegate = new DataTypesDelegate(this);
    this->_datatypesmenu = new DataTypesMenu(this);

    connect(this->_datatypesmenu, SIGNAL(endianChanged(int)),this->_datatypesmodel, SLOT(setEndian(int)));
    connect(this->_datatypesmenu, SIGNAL(baseChanged(int)), this->_datatypesmodel, SLOT(setBase(int)));

    this->_datatypesmenu->setBase(16); /* Hex By Default */
    this->_datatypesmenu->setEndian(Endianness::platformEndian());
    ui->dataTypesTable->setModel(this->_datatypesmodel);
    ui->dataTypesTable->setItemDelegate(this->_datatypesdelegate);
}

DataTypesModel *DataTypesWidget::model()
{
    return this->_datatypesmodel;
}

void DataTypesWidget::setData(QHexEditData* hexeditdata)
{
    this->_datatypesmodel->setData(hexeditdata);
    ui->dataTypesTable->resizeColumnsToContents();
    ui->dataTypesTable->resizeRowsToContents();
}

DataTypesWidget::~DataTypesWidget()
{
    delete ui;
}

void DataTypesWidget::on_dataTypesTable_customContextMenuRequested(const QPoint &pos)
{
    this->_datatypesmenu->exec(this->mapToGlobal(pos));
}
