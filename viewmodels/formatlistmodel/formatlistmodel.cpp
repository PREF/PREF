#include "formatlistmodel.h"

FormatListModel::FormatListModel(QObject *parent): QAbstractItemModel(parent)
{
    QImage img;
    img.load(":/misc_icons/res/format.png");

    this->_icoformat = img.scaled(16, 16, Qt::KeepAspectRatio);
    this->_category = CategoryManager::globalCategory();
}

void FormatListModel::changeCategory(const CategoryManager::CategoryPtr &c)
{
    this->_category = c;
    emit dataChanged(this->createIndex(0, 0), this->createIndex(this->_category->formatsCount() - 1, this->columnCount()));
}

CategoryManager::CategoryPtr FormatListModel::selectedCategory() const
{
    return this->_category;
}

int FormatListModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant FormatListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Name";

            case 1:
                return "Author";

            case 2:
                return "Version";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant FormatListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        const FormatDefinition* f = FormatList::instance()->format(this->_category->globalFormatIndex(index.row()));

        switch(index.column())
        {
            case 0:
                return f->name();

            case 1:
                return f->author();

            case 2:
                return f->version();

            default:
                break;
        }
    }
    else if((role == Qt::DecorationRole) && (index.column() == 0))
        return this->_icoformat;

    return QVariant();
}

QModelIndex FormatListModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex FormatListModel::parent(const QModelIndex &) const
{
    return QModelIndex(); /* It's a List: No Parents! */
}

int FormatListModel::rowCount(const QModelIndex &) const
{
    return this->_category->formatsCount();
}

Qt::ItemFlags FormatListModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
