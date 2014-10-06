#include "registerset.h"

namespace PrefSDK
{
    RegisterSet::RegisterSet(DataType::Type registertype, QObject *parent) : QObject(parent), _registertype(registertype)
    {
    }

    QString RegisterSet::name(const DataValue &id) const
    {
        return this->_names[id];
    }

    int RegisterSet::metaIndex(lua_State *l, lua_Integer key)
    {
        DataValue dv = DataValue::create(key, this->_registertype);

        if(this->_names.contains(dv))
        {
            lua_pushstring(l, this->_names[dv].toUtf8().constData());
            return 1;
        }

        throw PrefException(QString("RegisterSet::metaIndex(): Invalid Register Id: %1h").arg(dv.toString(16)));
        return 0;
    }

    int RegisterSet::metaIndex(lua_State *l, QString key)
    {
        if(this->_ids.contains(key))
        {
            lua_pushinteger(l, this->_ids[key].compatibleValue<lua_Integer>());
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

        DataValue dv = DataValue::create(key, this->_registertype);

        QString regname = QString::fromUtf8(lua_tostring(l, 3));
        this->_ids[regname] = dv;
        this->_names[dv] = regname;
        return true;
    }
}
