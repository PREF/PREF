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
        else
            throw PrefException(QString("ByteArray.__index: Expected 'integer' type, '%1' given").arg(QString::fromUtf8(luaL_typename(l, 2))));

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
                throw PrefException(QString("ByteArray.__newindex: Unsupported Type '%1'").arg(QString::fromUtf8(luaL_typename(l, 3))));
        }
        else
            throw PrefException(QString("ByteArray.__newindex: Unsupported Type '%1'").arg(QString::fromUtf8(luaL_typename(l, 2))));

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

        QByteArray* self = reinterpret_cast<QByteArray*>(lua_touserdata(l, lua_upvalueindex(1)));
        int t = lua_type(l, 2);

        if(t == LUA_TSTRING)
            self->append(QString::fromUtf8(lua_tostring(l, 2)));
        else if(t == LUA_TUSERDATA)
        {
            lua_getfield(l, 2, "__type");
            QMetaType::Type type = static_cast<QMetaType::Type>(lua_tointeger(l, -1));
            lua_pop(l, 1);

            if(type != QMetaType::QByteArray)
            {
                throw PrefException("ByteArray.append(): Expected 'ByteArray' Type");
                return 0;
            }


            QByteArray* ba = reinterpret_cast<QByteArray*>(lua_touserdata(l, 2));
            self->append(*ba);
        }

        return 0;
    }

    int ByteArray::bytearray_length(lua_State *l)
    {
        QByteArray* self = reinterpret_cast<QByteArray*>(lua_touserdata(l, lua_upvalueindex(1)));

        lua_pushinteger(l, self->length());
        return 1;
    }

    void ByteArray::pushMetaTable(lua_State *l, int userdataidx)
    {
        lua_newtable(l);

        lua_pushcfunction(l, &ByteArray::metaIndex);
        lua_setfield(l, -2, "__index");

        lua_pushcfunction(l, &ByteArray::metaNewIndex);
        lua_setfield(l, -2, "__newindex");

        lua_pushvalue(l, userdataidx);
        lua_pushcclosure(l, &ByteArray::bytearray_append, 1);
        lua_setfield(l, -2, "append");

        lua_pushvalue(l, userdataidx);
        lua_pushcclosure(l, &ByteArray::bytearray_length, 1);
        lua_setfield(l, -2, "length");

        lua_pushinteger(l, QMetaType::QByteArray);
        lua_setfield(l, -2, "__type");
    }

    void ByteArray::push(lua_State *l, const QByteArray& bytearray)
    {
        new (lua_newuserdata(l, sizeof(QByteArray*))) QByteArray(bytearray);
        ByteArray::pushMetaTable(l, -1);
        lua_setmetatable(l, -2);
    }
}
