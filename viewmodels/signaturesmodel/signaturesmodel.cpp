#include "signaturesmodel.h"

SignaturesModel::SignaturesModel(QObject *parent): QAbstractItemModel(parent)
{
}

void SignaturesModel::setData(const SignaturesModel::OffsetList &offsetlist, const SignaturesModel::SignatureMap &signatures)
{
    this->beginInsertRows(QModelIndex(), offsetlist.length(), offsetlist.length());
    this->_offsetlist = offsetlist;
    this->_signatures = signatures;
    this->endInsertRows();
}

qint64 SignaturesModel::signatureOffset(int i) const
{
    return this->_offsetlist[i];
}

qint64 SignaturesModel::signatureLength(int i) const
{
    return this->_signatures[this->_offsetlist[i]].first;
}

QString SignaturesModel::signatureName(int i) const
{
    return this->_signatures[this->_offsetlist[i]].second;
}

int SignaturesModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant SignaturesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Offset";

            case 1:
                return "Name";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant SignaturesModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        qint64 offset = this->_offsetlist[index.row()];

        if(index.column() == 0)
            return QString("0x").append(QString("%1").arg(offset, 8, 16, QLatin1Char('0')).toUpper());
        else if(index.column() == 1)
            return this->_signatures[offset].second;
    }

    return QVariant();
}

QModelIndex SignaturesModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex SignaturesModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int SignaturesModel::rowCount(const QModelIndex &) const
{
    return this->_offsetlist.length();
}

Qt::ItemFlags SignaturesModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
