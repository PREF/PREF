#include "formatlistmodel.h"

FormatListModel::FormatListModel(QObject *parent): QAbstractItemModel(parent)
{
    QImage img;
    img.load(":/misc_icons/res/format.png");

    this->_icoformat = img.scaled(16, 16, Qt::KeepAspectRatio);
    this->_category = CategoryContext::GLOBAL_CATEGORY;
    this->_categoryctx = PrefContext::instance()->formats()->categories();
}

void FormatListModel::setCategory(const char* category)
{   
    this->_category = category;
    const CategoryContext::FormatList& fl = this->_categoryctx->formats(category);

    emit dataChanged(this->createIndex(0, 0), this->createIndex(fl.size() - 1, this->columnCount()));
}

const char *FormatListModel::category() const
{
    return this->_category;
}

int FormatListModel::columnCount(const QModelIndex &) const
{
    return 4;
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
                return "Category";

            case 2:
                return "Author";

            case 3:
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
        const CategoryContext::FormatList& fl = this->_categoryctx->formats(this->_category);
        const FormatDefinition* fd = fl.at(index.row());

        switch(index.column())
        {
            case 0:
                return fd->name();

            case 1:
                return fd->category();

            case 2:
                return fd->author();

            case 3:
                return fd->version();

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
    return this->_categoryctx->formats(this->_category).size();
}

Qt::ItemFlags FormatListModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
