#include "formatview.h"
#include "ui_formatview.h"

FormatView::FormatView(QHexEditData* hexeditdata, QWidget *parent): AbstractView(parent), ui(new Ui::FormatView)
{
    ui->setupUi(this);
    this->_bytebuffer = new ByteBuffer(SDKManager::state(), hexeditdata);

    this->createHexView();
    this->createBinaryView();
    this->createChartView();
    this->createDisassemblerView();
    this->createStringFinderView();
}

void FormatView::save()
{
    this->_bytebuffer->save();
}

void FormatView::save(QString filename)
{
    this->_bytebuffer->save(filename);
}

FormatView::Views FormatView::currentView()
{
    switch(ui->tabWidget->currentIndex())
    {
        case 0:
            return FormatView::HexView;

        case 1:
            return FormatView::BinaryView;

        case 2:
            return FormatView::ChartView;

        case 3:
            return FormatView::DisassemblerView;

        case 4:
            return FormatView::StringFinderView;

        default:
            break;
    }

    return FormatView::CustomView;
}

ByteBuffer* FormatView::buffer()
{
    return this->_bytebuffer;
}

FormatView::~FormatView()
{
    delete ui;
}

bool FormatView::canSave() const
{
    return true;
}

void FormatView::createHexView()
{
    this->_hexeditview = new HexEditViewPage(this->_bytebuffer, this);
    ui->tabWidget->addTab(this->_hexeditview, "Hex Edit");
}

void FormatView::createBinaryView()
{
    this->_binaryview = new BinaryViewPage(this->_bytebuffer);
    connect(this->_binaryview, SIGNAL(gotoTriggered(qint64)), this, SLOT(jumpToOffset(qint64)));
    ui->tabWidget->addTab(this->_binaryview, "Binary View");
}

void FormatView::createChartView()
{
    this->_chartview = new ChartViewPage(this->_bytebuffer, this->_hexeditview->hexEdit(), this);
    ui->tabWidget->addTab(this->_chartview, "Chart");
}

void FormatView::createDisassemblerView()
{
    this->_disassemblerview = new DisassemblerViewPage(this->_bytebuffer, this);
    ui->tabWidget->addTab(this->_disassemblerview, "Disassembler");
}

void FormatView::createStringFinderView()
{
    this->_stringfinderview = new StringFinderViewPage(this->_bytebuffer);
    connect(this->_stringfinderview, SIGNAL(gotoTriggered(qint64, qint64)), this, SLOT(jumpToOffset(qint64,qint64)));
    ui->tabWidget->addTab(this->_stringfinderview, "Strings");
}

void FormatView::on_tabWidget_currentChanged(int)
{
    emit viewChanged(this->currentView());
}

void FormatView::jumpToOffset(qint64 offset, qint64 length)
{
    ui->tabWidget->setCurrentIndex(0);
    this->_hexeditview->gotoOffset(offset, length);
}

void FormatView::on_tbFormats_clicked()
{
    FormatsDialog fd(SDKManager::state(), this->_bytebuffer->length(), this->topLevelWidget());
    int res = fd.exec();

    if(res == FormatsDialog::Accepted)
    {
        this->_formatdefinition = fd.selectedFormat();
        this->_hexeditview->loadFormat(this->_formatdefinition, fd.offset());
        this->_disassemblerview->setData(this->_hexeditview->model(), this->_formatdefinition);
        ui->tbFormatOptions->setEnabled(this->_formatdefinition->hasOptions());
    }
}

void FormatView::on_tbFormatOptions_clicked()
{
    FormatOptionsDialog fod(this->_formatdefinition, this->_hexeditview->model(), this->_bytebuffer, this->topLevelWidget());
    fod.exec();
}

void FormatView::on_tbSignatureScanner_clicked()
{
    this->_hexeditview->scanSignatures(ui->tbSignatureScanner->isChecked());
}

void FormatView::on_tbEntropy_clicked()
{
    if(ui->tbEntropy->isChecked())
        this->_hexeditview->binaryNavigator()->displayEntropy();
    else
        this->_hexeditview->binaryNavigator()->displayDefault();
}
