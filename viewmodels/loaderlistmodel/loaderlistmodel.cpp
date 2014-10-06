#include "loaderlistmodel.h"

LoaderListModel::LoaderListModel(QHexEditData *hexeditdata, QObject *parent): QAbstractItemModel(parent)
{
    this->validateLoaders(hexeditdata);
    this->_icoloader.load(":/action_icons/res/cpu.png");
    this->_icoloader = this->_icoloader.scaled(22, 22);
}

ProcessorLoader* LoaderListModel::loader(int idx) const
{
    return this->_loaders[idx];
}

void LoaderListModel::validateLoaders(QHexEditData *hexeditdata)
{
    LoaderList* loaderlist = LoaderList::instance();

    for(int i = 0; i < loaderlist->length(); i++)
    {
        ProcessorLoader* pl = loaderlist->loader(i);

        if(pl->validate(hexeditdata))
            this->_loaders.append(pl);
    }
}

int LoaderListModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant LoaderListModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant LoaderListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        ProcessorLoader* pl = this->_loaders[index.row()];

        switch(index.column())
        {
            case 0:
                return pl->name();

            case 1:
                return pl->author();

            case 2:
                return pl->version();

            default:
                break;
        }
    }
    else if(role == Qt::DecorationRole && index.column() == 0)
        return this->_icoloader;

    return QVariant();
}

QModelIndex LoaderListModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex LoaderListModel::parent(const QModelIndex &) const
{
    return QModelIndex(); /* It's a List: No Parents! */
}

int LoaderListModel::rowCount(const QModelIndex &) const
{
    return this->_loaders.length();
}

Qt::ItemFlags LoaderListModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
