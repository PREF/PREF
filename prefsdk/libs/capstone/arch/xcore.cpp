#include "xcore.h"

static int luacapstone_memoperand_metaIndex(lua_State* l)
{
    xcore_op_mem* memop = *((xcore_op_mem**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(xcore_op_mem, field);
    const xcore_op_mem_FieldInfo* fi = GetField(xcore_op_mem, field);

    if(fi->IsIntegral)
        lua_pushinteger(l, fieldValue<lua_Integer>(memop, fi));
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_operand_metaIndex(lua_State* l)
{
    cs_xcore_op* op = (*(cs_xcore_op**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_xcore_op, field);
    const cs_xcore_op_FieldInfo* fi = GetField(cs_xcore_op, field);

    if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(op, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(op->mem), &luacapstone_memoperand_metaIndex, "__xcore_operand_mem");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_operandarray_metaIndex(lua_State* l)
{
    cs_xcore_op** pop = (cs_xcore_op**)lua_touserdata(l, 1);
    lua_pushcstruct(l, (*pop) + (lua_tointeger(l, 2) - 1), &luacapstone_operand_metaIndex, "__xcore_operand");
    return 1;
}

static int luacapstone_xcore_metaIndex(lua_State* l)
{
    cs_xcore* xcore = (*(cs_xcore**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_xcore, field);
    const cs_xcore_FieldInfo* fi = GetField(cs_xcore, field);

    if(fi->IsIntegral)
        lua_pushinteger(l, fieldValue<lua_Integer>(xcore, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(xcore->operands), &luacapstone_operandarray_metaIndex, "__xcore_operands");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

void luaCS_pushxcorestruct(lua_State *l, cs_xcore *xcore)
{
    lua_pushcstruct(l, xcore, &luacapstone_xcore_metaIndex, "__detail_xcore");
}

