#include "stringsmodel.h"
#include <QColor>

StringsModel::StringsModel(QHexEditData *hexeditdata, QObject *parent) : BasicListModel(parent)
{
    this->_reader = new QHexEditDataReader(hexeditdata, this);
}

void StringsModel::initialize(const ByteElaborator::StringList &stringlist)
{
    this->beginResetModel();
    this->_stringlist = stringlist;
    this->endResetModel();
}

int StringsModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant StringsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((orientation != Qt::Horizontal) || (role != Qt::DisplayRole))
        return QVariant();

    if(section == 0)
        return tr("Offset");
    else if(section == 1)
        return tr("String");

    return QVariant();
}

QVariant StringsModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        const ByteElaborator::StringRange& sr = this->_stringlist[index.row()];

        if(index.column() == 0)
            return QString::number(sr.Start, 16).toUpper() + "h";
        else if(index.column() == 1)
            return this->string(sr);
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 0)
            return QColor(Qt::darkBlue);
        else if(index.column() == 1)
            return QColor(Qt::darkGreen);
    }

    return BasicListModel::data(index, role);
}

int StringsModel::rowCount(const QModelIndex &) const
{
    return this->_stringlist.size();
}

QString StringsModel::string(const ByteElaborator::StringRange &sr) const
{
    return this->_reader->readString(sr.Start, sr.End);
}
