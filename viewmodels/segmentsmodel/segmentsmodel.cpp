#include "segmentsmodel.h"

SegmentsModel::SegmentsModel(DisassemblerListing* listing, QObject *parent): QAbstractItemModel(parent), _listing(listing)
{

}

int SegmentsModel::columnCount(const QModelIndex &) const
{
    return 5;
}

QVariant SegmentsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Name";

            case 1:
                return "Start Address";

            case 2:
                return "End Address";

            case 3:
                return "Base Offset";

            case 4:
                return "Type";

            case 5:
                return "N. Functions";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant SegmentsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        Segment* segment = this->_listing->segment(index.row());

        switch(index.column())
        {
            case 0:
                return segment->name();

            case 1:
                return QString("%1").arg(segment->startAddress(), 8, 16, QLatin1Char('0')).toUpper();

            case 2:
                return QString("%1").arg(segment->endAddress(), 8, 16, QLatin1Char('0')).toUpper();

            case 3:
                return QString("%1").arg(segment->baseOffset(), 8, 16, QLatin1Char('0')).toUpper();

            case 4:
                return (segment->type() == SegmentTypes::Code ? "Code" : "Data");

            case 5:
                return QString::number(segment->functionsCount());

            default:
                break;
        }
    }
    else if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QModelIndex SegmentsModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex SegmentsModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int SegmentsModel::rowCount(const QModelIndex &) const
{
    return this->_listing->segmentsCount();
}

Qt::ItemFlags SegmentsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
