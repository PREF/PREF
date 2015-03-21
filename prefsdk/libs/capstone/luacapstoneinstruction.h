#ifndef LUACAPSTONEINSTRUCTION_H
#define LUACAPSTONEINSTRUCTION_H

#include <lua.hpp>
#include <capstone.h>
#include "luacapstonemacros.h"
#include "metadata.h"

int luaCS_pushinsnarray(lua_State* l, cs_insn* insn);

#endif // LUACAPSTONEINSTRUCTION_H

