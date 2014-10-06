#include "entrypointsmodel.h"

EntryPointsModel::EntryPointsModel(DisassemblerListing *listing, QObject *parent): QAbstractItemModel(parent), _entrypoints(listing->entryPoints()), _listing(listing)
{

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
                return f->startAddress().toString(16);

            case 1:
                return this->_listing->symbolTable()->get(f->startAddress());

            case 2:
                return this->_listing->findSegment(f)->name();

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
