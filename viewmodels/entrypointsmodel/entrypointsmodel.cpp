#include "entrypointsmodel.h"

EntryPointsModel::EntryPointsModel(DisassemblerListing *listing, QObject *parent): QAbstractItemModel(parent), _entrypoints(listing->entryPoints()), _listing(listing)
{
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);
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
        Function* f = reinterpret_cast<Function*>(index.internalPointer());

        switch(index.column())
        {
            case 0:
                return f->startAddress().toString(16).append("h");

            case 1:
                return this->_listing->symbolTable()->get(f->startAddress());

            case 2:
                return this->_listing->findSegment(f)->name();

            default:
                break;
        }
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 0)
            return QColor(Qt::darkBlue);
        else
            return QColor(Qt::darkGreen);
    }
    else if(role == Qt::FontRole)
        return this->_monospacefont;
    else if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QModelIndex EntryPointsModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column, this->_entrypoints[row]);
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
