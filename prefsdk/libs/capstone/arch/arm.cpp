#include "arm.h"

static int luacapstone_memoperand_metaIndex(lua_State* l)
{
    arm_op_mem* memop = *((arm_op_mem**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(arm_op_mem, field);
    const arm_op_mem_FieldInfo* fi = GetField(arm_op_mem, field);

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
    cs_arm_op* op = (*(cs_arm_op**)lua_touserdata(l, 1));
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
    cs_arm_op* op = (*(cs_arm_op**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_arm_op, field);
    const cs_arm_op_FieldInfo* fi = GetField(cs_arm_op, field);

    if(fi->IsBoolean)
        lua_pushboolean(l, fieldValue<bool>(op, fi));
    else if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(op, fi));
    else if(fi->IsFloatingPoint)
        lua_pushnumber(l, fieldValue<lua_Number>(op, fi));
    else if(fi->IsStruct)
    {
        if(IsField(fi, "mem"))
            lua_pushcstruct(l, &(op->mem), &luacapstone_memoperand_metaIndex, "__arm_operand_mem");
        else if(IsField(fi, "shift")) /* Nested Anonymous Structure */
            lua_pushcstruct(l, op, &luacapstone_operand_shift_metaIndex, "__arm_operand_shift_mem");
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
    cs_arm_op** pop = (cs_arm_op**)lua_touserdata(l, 1);
    lua_pushcstruct(l, (*pop) + (lua_tointeger(l, 2) - 1), &luacapstone_operand_metaIndex, "__arm_operand");
    return 1;
}

static int luacapstone_arm_metaIndex(lua_State* l)
{
    cs_arm* arm = (*(cs_arm**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_arm, field);
    const cs_arm_FieldInfo* fi = GetField(cs_arm, field);

    if(fi->IsBoolean)
        lua_pushboolean(l, fieldValue<bool>(arm, fi));
    else if(fi->IsIntegral || fi->IsEnum)
        lua_pushinteger(l, fieldValue<lua_Integer>(arm, fi));
    else if(fi->IsStruct)
        lua_pushcstruct(l, &(arm->operands), &luacapstone_operandarray_metaIndex, "__arm_operands");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

void luaCS_pusharmstruct(lua_State *l, cs_arm *arm)
{
    lua_pushcstruct(l, arm, &luacapstone_arm_metaIndex, "__detail_arm");
}
