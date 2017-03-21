#include "comparemodel.h"

CompareModel::CompareModel(const CompareModel::OffsetList &offsetlist, const CompareModel::DifferenceMap &differences, QObject *parent): QAbstractItemModel(parent), _offsetlist(offsetlist), _differencemap(differences)
{

}

int CompareModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant CompareModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Start Offset";

            case 1:
                return "End Offset";

            case 2:
                return "Length";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant CompareModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        qint64 startoffset = this->_offsetlist[index.row()];
        qint64 endoffset = this->_differencemap[startoffset];

        if(index.column() == 0)
            return QString("0x").append(QString("%1").arg(startoffset, 8, 16, QLatin1Char('0')).toUpper());
        else if(index.column() == 1)
            return QString("0x").append(QString("%1").arg(endoffset, 8, 16, QLatin1Char('0')).toUpper());
        else if(index.column() == 2)
            return QString::number((endoffset - startoffset) + 1);
    }

    return QVariant();
}

QModelIndex CompareModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex CompareModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int CompareModel::rowCount(const QModelIndex &) const
{
    return this->_offsetlist.length();
}

Qt::ItemFlags CompareModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
