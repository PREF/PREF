#include "chartviewpage.h"
#include "ui_chartviewpage.h"

QMap<uchar, QString> ChartViewPage::_nonasciichars;

ChartViewPage::ChartViewPage(QHexEditData* hexeditdata, QHexEdit *hexedit, QWidget *parent): QWidget(parent), ui(new Ui::ChartViewPage)
{
    if(ChartViewPage::_nonasciichars.isEmpty())
        ChartViewPage::initNonAsciiChars();

    ui->setupUi(this);

    this->_hexedit = hexedit;
    this->_hexeditdata = hexeditdata;

    this->_charthelper = new ChartHelper();
    this->_toolbar = new ElaborateToolBar();

    ui->mainLayout->insertWidget(0, this->_toolbar);
    this->createListModel();

    connect(this->_toolbar, SIGNAL(startTriggered()), this, SLOT(calculate()));
    connect(this->_toolbar, SIGNAL(stopTriggered()), this->_charthelper, SLOT(stop()));
    connect(this->_charthelper, SIGNAL(progressChanged(int)), ui->progressBar, SLOT(setValue(int)));
    connect(this->_charthelper, SIGNAL(finished()), this, SLOT(onChartFinished()));
    connect(this->_charthelper, SIGNAL(finished()), this->_toolbar, SLOT(elaborationCompleted()));
}

ChartViewPage::~ChartViewPage()
{
    delete ui;
}

void ChartViewPage::initNonAsciiChars()
{
    ChartViewPage::_nonasciichars.insert(0x00, "NUL");
    ChartViewPage::_nonasciichars.insert(0x01, "SOH");
    ChartViewPage::_nonasciichars.insert(0x02, "STX");
    ChartViewPage::_nonasciichars.insert(0x03, "ETX");
    ChartViewPage::_nonasciichars.insert(0x04, "EOT");
    ChartViewPage::_nonasciichars.insert(0x05, "ENQ");
    ChartViewPage::_nonasciichars.insert(0x06, "ACK");
    ChartViewPage::_nonasciichars.insert(0x07, "BEL");
    ChartViewPage::_nonasciichars.insert(0x08, "BS");
    ChartViewPage::_nonasciichars.insert(0x09, "TAB");
    ChartViewPage::_nonasciichars.insert(0x0A, "LF");
    ChartViewPage::_nonasciichars.insert(0x0B, "VT");
    ChartViewPage::_nonasciichars.insert(0x0C, "FF");
    ChartViewPage::_nonasciichars.insert(0x0D, "CR");
    ChartViewPage::_nonasciichars.insert(0x0E, "SO");
    ChartViewPage::_nonasciichars.insert(0x0F, "SI");
    ChartViewPage::_nonasciichars.insert(0x10, "DLE");
    ChartViewPage::_nonasciichars.insert(0x11, "DC1");
    ChartViewPage::_nonasciichars.insert(0x12, "DC2");
    ChartViewPage::_nonasciichars.insert(0x13, "DC3");
    ChartViewPage::_nonasciichars.insert(0x14, "DC4");
    ChartViewPage::_nonasciichars.insert(0x15, "NAK");
    ChartViewPage::_nonasciichars.insert(0x16, "SYN");
    ChartViewPage::_nonasciichars.insert(0x17, "ETB");
    ChartViewPage::_nonasciichars.insert(0x18, "CAN");
    ChartViewPage::_nonasciichars.insert(0x19, "EM");
    ChartViewPage::_nonasciichars.insert(0x1A, "SUB");
    ChartViewPage::_nonasciichars.insert(0x1B, "ESC");
    ChartViewPage::_nonasciichars.insert(0x1C, "FS");
    ChartViewPage::_nonasciichars.insert(0x1D, "GS");
    ChartViewPage::_nonasciichars.insert(0x1E, "RS");
    ChartViewPage::_nonasciichars.insert(0x1F, "US");
    ChartViewPage::_nonasciichars.insert(0x20, "Space");
    ChartViewPage::_nonasciichars.insert(0x7F, "DEL");
}

void ChartViewPage::createListModel()
{
    QStandardItemModel* model = new QStandardItemModel(0, 3, this);
    model->setHorizontalHeaderLabels(QStringList() << "Char" << "Byte" << "Count" << "%");
    ui->lisOccurrence->setModel(model);

    for(int i = 0; i <= 0xFF; i++)
    {
        QString s;

        if(ChartViewPage::_nonasciichars.contains(i))
            s = ChartViewPage::_nonasciichars[i];
        else
            s = QString(QChar(i));


        QStandardItem* item = new QStandardItem(s);
        model->appendRow(item);
        model->setItem(item->row(), 1, new QStandardItem(QString("0x").append(QString("%1").arg(i, 2, 16, QLatin1Char('0')).toUpper())));
    }

    for(int i = 0; i < model->columnCount(); i++)
        ui->lisOccurrence->resizeColumnToContents(i);
}

void ChartViewPage::calculate()
{
    ui->progressBar->setValue(0);
    QtConcurrent::run(this->_charthelper, &ChartHelper::run, this->_hexeditdata, 0, this->_hexeditdata->length());
}

void ChartViewPage::updateModel()
{
    uint imax = 0xFFFFFFFF;
    qint64 len = this->_hexeditdata->length(), max = 0;

    QStandardItemModel* model = (QStandardItemModel*)ui->lisOccurrence->model();
    QList<qint64> occlist = this->_charthelper->occurrences();

    for(uint i = 0; i <= 0xFF; i++)
    {
        QModelIndex idx1 = model->index(i, 2);
        QModelIndex idx2 = model->index(i, 3);

        qint64 count = occlist[i];

        if(count > max)
        {
            imax = i;
            max = count;
        }

        model->setData(idx1, count, Qt::DisplayRole);
        model->setData(idx2, QString("%1%").arg(((qreal)count / (qreal)len) * 100));
    }

    if(imax != 0xFFFFFFFF)
    {
        QModelIndex idx0 = model->index(imax, 0);
        QModelIndex idx1 = model->index(imax, 1);
        QModelIndex idx2 = model->index(imax, 2);
        QModelIndex idx3 = model->index(imax, 3);

        model->setData(idx0, QColor(Qt::yellow), Qt::BackgroundRole);
        model->setData(idx1, QColor(Qt::yellow), Qt::BackgroundRole);
        model->setData(idx2, QColor(Qt::yellow), Qt::BackgroundRole);
        model->setData(idx3, QColor(Qt::yellow), Qt::BackgroundRole);
    }

    qreal e = entropy(occlist, len);
    ui->lblEntropy->setText(QString::number(e));

    QPalette p = ui->lblEntropy->palette();
    p.setColor(ui->lblEntropy->foregroundRole(), ByteColors::entropyColor(e));
    ui->lblEntropy->setPalette(p);

    for(int i = 0; i < model->columnCount(); i++)
        ui->lisOccurrence->resizeColumnToContents(i);
}

void ChartViewPage::onChartFinished()
{    
    ui->histogram->setData(this->_charthelper->occurrences());
    this->updateModel();
}
