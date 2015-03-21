#include "sysz.h"

static int luacapstone_memoperand_metaIndex(lua_State* l)
{
    sysz_op_mem* memop = *((sysz_op_mem**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(sysz_op_mem, field);
    const sysz_op_mem_FieldInfo* fi = GetField(sysz_op_mem, field);

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
    cs_sysz_op* op = (*(cs_sysz_op**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_sysz_op, field);
    const cs_sysz_op_FieldInfo* fi = GetField(cs_sysz_op, field);

    if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(op, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(op->mem), &luacapstone_memoperand_metaIndex, "__sysz_operand_mem");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_operandarray_metaIndex(lua_State* l)
{
    cs_sysz_op** pop = (cs_sysz_op**)lua_touserdata(l, 1);
    lua_pushcstruct(l, (*pop) + (lua_tointeger(l, 2) - 1), &luacapstone_operand_metaIndex, "__sysz_operand");
    return 1;
}

static int luacapstone_sysz_metaIndex(lua_State* l)
{
    cs_sysz* sysz = (*(cs_sysz**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_sysz, field);
    const cs_sysz_FieldInfo* fi = GetField(cs_sysz, field);

    if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(sysz, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(sysz->operands), &luacapstone_operandarray_metaIndex, "__sysz_operands");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

void luaCS_pushsyszstruct(lua_State *l, cs_sysz *sysz)
{
    lua_pushcstruct(l, sysz, &luacapstone_sysz_metaIndex, "__detail_sysz");
}

