#include "qtlua.h"

namespace PrefSDK
{
    QtLua::LuaFunction::LuaFunction(): _state(nullptr), _isselftable(false)
    {
        this->_storedfunc.RegistryIdx = LUA_REFNIL;
    }

    QtLua::LuaFunction::LuaFunction(lua_State *l, int idx, QObject* __this): _state(l), _isselftable(false)
    {
        this->_selfparameter.This = __this;
        this->saveReference(l, idx);
    }

    QtLua::LuaFunction::LuaFunction(lua_State *l, int idx, const LuaTable *__self): _state(l), _isselftable(true)
    {
        this->_selfparameter.Self = __self;
        this->saveReference(l, idx);
    }

    QtLua::LuaFunction::LuaFunction(const QtLua::LuaFunction &lf)
    {
        *this = lf;
    }

    QtLua::LuaFunction::~LuaFunction()
    {
        if(!this->_iscfunction && (this->_storedfunc.RegistryIdx != LUA_REFNIL))
            luaL_unref(this->_state, LUA_REGISTRYINDEX, this->_storedfunc.RegistryIdx);

        this->_storedfunc.RegistryIdx = LUA_REFNIL;
        this->_state = nullptr;
        this->_selfparameter.This = nullptr;
    }

    bool QtLua::LuaFunction::pushSelf(lua_State *l) const
    {
        if(this->_isselftable)
        {
            this->_selfparameter.Self->push(l);
            return true;
        }
        else
        {
            QtLua::pushObject(l, this->_selfparameter.This);
            return true;
        }

        return false;
    }

    void QtLua::LuaFunction::push(lua_State* l) const
    {
        if(this->_iscfunction)
            lua_pushcfunction((!l ? this->_state: l), this->_storedfunc.CFunction);
        else if(this->_storedfunc.RegistryIdx != LUA_REFNIL)
            lua_rawgeti((!l ? this->_state : l), LUA_REGISTRYINDEX, this->_storedfunc.RegistryIdx);
    }

    bool QtLua::LuaFunction::operator()(int nargs, bool hasresult) const
    {
        if(this->pushSelf(this->_state)) /* Push self, if any */
        {
            lua_insert(this->_state, 1); /* Move to Bottom */
            nargs++;
        }

        this->push(this->_state);
        lua_insert(this->_state, 1); /* Move to Bottom */
        return lua_pcall(this->_state, nargs, (hasresult ? 1 : 0), 0) != 0;
    }

    QtLua::LuaFunction &QtLua::QtLua::LuaFunction::operator=(const QtLua::QtLua::LuaFunction &lf)
    {
        this->_state = lf._state;
        this->_iscfunction = lf._iscfunction;
        this->_selfparameter = lf._selfparameter;

        if(lf._iscfunction)
            this->_storedfunc.CFunction = lf._storedfunc.CFunction;
        else if(lf._storedfunc.RegistryIdx != LUA_REFNIL)
        {
            lf.push();
            this->_storedfunc.RegistryIdx = luaL_ref(lf._state, LUA_REGISTRYINDEX);
        }

        return *this;
    }

    bool QtLua::LuaFunction::isValid() const
    {
        return this->_storedfunc.RegistryIdx != LUA_REFNIL;
    }

    lua_State *QtLua::LuaFunction::state() const
    {
        return this->_state;
    }

    void QtLua::LuaFunction::saveReference(lua_State *l, int idx)
    {
        this->_iscfunction = lua_iscfunction(l, idx);

        if(this->_iscfunction)
            this->_storedfunc.CFunction = lua_tocfunction(l, idx);
        else
        {
            lua_pushvalue(l, idx);
            this->_storedfunc.RegistryIdx = luaL_ref(l, LUA_REGISTRYINDEX);
        }
    }

    QtLua::LuaTable::LuaTable(): _state(nullptr), _registryidx(LUA_REFNIL)
    {

    }

