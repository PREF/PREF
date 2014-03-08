#include "luastackviewmodel.h"

LuaStackViewModel::LuaStackViewModel(lua_State *l, QObject *parent): QAbstractItemModel(parent)
{
    this->_state = l;
}

void LuaStackViewModel::updateTop()
{
    this->_top = lua_gettop(this->_state);
    this->generateStackList();
    emit dataChanged(this->createIndex(0, 0), this->createIndex(this->_top, this->columnCount()));
}

int LuaStackViewModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant LuaStackViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Type";

            case 1:
                return "Value";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant LuaStackViewModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
            return this->_stacklist[index.row()].first;
        else if(index.column() == 1)
            return this->_stacklist[index.row()].second;
    }

    return QVariant();
}

QModelIndex LuaStackViewModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex LuaStackViewModel::parent(const QModelIndex&) const
{
    return QModelIndex();
}

int LuaStackViewModel::rowCount(const QModelIndex&) const
{
    return this->_stacklist.count();
}

Qt::ItemFlags LuaStackViewModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void LuaStackViewModel::generateStackList()
{
    this->_stacklist.clear();

    for(int i = 1; i <= this->_top; i++)
    {
        StackItem si;
        LuaTypes::LuaType t = luaT_typeof(this->_state, i);

        if(t == LuaTypes::Table)
        {
            int len = luaT_tablelength(this->_state, i);
            QString s = QString("Table with %1 %2").arg(QString::number(luaT_tablelength(this->_state, i)), (len > 0 ? "items" : "item"));
            si = StackItem(luaT_typename(t), s);
        }
        else
            si = StackItem(luaT_typename(t), luaT_typevalue(this->_state, i));

        this->_stacklist.append(si);
    }
}
