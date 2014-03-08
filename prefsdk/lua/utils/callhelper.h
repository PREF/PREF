#ifndef LUA_CALLHELPER_H
#define LUA_CALLHELPER_H

#include <lua.hpp>
#include "../utils/safecall.h"
#include "../utils/tuplestack.h"
#include "../luaobject.h"

namespace Lua
{
    namespace Utils
    {
        class CallHelperBase
        {
            public:
                CallHelperBase(LuaObject* obj);
                CallHelperBase(const CallHelperBase& ch);
                virtual CallHelperBase& operator=(const CallHelperBase& rhs);

            protected:
                LuaObject* object();

            private:
                LuaObject* _object;
        };

        template<typename ReturnType, typename... Args> class CallHelper: public CallHelperBase
        {
            private:
                typedef CallHelper<ReturnType, Args...> ClassType;

            public:
                CallHelper(LuaObject* obj): CallHelperBase(obj) { }
                CallHelper(const ClassType& cf): CallHelperBase(cf) { }

                operator ReturnType()
                {
                    return this->_result;
                }

                ReturnValue<ReturnType> operator()(LuaObject* self, Args... args)
                {
                    ReturnType ret;
                    int argcount = sizeof...(args);

                    this->object()->push();

                    if(self)
                    {
                        argcount++;
                        self->push();
                    }

                    std::tuple<Args...> tupleargs(args...);
                    Utils::Stack::tupleToStack(this->object()->state(), tupleargs);
                    Utils::luaU_safecall(this->object()->state(), argcount, true);

                    luaT_getvalue(this->object()->state(), -1, ret);
                    lua_pop(this->object()->state(), 1);
                    return ReturnValue<ReturnType>(ret);
                }
        };

        template<typename... Args> class CallHelper<void, Args...>: public CallHelperBase
        {
            private:
                typedef CallHelper<void, Args...> ClassType;

            public:
                CallHelper(LuaObject* obj): CallHelperBase(obj) { }
                CallHelper(const ClassType& cf): CallHelperBase(cf) { }

                operator void()
                {

                }

                ReturnValue<void> operator()(LuaObject* self, Args... args)
                {
                    int argcount = sizeof...(args);
                    this->object()->push();

                    if(self)
                    {
                        argcount++;
                        self->push();
                    }

                    std::tuple<Args...> tupleargs(args...);
                    Utils::Stack::tupleToStack(this->object()->state(), tupleargs);
                    Utils::luaU_safecall(this->object()->state(), argcount, false);
                    return ReturnValue<void>();
                }
        };

        template<typename ReturnType> class CallHelper<ReturnType>: public CallHelperBase
        {
            private:
                typedef CallHelper<ReturnType> ClassType;

            public:
                CallHelper(LuaObject* obj): CallHelperBase(obj) { }
                CallHelper(const ClassType& cf): CallHelperBase(cf) { }

                operator ReturnType()
                {
                    return this->_result;
                }

                ReturnValue<ReturnType> operator()(LuaObject* self)
                {
                    ReturnType ret;

                    this->object()->push();

                    if(self)
                        self->push();

                    Utils::luaU_safecall(this->object()->state(), self ? 1 : 0, true);
                    luaT_getvalue(this->object()->state(), -1, ret);
                    lua_pop(this->object()->state(), 1);
                    return ReturnValue<ReturnType>(ret);
                }
        };

        template<> class CallHelper<void>: public CallHelperBase
        {
            private:
                typedef CallHelper<void> ClassType;

            public:
                CallHelper(LuaObject* obj): CallHelperBase(obj) { }
                CallHelper(const ClassType& cf): CallHelperBase(cf) { }

                operator void()
                {

                }

                ReturnValue<void> operator()(LuaObject* self)
                {
                    this->object()->push();

                    if(self)
                        self->push();

                    Utils::luaU_safecall(this->object()->state(), self ? 1 : 0, false);
                    return ReturnValue<void>();
                }
        };
    }
}

#endif // LUA_CALLHELPER_H
