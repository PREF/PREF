#include "mips.h"

static int luacapstone_memoperand_metaIndex(lua_State* l)
{
    mips_op_mem* memop = *((mips_op_mem**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(mips_op_mem, field);
    const mips_op_mem_FieldInfo* fi = GetField(mips_op_mem, field);

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
    cs_mips_op* op = (*(cs_mips_op**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_mips_op, field);
    const cs_mips_op_FieldInfo* fi = GetField(cs_mips_op, field);

    if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(op, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(op->mem), &luacapstone_memoperand_metaIndex, "__mips_operand_mem");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_operandarray_metaIndex(lua_State* l)
{
    cs_mips_op** pop = (cs_mips_op**)lua_touserdata(l, 1);
    lua_pushcstruct(l, (*pop) + (lua_tointeger(l, 2) - 1), &luacapstone_operand_metaIndex, "__mips_operand");
    return 1;
}

static int luacapstone_mips_metaIndex(lua_State* l)
{
    cs_mips* mips = (*(cs_mips**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_mips, field);
    const cs_mips_FieldInfo* fi = GetField(cs_mips, field);

    if(fi->IsIntegral)
        lua_pushinteger(l, fieldValue<lua_Integer>(mips, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(mips->operands), &luacapstone_operandarray_metaIndex, "__mips_operands");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

void luaCS_pushmipsstruct(lua_State *l, cs_mips *mips)
{
    lua_pushcstruct(l, mips, &luacapstone_mips_metaIndex, "__detail_mips");
}
