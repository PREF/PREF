#ifndef LUA_LUAFUNCTION_H
#define LUA_LUAFUNCTION_H

#include "../luareference.h"
#include "../utils/tuplestack.h"
#include "../utils/callhelper.h"

namespace Lua
{
    class LuaFunction: public LuaReference
    {
        public:
            typedef std::shared_ptr<LuaFunction> Ptr;

        public:
            LuaFunction(lua_State *l, lua_String name);
            LuaFunction(lua_State* l, int index);
            virtual int argumentCount() const;
    };

    template<typename ReturnType, typename... Args> class LuaFunctionT: public LuaFunction
    {
        private:
            typedef LuaFunctionT<ReturnType, Args...> ClassType;
            typedef Utils::CallHelper<ReturnType, Args...> CallerType;

        public:
            typedef std::shared_ptr<ClassType> Ptr;

        public:
            LuaFunctionT(lua_State *l, lua_String name): LuaFunction(l, name) { }
            LuaFunctionT(lua_State* l, int index): LuaFunction(l, index) { }

            static ClassType::Ptr global(lua_State* l, lua_String name)
            {
                return ClassType::Ptr(new ClassType(l, name));
            }

            static ClassType::Ptr fromStack(lua_State* l, int index)
            {
                return ClassType::Ptr(new ClassType(l, index));
            }

            static ClassType::Ptr convertFunction(LuaFunction::Ptr func)
            {
                func->push();
                ClassType::Ptr ct = ClassType::fromStack(func->state(), -1);
                lua_pop(func->state(), 1);
                return ct;
            }

            virtual int argumentCount() const
            {
                return sizeof...(Args);
            }

            virtual LuaTypes::LuaType returnType() const
            {
                return TypeOf<ReturnType>::LuaType;
            }

            ReturnValue<ReturnType> operator()(Args... args)
            {
                ClassType::CallerType ct(this);
                return ct(nullptr, args...);
            }
    };

    template<> struct TypeOf<LuaFunction::Ptr>
    {
        typedef LuaFunction::Ptr Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::Function;
        static constexpr const char* TypeName = "Function";
    };

    template<> struct ValueExtractor<LuaFunction::Ptr>
    {
        static LuaFunction::Ptr get(lua_State *l)
        {
            LuaFunction::Ptr val(new LuaFunction(l, -1));
            lua_pop(l, 1);
            return val;
        }
    };

    void luaT_getvalue(lua_State *l, int index, LuaFunction::Ptr &v);
    void luaT_pushvalue(lua_State *, const LuaFunction::Ptr& v);
}

#endif // LUA_LUAFUNCTION_H
