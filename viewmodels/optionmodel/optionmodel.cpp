#include "optionmodel.h"

OptionModel::OptionModel(const FormatDefinition *formatdefinition, QObject *parent): QAbstractItemModel(parent), _formatdefinition(formatdefinition)
{
    QImage img;
    img.load(":/misc_icons/res/formatoptions.png");

    this->_icooptions = img.scaled(16, 16, Qt::KeepAspectRatio);
}

int OptionModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant OptionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Option";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant OptionModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.column() == 0)
    {
        if(role == Qt::DisplayRole)
            return QString::fromUtf8(this->_formatdefinition->OptionName(index.row()));
        else if(role == Qt::DecorationRole)
            return this->_icooptions;
    }

    return QVariant();
}

QModelIndex OptionModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex OptionModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int OptionModel::rowCount(const QModelIndex &) const
{
    return this->_formatdefinition->OptionCount();
}

Qt::ItemFlags OptionModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
