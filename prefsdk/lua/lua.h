#ifndef LUA_H
#define LUA_H

#include "luathread.h"
#include "table/luatable.h"
#include "table/luactable.h"
#include "function/luafunction.h"
#include "function/luacfunction.h"

#define STRICT_LUA "local getinfo, error, rawset, rawget = debug.getinfo, error, rawset, rawget\n" \
                   "local mt = getmetatable(_G)\n" \
                   "if mt == nil then\n" \
                   "  mt = {}\n" \
                   "  setmetatable(_G, mt)\n" \
                   "end\n" \
                   "mt.__declared = {}\n" \
                   "local function what ()\n" \
                   "  local d = getinfo(3, \"S\")\n" \
                   "  return d and d.what or \"C\"\n" \
                   "end\n" \
                   "mt.__newindex = function (t, n, v)\n" \
                   "  if not mt.__declared[n] then\n" \
                   "  local w = what()\n" \
                   "  if w ~= \"main\" and w ~= \"C\" then\n" \
                   "    error(\"assign to undeclared variable '\"..n..\"'\", 2)\n" \
                   "  end\n" \
                   "  mt.__declared[n] = true\n" \
                   "end\n" \
                   "rawset(t, n, v)\n" \
                   "end\n" \
                   "mt.__index = function (t, n)\n" \
                   "  if not mt.__declared[n] and what() ~= \"C\" then\n" \
                   "    error(\"variable '\"..n..\"' is not declared\", 2)\n" \
                   "  end\n" \
                   "  return rawget(t, n)\n" \
                   "end\n"

namespace Lua
{
    lua_State* luaW_newstate(bool strict = false);
    void luaW_addsearchpath(lua_State *l, const char* s);
    void luaW_dofile(lua_State *l, const char* file);
    void luaW_dostring(lua_State* l, const char* s);
}
#endif // LUA_H
