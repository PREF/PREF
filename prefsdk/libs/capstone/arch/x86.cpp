#include "x86.h"

static int luacapstone_memoperand_metaIndex(lua_State* l)
{
    x86_op_mem* memop = *((x86_op_mem**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(x86_op_mem, field);
    const x86_op_mem_FieldInfo* fi = GetField(x86_op_mem, field);

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
    cs_x86_op* op = (*(cs_x86_op**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_x86_op, field);
    const cs_x86_op_FieldInfo* fi = GetField(cs_x86_op, field);

    if(fi->IsBoolean)
        lua_pushboolean(l, fieldValue<bool>(op, fi));
    else if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(op, fi));
    else if(fi->IsFloatingPoint)
        lua_pushnumber(l, fieldValue<lua_Number>(op, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(op->mem), &luacapstone_memoperand_metaIndex, "__x86_operand_mem");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_operandarray_metaIndex(lua_State* l)
{
    cs_x86_op** pop = (cs_x86_op**)lua_touserdata(l, 1);
    lua_pushcstruct(l, (*pop) + (lua_tointeger(l, 2) - 1), &luacapstone_operand_metaIndex, "__x86_operand");
    return 1;
}

static int luacapstone_x86_metaIndex(lua_State* l)
{
    cs_x86* x86 = (*(cs_x86**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_x86, field);
    const cs_x86_FieldInfo* fi = GetField(cs_x86, field);

    if(fi->IsIntegral)
        lua_pushinteger(l, fieldValue<lua_Integer>(x86, fi));
    else if(fi->IsArray)
    {
        if(IsField(fi, "prefix"))
            lua_pushcarray(l, x86->prefix, ArraySize(x86->prefix));
        else if(IsField(fi, "opcode"))
            lua_pushcarray(l, x86->opcode, ArraySize(x86->opcode));
        else if(IsField(fi, "operands"))
            lua_pushcstruct(l, &(x86->operands), &luacapstone_operandarray_metaIndex, "__x86_operands");
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

void luaCS_pushx86struct(lua_State *l, cs_x86 *x86)
{
    lua_pushcstruct(l, x86, &luacapstone_x86_metaIndex, "__detail_x86");
}
