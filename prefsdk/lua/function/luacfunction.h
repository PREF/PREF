#ifndef LUA_LUACFUNCTION_H
#define LUA_LUACFUNCTION_H

#include <string>
#include <cstring>
#include "../luaobject.h"
#include "../utils/safecall.h"
#include "../utils/tuplestack.h"
#include "../utils/tuplecall.h"
#include "../utils/mangler.h"
#include "../utils/overloadtable.h"

namespace Lua
{
    namespace Private
    {
        template<typename ReturnType, typename... Args> struct CallCFunction
        {
            ReturnValue<ReturnType> operator()(ReturnType (*func)(Args...), Args... args)
            {
                ReturnValue<ReturnType> r = func(args...);
                return r;
            }

            int operator()(lua_State* l, ReturnType (*func)(Args...))
            {
                std::tuple<Args...> args;
                Utils::Stack::stackToTuple(l, args);
                ReturnType r = Utils::FunctionCall::call(func, args);

                luaT_pushvalue(l, r);
                return 1;
            }
        };

        template<typename... Args> struct CallCFunction<void, Args...>
        {
            ReturnValue<void> operator()(void (*func)(Args...), Args... args)
            {
                func(args...);
                return ReturnValue<void>();
            }

            int operator()(lua_State* l, void (*func)(Args...))
            {
                std::tuple<Args...> args;
                Utils::Stack::stackToTuple(l, args);
                Utils::FunctionCall::call(func, args);
                return 0;
            }
        };

        template<typename ReturnType> struct CallCFunction<ReturnType>
        {
            ReturnValue<ReturnType> operator()(ReturnType (*func)())
            {
                ReturnValue<ReturnType> r = func();
                return r;
            }

            int operator()(lua_State* l, ReturnType (*func)())
            {
                ReturnType r = func();
                luaT_pushvalue(l, r);
                return 1;
            }
        };

        template<> struct CallCFunction<void>
        {
            ReturnValue<void> operator()(void (*func)())
            {
                func();
                return ReturnValue<void>();
            }

            int operator()(lua_State*, void (*func)())
            {
                func();
                return 0;
            }
        };
    }

    class LuaCFunction: public LuaObject
    {
        public:
            typedef std::shared_ptr<LuaCFunction> Ptr;

        public:
            LuaCFunction(lua_State* l);
            virtual ~LuaCFunction();
            bool hasReturnType() const;
            virtual int argumentCount() const;
            virtual LuaTypes::LuaType returnType() const = 0;
            virtual const char* mangledName(const char*) const = 0;
            virtual operator lua_CFunction() const;

        protected:
            static bool exists(lua_State* l, lua_String funcname);
            static lua_CFunction dispatcher();
            virtual lua_CFunction luaCFunction() const = 0;

        public: /* Overriden Methods */
            virtual void push();
            virtual lua_Integer length() const;
            virtual LuaTypes::LuaType type() const;

        protected:
            static Utils::OverloadTable<LuaCFunction::Ptr> _globaloverloads;
    };

    template<typename ReturnType, typename... Args> class LuaCFunctionT: public LuaCFunction
    {
        private:
            typedef LuaCFunctionT<ReturnType, Args...> ClassType;

        public:
            typedef ReturnType(*FunctionType)(Args...);
            typedef std::shared_ptr<ClassType> Ptr;

        public:
            LuaCFunctionT(lua_State* l, ClassType::FunctionType func) : LuaCFunction(l), _func(func) { }
            virtual ~LuaCFunctionT() { }

            static ClassType::Ptr create(lua_State* l, ClassType::FunctionType func)
            {
                return ClassType::Ptr(new ClassType(l, func));
            }

            static ClassType::Ptr createGlobal(lua_State* l, lua_String name, ClassType::FunctionType func)
            {
                ClassType::Ptr p(new ClassType(l, func));

                LuaCFunction::_globaloverloads.insert(name, p);

                if(!LuaCFunction::exists(l, name)) /* Push Dispatcher, if not exists */
                {
                    lua_pushstring(l, name);
                    lua_pushcclosure(l, LuaCFunction::dispatcher(), 1);
                    lua_setglobal(l, name);
                }

                return p;
            }

            ReturnValue<ReturnType> operator()(Args... args) const
            {
                ClassType::FunctionType f = this->function();
                return Private::CallCFunction<ReturnType, Args...>()(f, args...);
            }

        public: /* Overriden Methods */
            virtual int argumentCount() const
            {
                return sizeof...(Args);
            }

            virtual const char* mangledName(const char* funcname) const
            {
                std::string s = funcname;
                Utils::Mangler::MangledNameT<Args...>()(&s);
                return strdup(s.c_str());
            }

            virtual LuaTypes::LuaType returnType() const
            {
                return TypeOf<ReturnType>::LuaType;
            }

        protected:
            ClassType::FunctionType function() const
            {
                return this->_func;
            }

        protected:  /* Overriden Methods */
            virtual lua_CFunction luaCFunction() const
            {
                auto f = [](lua_State* l) -> int {
                    ClassType* thethis = reinterpret_cast<ClassType*>(lua_touserdata(l, lua_upvalueindex(1)));

                    if(thethis)
                        return Private::CallCFunction<ReturnType, Args...>()(l, thethis->function());

                    throw LuaException("LuaCFunction::luaCallback(): 'this' Parameter NULL");
                };

                return static_cast<lua_CFunction>(f);
            }

        private:
            ClassType::FunctionType _func;
    };
}

#endif // LUA_LUACFUNCTION_H
