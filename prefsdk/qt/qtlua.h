#ifndef PREFSDK_QTLUA_H
#define PREFSDK_QTLUA_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "lua.hpp"
#include "qluadialog.h"

namespace PrefSDK
{
class QtLua
{
    private:
        QtLua();
        QtLua(const QtLua&);
        static void pushObject(lua_State* l, QObject* obj);

    public:
        static void open(lua_State* l);

        template<typename T> static void registerClass(const QString& customname = QString())
        {
            lua_getglobal(QtLua::_state, "qt");

            lua_pushcfunction(QtLua::_state, &QtLua::constructorT<T>);
            lua_setfield(QtLua::_state, -2, (customname.isEmpty() ? T::staticMetaObject.className() : customname.toUtf8().constData()));
        }

    private:
        static bool isMethod(const QMetaObject* metaobj, const QString& member, int &idx);
        static bool isProperty(const QMetaObject* metaobj, const QString& member, int &idx);
        static void pushMetaTable(lua_State* l);
        static void registerCore();

    private:
        template<typename T> static int constructorT(lua_State* l)
        {
            QtLua::pushObject(l, new T());
            return 1;
        }

        static int metaIndex(lua_State* l);
        static int metaNewIndex(lua_State* l);
        static int metaGc(lua_State* l);
        static int methodCall(lua_State* l);

    private:
        static bool _registered;
        static lua_State* _state;
        static QVector<luaL_Reg> _methods;
    };
}

#endif // PREFSDK_QTLUA_H
