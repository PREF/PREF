#ifndef LUA_LUAMETHOD_H
#define LUA_LUAMETHOD_H

#include "../luareference.h"
#include "../utils/tuplestack.h"
#include "../utils/callhelper.h"

namespace Lua
{
    class LuaMethod : public LuaReference
    {
        public:
            typedef std::shared_ptr<LuaMethod> Ptr;

        public:
            LuaMethod(lua_State *l, lua_String name);
            LuaMethod(lua_State* l, int index);
            virtual int argumentCount() const;

        private:
            LuaReference::Ptr _self;
    };

    template<typename ReturnType, typename... Args> class LuaMethodT: public LuaMethod
    {
        private:
            typedef LuaMethodT<ReturnType, Args...> ClassType;
            typedef Utils::CallHelper<ReturnType, Args...> CallerType;

        public:
            typedef std::shared_ptr<ClassType> Ptr;

        public:
            LuaMethodT(lua_State *l, lua_String name): LuaMethod(l, name) { }
            LuaMethodT(lua_State* l, int index): LuaMethod(l, index) { }

            static ClassType::Ptr fromGlobal(lua_State* l, lua_String name)
            {
                return ClassType::Ptr(new ClassType(l, name));
            }

            static ClassType::Ptr fromStack(lua_State* l, int index)
            {
                return ClassType::Ptr(new ClassType(l, index));
            }

            virtual int argumentCount() const
            {
                return sizeof...(Args);
            }

            virtual LuaTypes::LuaType returnType() const
            {
                return TypeOf<ReturnType>::LuaType;
            }

            template<typename Self> ReturnValue<ReturnType> operator()(Self self, Args... args)
            {
                ClassType::CallerType ct(this);
                return ct(self, args...);
            }
    };

    template<> struct TypeOf<LuaMethod::Ptr>
    {
        typedef LuaMethod::Ptr Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::Function;
        static constexpr const char* TypeName = "Function";
    };

    template<> struct ValueExtractor<LuaMethod::Ptr>
    {
        static LuaMethod::Ptr get(lua_State *l)
        {
            LuaMethod::Ptr val(new LuaMethod(l, -1));
            lua_pop(l, 1);
            return val;
        }
    };
}

#endif // LUA_LUAMETHOD_H
