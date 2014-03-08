#include "crossreferencemodel.h"

CrossReferenceModel::CrossReferenceModel(QObject *parent): QAbstractItemModel(parent)
{
    //this->_func = func;
    //this->_disasmlisting = disasmlisting;
}

void CrossReferenceModel::addCrossReference(quint64 va)
{
    int len = this->_xreflist.length();

    this->beginInsertRows(QModelIndex(), len, len);
    this->_xreflist.append(va);
    this->endInsertRows();
}

quint64 CrossReferenceModel::xrefCount()
{
    return this->_xreflist.length();
}

int CrossReferenceModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant CrossReferenceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Direction";

            case 1:
                return "Virtual Address";

            case 2:
                return "Instruction";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant CrossReferenceModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    /*
    if(role == Qt::DisplayRole)
    {
        quint64 va = this->_xreflist.at(index.row());
        DisassemblerListing::DisassemblerItem item = this->_disasmlisting->itemFromVA(va);

        if(index.column() == 0)
        {
            if(va > this->_func.VirtualAddress)
                return "Down";
            else if(va < this->_func.VirtualAddress)
                return "Up";
            else
                return "---";
        }
        else if(index.column() == 1)
            return QString("%1").arg(va, 8, 16, QLatin1Char('0')).toUpper();
        //else if(index.column() == 2)
            //return DisassembledInstructionManager::generateInstruction(item.second);
    }
    */

    return QVariant();
}

QModelIndex CrossReferenceModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex CrossReferenceModel::parent(const QModelIndex &) const
{
    return QModelIndex(); /* It's a List: No Parents! */
}

int CrossReferenceModel::rowCount(const QModelIndex&) const
{
    return this->_xreflist.length();
}

Qt::ItemFlags CrossReferenceModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(index.column() == 1)
        flags |= Qt::ItemIsEditable;

    return flags;
}
