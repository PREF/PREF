#include "sparc.h"

static int luacapstone_memoperand_metaIndex(lua_State* l)
{
    sparc_op_mem* memop = *((sparc_op_mem**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(sparc_op_mem, field);
    const sparc_op_mem_FieldInfo* fi = GetField(sparc_op_mem, field);

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
    cs_sparc_op* op = (*(cs_sparc_op**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_sparc_op, field);
    const cs_sparc_op_FieldInfo* fi = GetField(cs_sparc_op, field);

    if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(op, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(op->mem), &luacapstone_memoperand_metaIndex, "__sparc_operand_mem");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_operandarray_metaIndex(lua_State* l)
{
    cs_sparc_op** pop = (cs_sparc_op**)lua_touserdata(l, 1);
    lua_pushcstruct(l, (*pop) + (lua_tointeger(l, 2) - 1), &luacapstone_operand_metaIndex, "__sparc_operand");
    return 1;
}

static int luacapstone_sparc_metaIndex(lua_State* l)
{
    cs_sparc* sparc = (*(cs_sparc**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_sparc, field);
    const cs_sparc_FieldInfo* fi = GetField(cs_sparc, field);

    if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(sparc, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(sparc->operands), &luacapstone_operandarray_metaIndex, "__sparc_operands");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

void luaCS_pushsparcstruct(lua_State *l, cs_sparc *sparc)
{
    lua_pushcstruct(l, sparc, &luacapstone_sparc_metaIndex, "__detail_sparc");
}