    QtLua::LuaTable::LuaTable(lua_State *l, int idx): _state(l)
    {
        lua_pushvalue(l, idx);
        this->_registryidx = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    QtLua::LuaTable::LuaTable(const QtLua::LuaTable &tc)
    {
        *this = tc;
    }

    QtLua::LuaTable::~LuaTable()
    {
        if(this->_registryidx != LUA_REFNIL)
            luaL_unref(this->_state, LUA_REGISTRYINDEX, this->_registryidx);

        this->_state = nullptr;
    }

    QtLua::LuaTable &QtLua::LuaTable::operator=(const QtLua::LuaTable &tc)
    {
        this->_state = tc._state;

        if(tc._registryidx != LUA_REFNIL)
        {
            tc.push();
            this->_registryidx = luaL_ref(tc._state, LUA_REGISTRYINDEX);
        }

        return *this;
    }

    void QtLua::LuaTable::push(lua_State* l) const
    {
        lua_rawgeti((!l ? this->_state : l), LUA_REGISTRYINDEX, this->_registryidx);
    }

    void QtLua::LuaTable::bind(QObject *qobject)
    {
        this->push();

        lua_newtable(this->_state);
        QtLua::pushObject(this->_state, qobject);
        lua_setfield(this->_state, -2, "__index");

        lua_setmetatable(this->_state, -2); /* Set QObject as metatable */
        lua_pop(this->_state, 1);
    }

    bool QtLua::LuaTable::isValid() const
    {
        return this->_registryidx != LUA_REFNIL;
    }

    lua_State *QtLua::LuaTable::instance() const
    {
        return this->_state;
    }

    void QtLua::LuaTable::getField(const QString &name, int expectedtype) const
    {
        this->push();
        lua_getfield(this->_state, -1, name.toUtf8().constData());
        int fieldtype = lua_type(this->_state, -1);

        if(fieldtype != expectedtype)
        {
            lua_pop(this->_state, 1);
            throw PrefException(QString("LuaTable::getField(): Expected '%1' type for '%2' field, '%3' given").arg(QString::fromUtf8(lua_typename(this->_state, expectedtype)),
                                                                                                                   name,
                                                                                                                   QString::fromUtf8(lua_typename(this->_state, fieldtype))));
            return;
        }

        lua_remove(this->_state, -2); /* Pop Table From Stack */
    }

    void QtLua::LuaTable::setField(const QString &name, QObject *qobject)
    {
        this->push();
        QtLua::pushObject(this->_state, qobject);
        lua_setfield(this->_state, -2, name.toUtf8().constData());
        lua_pop(this->_state, 1);
    }

    bool QtLua::LuaTable::fieldExists(const QString &name) const
    {
        this->push();
        lua_getfield(this->_state, -1, name.toUtf8().constData());

        bool exists = !lua_isnoneornil(this->_state, -1);
        lua_pop(this->_state, 2);
        return exists;
    }

    QtLua::LuaTable QtLua::LuaTable::getTable(const QString &name) const
    {
        this->getField(name, LUA_TTABLE);
        QtLua::LuaTable t(this->_state, -1);
        lua_pop(this->_state, 1);
        return t;
    }

    QtLua::LuaFunction QtLua::LuaTable::getFunction(const QString &name) const
    {
        this->getField(name, LUA_TFUNCTION);
        QtLua::LuaFunction f(this->_state, -1, this);
        lua_pop(this->_state, 1);
        return f;
    }

    QString QtLua::LuaTable::getString(const QString &name) const
    {
        this->getField(name, LUA_TSTRING);
        QString s = QString::fromUtf8(lua_tostring(this->_state, -1));
        lua_pop(this->_state, 1);
        return s;
    }

    lua_Integer QtLua::LuaTable::getInteger(const QString &name) const
    {
        this->getField(name, LUA_TNUMBER);
        lua_Integer i = lua_tointeger(this->_state, -1);
        lua_pop(this->_state, 1);
        return i;
    }

    bool QtLua::LuaTable::getBoolean(const QString &name) const
    {
        this->getField(name, LUA_TBOOLEAN);
        bool b = lua_toboolean(this->_state, -1) == true;
        lua_pop(this->_state, 1);
        return b;
    }

    lua_State* QtLua::_state = nullptr;
    QVector<luaL_Reg> QtLua::_methods;

    QtLua::QtLua()
    {

    }

    void QtLua::pushObject(lua_State* l, QObject *obj, QtLua::ObjectOwnership ownership)
    {
        QObject** pqobject = reinterpret_cast<QObject**>(lua_newuserdata(l, sizeof(QObject*)));
        *pqobject = obj;

        QtLua::pushMetaTable(l, -1, ownership);
        lua_setmetatable(l, -2);
    }

    void QtLua::pushEnum(lua_State *l, const QMetaEnum &metaenum)
    {
        lua_newtable(l);

        for(int i = 0; i < metaenum.keyCount(); i++)
        {
            const char* k = metaenum.key(i);
            int v = metaenum.keyToValue(k);

            lua_pushinteger(l, v);
            lua_setfield(l, -2, k);
        }
    }

    void QtLua::registerObjectOwnership(lua_State *l)
    {
        lua_newtable(l);

        lua_pushinteger(l, QtLua::CppOwnership);
        lua_setfield(l, -2, "CppOwnership");

        lua_pushinteger(l, QtLua::LuaOwnership);
        lua_setfield(l, -2, "LuaOwnership");

        lua_setfield(l, -2, "ObjectOwnership");
    }

    void QtLua::open(lua_State *l)
    {
        if(QtLua::_state)
            return;

        qRegisterMetaType<lua_Integer>("lua_Integer");

        QtLua::_state = l;
        QtLua::_methods.append( {nullptr, nullptr} );

        luaL_register(l, "qt", QtLua::_methods.cbegin());
        QtLua::registerObjectOwnership(l);
        lua_pop(l, 1); /* Pop Table */
    }

    bool QtLua::isMethod(const QMetaObject *metaobj, const QString &member, int& idx)
    {
        for(int i = 0; i < metaobj->methodCount(); i++)
        {
            QMetaMethod metamethod = metaobj->method(i);

            if(QString::compare(member, metamethod.name()) == 0)
            {
                idx = i;
                return true;
            }
        }

        idx = -1;
        return false;
    }

    bool QtLua::isProperty(const QMetaObject *metaobj, const QString &member, int& idx)
    {
        for(int i = 0; i < metaobj->propertyCount(); i++)
        {
            QMetaProperty metaproperty = metaobj->property(i);

            if(QString::compare(member, metaproperty.name()) == 0)
            {
                idx = i;
                return true;
            }
        }

        idx = -1;
        return false;
    }

    bool QtLua::checkMetaIndexOverride(lua_State *l, QObject* qobject, const QMetaObject* metaobj)
    {
        int residx;

        if(QtLua::isMethod(metaobj, "metaIndex", residx))
        {
            bool success = false;
            int res = 0, t = lua_type(l, 2);

            if((t == LUA_TSTRING) && (metaobj->indexOfMethod(metaobj->normalizedSignature("metaIndex(lua_State*, QString)") )!= -1))
                success = metaobj->invokeMethod(qobject, "metaIndex", Q_RETURN_ARG(int, res), Q_ARG(lua_State*, l), Q_ARG(QString, QString::fromUtf8(lua_tostring(l, 2))));
            else if((t == LUA_TNUMBER) && (metaobj->indexOfMethod(metaobj->normalizedSignature("metaIndex(lua_State*, lua_Integer)")) != -1))
                success = metaobj->invokeMethod(qobject, "metaIndex", Q_RETURN_ARG(int, res), Q_ARG(lua_State*, l), Q_ARG(lua_Integer, lua_tointeger(l, 2)));

            return success && res;
        }

        return false;
    }

    bool QtLua::checkMetaNewIndexOverride(lua_State *l, QObject *qobject, const QMetaObject *metaobj)
    {
        int residx;

        if(QtLua::isMethod(metaobj, "metaNewIndex", residx))
        {
            bool success = false, res = false;
            int t = lua_type(l, 2);

            if((t == LUA_TSTRING) && (metaobj->indexOfMethod(metaobj->normalizedSignature("metaNewIndex(lua_State*, QString)") ) != -1))
                success = metaobj->invokeMethod(qobject, "metaNewIndex", Q_RETURN_ARG(bool, res), Q_ARG(lua_State*, l), Q_ARG(QString, QString::fromUtf8(lua_tostring(l, 2))));
            else if((t == LUA_TNUMBER) && (metaobj->indexOfMethod(metaobj->normalizedSignature("metaNewIndex(lua_State*, lua_Integer)")) != -1))
                success = metaobj->invokeMethod(qobject, "metaNewIndex", Q_RETURN_ARG(bool, res), Q_ARG(lua_State*, l), Q_ARG(lua_Integer, lua_tointeger(l, 2)));

            return success && res;
        }

        return false;
    }

    void QtLua::pushMetaTable(lua_State *l, int userdataidx, QtLua::ObjectOwnership ownership)
    {
        lua_newtable(l);

        lua_pushvalue(l, userdataidx - 1); /* Pass UserData as UpValue */
        lua_pushcclosure(l, &QtLua::metaIndex, 1);
        lua_setfield(l, -2, "__index");

        lua_pushvalue(l, userdataidx - 1); /* Pass UserData as UpValue */
        lua_pushcclosure(l, &QtLua::metaNewIndex, 1);
        lua_setfield(l, -2, "__newindex");

        lua_pushinteger(l, QMetaType::QObjectStar);
        lua_setfield(l, -2, "__type");

        if(ownership == QtLua::LuaOwnership)
        {
            lua_pushcfunction(l, &QtLua::metaGc);
            lua_setfield(l, -2, "__gc");
        }
    }

    int QtLua::metaIndex(lua_State *l)
    {
        int residx = -1;
        QObject* qobject = *(reinterpret_cast<QObject**>(lua_touserdata(l, lua_upvalueindex(1))));
        const QMetaObject* metaobj = qobject->metaObject();

        if(QtLua::checkMetaIndexOverride(l, qobject, metaobj))  /* Allow MetaTable override */
            return 1;

        if(lua_type(l, 2) != LUA_TSTRING)
        {
            throw PrefException(QString("QtLua::metaIndex(): Invalid Member Type for '%1' (%2)").arg(QString::fromUtf8(metaobj->className()), QString::fromUtf8(luaL_typename(l, 2))));
            return 0;
        }

        QString member = QString::fromUtf8(lua_tostring(l, 2));

        if(QtLua::isMethod(metaobj, member, residx))
        {
            QMetaMethod metamethod = metaobj->method(residx);

            lua_pushvalue(l, lua_upvalueindex(1));  /* Pass UserData as UpValue */
            lua_pushinteger(l, residx);
            lua_pushstring(l, metamethod.name().constData());
            lua_pushcclosure(l, &QtLua::methodCall, 3);
        }
        else if(QtLua::isProperty(metaobj, member, residx))
        {
            QMetaProperty metaproperty = metaobj->property(residx);
            int proptype = metaproperty.userType();

            if(!metaproperty.isReadable())
            {
                throw PrefException(QString("QtLua::metaIndex(): Property '%1' is Write-Only for '%2'").arg(member, QString::fromUtf8(metaobj->className())));
                return 0;
            }

            QVariant v = metaproperty.read(qobject);

            if(v.userType() == QMetaType::Bool)
                lua_pushboolean(l, v.toBool());
            else if(v.userType() == QMetaType::type("lua_Integer"))
                lua_pushinteger(l, v.value<lua_Integer>());
            else if(v.userType() == QMetaType::Int)
                lua_pushinteger(l, v.toInt());
            else if(v.userType() == QMetaType::UInt)
                lua_pushinteger(l, v.toUInt());
            else if(v.userType() == QMetaType::Long)
                lua_pushinteger(l, v.value<long>());
            else if(v.userType() == QMetaType::LongLong)
                lua_pushinteger(l, v.toLongLong());
            else if(v.userType() == QMetaType::ULongLong)
                lua_pushinteger(l, v.toULongLong());
            else if(v.userType() == QMetaType::Double)
                lua_pushnumber(l, v.toDouble());
            else if(v.userType() == QMetaType::QString)
                lua_pushstring(l, v.toString().toUtf8().constData());
            else if(v.userType() == QMetaType::QByteArray)
                ByteArray::push(l, v.toByteArray());
            else if(v.userType() == QMetaType::type("PrefSDK::QtLua::LuaTable"))
                v.value<QtLua::LuaTable>().push();
            else if(v.userType() == QMetaType::type("PrefSDK::QtLua::LuaFunction"))
                v.value<QtLua::LuaFunction>().push();

            else if(v.canConvert(QMetaType::QObjectStar))
                QtLua::pushObject(l, v.value<QObject*>());
            else
            {
                throw PrefException(QString("QtLua::metaIndex(): Unsupported Type: '%1' for '%2'").arg(QString::fromUtf8(QMetaType::typeName(proptype)), QString::fromUtf8(metaobj->className())));
                return 0;
            }
        }
        else
            lua_pushnil(l); /* Return null */

        return 1;
    }

    int QtLua::metaNewIndex(lua_State *l)
    {
        QObject* qobject = *(reinterpret_cast<QObject**>(lua_touserdata(l, lua_upvalueindex(1))));
        const QMetaObject* metaobj = qobject->metaObject();

        if(QtLua::checkMetaNewIndexOverride(l, qobject, metaobj))  /* Allow MetaTable override */
            return 0;

        int propidx = -1;
        QString member = QString::fromUtf8(lua_tostring(l, 2));

        if(QtLua::isProperty(metaobj, member, propidx))
        {
            QMetaProperty metaproperty = metaobj->property(propidx);
            int proptype = metaproperty.userType();
            int luaproptype = lua_type(l, 3);

            if(!metaproperty.isWritable())
            {
                throw PrefException(QString("QtLua::metaIndex(): Property '%1' is Read-Only for '%2'").arg(member, QString::fromUtf8(metaobj->className())));
                return 0;
            }

            if(luaproptype == LUA_TBOOLEAN && proptype == QMetaType::Bool)
                metaproperty.write(qobject, QVariant((lua_toboolean(l, 3) == true)));
            else if(luaproptype == LUA_TNUMBER)
            {
                if(proptype == QMetaType::type("lua_Integer"))
                    metaproperty.write(qobject, QVariant::fromValue(lua_tointeger(l, 3)));
                else if((proptype == QMetaType::Int))
                    metaproperty.write(qobject, QVariant(static_cast<int>(lua_tointeger(l, 3))));
                else if(proptype == QMetaType::UInt)
                    metaproperty.write(qobject, QVariant(static_cast<uint>(lua_tointeger(l, 3))));
                else if(proptype == QMetaType::LongLong)
                    metaproperty.write(qobject, QVariant(static_cast<qlonglong>(lua_tointeger(l, 3))));
                else if(proptype == QMetaType::ULongLong)
                    metaproperty.write(qobject, QVariant(static_cast<qulonglong>(lua_tointeger(l, 3))));
                else if(proptype == QMetaType::Double)
                    metaproperty.write(qobject, QVariant(lua_tonumber(l, 3)));
                else
                    throw PrefException(QString("QtLua::metaNewIndex(): Unsupported Integer Type: '%1' for '%2'").arg(QString::fromUtf8(QMetaType::typeName(proptype)), QString::fromUtf8(metaobj->className())));
            }
            else if(luaproptype == LUA_TSTRING)
                metaproperty.write(qobject, QVariant(QString::fromUtf8(lua_tostring(l, 3))));
            else if(luaproptype == LUA_TTABLE)
                metaproperty.write(qobject, QVariant::fromValue(QtLua::LuaTable(l, 3)));
            else if(luaproptype == LUA_TFUNCTION)
                metaproperty.write(qobject, QVariant::fromValue(QtLua::LuaFunction(l, 3, qobject)));
            else
                throw PrefException(QString("QtLua::metaNewIndex(): Unsupported Lua Type: '%1' for '%2'").arg(QString::fromUtf8(lua_typename(l, luaproptype)), QString::fromUtf8(metaobj->className())));
        }
        else
            throw PrefException(QString("QtLua::metaNewIndex(): Invalid Property: '%1' for '%2'").arg(member, QString::fromUtf8(metaobj->className())));

        return 0;
    }

    int QtLua::metaGc(lua_State *l)
    {
        QObject* qobject = *(reinterpret_cast<QObject**>(lua_touserdata(l, 1)));
        qobject->~QObject(); /* Call Destructor */
        return 0;
    }

    int QtLua::methodCall(lua_State *l)
    {
        int argc = lua_gettop(l);
        QObject** self = reinterpret_cast<QObject**>(lua_touserdata(l, lua_upvalueindex(1)));
        lua_Integer methodidx = lua_tointeger(l, lua_upvalueindex(2));
        const char* methodname = lua_tostring(l, lua_upvalueindex(3));

        if(!self)
        {
            throw PrefException(QString("QtLua::methodCall(): Invalid 'self' for '%1' (maybe you'll need to replace '.' with ':') ").arg(QString::fromUtf8(methodname)));
            return 0;
        }

        QObject* qobject = *self;
        const QMetaObject* metaobj = qobject->metaObject();
        QMetaMethod metamethod = metaobj->method(methodidx);

        QVector<QVariant> args(10, QVariant());

        /* function mt.__ctor(metaself, self, ...) */

        for(int i = 2; i <= argc; i++)
        {
            int type = lua_type(l, i);
            int n = i - 2;

            if(type == LUA_TBOOLEAN)
                args[n] = QVariant((lua_toboolean(l, i) == true));
            else if(type == LUA_TNUMBER)
                args[n] = QVariant::fromValue(static_cast<lua_Integer>(lua_tointeger(l, i)));
            else if(type == LUA_TSTRING)
                args[n] = QVariant(QString::fromUtf8(lua_tostring(l, i)));
            else if(type == LUA_TTABLE)
                args[n] = QVariant::fromValue(QtLua::LuaTable(l, i));
            else if(type == LUA_TFUNCTION)
                args[n] = QVariant::fromValue(QtLua::LuaFunction(l, i, qobject));
            else if(type == LUA_TUSERDATA)
            {
                lua_getfield(l, i, "__type");
                QMetaType::Type metatype = static_cast<QMetaType::Type>(lua_tointeger(l, -1));
                lua_pop(l, 1);

                if(metatype == QMetaType::QByteArray)
                    args[n] = QVariant::fromValue(*(reinterpret_cast<QByteArray*>(lua_touserdata(l, i))));
                else
                    args[n] = QVariant::fromValue(*(reinterpret_cast<QObject**>(lua_touserdata(l, i))));
            }
            else
                throw PrefException(QString("QtLua::methodCall(): Unsupported Parameter Lua Type: '%1' for '%2'").arg(QString::fromUtf8(lua_typename(l, type)), QString::fromUtf8(methodname)));
        }

        int returntype = metamethod.returnType();

        if(returntype == QMetaType::UnknownType)
        {
            throw PrefException(QString("Invalid MetaType: '%1' (maybe you need to register it with qRegisterMetaType())").arg(QString::fromUtf8(metamethod.typeName())));
            return 0;
        }

        bool isretvoid = (returntype == QMetaType::Void);
        QVariant retvar = (!isretvoid ? QVariant(returntype, nullptr) : QVariant());
        void* retdata = retvar.data();

        bool success = QMetaObject::invokeMethod(qobject, methodname, (!isretvoid ? QGenericReturnArgument(metamethod.typeName(), ((retvar.canConvert(QMetaType::QObjectStar)) ? &retdata : retdata)) : QGenericReturnArgument()),
                                                 (args[0].isValid() ? QGenericArgument(args[0].typeName(), args[0].data()) : QGenericArgument(0)),
                                                 (args[1].isValid() ? QGenericArgument(args[1].typeName(), args[1].data()) : QGenericArgument(0)),
                                                 (args[2].isValid() ? QGenericArgument(args[2].typeName(), args[2].data()) : QGenericArgument(0)),
                                                 (args[3].isValid() ? QGenericArgument(args[3].typeName(), args[3].data()) : QGenericArgument(0)),
                                                 (args[4].isValid() ? QGenericArgument(args[4].typeName(), args[4].data()) : QGenericArgument(0)),
                                                 (args[5].isValid() ? QGenericArgument(args[5].typeName(), args[5].data()) : QGenericArgument(0)),
                                                 (args[6].isValid() ? QGenericArgument(args[6].typeName(), args[6].data()) : QGenericArgument(0)),
                                                 (args[7].isValid() ? QGenericArgument(args[7].typeName(), args[7].data()) : QGenericArgument(0)),
                                                 (args[8].isValid() ? QGenericArgument(args[8].typeName(), args[8].data()) : QGenericArgument(0)),
                                                 (args[9].isValid() ? QGenericArgument(args[9].typeName(), args[9].data()) : QGenericArgument(0)));

        if(!success)
        {
            throw PrefException(QString("QtLua::methodCall(): Failed to call '%1'").arg(QString::fromUtf8(methodname)));
            return 0;
        }

        if(isretvoid)
            return 0;

        if(retvar.canConvert(QMetaType::QObjectStar))
        {
            if(retdata)
                QtLua::pushObject(l, reinterpret_cast<QObject*>(retdata));
            else
                lua_pushnil(l); /* Support 'null' return value */
        }
        else if(returntype == QMetaType::Bool)
            lua_pushboolean(l, retvar.toBool());
        else if(returntype == QMetaType::Int)
            lua_pushinteger(l, retvar.toInt());
        else if(returntype == QMetaType::UInt)
            lua_pushinteger(l, retvar.toUInt());
        else if(returntype == QMetaType::LongLong)
            lua_pushinteger(l, retvar.toLongLong());
        else if(returntype == QMetaType::Double)
            lua_pushnumber(l, retvar.toDouble());
        else if(returntype == QMetaType::QString)
            lua_pushstring(l, retvar.toString().toUtf8().constData());
        else if(returntype == QMetaType::QByteArray)
            ByteArray::push(l, retvar.toByteArray());
        else if(returntype == QMetaType::type("lua_Integer"))
            lua_pushinteger(l, retvar.value<lua_Integer>());
        else
            isretvoid = true; /* Unknown type, ignore it */

        return !isretvoid;
    }
}
