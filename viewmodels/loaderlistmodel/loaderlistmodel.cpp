#include "loaderlistmodel.h"

LoaderListModel::LoaderListModel(QHexEditData *hexeditdata, QObject *parent): QAbstractItemModel(parent)
{
    this->validateLoaders(hexeditdata);
    this->_icoloader.load(":/action_icons/res/cpu.png");
    this->_icoloader = this->_icoloader.scaled(22, 22);
}

LoaderList::LoaderId LoaderListModel::loader(int idx) const
{
    return this->_loaders[idx];
}

void LoaderListModel::validateLoaders(QHexEditData *hexeditdata)
{
    lua_State* l = SDKManager::state();

    lua_getglobal(l, "Sdk");
    lua_getfield(l, -1, "validateLoaders");
    lua_pushlightuserdata(l, this);
    lua_pushlightuserdata(l, hexeditdata);

    int res = lua_pcall(l, 2, 0, 0);

    if(res != 0)
        DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(l, -1)));

    lua_pop(l, (res ? 2 : 1));
}

void LoaderListModel::setValid(LoaderList::LoaderId loaderid)
{
    int l = this->_loaders.length();

    this->beginInsertRows(QModelIndex(), l, l);
    this->_loaders.append(loaderid);
    this->endInsertRows();
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
        LoaderList::Loader& l = LoaderList::loader(this->_loaders[index.row()]);

        switch(index.column())
        {
            case 0:
                return l.name();

            case 1:
                return l.author();

            case 2:
                return l.version();

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
