#include "luaobject.h"

namespace Lua
{
    LuaObject::LuaObject(lua_State *l): _state(l)
    {
    }

    LuaObject::~LuaObject()
    {
        this->_state = NULL;
    }

    lua_State *LuaObject::state() const
    {
        return this->_state;
    }

    bool LuaObject::isNull()
    {
        return this->type() == LuaTypes::Nil;
    }

    void LuaObject::pushGlobal(lua_String name)
    {
        this->push();
        lua_setglobal(this->_state, name);
    }
}
