#include "qtlua.h"

namespace PrefSDK
{
    bool QtLua::_registered = false;
    lua_State* QtLua::_state = nullptr;
    QVector<luaL_Reg> QtLua::_methods;

    QtLua::QtLua()
    {

    }

    QtLua::QtLua(const QtLua &)
    {

    }

    void QtLua::pushObject(lua_State* l, QObject *obj)
    {
        QObject** pqobject = reinterpret_cast<QObject**>(lua_newuserdata(l, sizeof(QObject*)));
        *pqobject = obj;

        QtLua::pushMetaTable(l);
        lua_setmetatable(l, -2);
    }

    void QtLua::open(lua_State *l)
    {
        if(QtLua::_registered)
            return;

        QtLua::_registered = true;
        QtLua::_state = l;
        QtLua::_methods.append( {nullptr, nullptr} );

        luaL_register(l, "qt", QtLua::_methods.cbegin());
        lua_pop(l, 1); /* Pop Table */

        QtLua::registerCore();
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

    void QtLua::pushMetaTable(lua_State *l)
    {
        lua_newtable(l);
        lua_pushcfunction(l, &QtLua::metaIndex);
        lua_setfield(l, -2, "__index");
        lua_pushcfunction(l, &QtLua::metaNewIndex);
        lua_setfield(l, -2, "__newindex");
        lua_pushcfunction(l, &QtLua::metaGc);
        lua_setfield(l, -2, "__gc");
    }

    void QtLua::registerCore()
    {
        /* QtWidgets */
        QtLua::registerClass<QWidget>();
        QtLua::registerClass<QLuaDialog>("QDialog");
        QtLua::registerClass<QMessageBox>();
    }

    int QtLua::metaIndex(lua_State *l)
    {
        int residx = -1;
        QObject* qobject = *(reinterpret_cast<QObject**>(lua_touserdata(l, 1)));
        QString member = QString::fromUtf8(lua_tostring(l, 2));

        const QMetaObject* metaobj = qobject->metaObject();

        if(QtLua::isMethod(metaobj, member, residx))
        {
            QMetaMethod metamethod = metaobj->method(residx);

            lua_pushinteger(l, residx);
            lua_pushstring(l, metamethod.name().constData());
            lua_pushcclosure(l, &QtLua::methodCall, 2);
        }
        else if(QtLua::isProperty(metaobj, member, residx))
        {
            QMetaProperty metaproperty = metaobj->property(residx);
            QVariant::Type proptype = metaproperty.type();

            if(!metaproperty.isReadable())
                return 0;

            if(proptype == QVariant::Bool)
                lua_pushboolean(l, metaproperty.read(qobject).toBool());
            else if(proptype == QVariant::Int)
                lua_pushinteger(l, metaproperty.read(qobject).toInt());
            else if(proptype == QVariant::UInt)
                lua_pushinteger(l, metaproperty.read(qobject).toUInt());
            else if(proptype == QVariant::LongLong)
                lua_pushinteger(l, metaproperty.read(qobject).toLongLong());
            else if(proptype == QVariant::Double)
                lua_pushnumber(l, metaproperty.read(qobject).toDouble());
            else if(proptype == QVariant::String)
                lua_pushstring(l, metaproperty.read(qobject).toString().toUtf8().constData());
            else
            {
                Q_ASSERT(false);
                return 0;
            }
        }
        else
            return 0;

        return 1;
    }

    int QtLua::metaNewIndex(lua_State *l)
    {
        QObject* qobject = *(reinterpret_cast<QObject**>(lua_touserdata(l, 1)));
        QString member = QString::fromUtf8(lua_tostring(l, 2));

        int propidx = -1;
        const QMetaObject* metaobj = qobject->metaObject();

        if(QtLua::isProperty(metaobj, member, propidx))
        {
            QMetaProperty metaproperty = metaobj->property(propidx);
            QVariant::Type proptype = metaproperty.type();
            int luaproptype = lua_type(l, 3);

            if(!metaproperty.isWritable())
                return 0;

            if(luaproptype == LUA_TBOOLEAN && proptype == QVariant::Bool)
                metaproperty.write(qobject, QVariant((lua_toboolean(l, 3) == true)));
            else if(luaproptype == LUA_TNUMBER)
            {
                if((proptype == QVariant::Int))
                    metaproperty.write(qobject, QVariant(static_cast<int>(lua_tointeger(l, 3))));
                else if(proptype == QVariant::UInt)
                    metaproperty.write(qobject, QVariant(static_cast<uint>(lua_tointeger(l, 3))));
                else if(proptype == QVariant::LongLong)
                    metaproperty.write(qobject, QVariant(static_cast<qlonglong>(lua_tointeger(l, 3))));
                else if(proptype == QVariant::Double)
                    metaproperty.write(qobject, QVariant(lua_tonumber(l, 3)));
                else
                    Q_ASSERT(false);
            }
            else if(luaproptype == LUA_TSTRING)
                metaproperty.write(qobject, QVariant(QString::fromUtf8(lua_tostring(l, 3))));
            else
                Q_ASSERT(false);
        }
        else
            Q_ASSERT(false);

        return 0;
    }

    int QtLua::metaGc(lua_State *l)
    {
        QObject* qobject = *(reinterpret_cast<QObject**>(lua_touserdata(l, 1)));
        qobject->~QObject();
        return 0;
    }

    int QtLua::methodCall(lua_State *l)
    {
        int argc = lua_gettop(l);
        QObject* qobject = *(reinterpret_cast<QObject**>(lua_touserdata(l, 1)));
        lua_Integer methodidx = lua_tointeger(l, lua_upvalueindex(1));
        const char* methodname = lua_tostring(l, lua_upvalueindex(2));

        const QMetaObject* metaobj = qobject->metaObject();
        QMetaMethod metamethod = metaobj->method(methodidx);

        QVector<QVariant> args(10, QVariant());

        /* function mt.__ctor(metaself, self, ...) */

        for(int i = 2; i <= argc; i++)
        {
            int type = lua_type(l, i);

            if(type == LUA_TBOOLEAN)
                args[i - 2] = QVariant((lua_toboolean(l, i) == true));
            else if(type == LUA_TNUMBER)
                args[i - 2] = QVariant(static_cast<int>(lua_tointeger(l, i)));
            else if(type == LUA_TSTRING)
                args[i - 2] = QVariant(QString::fromUtf8(lua_tostring(l, i)));
            else if((type == LUA_TUSERDATA))
                args[i - 2] = QVariant::fromValue(*(reinterpret_cast<QObject**>(lua_touserdata(l, i))));
            else
                Q_ASSERT(false);
        }

        int res = 0;
        QGenericReturnArgument retarg;
        int returntype = metamethod.returnType();
        QVariant retvar(returntype);

        if(returntype != QMetaType::Void)
        {
            retarg = QGenericReturnArgument(QMetaType::typeName(metamethod.returnType()), retvar.data());
            res = 1;
        }

        QMetaObject::invokeMethod(qobject, methodname, retarg, (args[0].isValid() ? QGenericArgument(args[0].typeName(), args[0].data()) : QGenericArgument(0)),
                                                               (args[1].isValid() ? QGenericArgument(args[1].typeName(), args[1].data()) : QGenericArgument(0)),
                                                               (args[2].isValid() ? QGenericArgument(args[2].typeName(), args[2].data()) : QGenericArgument(0)),
                                                               (args[3].isValid() ? QGenericArgument(args[3].typeName(), args[3].data()) : QGenericArgument(0)),
                                                               (args[4].isValid() ? QGenericArgument(args[4].typeName(), args[4].data()) : QGenericArgument(0)),
                                                               (args[5].isValid() ? QGenericArgument(args[5].typeName(), args[5].data()) : QGenericArgument(0)),
                                                               (args[6].isValid() ? QGenericArgument(args[6].typeName(), args[6].data()) : QGenericArgument(0)),
                                                               (args[7].isValid() ? QGenericArgument(args[7].typeName(), args[7].data()) : QGenericArgument(0)),
                                                               (args[8].isValid() ? QGenericArgument(args[8].typeName(), args[8].data()) : QGenericArgument(0)),
                                                               (args[9].isValid() ? QGenericArgument(args[9].typeName(), args[9].data()) : QGenericArgument(0)));

        if(returntype == QMetaType::QObjectStar)
            QtLua::pushObject(l, retvar.value<QObject*>());
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
        else
            res = 0;

        return res;
    }
}
