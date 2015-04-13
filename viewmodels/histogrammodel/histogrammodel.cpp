#include "histogrammodel.h"

QMap<uchar, QString>  HistogramModel::_nonasciichars;

HistogramModel::HistogramModel(const HistogramChart &histogramchart, QHexEditData *hexeditdata, QObject *parent): QAbstractItemModel(parent), _histogramchart(histogramchart), _hexeditdata(hexeditdata)
{
    if(HistogramModel::_nonasciichars.isEmpty())
        HistogramModel::initNonAsciiChars();

    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);
}

void HistogramModel::updateStats()
{
    this->beginInsertRows(QModelIndex(), 0, this->_histogramchart.result().Counts.size());
    this->endInsertRows();
}

void HistogramModel::initNonAsciiChars()
{
    HistogramModel::_nonasciichars.insert(0x00, "NUL");
    HistogramModel::_nonasciichars.insert(0x01, "SOH");
    HistogramModel::_nonasciichars.insert(0x02, "STX");
    HistogramModel::_nonasciichars.insert(0x03, "ETX");
    HistogramModel::_nonasciichars.insert(0x04, "EOT");
    HistogramModel::_nonasciichars.insert(0x05, "ENQ");
    HistogramModel::_nonasciichars.insert(0x06, "ACK");
    HistogramModel::_nonasciichars.insert(0x07, "BEL");
    HistogramModel::_nonasciichars.insert(0x08, "BS");
    HistogramModel::_nonasciichars.insert(0x09, "TAB");
    HistogramModel::_nonasciichars.insert(0x0A, "LF");
    HistogramModel::_nonasciichars.insert(0x0B, "VT");
    HistogramModel::_nonasciichars.insert(0x0C, "FF");
    HistogramModel::_nonasciichars.insert(0x0D, "CR");
    HistogramModel::_nonasciichars.insert(0x0E, "SO");
    HistogramModel::_nonasciichars.insert(0x0F, "SI");
    HistogramModel::_nonasciichars.insert(0x10, "DLE");
    HistogramModel::_nonasciichars.insert(0x11, "DC1");
    HistogramModel::_nonasciichars.insert(0x12, "DC2");
    HistogramModel::_nonasciichars.insert(0x13, "DC3");
    HistogramModel::_nonasciichars.insert(0x14, "DC4");
    HistogramModel::_nonasciichars.insert(0x15, "NAK");
    HistogramModel::_nonasciichars.insert(0x16, "SYN");
    HistogramModel::_nonasciichars.insert(0x17, "ETB");
    HistogramModel::_nonasciichars.insert(0x18, "CAN");
    HistogramModel::_nonasciichars.insert(0x19, "EM");
    HistogramModel::_nonasciichars.insert(0x1A, "SUB");
    HistogramModel::_nonasciichars.insert(0x1B, "ESC");
    HistogramModel::_nonasciichars.insert(0x1C, "FS");
    HistogramModel::_nonasciichars.insert(0x1D, "GS");
    HistogramModel::_nonasciichars.insert(0x1E, "RS");
    HistogramModel::_nonasciichars.insert(0x1F, "US");
    HistogramModel::_nonasciichars.insert(0x20, "Space");
    HistogramModel::_nonasciichars.insert(0x7F, "DEL");
}

int HistogramModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant HistogramModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Char";

            case 1:
                return "Byte";

            case 2:
                return "Count";

            case 3:
                return "%";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant HistogramModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    const ByteElaborator::CountResult& cr = this->_histogramchart.result();

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            if(HistogramModel::_nonasciichars.contains(index.row()))
                return HistogramModel::_nonasciichars[index.row()];

            return QString(QChar(index.row()));
        }
        else if(index.column() == 1)
            return QString("%1").arg(index.row(), 2, 16, QLatin1Char('0')).toUpper().append("h");
        else if(index.column() == 2)
            return QString::number(cr.Counts.at(index.row()));
        else if(index.column() == 3)
            return QString("%1%").arg((static_cast<qreal>(cr.Counts.at(index.row())) / static_cast<qreal>(this->_hexeditdata->length())) * 100);
    }
    else if((role == Qt::ForegroundRole) && (index.row() != cr.MaxByte))  /* NOTE: Testing needed */
    {
        if(index.column() == 0)
            return QColor(Qt::darkGreen);

        return QColor(Qt::darkBlue);
    }
    else if((role == Qt::BackgroundRole) && (index.row() == cr.MaxByte)) /* NOTE: Testing needed */
        return QColor(Qt::yellow);
    else if(role == Qt::FontRole)
        return this->_monospacefont;

    return QVariant();
}

QModelIndex HistogramModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex HistogramModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int HistogramModel::rowCount(const QModelIndex &) const
{
    return this->_histogramchart.result().Counts.size();
}

Qt::ItemFlags HistogramModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
