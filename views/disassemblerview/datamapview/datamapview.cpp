#include "datamapview.h"
#include "ui_datamapview.h"

DataMapView::DataMapView(QWidget *parent): QWidget(parent), ui(new Ui::DataMapView), _listing(nullptr), _datamapmodel(nullptr)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 1);
    ui->hexView->setReadOnly(true);

    this->_stringcolor = QColor(Qt::darkGreen);
    this->_addresscolor = QColor(Qt::darkCyan);
    this->_addressalternatecolor = QColor(Qt::yellow);
}

void DataMapView::setListing(DisassemblerListing *listing)
{
    this->_listing = listing;
    ui->hexView->setData(listing->data());

    this->_datamapmodel = new DataMapModel(listing, ui->dataView);
    ui->dataView->setModel(this->_datamapmodel);

    for(int i = 0; i < this->_datamapmodel->columnCount() - 1; i++)
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
    const DisassemblerListing::VariableList& variables = this->_listing->variables();
    SymbolTable* symbols = this->_listing->symbolTable();

    foreach(DataValue variable, variables)
    {
        Segment* segment = this->_listing->findSegment(variable);
        Symbol* symbol = symbols->get(variable);
        qint64 offset = ((variable - segment->startAddress()) + segment->baseOffset()).compatibleValue<qint64>();
        qint64 endoffset = (offset + symbol->size().compatibleValue<qint64>()) - 1;

        if(symbol->type() == Symbol::Address)
            color = ((color == this->_addresscolor) ? this->_addressalternatecolor : this->_addresscolor);
        else if(symbol->type() == Symbol::String)
            color = this->_stringcolor;

        ui->hexView->highlightBackground(offset, endoffset, color);
        ui->hexView->commentRange(offset, endoffset, symbol->name());
    }
}

void DataMapView::on_dataView_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    const DisassemblerListing::VariableList& variables = this->_listing->variables();
    SymbolTable* symbols = this->_listing->symbolTable();
    DataValue variable = variables[index.row()];
    Symbol* symbol = symbols->get(variable);
    Segment* segment = this->_listing->findSegment(variable);
    qint64 offset = ((variable - segment->startAddress()) + segment->baseOffset()).compatibleValue<qint64>();

    ui->hexView->setSelectionRange(offset, symbol->size().compatibleValue<qint64>());
}
