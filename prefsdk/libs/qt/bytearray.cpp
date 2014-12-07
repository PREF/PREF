#include "bytearray.h"

namespace PrefSDK
{
    ByteArray::ByteArray()
    {

    }

    int ByteArray::metaIndex(lua_State *l)
    {
        QByteArray* self = reinterpret_cast<QByteArray*>(lua_touserdata(l, 1));

        if(lua_type(l, 2) == LUA_TNUMBER)
            lua_pushinteger(l, self->at(lua_tointeger(l, 2)));
        else if(lua_type(l, 2) == LUA_TSTRING)
        {
            QString name = QString::fromUtf8(lua_tostring(l, 2));

            if(name == "append")
                return ByteArray::bytearray_append(l);
            else if(name == "length")
                return ByteArray::bytearray_length(l);
            else
                lua_pushnil(l);
        }
        else
            lua_pushnil(l);

        return 1;
    }

    int ByteArray::metaNewIndex(lua_State *l)
    {
        QByteArray* self = reinterpret_cast<QByteArray*>(lua_touserdata(l, 1));

        if(lua_type(l, 2) == LUA_TNUMBER)
        {
            if(lua_type(l, 3) == LUA_TNUMBER)
                self->data()[lua_tointeger(l, 2)] = lua_tointeger(l, 3);
            else if(lua_type(l, 3) == LUA_TSTRING)
                self->data()[lua_tointeger(l, 2)] = lua_tostring(l, 3)[0];
            else
                throw PrefException(QString("ByteArray.__newindex: Unsupported Type '%1'").arg(luaL_typename(l, 3)));
        }
        else
            throw PrefException(QString("ByteArray.__newindex: Unsupported Type '%1'").arg(luaL_typename(l, 2)));

        return 0;
    }

    int ByteArray::bytearray_append(lua_State *l)
    {
        int argc = lua_gettop(l);

        if(argc != 2)
        {
            throw PrefException(QString("ByteArray.append(): Expected 1 argument, %1 given").arg(argc - 1));
            return 0;
        }

        QByteArray* self = reinterpret_cast<QByteArray*>(lua_touserdata(l, 1));
        int t = lua_type(l, 2);

        if(t == LUA_TSTRING)
            self->append(QString::fromUtf8(lua_tostring(l, 2)));
        else if(t == LUA_TUSERDATA)
        {
            QByteArray* ba = reinterpret_cast<QByteArray*>(lua_touserdata(l, 2));

            if(!ba)
            {
                throw PrefException("ByteArray.append(): Expected 'ByteArray' Type");
                return 0;
            }

            self->append(*ba);
        }

        return 0;
    }

    int ByteArray::bytearray_length(lua_State *l)
    {
        QByteArray* self = reinterpret_cast<QByteArray*>(lua_touserdata(l, 1));

        lua_pushinteger(l, self->length());
        return 1;
    }

    void ByteArray::pushMetaTable(lua_State *l)
    {
        lua_newtable(l);

        lua_pushcfunction(l, &ByteArray::metaIndex);
        lua_setfield(l, -2, "__index");

        lua_pushcfunction(l, &ByteArray::metaNewIndex);
        lua_setfield(l, -2, "__newindex");

        lua_pushinteger(l, QMetaType::QByteArray);
        lua_setfield(l, -2, "__type");
    }

    void ByteArray::push(lua_State *l, const QByteArray& bytearray)
    {
        QByteArray* pbytearray = reinterpret_cast<QByteArray*>(lua_newuserdata(l, sizeof(QByteArray*)));
        *pbytearray = bytearray;

        ByteArray::pushMetaTable(l);
        lua_setmetatable(l, -2);
    }
}
