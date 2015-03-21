#include "arm64.h"
#include <string.h>

static int luacapstone_memoperand_metaIndex(lua_State* l)
{
    arm64_op_mem* memop = *((arm64_op_mem**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(arm64_op_mem, field);
    const arm64_op_mem_FieldInfo* fi = GetField(arm64_op_mem, field);

    if(fi->IsIntegral)
        lua_pushinteger(l, fieldValue<lua_Integer>(memop, fi));
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_operand_shift_metaIndex(lua_State* l)
{
    cs_arm64_op* op = (*(cs_arm64_op**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    if(!strcmp(field, "type"))
        lua_pushinteger(l, op->shift.type);
    else if(!strcmp(field, "value"))
        lua_pushinteger(l, op->shift.value);
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_operand_metaIndex(lua_State* l)
{
    cs_arm64_op* op = (*(cs_arm64_op**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_arm64_op, field);
    const cs_arm64_op_FieldInfo* fi = GetField(cs_arm64_op, field);

    if(fi->IsBoolean)
        lua_pushboolean(l, fieldValue<bool>(op, fi));
    else if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(op, fi));
    else if(fi->IsFloatingPoint)
        lua_pushnumber(l, fieldValue<lua_Number>(op, fi));
    else if(fi->IsStruct)
    {
        if(IsField(fi, "mem"))
            lua_pushcstruct(l, &(op->mem), &luacapstone_memoperand_metaIndex, "__arm64_operand_mem");
        else if(IsField(fi, "shift")) /* Nested Anonymous Structure */
            lua_pushcstruct(l, op, &luacapstone_operand_shift_metaIndex, "__arm64_operand_shift_mem");
        else
        {
            NOT_IMPLEMENTED(field);
        }
    }
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_operandarray_metaIndex(lua_State* l)
{
    cs_arm64_op** pop = (cs_arm64_op**)lua_touserdata(l, 1);
    lua_pushcstruct(l, (*pop) + (lua_tointeger(l, 2) - 1), &luacapstone_operand_metaIndex, "__arm64_operand");
    return 1;
}

static int luacapstone_arm64_metaIndex(lua_State* l)
{
    cs_arm64* arm64 = (*(cs_arm64**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_arm64, field);
    const cs_arm64_FieldInfo* fi = GetField(cs_arm64, field);

    if(fi->IsBoolean)
        lua_pushboolean(l, fieldValue<bool>(arm64, fi));
    else if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(arm64, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(arm64->operands), &luacapstone_operandarray_metaIndex, "__arm64_operands");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

void luaCS_pusharm64struct(lua_State *l, cs_arm64 *arm64)
{
    lua_pushcstruct(l, arm64, &luacapstone_arm64_metaIndex, "__detail_arm64");
}
