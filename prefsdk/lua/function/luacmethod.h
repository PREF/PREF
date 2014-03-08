#ifndef LUA_LUACMETHOD_H
#define LUA_LUACMETHOD_H

#include <cstring>
#include "../luaobject.h"
#include "../utils/tuplestack.h"
#include "../utils/tuplecall.h"
#include "../utils/mangler.h"

namespace Lua
{
    namespace Private
    {
        template<typename ReturnType, typename Self, typename... Args> struct CallCMethod
        {
            ReturnValue<ReturnType> operator()(ReturnType (Self::*func)(Args...), Self* self, Args... args)
            {
                ReturnValue<ReturnType> r = (self->*func)(args...);
                return r;
            }

            int operator()(lua_State* l, Self* self, ReturnType (Self::*func)(Args...))
            {
                if((lua_gettop(l) - sizeof...(Args)) == 1)
                    lua_remove(l, 1); /* Strip Self */

                std::tuple<Args...> args;
                Utils::Stack::stackToTuple(l, args);
                ReturnType r = Utils::MethodCall::call(self, args, func);

                luaT_pushvalue(l, r);
                return 1;
            }
        };

        template<typename Self, typename... Args> struct CallCMethod<void, Self, Args...>
        {
            ReturnValue<void> operator()(void (Self::*func)(Args...), Self* self, Args... args)
            {
                (self->*func)(args...);
                return ReturnValue<void>();
            }

            int operator()(lua_State* l, Self* self, void (Self::*func)(Args...))
            {
                if((lua_gettop(l) - sizeof...(Args)) == 1)
                    lua_remove(l, 1); /* Strip Self */

                std::tuple<Args...> args;
                Utils::Stack::stackToTuple(l, args);
                Utils::MethodCall::call(self, args, func);
                return 0;
            }
        };

        template<typename Self, typename ReturnType> struct CallCMethod<ReturnType, Self>
        {
            ReturnValue<ReturnType> operator()(Self* self, ReturnType (Self::*func)())
            {
                ReturnValue<ReturnType> r = (self->*func)();
                return r;
            }

            int operator()(lua_State* l, Self* self, ReturnType (Self::*func)())
            {
                ReturnType r = (self->*func)();
                luaT_pushvalue(l, r);
                return 1;
            }
        };

        template<typename Self> struct CallCMethod<void, Self>
        {
            ReturnValue<void> operator()(Self* self, void (Self::*func)())
            {
                (self->*func)();
                return ReturnValue<void>();
            }

            int operator()(lua_State*, Self* self, void (Self::*func)())
            {
                (self->*func)();
                return 0;
            }
        };
    }

    class LuaCMethod: public LuaObject
    {
        public:
            typedef std::shared_ptr<LuaCMethod> Ptr;

        public:
            LuaCMethod(lua_State* l);
            bool hasReturnType() const;
            virtual int argumentCount() const;
            virtual LuaTypes::LuaType returnType() const = 0;
            virtual const char* mangledName(const char*) const = 0;
            virtual operator lua_CFunction() const;

        protected:
            virtual lua_CFunction luaCFunction() const = 0;

        public: /* Overriden Methods */
            virtual lua_Integer length() const;
            virtual LuaTypes::LuaType type() const;
            virtual void push();
    };

    template<typename ReturnType, typename Self, typename... Args> class LuaCMethodT: public LuaCMethod
    {
        private:
            typedef LuaCMethodT<ReturnType, Self, Args...> ClassType;

        public:
            typedef ReturnType(Self::*MethodType)(Args...);
            typedef std::shared_ptr<ClassType> Ptr;

        public:
            LuaCMethodT(lua_State* l, Self* self, ClassType::MethodType func) : LuaCMethod(l), _self(self), _func(func) { }
            LuaCMethodT(lua_State* l, Self* self, int index) : LuaCMethod(l, index), _self(self) { }

            static ClassType::Ptr create(lua_State* l, Self* self, ClassType::MethodType func)
            {
                return ClassType::Ptr(new ClassType(l, self, func));
            }

            static ClassType::Ptr fromStack(lua_State* l, Self* self, int index)
            {
                return ClassType::Ptr(new ClassType(l, self, index));
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

            ReturnValue<ReturnType> operator()(Args... args) const
            {
                ClassType::MethodType f = this->function();
                return Private::CallCMethod<ReturnType, Self, Args...>()(f, this->_self, args...);
            }

        protected: /* Overriden Methods */
            virtual lua_CFunction luaCFunction() const
            {
                auto f = [](lua_State* l) -> int {
                    ClassType* thethis = reinterpret_cast<ClassType*>(lua_touserdata(l, lua_upvalueindex(1)));

                    if(thethis)
                        return Private::CallCMethod<ReturnType, Self, Args...>()(l, thethis->self(), thethis->function());

                    throw LuaException("LuaCFunction::luaCallback(): 'this' Parameter NULL");
                };

                return static_cast<lua_CFunction>(f);
            }

        protected:
            ClassType::MethodType function() const
            {
                return this->_func;
            }

            Self* self()
            {
                return this->_self;
            }

        private:
            Self* _self;
            ClassType::MethodType _func;
    };
}

#endif // LUA_LUACMETHOD_H
