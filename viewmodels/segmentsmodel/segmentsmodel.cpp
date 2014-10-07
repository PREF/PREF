#include "segmentsmodel.h"

SegmentsModel::SegmentsModel(DisassemblerListing* listing, QObject *parent): QAbstractItemModel(parent), _listing(listing)
{
    this->_segments = listing->segments().values();

    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);
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
        Segment* segment = this->_segments[index.row()];

        switch(index.column())
        {
            case 0:
                return segment->name();

            case 1:
                return segment->startAddress().toString(16).append("h");

            case 2:
                return segment->endAddress().toString(16).append("h");

            case 3:
                return segment->baseOffset().toString(16).append("h");

            case 4:
                return (segment->type() == Segment::Code ? "Code" : "Data");

            default:
                break;
        }
    }
    else if(role == Qt::ForegroundRole)
    {
        if((index.column() == 0) || (index.column() == 4))
            return QColor(Qt::darkGreen);
        else
            return QColor(Qt::darkBlue);
    }
    else if(role == Qt::FontRole)
        return this->_monospacefont;
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
    return this->_segments.count();
}

Qt::ItemFlags SegmentsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
