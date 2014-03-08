#include "datatypesview.h"
#include "ui_datatypesview.h"

DataTypesView::DataTypesView(QWidget *parent): QWidget(parent), ui(new Ui::DataTypesView)
{
    ui->setupUi(this);

    this->_datatypesmodel = new DataTypesModel(this);
    this->_datatypesmenu = new DataTypesMenu(this);

    connect(this->_datatypesmodel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(resizeColumns(QModelIndex,QModelIndex)));
    connect(this->_datatypesmenu, SIGNAL(endianChanged(QSysInfo::Endian)),this->_datatypesmodel, SLOT(setEndian(QSysInfo::Endian)));
    connect(this->_datatypesmenu, SIGNAL(baseChanged(int)), this->_datatypesmodel, SLOT(setBase(int)));

    this->_datatypesmenu->setBase(16); /* Hex By Default */
    this->_datatypesmenu->setEndian(QSysInfo::ByteOrder);
    ui->tvDataTypes->setModel(this->_datatypesmodel);
}

DataTypesModel *DataTypesView::model()
{
    return this->_datatypesmodel;
}

void DataTypesView::setData(ByteBuffer* bytebuffer)
{
    this->_datatypesmodel->setData(bytebuffer);
}

DataTypesView::~DataTypesView()
{
    delete ui;
}

void DataTypesView::resizeColumns(QModelIndex, QModelIndex)
{
    for(int i = 0; i < this->_datatypesmodel->columnCount(); i++)
        ui->tvDataTypes->resizeColumnToContents(i);
}

void DataTypesView::on_tvDataTypes_customContextMenuRequested(const QPoint &pos)
{
    this->_datatypesmenu->exec(this->mapToGlobal(pos));
}
