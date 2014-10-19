#include "registerset.h"

namespace PrefSDK
{
    RegisterSet::RegisterSet(DataType::Type registertype, QObject *parent) : QObject(parent), _registertype(registertype)
    {
    }

    QString RegisterSet::name(lua_Integer id) const
    {
        return this->_idmap[id]->name();
    }

    Register *RegisterSet::registerById(lua_Integer id)
    {
        return this->_idmap[id];
    }

    int RegisterSet::metaIndex(lua_State *l, lua_Integer key)
    {
        if(this->_idmap.contains(key))
        {
            QtLua::pushObject(l, this->_idmap[key]);
            return 1;
        }

        throw PrefException(QString("RegisterSet::metaIndex(): Invalid Register Id: %1h").arg(QString::number(key, 16).toUpper()));
        return 0;
    }

    int RegisterSet::metaIndex(lua_State *l, QString key)
    {
        if(this->_namemap.contains(key))
        {
            QtLua::pushObject(l, this->_namemap[key]);
            return 1;
        }

        throw PrefException(QString("RegisterSet::metaIndex(): Invalid Register Name: %1").arg(key));
        return 0;
    }

    bool RegisterSet::metaNewIndex(lua_State *l, lua_Integer key)
    {
        int t = lua_type(l, 3);

        if(t != LUA_TSTRING)
        {
            throw PrefException(QString("RegisterSet::metaNewIndex(): Expected 'string' type not '%1'").arg(QString::fromUtf8(lua_typename(l, t))));
            return false;
        }

        QString registername = QString::fromUtf8(lua_tostring(l, 3));
        Register* reg = new Register(key, registername, this->_registertype, this);

        this->_namemap[registername] = reg;
        this->_idmap[key] = reg;
        return true;
    }
}
