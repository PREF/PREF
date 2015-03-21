#include "ppc.h"

static int luacapstone_crxoperand_metaIndex(lua_State* l)
{
    ppc_op_crx* crxop = *((ppc_op_crx**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(ppc_op_crx, field);
    const ppc_op_crx_FieldInfo* fi = GetField(ppc_op_crx, field);

    if(fi->IsIntegral)
        lua_pushinteger(l, fieldValue<lua_Integer>(crxop, fi));
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_memoperand_metaIndex(lua_State* l)
{
    ppc_op_mem* memop = *((ppc_op_mem**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_ppc_op, field);
    const ppc_op_mem_FieldInfo* fi = GetField(ppc_op_mem, field);

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
    cs_ppc_op* op = (*(cs_ppc_op**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_ppc_op, field);
    const cs_ppc_op_FieldInfo* fi = GetField(cs_ppc_op, field);

    if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(op, fi));
    else if(fi->IsStruct)
    {
        if(IsField(fi, "mem"))
            lua_pushcstruct(l, &(op->mem), &luacapstone_memoperand_metaIndex, "__ppc_operand_mem");
        else if(IsField(fi, "crx"))
            lua_pushcstruct(l, &(op->crx), &luacapstone_crxoperand_metaIndex, "__ppc_operand_crx");
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
    cs_ppc_op** pop = (cs_ppc_op**)lua_touserdata(l, 1);
    lua_pushcstruct(l, (*pop) + (lua_tointeger(l, 2) - 1), &luacapstone_operand_metaIndex, "__ppc_operand");
    return 1;
}

static int luacapstone_ppc_metaIndex(lua_State* l)
{
    cs_ppc* ppc = (*(cs_ppc**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_ppc, field);
    const cs_ppc_FieldInfo* fi = GetField(cs_ppc, field);

    if(fi->IsBoolean)
        lua_pushboolean(l, fieldValue<bool>(ppc, fi));
    else if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(ppc, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(ppc->operands), &luacapstone_operandarray_metaIndex, "__ppc_operands");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

void luaCS_pushppcstruct(lua_State *l, cs_ppc *ppc)
{
    lua_pushcstruct(l, ppc, &luacapstone_ppc_metaIndex, "__detail_ppc");
}
