#include "chartwidget.h"
#include "ui_chartwidget.h"

QMap<uchar, QString> ChartWidget::_nonasciichars;

ChartWidget::ChartWidget(QWidget *parent): QWidget(parent), ui(new Ui::ChartWidget)
{
    if(ChartWidget::_nonasciichars.isEmpty())
        ChartWidget::initNonAsciiChars();

    ui->setupUi(this);

    this->createListModel();
}

void ChartWidget::plot(QHexEditData *hexeditdata)
{
    if(!hexeditdata->length())
        return;

    QtConcurrent::run(this, &ChartWidget::elaborate, hexeditdata);
}

ChartWidget::~ChartWidget()
{
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
    QStandardItemModel* model = new QStandardItemModel(0, 3, this);
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

    for(int i = 0; i < model->columnCount(); i++)
        ui->lisOccurrence->resizeColumnToContents(i);
}

void ChartWidget::elaborate(QHexEditData *hexeditdata)
{
    qint64 maxocc = 0;
    QList<qint64> occurrences;
    QHexEditDataReader reader(hexeditdata);

    for(int c = 0x00; c <= 0xFF; c++)
        occurrences.append(0);

    for(qint64 i = 0; i < hexeditdata->length(); i++)
    {
        uchar b = reader.at(i);
        occurrences[b] += i;

        if(occurrences[b] > maxocc)
            maxocc = occurrences[b];
    }

    ui->histogram->setData(occurrences);
}
