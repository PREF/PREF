#include "datamapview.h"
#include "ui_datamapview.h"

DataMapView::DataMapView(QWidget *parent): QWidget(parent), ui(new Ui::DataMapView), _listing(nullptr), _variablesmodel(nullptr)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 1);
    ui->hexView->setReadOnly(true);

    this->_stringcolor = QColor("lightgreen");
    this->_addresscolor = QColor("lightblue");
    this->_addressalternatecolor = QColor(Qt::yellow);
}

void DataMapView::setModel(VariablesModel* model)
{
    this->_variablesmodel = model;
    this->_listing = model->listing();

    ui->hexView->setData(this->_listing->data());
    ui->dataView->setModel(this->_variablesmodel);

    for(int i = 0; i < this->_variablesmodel->columnCount() - 1; i++)
        ui->dataView->resizeColumnToContents(i);

    ui->dataView->resizeRowsToContents();
    this->highlightData();
}

void DataMapView::selectRange(const DataValue &offset, const DataValue &length)
{
    ui->hexView->setSelectionRange(offset.compatibleValue<qint64>(), length.compatibleValue<qint64>());
}

DataMapView::~DataMapView()
{
    delete ui;
}

void DataMapView::highlightData()
{
    QColor color;
    Symbol* lastsymbol = nullptr;

    for(int i = 0; i < this->_variablesmodel->rowCount(); i++)
    {
        Symbol* symbol = reinterpret_cast<Symbol*>(this->_variablesmodel->index(i, 0).internalPointer());
        Segment* segment = this->_listing->findSegment(symbol->address());

        if(!segment)
            continue;

        if(symbol->type() == Symbol::Address)
        {
            if(lastsymbol && ((lastsymbol->address() + lastsymbol->size())) == symbol->address())
                color = ((color == this->_addresscolor) ? this->_addressalternatecolor : this->_addresscolor);
            else
                color = this->_addresscolor;
        }
        else /* if(symbol->type() == Symbol::String) */
            color = this->_stringcolor;

        qint64 offset = ((symbol->address() - segment->startAddress()) + segment->baseOffset()).compatibleValue<qint64>();
        qint64 endoffset = (offset + symbol->size().compatibleValue<qint64>()) - 1;
        ui->hexView->highlightBackground(offset, endoffset, color);
        ui->hexView->commentRange(offset, endoffset, symbol->name());
        lastsymbol = symbol;
    }
}

void DataMapView::on_dataView_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    Symbol* symbol = reinterpret_cast<Symbol*>(index.internalPointer());
    Segment* segment = this->_listing->findSegment(symbol->address());

    if(!segment)
        return;

    qint64 offset = ((symbol->address() - segment->startAddress()) + segment->baseOffset()).compatibleValue<qint64>();
    ui->hexView->setSelectionRange(offset, symbol->size().compatibleValue<qint64>());
}
