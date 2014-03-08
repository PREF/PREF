#include "luatable.h"

namespace Lua
{
    const lua_String LuaTable::COBJECT_FIELD = "__cthis__";

    LuaTable::LuaTable(lua_State *l): LuaReference(l)
    {
        lua_newtable(l);

        /* Set metamethod '__index' */
        lua_pushstring(l, "__index");
        lua_pushvalue(l, -2);
        lua_settable(l, -3);

        this->_ref = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    LuaTable::LuaTable(lua_State *l, lua_String name): LuaReference(l)
    {
        lua_getglobal(l, name);
        this->_ref = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    LuaTable::LuaTable(lua_State *l, const LuaTable::Ptr& meta): LuaReference(l)
    {
        lua_newtable(l);
        meta->push();
        lua_setmetatable(l, -2);

        this->_ref = luaL_ref(l, LUA_REGISTRYINDEX);
    }

    LuaTable::LuaTable(lua_State *l, int index): LuaReference(l, index)
    {

    }

    LuaTable::Ptr LuaTable::create(lua_State *l)
    {
        return LuaTable::Ptr(new LuaTable(l));
    }

    LuaTable::Ptr LuaTable::create(lua_State *l, const LuaTable::Ptr &meta)
    {
        return LuaTable::Ptr(new LuaTable(l, meta));
    }

    LuaTable::Ptr LuaTable::fromStack(lua_State *l, int index)
    {
        return LuaTable::Ptr(new LuaTable(l, index));
    }

    LuaTable::Ptr LuaTable::global(lua_State *l, lua_String name)
    {
        return LuaTable::Ptr(new LuaTable(l, name));
    }

    void LuaTable::stripSelf(lua_State *l, int& argcount)
    {
        this->push();

        if((luaT_typeof(l, 1) == LuaTypes::Table) && lua_compare(l, 1, -1, LUA_OPEQ))
        {
            lua_remove(l, 1); /* Remove Self */
            argcount--;
        }

        lua_pop(l, 1);
    }

    lua_CFunction LuaTable::methodDispatcher()
    {
        auto f = [](lua_State* l) -> int {
            int argcount = lua_gettop(l);
            LuaTable* thethis = reinterpret_cast<LuaTable*>(lua_touserdata(l, lua_upvalueindex(1)));
            std::string func = lua_tostring(l, lua_upvalueindex(2));
            std::string mangledfunc = func;

            thethis->stripSelf(l, argcount);
            Utils::Mangler::MangledName()(l, argcount, &mangledfunc);

            if(!thethis->_tmoverloads.contains(func.c_str(), mangledfunc.c_str()))
                throw LuaOverloadException(Utils::Mangler::functionPrototype(l, func, argcount));

            LuaCMethod::Ptr mp = thethis->_tmoverloads.get(func.c_str(), mangledfunc.c_str());
            mp->push();

            for(int i = 1; i <= argcount; i++)
                lua_pushvalue(l, i);  /* Push/Forward Arguments */

            Utils::luaU_safecall(l, argcount, mp->hasReturnType());
            return (mp->hasReturnType() ? 1 : 0);
        };

        return static_cast<lua_CFunction>(f);
    }

    lua_CFunction LuaTable::functionDispatcher()
    {
        auto f = [](lua_State* l) -> int {
            int argcount = lua_gettop(l);
            LuaTable* thethis = reinterpret_cast<LuaTable*>(lua_touserdata(l, lua_upvalueindex(1)));
            std::string func = lua_tostring(l, lua_upvalueindex(2));
            std::string mangledfunc = func;

            thethis->stripSelf(l, argcount);
            Utils::Mangler::MangledName()(l, argcount, &mangledfunc);

            if(!thethis->_tfoverloads.contains(func.c_str(), mangledfunc.c_str()))
                throw LuaOverloadException(Utils::Mangler::functionPrototype(l, func, argcount));

            LuaCFunction::Ptr mp = thethis->_tfoverloads.get(func.c_str(), mangledfunc.c_str());
            mp->push();

            for(int i = 1; i <= argcount; i++)
                lua_pushvalue(l, i);  /* Push/Forward Arguments */

            Utils::luaU_safecall(l, argcount, mp->hasReturnType());
            return (mp->hasReturnType() ? 1 : 0);
        };

        return static_cast<lua_CFunction>(f);
    }

    bool LuaTable::containsKey(const LuaReference::Ptr& key)
    {
        this->push();
        key->push();
        lua_gettable(this->state(), -2);

        bool res = !lua_isnil(this->state(), -1);
        lua_pop(this->state(), 2);
        return res;
    }

    bool LuaTable::isEmpty()
    {
        this->push();

        lua_pushnil(this->state());
        bool res = lua_next(this->state(), -2);

        if(res)
            lua_pop(this->state(), 2); /* Pop Key <-> Value, the table is not empty */

        return !res;
    }

    LuaTable::Iterator LuaTable::begin()
    {
        return LuaTable::Iterator(this);
    }

    LuaTable::Iterator LuaTable::end()
    {
        return LuaTable::Iterator(this, true);
    }

    void LuaTable::setMe(lua_LightUserData thethis)
    {
        this->set(LuaTable::COBJECT_FIELD, thethis);
    }

    int LuaTable::itemCount()
    {
        this->push();
        int len = luaT_tablelength(this->state(), -1);

        lua_pop(this->state(), 1);
        return len;
    }

    void LuaTable::set(lua_Number key, lua_LightUserData value)
    {
        LuaTable::ValueSetter<lua_Number, lua_LightUserData>()(this, key, value);
    }

    void LuaTable::set(lua_String key, lua_LightUserData value)
    {
        LuaTable::ValueSetter<lua_String, lua_LightUserData>()(this, key, value);
    }

    void LuaTable::set(lua_Number key, lua_Number value)
    {
        LuaTable::ValueSetter<lua_Number, lua_Number>()(this, key, value);
    }

    void LuaTable::set(lua_String key, lua_String value)
    {
        LuaTable::ValueSetter<lua_String, lua_String>()(this, key, value);
    }

    void LuaTable::set(lua_String key, lua_Number value)
    {
        LuaTable::ValueSetter<lua_String, lua_Number>()(this, key, value);
    }

    void LuaTable::set(lua_Number key, lua_String value)
    {
        LuaTable::ValueSetter<lua_Number, lua_String>()(this, key, value);
    }

    void LuaTable::set(lua_Number key, lua_CFunction value, void* upvalue)
    {
        LuaTable::LuaFunctionSetter<lua_Number>()(this, key, value, upvalue);
    }

    void LuaTable::set(lua_String key, lua_CFunction value, void *upvalue)
    {
        LuaTable::LuaFunctionSetter<lua_String>()(this, key, value, upvalue);
    }

    void LuaTable::set(lua_Number key, LuaTable::Ptr value)
    {
        LuaTable::ReferenceSetter<lua_Number>()(this, key, value);
    }

    void LuaTable::set(lua_String key, LuaTable::Ptr value)
    {
        LuaTable::ReferenceSetter<lua_String>()(this, key, value);
    }

    void LuaTable::set(lua_Number key, LuaCMethod::Ptr value)
    {
        LuaTable::MethodSetter<lua_Number>()(this, key, value);
    }

    void LuaTable::set(lua_String key, LuaCMethod::Ptr value)
    {
        this->_tmoverloads.insert(key, value);

        if(!this->containsKey(key))
        {
            this->push();

            /* Key */
            lua_pushstring(this->state(), key);

            /* Value */
            lua_pushlightuserdata(this->state(), reinterpret_cast<lua_LightUserData>(this));
            lua_pushstring(this->state(), key);
            lua_pushcclosure(this->state(), LuaTable::methodDispatcher(), 2);

            lua_settable(this->state(), -3);
            lua_pop(this->state(), 1);
        }
    }

    void LuaTable::set(lua_Number key, LuaCFunction::Ptr value)
    {
        LuaTable::FunctionSetter<lua_Number>()(this, key, value);
    }

    void LuaTable::set(lua_String key, LuaCFunction::Ptr value)
    {
        this->_tfoverloads.insert(key, value);

        if(!this->containsKey(key))
        {
            this->push();

            /* Key */
            lua_pushstring(this->state(), key);

            /* Value */
            lua_pushlightuserdata(this->state(), reinterpret_cast<lua_LightUserData>(this));
            lua_pushstring(this->state(), key);
            lua_pushcclosure(this->state(), LuaTable::functionDispatcher(), 2);

            lua_settable(this->state(), -3);
            lua_pop(this->state(), 1);
        }
    }

    void luaT_getvalue(lua_State *l, int index, LuaTable::Ptr &v)
    {
        v = LuaTable::fromStack(l, index);
    }

    void luaT_pushvalue(lua_State*, const LuaTable::Ptr &v)
    {
        v->push();
    }
}
