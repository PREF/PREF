#include "optionmodel.h"

OptionModel::OptionModel(const FormatList::Format &format, QObject *parent): QAbstractItemModel(parent), _format(format)
{
    QImage img;
    img.load(":/misc_icons/res/formatoptions.png");

    this->_icooptions = img.scaled(16, 16, Qt::KeepAspectRatio);
}

int OptionModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant OptionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Index";

            case 1:
                return "Name";

            case 2:
                return "Description";

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

    if(role == Qt::DisplayRole)
    {
        FormatList::Format::Option option = this->_format.option(index.row());

        switch(index.column())
        {
            case 0:
                return QString::number(option.index());

            case 1:
                return option.name();

            case 2:
                return option.description();

            default:
                break;
        }
    }
    else if(role == Qt::DecorationRole && index.column() == 0)
        return this->_icooptions;

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
    return this->_format.optionsCount();
}

Qt::ItemFlags OptionModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
