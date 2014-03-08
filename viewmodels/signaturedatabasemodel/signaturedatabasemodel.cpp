#include "signaturedatabasemodel.h"

SignatureDatabaseModel::SignatureDatabaseModel(QObject *parent): QAbstractItemModel(parent), _markedidx(-1)
{
    QImage img;
    img.load(":/misc_icons/res/gear.png");

    this->_icosignature = img.scaled(16, 16, Qt::KeepAspectRatio);
}

bool SignatureDatabaseModel::isPatternMarked()
{
    return this->_markedidx != -1;
}

qint64 SignatureDatabaseModel::markedPatternId()
{
    if(this->_markedidx == -1)
        return -1;

    return SignatureDatabase::database()->signatureId(this->_markedidx);
}

void SignatureDatabaseModel::updateData()
{
    emit layoutChanged(); /* Update the view */
}

void SignatureDatabaseModel::markPattern(qint64 idx)
{
    qint64 oldidx = this->_markedidx;

    if(oldidx != idx)
    {
        this->_markedidx = idx;

        if(idx != -1)
            emit dataChanged(this->index(idx, 0), this->index(idx, this->columnCount() - 1));
    }
    else
        this->_markedidx = -1; /* Just Unset */

    emit dataChanged(this->index(oldidx, 0), this->index(oldidx, this->columnCount() - 1));
}

int SignatureDatabaseModel::columnCount(const QModelIndex &) const
{
    return 6;
}

QVariant SignatureDatabaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return QVariant();

            case 1:
                return "Signature ID";

            case 2:
                return "Name";

            case 3:
                return "Samples";

            case 4:
                return "Max Depth";

            case 5:
                return "Marked";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant SignatureDatabaseModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::EditRole || role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case 0:
                return QVariant();

            case 1:
                return index.internalId();

            case 2:
                return SignatureDatabase::database()->signatureName(index.internalId());

            case 3:
                return SignatureDatabase::database()->signatureSamples(index.internalId());

            case 4:
                return SignatureDatabase::database()->signatureMaxDepth(index.internalId());

            case 5:
                return (this->_markedidx == index.row() ? "Yes" : "No");

            default:
                break;
        }
    }
    else if(role == Qt::DecorationRole && index.column() == 0)
        return this->_icosignature;
    else if(role == Qt::BackgroundRole && this->_markedidx == index.row())
        return QColor(Qt::yellow);

    return QVariant();
}

bool SignatureDatabaseModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole && index.isValid() && index.column() == 1)
    {
        qint64 id = SignatureDatabase::database()->signatureId(index.row());
        QString newvalue = value.toString(), signame = SignatureDatabase::database()->signatureName(id);

        if(newvalue.isEmpty() || !QString::compare(newvalue, signame))
            return false;

        SignatureDatabase::database()->updateName(id, newvalue);
        return true;
    }

    return QAbstractItemModel::setData(index, value, role);
}

QModelIndex SignatureDatabaseModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column, SignatureDatabase::database()->signatureId(row));
}

QModelIndex SignatureDatabaseModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int SignatureDatabaseModel::rowCount(const QModelIndex &) const
{
    return SignatureDatabase::database()->signatureCount();
}

Qt::ItemFlags SignatureDatabaseModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(index.column() == 1)
        flags |= Qt::ItemIsEditable;

    return flags;
}
