#include "formatview.h"
#include "ui_formatview.h"

FormatView::FormatView(QHexEditData* hexeditdata, QWidget *parent): AbstractView(parent), ui(new Ui::FormatView), _formatid(nullptr), _hexeditdata(hexeditdata)
{
    ui->setupUi(this);

    this->createHexView();
    this->createBinaryView();
    this->createChartView();
    this->createStringFinderView();
}

void FormatView::save()
{
    this->_hexeditdata->save();
}

void FormatView::save(QString filename)
{
    QFile f(filename);
    this->_hexeditdata->saveTo(&f);
    f.close();
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
            return FormatView::StringFinderView;

        case 4:
            return FormatView::DisassemblerView;

        default:
            break;
    }

    return FormatView::CustomView;
}

FormatView::~FormatView()
{
    delete ui;
}

bool FormatView::canSave() const
{
    return true;
}

void FormatView::closeEvent(QCloseEvent *event)
{
    FormatList::removeLoadedFormat(this->_hexeditdata);
    AbstractView::closeEvent(event);
}

void FormatView::createHexView()
{
    this->_hexeditview = new HexEditViewPage(this->_hexeditdata, this);
    connect(this->_hexeditview, SIGNAL(formatLoaded(FormatList::FormatId)), this, SLOT(onFormatLoaded(FormatList::FormatId)));
    ui->tabWidget->addTab(this->_hexeditview, "Hex Edit");
}

void FormatView::createBinaryView()
{
    this->_binaryview = new BinaryViewPage(this->_hexeditdata);
    connect(this->_binaryview, SIGNAL(gotoTriggered(qint64)), this, SLOT(jumpToOffset(qint64)));
    ui->tabWidget->addTab(this->_binaryview, "Binary View");
}

void FormatView::createChartView()
{
    this->_chartview = new ChartViewPage(this->_hexeditdata, this->_hexeditview->hexEdit(), this);
    ui->tabWidget->addTab(this->_chartview, "Chart");
}

void FormatView::createDisassemblerView(FormatTree* formattree)
{
    this->_disassemblerview = new DisassemblerViewPage(this->_hexeditdata, formattree, this);
    ui->tabWidget->addTab(this->_disassemblerview, "Disassembler");
}

void FormatView::createStringFinderView()
{
    this->_stringfinderview = new StringFinderViewPage(this->_hexeditdata);
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

void FormatView::onFormatLoaded(FormatList::FormatId formatid)
{
    FormatList::Format& format = FormatList::formatFromId(formatid);

    if(format.canDisassemble())
        this->createDisassemblerView(this->_hexeditview->tree());
}
