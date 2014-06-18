#include "entrypointsmodel.h"

EntryPointsModel::EntryPointsModel(DisassemblerListing *listing, QObject *parent): QAbstractItemModel(parent), _listing(listing)
{
    for(int i = 0; i < listing->segmentsCount(); i++)
    {
        Segment* segment = listing->segment(i);

        for(int j = 0; j < segment->entryPointsCount(); j++)
        {
            Function* f = segment->entryPoint(j);
            this->_entrypoints.append(f);
        }
    }

    std::sort(this->_entrypoints.begin(), this->_entrypoints.end(), &EntryPointsModel::sortByAddress);
}

bool EntryPointsModel::sortByAddress(Function *f1, Function *f2)
{
    return f1->startAddress() < f2->startAddress();
}

int EntryPointsModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant EntryPointsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Address";

            case 1:
                return "Name";

            case 2:
                return "Segment";

            default:
                break;
        }
    }
    else if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QVariant EntryPointsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        Function* f = this->_entrypoints[index.row()];

        switch(index.column())
        {
            case 0:
                return QString("%1").arg(f->startAddress(), 8, 16, QLatin1Char('0')).toUpper();

            case 1:
                return this->_listing->symbolTable()[f->startAddress()]->name();

            case 2:
                return f->segmentName();

            default:
                break;
        }
    }

    return QVariant();
}

QModelIndex EntryPointsModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex EntryPointsModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int EntryPointsModel::rowCount(const QModelIndex &) const
{
    return this->_entrypoints.count();
}

Qt::ItemFlags EntryPointsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
