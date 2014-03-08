#include "datamapmodel.h"

DataMapModel::DataMapModel(QObject *parent): QAbstractItemModel(parent)
{

}

int DataMapModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant DataMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Offset";

            case 1:
                return "Virtual Address";

            case 2:
                return "Size";

            case 3:
                return "Name";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant DataMapModel::data(const QModelIndex &index, int role) const
{
    /*
    if(!this->_disasmlisting || !this->_disasmlisting->itemCount() || !index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        DataInfo di = this->_disasmlisting->data(index.row());

        switch(index.column())
        {
            case 0:
                return QString("%1").arg(di.Offset, 8, 16, QLatin1Char('0')).toUpper().append("h");

            case 1:

                return QString("%1").arg(di.VirtualAddress, 8, 16, QLatin1Char('0')).toUpper().append("h");

            case 2:
                return QString::number(di.Size);

            case 3:
                return di.Name;

            default:
                break;
        }
    }
    */

    return QVariant();
}

QModelIndex DataMapModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex DataMapModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int DataMapModel::rowCount(const QModelIndex &) const
{
    //if(this->_disasmlisting)
        //return this->_disasmlisting->dataCount();

    return 0;
}

Qt::ItemFlags DataMapModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
