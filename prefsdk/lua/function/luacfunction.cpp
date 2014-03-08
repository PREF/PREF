#include "luacfunction.h"

namespace Lua
{
    Utils::OverloadTable<LuaCFunction::Ptr> LuaCFunction::_globaloverloads;

    LuaCFunction::LuaCFunction(lua_State *l): LuaObject(l)
    {
    }

    LuaCFunction::~LuaCFunction()
    {

    }

    bool LuaCFunction::hasReturnType() const
    {
        return this->returnType() != LuaTypes::None;
    }

    int LuaCFunction::argumentCount() const
    {
        return 0;
    }

    bool LuaCFunction::exists(lua_State *l, lua_String funcname)
    {
        bool res = true;
        lua_getglobal(l, funcname);

        if(lua_isnoneornil(l, -1))
            res = false;

        lua_pop(l, 1);
        return res;
    }

    lua_CFunction LuaCFunction::dispatcher()
    {
        auto f = [](lua_State* l) -> int {
            int argcount = lua_gettop(l);
            std::string func = lua_tostring(l, lua_upvalueindex(1));
            std::string mangledfunc = func;

            Utils::Mangler::MangledName()(l, argcount, &mangledfunc);

            if(!LuaCFunction::_globaloverloads.contains(func.c_str(), mangledfunc.c_str()))
                throw LuaOverloadException(Utils::Mangler::functionPrototype(l, func, argcount));

            LuaCFunction::Ptr fp = LuaCFunction::_globaloverloads.get(func.c_str(), mangledfunc.c_str());
            fp->push();

            for(int i = 1; i <= argcount; i++)
                lua_pushvalue(l, i);  /* Push/Forward Arguments */

            Utils::luaU_safecall(l, argcount, fp->hasReturnType());
            return (fp->hasReturnType() ? 1 : 0);
        };

        return static_cast<lua_CFunction>(f);
    }

    void LuaCFunction::push()
    {
        luaT_pushvalue(this->state(), reinterpret_cast<lua_LightUserData>(this));
        lua_pushcclosure(this->state(), this->luaCFunction(), 1);
    }

    lua_Integer LuaCFunction::length() const
    {
        return 0;
    }

    LuaTypes::LuaType LuaCFunction::type() const
    {
        return LuaTypes::Function;
    }

    LuaCFunction::operator lua_CFunction() const
    {
        return this->luaCFunction();
    }
}
