#ifndef PREFSDK_QTLUA_H
#define PREFSDK_QTLUA_H

#include "lua.hpp"
#include "prefsdk/prefexception.h"

#include <QObject>
#include <QMetaObject>
#include <QMetaEnum>
#include <QVector>
#include <QFileInfo>
#include <QDebug>

namespace PrefSDK
{
    class QtLua
    {
        public:
            enum ObjectOwnership { CppOwnership, LuaOwnership };

            class LuaFunction
            {
                private:
                    union StoredFunction { int RegistryIdx; lua_CFunction CFunction; };

                public:
                    LuaFunction();
                    LuaFunction(lua_State* l, int idx, QObject* __this);
                    LuaFunction(const QtLua::LuaFunction& lf);
                    ~LuaFunction();
                    void push(lua_State *l = nullptr) const;
                    bool operator()(int nargs, int nresults = 0, bool threaded = false) const;
                    QtLua::LuaFunction& operator=(const QtLua::LuaFunction& lf);
                    bool isValid() const;
                    lua_State* state() const;

                private:
                    lua_State* _state;
                    bool _iscfunction;
                    StoredFunction _storedfunc;
                    QObject* __this;
            };

        private:
            QtLua();

        public:
            static void open(lua_State* l);
            static void pushObject(lua_State* l, QObject* obj,  QtLua::ObjectOwnership ownership = QtLua::CppOwnership);
            static void pushEnum(lua_State* l, const QMetaEnum& metaenum);
            static bool isQObject(lua_State* l, int idx);
            static QObject* toQObject(lua_State* l, int idx);

            template<typename T> static void registerClass(const QString& customname = QString())
            {
                lua_getglobal(QtLua::_state, "qt");

                lua_pushcfunction(QtLua::_state, &QtLua::constructorT<T>);
                lua_setfield(QtLua::_state, -2, (customname.isEmpty() ? T::staticMetaObject.className() : customname.toUtf8().constData()));
                lua_pop(QtLua::_state, 1);
            }

        private:
            static void registerObjectOwnership(lua_State* l);
            static bool isMethod(const QMetaObject* metaobj, const QString& member, int &idx);
            static bool isProperty(const QMetaObject* metaobj, const QString& member, int &idx);
            static bool checkMetaIndexOverride(lua_State* l, QObject *qobject, const QMetaObject *metaobj);
            static bool checkMetaNewIndexOverride(lua_State* l, QObject *qobject, const QMetaObject *metaobj);
            static void pushMetaTable(lua_State* l, QtLua::ObjectOwnership ownership);

        private:
            template<typename T> static int constructorT(lua_State* l)
            {
                int argc = lua_gettop(l);
                QtLua::ObjectOwnership ownership = QtLua::LuaOwnership;

                if(argc)
                {
                    int t = lua_type(l, 1);

                    if(t != LUA_TNUMBER)
                    {
                        throw PrefException(QString("QtLua::constructorT(): Expected 'ObjectOwnership', type '%1' given").arg(lua_typename(l, t)));
                        return 0;
                    }

                    ownership = static_cast<QtLua::ObjectOwnership>(lua_tointeger(l, 1));
                }

                QtLua::pushObject(l, new T(), ownership);
                return 1;
            }

            static int qmlLoad(lua_State* l);
            static int metaIndex(lua_State* l);
            static int metaNewIndex(lua_State* l);
            static int metaGc(lua_State* l);
            static int methodCall(lua_State* l);

        private:
            static lua_State* _state;
            static QVector<luaL_Reg> _methods;
    };
}

Q_DECLARE_METATYPE(PrefSDK::QtLua::LuaFunction)

#endif // PREFSDK_QTLUA_H
