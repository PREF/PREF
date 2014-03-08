#include "stringoffsetmodel.h"

StringOffsetModel::StringOffsetModel(ByteBuffer *bytebuffer, QObject *parent): QAbstractItemModel(parent)
{
    this->_bytebuffer = bytebuffer;
}

int StringOffsetModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant StringOffsetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Offset";

            case 1:
                return "String";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant StringOffsetModel::data(const QModelIndex &index, int role) const
{
    /*
    if(!this->_disasmlisting || !this->_disasmlisting->itemCount() || !index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
            return QString("0x").append(QString("%1").arg(this->_disasmlisting->string(index.row()).VirtualAddress, 8, 16, QLatin1Char('0')).toUpper());
        else if(index.column() == 1)
        {
            DataInfo di = this->_disasmlisting->string(index.row());
            return this->_disasmlisting->generateString(this->_hexeditdata, di.VirtualAddress, STRING_MAX_LENGTH);
        }
    }
    */

    return QVariant();
}

QModelIndex StringOffsetModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex StringOffsetModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int StringOffsetModel::rowCount(const QModelIndex &) const
{
    //if(this->_disasmlisting)
        //return this->_disasmlisting->stringCount();

    return 0;
}

Qt::ItemFlags StringOffsetModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
