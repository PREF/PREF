#include "chartwidget.h"
#include "ui_chartwidget.h"

QMap<uchar, QString> ChartWidget::_nonasciichars;

ChartWidget::ChartWidget(QWidget *parent): WorkerTab(parent), ui(new Ui::ChartWidget), _hexeditdata(nullptr)
{
    if(ChartWidget::_nonasciichars.isEmpty())
        ChartWidget::initNonAsciiChars();

    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 1);
    this->createListModel();

    connect(&this->_worker, SIGNAL(started()), this, SIGNAL(workStarted()));
    connect(&this->_worker, SIGNAL(finished()), this, SLOT(onElaborationCompleted()));
    connect(&this->_worker, SIGNAL(finished()), this, SIGNAL(workFinished()));
}

void ChartWidget::plot(QHexEditData *hexeditdata)
{
    if(!hexeditdata || !hexeditdata->length())
        return;

    this->_hexeditdata = hexeditdata;
    this->updateEntropyText("Calculating...", qApp->palette().text().color());

    this->_worker.setData(hexeditdata);
    this->_worker.start(QThread::LowPriority);
}

ChartWidget::~ChartWidget()
{
    if(this->_worker.isRunning())
    {
        this->_worker.abort();
        this->_worker.wait();
    }

    delete ui;
}

void ChartWidget::initNonAsciiChars()
{
    ChartWidget::_nonasciichars.insert(0x00, "NUL");
    ChartWidget::_nonasciichars.insert(0x01, "SOH");
    ChartWidget::_nonasciichars.insert(0x02, "STX");
    ChartWidget::_nonasciichars.insert(0x03, "ETX");
    ChartWidget::_nonasciichars.insert(0x04, "EOT");
    ChartWidget::_nonasciichars.insert(0x05, "ENQ");
    ChartWidget::_nonasciichars.insert(0x06, "ACK");
    ChartWidget::_nonasciichars.insert(0x07, "BEL");
    ChartWidget::_nonasciichars.insert(0x08, "BS");
    ChartWidget::_nonasciichars.insert(0x09, "TAB");
    ChartWidget::_nonasciichars.insert(0x0A, "LF");
    ChartWidget::_nonasciichars.insert(0x0B, "VT");
    ChartWidget::_nonasciichars.insert(0x0C, "FF");
    ChartWidget::_nonasciichars.insert(0x0D, "CR");
    ChartWidget::_nonasciichars.insert(0x0E, "SO");
    ChartWidget::_nonasciichars.insert(0x0F, "SI");
    ChartWidget::_nonasciichars.insert(0x10, "DLE");
    ChartWidget::_nonasciichars.insert(0x11, "DC1");
    ChartWidget::_nonasciichars.insert(0x12, "DC2");
    ChartWidget::_nonasciichars.insert(0x13, "DC3");
    ChartWidget::_nonasciichars.insert(0x14, "DC4");
    ChartWidget::_nonasciichars.insert(0x15, "NAK");
    ChartWidget::_nonasciichars.insert(0x16, "SYN");
    ChartWidget::_nonasciichars.insert(0x17, "ETB");
    ChartWidget::_nonasciichars.insert(0x18, "CAN");
    ChartWidget::_nonasciichars.insert(0x19, "EM");
    ChartWidget::_nonasciichars.insert(0x1A, "SUB");
    ChartWidget::_nonasciichars.insert(0x1B, "ESC");
    ChartWidget::_nonasciichars.insert(0x1C, "FS");
    ChartWidget::_nonasciichars.insert(0x1D, "GS");
    ChartWidget::_nonasciichars.insert(0x1E, "RS");
    ChartWidget::_nonasciichars.insert(0x1F, "US");
    ChartWidget::_nonasciichars.insert(0x20, "Space");
    ChartWidget::_nonasciichars.insert(0x7F, "DEL");
}

void ChartWidget::createListModel()
{
    QStandardItemModel* model = new QStandardItemModel(0, 3, ui->lisOccurrence);
    model->setHorizontalHeaderLabels(QStringList() << "Char" << "Byte" << "Count" << "%");
    ui->lisOccurrence->setModel(model);

    for(int i = 0; i <= 0xFF; i++)
    {
        QString s;

        if(ChartWidget::_nonasciichars.contains(i))
            s = ChartWidget::_nonasciichars[i];
        else
            s = QString(QChar(i));


        QStandardItem* item = new QStandardItem(s);
        model->appendRow(item);
        model->setItem(item->row(), 1, new QStandardItem(QString("0x").append(QString("%1").arg(i, 2, 16, QLatin1Char('0')).toUpper())));
    }

    ui->lisOccurrence->resizeRowsToContents();
}

void ChartWidget::onElaborationCompleted()
{
    QList<qint64> occurrences = this->_worker.occurrences();
    ui->histogram->setData(occurrences);
    this->updateModel(occurrences);
    this->updateEntropy(occurrences);
}

void ChartWidget::updateModel(const QList<qint64>& occurrences)
{
    uint imax = 0xFFFFFFFF;
    qint64 max = 0;
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->lisOccurrence->model());

    for(uint i = 0; i <= 0xFF; i++)
    {
        QModelIndex idx1 = model->index(i, 2);
        QModelIndex idx2 = model->index(i, 3);

        qint64 count = occurrences[i];

        if(count > max)
        {
            imax = i;
            max = count;
        }

        model->setData(idx1, count, Qt::DisplayRole);
        model->setData(idx2, QString("%1%").arg((static_cast<qreal>(count) / static_cast<qreal>(this->_hexeditdata->length())) * 100));
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

    for(int i = 0; i < model->columnCount(); i++)
        ui->lisOccurrence->resizeColumnToContents(i);
}

void ChartWidget::updateEntropy(const QList<qint64> &occurrences)
{
    qreal e = entropy(occurrences, this->_hexeditdata->length());
    this->updateEntropyText(QString::number(e), ByteColors::entropyColor(e));
}

void ChartWidget::updateEntropyText(const QString &text, const QColor &forecolor)
{
    ui->lblEntropy->setText(text);

    QPalette p = ui->lblEntropy->palette();
    p.setColor(ui->lblEntropy->foregroundRole(), forecolor);
    ui->lblEntropy->setPalette(p);
}

void ChartWidget::on_tbHelp_clicked()
{
    ByteColorsDialog bcd(this);
    bcd.exec();
}
