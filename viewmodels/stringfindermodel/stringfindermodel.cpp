#include "stringfindermodel.h"

StringFinderModel::StringFinderModel(QObject *parent): QAbstractItemModel(parent)
{
    this->_filtered = false;
}

void StringFinderModel::clearItems()
{
    this->_offsetstringmap.clear();
}

void StringFinderModel::addItem(quint64 offset, QString string)
{
    int len = this->_offsetstringmap.length();

    this->beginInsertRows(QModelIndex(), len, len);
    this->_offsetstringmap.insert(offset, string);
    this->endInsertRows();
}

void StringFinderModel::filterString(QString s)
{
    this->_visiblestrings.clear();

    if(!this->_offsetstringmap.length() || s.isEmpty()) /* Not Filtered */
    {
        if(s.isEmpty())
            this->_filtered = false;

        emit dataChanged(this->index(0, 0), this->index(this->_offsetstringmap.length() - 1, this->columnCount()));
        return;
    }

    this->_filtered = true;

    for(int i = 0; i < this->_offsetstringmap.length(); i++)
    {
        QString fs = this->_offsetstringmap.value(i);

        if(fs.contains(s, Qt::CaseInsensitive))
            this->_visiblestrings.insert(this->_offsetstringmap.key(i), this->_offsetstringmap.value(i));
    }

    emit dataChanged(this->index(0, 0), this->index(this->_visiblestrings.length() - 1, this->columnCount()));
}

int StringFinderModel::visibleStringsCount()
{
    if(this->_filtered)
        return this->_visiblestrings.length();

    return this->_offsetstringmap.length();
}

bool StringFinderModel::filtered()
{
    return this->_filtered;
}

qint64 StringFinderModel::offset(int i)
{
    if(this->_filtered)
        return this->_visiblestrings.key(i);

    return this->_offsetstringmap.key(i);
}

QString StringFinderModel::string(int i)
{
    if(this->_filtered)
        return this->_visiblestrings.value(i);

    return this->_offsetstringmap.value(i);
}

int StringFinderModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant StringFinderModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant StringFinderModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            if(this->_filtered)
                return QString("0x").append(QString("%1").arg(this->_visiblestrings.key(index.row()), 8, 16, QLatin1Char('0')).toUpper());

            return QString("0x").append(QString("%1").arg(this->_offsetstringmap.key(index.row()), 8, 16, QLatin1Char('0')).toUpper());
        }
        else if(index.column() == 1)
        {
            if(this->_filtered)
                return this->_visiblestrings.value(index.row()).simplified();

            return this->_offsetstringmap.value(index.row()).simplified();
        }
    }

    return QVariant();
}

QModelIndex StringFinderModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex StringFinderModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int StringFinderModel::rowCount(const QModelIndex &) const
{
    if(this->_filtered)
        return this->_visiblestrings.length();

    return this->_offsetstringmap.length();
}

Qt::ItemFlags StringFinderModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
