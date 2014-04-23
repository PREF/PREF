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

int LuaStackViewModel::tableLength(int idx)
{
    if(lua_type(this->_state, idx) != LUA_TTABLE)
        return 0;

    int len = 0;
    lua_pushvalue(this->_state, idx);
    lua_pushnil(this->_state);

    while(lua_next(this->_state, -2))
    {
        lua_pop(this->_state, 1);
        len++;
    }

    lua_pop(this->_state, 1);
    return len;
}

QString LuaStackViewModel::typeValue(int idx)
{
    QString s;
    int t = lua_type(this->_state, idx);

    switch(t)
    {
        case LUA_TNUMBER:
            s = QString::number(lua_tonumber(this->_state, idx));
            break;

        case LUA_TSTRING:
            s = QString("'%1'").arg(QString::fromUtf8(lua_tostring(this->_state, idx)));
            break;

        case LUA_TBOOLEAN:
            s = (lua_toboolean(this->_state, idx) != 0 ? "true" : "false");
            break;

        case LUA_TUSERDATA:
            s = QString("%1").arg(reinterpret_cast<size_t>(lua_touserdata(this->_state, idx)), sizeof(size_t), 16, QLatin1Char('0'));
            break;

        case LUA_TNIL:
            s = "nil";
            break;

        default:
            s = QString::fromUtf8(lua_typename(this->_state, t));
            break;
    }

    return s;
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

    for(int i = this->_top; i >= 1; i--)
    {
        StackItem si;
        int t = lua_type(this->_state, i);

        if(t == LUA_TTABLE)
        {
            int len = this->tableLength(i);
            QString s = QString("Table with %1 %2").arg(QString::number(len), (len > 0 ? "items" : "item"));
            si = StackItem(QString::fromUtf8(lua_typename(this->_state, t)), s);
        }
        else
            si = StackItem(lua_typename(this->_state, t), this->typeValue(i));

        this->_stacklist.append(si);
    }
}
