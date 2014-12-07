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
        {
            lua_pushinteger(l, self->at(lua_tointeger(l, 2) - 1));
            return 1;
        }
        else if(lua_type(l, 2) == LUA_TSTRING)
        {
            QString name = QString::fromUtf8(lua_tostring(l, 2));

            if(name == "append")
            {
                lua_pushvalue(l, 1);
                lua_pushcclosure(l, &ByteArray::bytearray_append, 1);
                return 1;
            }
            else if(name == "length")
            {
                lua_pushinteger(l, self->length());
                return 1;
            }
            else if(name == "__type")
            {
                lua_pushinteger(l, static_cast<lua_Integer>(QMetaType::QByteArray));
                return 1;
            }
        }

        throw PrefException(QString("ByteArray.__index: Expected 'integer' type, '%1' given").arg(QString::fromUtf8(luaL_typename(l, 2))));
        return 0;
    }

    int ByteArray::metaNewIndex(lua_State *l)
    {
        QByteArray* self = reinterpret_cast<QByteArray*>(lua_touserdata(l, 1));

        if(lua_type(l, 2) == LUA_TNUMBER)
        {
            if(lua_type(l, 3) == LUA_TNUMBER)
                self->data()[lua_tointeger(l, 2) - 1] = lua_tointeger(l, 3);
            else if(lua_type(l, 3) == LUA_TSTRING)
                self->data()[lua_tointeger(l, 2) - 1] = lua_tostring(l, 3)[0];
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

        if(argc != 1)
        {
            throw PrefException(QString("ByteArray.append(): Expected 1 argument, %1 given").arg(argc));
            return 0;
        }

        QByteArray* self = reinterpret_cast<QByteArray*>(lua_touserdata(l, lua_upvalueindex(1)));
        int t = lua_type(l, 1);

        if(t == LUA_TSTRING)
            self->append(QString::fromUtf8(lua_tostring(l, 1)));
        else if(t == LUA_TNUMBER)
            self->append(static_cast<char>(lua_tointeger(l, 1)));
        else if(t == LUA_TUSERDATA)
        {
            lua_getfield(l, lua_upvalueindex(1), "__type");
            QMetaType::Type type = static_cast<QMetaType::Type>(lua_tointeger(l, -1));
            lua_pop(l, 1);

            if(type != QMetaType::QByteArray)
            {
                throw PrefException("ByteArray.append(): Expected 'ByteArray' Type");
                return 0;
            }


            QByteArray* ba = reinterpret_cast<QByteArray*>(lua_touserdata(l, 1));
            self->append(*ba);
        }
        else
            throw PrefException(QString("ByteArray.append(): Unsupported type '%1'").arg(lua_typename(l, t)));

        return 0;
    }

    void ByteArray::pushMetaTable(lua_State *l)
    {
        lua_newtable(l);

        lua_pushcfunction(l, &ByteArray::metaIndex);
        lua_setfield(l, -2, "__index");

        lua_pushcfunction(l, &ByteArray::metaNewIndex);
        lua_setfield(l, -2, "__newindex");
    }

    void ByteArray::push(lua_State *l, const QByteArray& bytearray)
    {
        new (lua_newuserdata(l, sizeof(QByteArray*))) QByteArray(bytearray);
        ByteArray::pushMetaTable(l);
        lua_setmetatable(l, -2);
    }
}
