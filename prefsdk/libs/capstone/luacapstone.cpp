#include "luacapstone.h"
#include "arch/x86.h"
#include "arch/arm64.h"
#include "arch/arm.h"
#include "arch/mips.h"
#include "arch/ppc.h"
#include "arch/sparc.h"
#include "arch/sysz.h"
#include "arch/xcore.h"

/* Start Of CapstoneIterator Metadata */
BeginStructureInfo(CapstoneIterator)
    StructureField(CapstoneIterator, cs_insn*, insn),
    StructureField(CapstoneIterator, const uint8_t*, code),
    StructureField(CapstoneIterator, uint64_t ,codesize),
    StructureField(CapstoneIterator, uint64_t, address)
EndStructureInfo(CapstoneIterator)
/* End Of CapstoneIterator Metadata */

/* MetaMethods */
static int luacapstone_detail_metaIndex(lua_State* l)
{
    cs_detail* detail = (*(cs_detail**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_detail, field);
    const cs_detail_FieldInfo* fi = GetField(cs_detail, field);

    if(fi->IsIntegral)
        lua_pushinteger(l, fieldValue<lua_Integer>(detail, fi));
    else if(fi->IsArray)
    {
        if(IsField(fi, "regs_read"))
            lua_pushcarray(l, detail->regs_read, detail->regs_read_count);
        else if(IsField(fi, "regs_write"))
            lua_pushcarray(l, detail->regs_write, detail->regs_write_count);
        else if(IsField(fi, "groups"))
            lua_pushcarray(l, detail->groups, detail->groups_count);
        else
        {
            NOT_IMPLEMENTED(field);
        }
    }
    else if(fi->IsStruct)
    {
        if(IsField(fi, "x86"))
            luaCS_pushx86struct(l, &(detail->x86));
        else if(IsField(fi, "arm64"))
            luaCS_pusharm64struct(l, &(detail->arm64));
        else if(IsField(fi, "arm"))
            luaCS_pusharmstruct(l, &(detail->arm));
        else if(IsField(fi, "mips"))
            luaCS_pushmipsstruct(l, &(detail->mips));
        else if(IsField(fi, "ppc"))
            luaCS_pushppcstruct(l, &(detail->ppc));
        else if(IsField(fi, "sparc"))
            luaCS_pushsparcstruct(l, &(detail->sparc));
        else if(IsField(fi, "sysz"))
            luaCS_pushsyszstruct(l, &(detail->sysz));
        else if(IsField(fi, "xcore"))
            luaCS_pushxcorestruct(l, &(detail->xcore));
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

static int luacapstone_insn_metaIndex(lua_State* l)
{
    cs_insn* insn = (*(cs_insn**)lua_touserdata(l, 1));
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(cs_insn, field);
    const cs_insn_FieldInfo* fi = GetField(cs_insn, field);

    if(fi->IsIntegral)
        lua_pushinteger(l, fieldValue<lua_Integer>(insn, fi));
    else if(fi->IsString)
        lua_pushstring(l, fieldValue<const char*>(insn, fi));
    else if(fi->IsArray && IsField(fi, "bytes"))
        lua_pushcarray(l, insn->bytes, insn->size);
    else if(fi->IsStruct && IsField(fi, "detail"))
        lua_pushcstruct(l, insn->detail, &luacapstone_detail_metaIndex, "__insn_detail");
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

static int luacapstone_insnarray_metaIndex(lua_State* l)
{
    cs_insn** pinsn = (cs_insn**)lua_touserdata(l, 1);
    lua_pushcstruct(l, (*pinsn) + (lua_tointeger(l, 2) - 1), &luacapstone_insn_metaIndex, "__insn");
    return 1;
}

static int luacapstone_iterator_metaIndex(lua_State* l)
{
    EXPECT_TYPE(LUA_TSTRING, 2);

    CapstoneIterator* it = (CapstoneIterator*)lua_touserdata(l, 1);
    const char* field = lua_tostring(l, 2);

    VALIDATE_FIELD(CapstoneIterator, field);
    const CapstoneIterator_FieldInfo* fi = GetField(CapstoneIterator, field);

    if(fi->IsStruct)
        lua_pushcstruct(l, it->insn, &luacapstone_insn_metaIndex, "__insn");
    else if(fi->IsPointer)
    {
        if((it->__codetype == LUA_TUSERDATA) || (it->__codetype == LUA_TLIGHTUSERDATA))
            lua_pushlightuserdata(l, (void*)it->code);
        else if(it->__codetype == LUA_TSTRING)
            lua_pushstring(l, (const char*)it->code);
        else
        {
            NOT_IMPLEMENTED(field);
        }
    }
    else if(fi->IsIntegral && !IsField(fi, "__codetype")) /* Don't push private fields */
        lua_pushinteger(l, fieldValue<lua_Integer>(it, fi));
    else
    {
        NOT_IMPLEMENTED(field);
    }

    return 1;
}

/* Capstone API */
static int luacapstone_open(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 2);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TNUMBER, 2);

    csh handle;
    cs_err err = cs_open((cs_arch)lua_tointeger(l, 1), (cs_mode)lua_tointeger(l, 2), &handle);
    lua_pushinteger(l, err);

    if(err == CS_ERR_OK)
    {
        lua_pushinteger(l, handle);
        return 2;
    }

    return 1;
}

static int luacapstone_close(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 1);
    EXPECT_TYPE(LUA_TNUMBER, 1);

    csh handle = lua_tointeger(l, 1);
    cs_err err = cs_close(&handle);
    lua_pushinteger(l, err);
    return 1;
}

static int luacapstone_option(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 3);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TNUMBER, 2);
    EXPECT_TYPE(LUA_TNUMBER, 3);

    cs_err err = cs_option((csh)lua_tointeger(l, 1), (cs_opt_type)lua_tointeger(l, 2), lua_tointeger(l, 3));
    lua_pushinteger(l, err);
    return 1;
}

static int luacapstone_malloc(lua_State *l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 1);
    EXPECT_TYPE(LUA_TNUMBER, 1);

    cs_insn* insn = cs_malloc((csh)lua_tointeger(l, 1));
    lua_pushcstruct(l, insn, &luacapstone_insn_metaIndex, "__insn");
    return 1;
}

static int luacapstone_free(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 2);
    EXPECT_TYPE(LUA_TUSERDATA, 1);
    EXPECT_TYPE(LUA_TNUMBER, 2);

    cs_free(*((cs_insn**)lua_touserdata(l, 1)), lua_tointeger(l, 2));
    return 0;
}

static int luacapstone_disasm(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_MIN_ARGC(argc, 4);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TNUMBER, 3);
    EXPECT_TYPE(LUA_TNUMBER, 4);

    int codetype = lua_type(l, 2);

    if((codetype != LUA_TUSERDATA) && (codetype != LUA_TLIGHTUSERDATA) && (codetype != LUA_TSTRING))
    {
        lua_pushfstring(l, "Expected userdata, lightuserdata or string types, %s given", lua_typename(l, codetype));
        lua_error(l);
        return 0;
    }

    const uint8_t* pcode = NULL;

    if(codetype == LUA_TSTRING)
        pcode = (const uint8_t*)lua_tostring(l, 2);
    else
        pcode = (const uint8_t*)lua_touserdata(l, 2);

    size_t count = 0;

    if(argc >= 5)
    {
        EXPECT_TYPE(LUA_TNUMBER, 5);
        count = lua_tointeger(l, 5);
    }

    cs_insn* insn = NULL;
    size_t rescount = cs_disasm((csh)lua_tointeger(l, 1), pcode, lua_tointeger(l, 3), lua_tointeger(l, 4), count, &insn);

    lua_pushinteger(l, rescount);
    lua_pushcstruct(l, insn, &luacapstone_insnarray_metaIndex, "__insnarray");
    return 2;
}

static int luacapstone_disasmiter(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 2);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TUSERDATA, 2);

    CapstoneIterator* it = (CapstoneIterator*)lua_touserdata(l, 2);
    bool res = cs_disasm_iter((csh)lua_tointeger(l, 1), &(it->code), &(it->codesize), &(it->address), it->insn);
    lua_pushboolean(l, res);
    return 1;
}

static int luacapstone_groupname(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 2);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TNUMBER, 2);

    const char* name = cs_group_name((csh)lua_tointeger(l, 1), lua_tointeger(l, 2));
    lua_pushstring(l, name);
    return 1;
}

static int luacapstone_instructiongroup(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 3);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TUSERDATA, 2);
    EXPECT_TYPE(LUA_TNUMBER, 3);

    bool res = cs_insn_group((csh)lua_tointeger(l, 1), *((cs_insn**)lua_touserdata(l, 2)), lua_tointeger(l, 3));
    lua_pushboolean(l, res);
    return 1;
}

static int luacapstone_instructionname(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 2);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TNUMBER, 2);

    const char* name = cs_insn_name((csh)lua_tointeger(l, 1), lua_tointeger(l, 2));
    lua_pushstring(l, name);
    return 1;
}

static int luacapstone_operandindex(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 4);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TUSERDATA, 2);
    EXPECT_TYPE(LUA_TNUMBER, 3);
    EXPECT_TYPE(LUA_TNUMBER, 4);

    int idx = cs_op_index((csh)lua_tointeger(l, 1), *((cs_insn**)lua_touserdata(l, 2)), lua_tointeger(l, 3), lua_tointeger(l, 4));
    lua_pushinteger(l, idx + 1);
    return 1;
}

static int luacapstone_operandcount(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 3);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TUSERDATA, 2);
    EXPECT_TYPE(LUA_TNUMBER, 3);

    int count = cs_op_count((csh)lua_tointeger(l, 1), *((cs_insn**)lua_touserdata(l, 2)), lua_tointeger(l, 3));
    lua_pushinteger(l, count);
    return 1;
}

static int luacapstone_registername(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 2);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TNUMBER, 2);

    const char* name = cs_reg_name((csh)lua_tointeger(l, 1), lua_tointeger(l, 2));
    lua_pushstring(l, name);
    return 1;
}

static int luacapstone_registerread(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 3);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TUSERDATA, 2);
    EXPECT_TYPE(LUA_TNUMBER, 3);

    bool res = cs_reg_read((csh)lua_tointeger(l, 1), *((cs_insn**)lua_touserdata(l, 2)), lua_tointeger(l, 3));
    lua_pushboolean(l, res);
    return 1;
}

static int luacapstone_registerwrite(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 3);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TUSERDATA, 2);
    EXPECT_TYPE(LUA_TNUMBER, 3);

    bool res = cs_reg_write((csh)lua_tointeger(l, 1), *((cs_insn**)lua_touserdata(l, 2)), lua_tointeger(l, 3));
    lua_pushboolean(l, res);
    return 1;
}

static int luacapstone_createiterator(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 4);
    EXPECT_TYPE(LUA_TNUMBER, 1);
    EXPECT_TYPE(LUA_TNUMBER, 3);
    EXPECT_TYPE(LUA_TNUMBER, 4);

    int codetype = lua_type(l, 2);

    if((codetype != LUA_TUSERDATA) && (codetype != LUA_TLIGHTUSERDATA) && (codetype != LUA_TSTRING))
    {
        lua_pushfstring(l, "Expected userdata, lightuserdata or string types, %s given", lua_typename(l, codetype));
        lua_error(l);
        return 0;
    }

    const uint8_t* code = NULL;

    if(codetype == LUA_TSTRING)
        code = (const uint8_t*)lua_tostring(l, 2);
    else
        code = (const uint8_t*)lua_touserdata(l, 2);

    csh handle = (csh)lua_tointeger(l, 1);
    CapstoneIterator* it = (CapstoneIterator*)lua_newuserdata(l, sizeof(CapstoneIterator));

    it->insn = cs_malloc(handle);
    it->code = code;
    it->codesize = (uint64_t)lua_tointeger(l, 3);
    it->address = (uint64_t)lua_tointeger(l, 4);
    it->__codetype = codetype; /* We need to know the original type */

    if(luaL_newmetatable(l, "__capstone_iterator"))
    {
        lua_pushcfunction(l, &luacapstone_iterator_metaIndex);
        lua_setfield(l, -2, "__index");
    }

    lua_setmetatable(l, -2);
    return 1;
}

static int luacapstone_freeiterator(lua_State* l)
{
    int argc = lua_gettop(l);

    EXPECT_ARGC(argc, 1);
    EXPECT_TYPE(LUA_TUSERDATA, 1);

    CapstoneIterator* it = (CapstoneIterator*)lua_touserdata(l, 2);
    cs_free(it->insn, 1);
    return 0;
}

static const luaL_Reg luacapstone_lib[] = { { "open", &luacapstone_open },
                                            { "close", &luacapstone_close },
                                            { "option", &luacapstone_option },
                                            { "malloc", &luacapstone_malloc },
                                            { "free", &luacapstone_free },
                                            { "disasm", &luacapstone_disasm },
                                            { "disasmiter", &luacapstone_disasmiter },
                                            { "groupname", &luacapstone_groupname },
                                            { "instructiongroup", &luacapstone_instructiongroup },
                                            { "instructionname", &luacapstone_instructionname },
                                            { "operandindex", &luacapstone_operandindex },
                                            { "operandcount", &luacapstone_operandcount },
                                            { "registername", &luacapstone_registername },
                                            { "registerread", &luacapstone_registerwrite },
                                            { "registerwrite", &luacapstone_registerread },
                                            { "createiterator", &luacapstone_createiterator },
                                            { "freeiterator", &luacapstone_freeiterator },
                                            { NULL, NULL } };

int luaopen_capstone(lua_State *l)
{
    luaL_register(l, LUACAPSTONE_LIBNAME, luacapstone_lib);

    set_capstone_const(l);
    set_arm64_const(l);
    set_arm_const(l);
    set_mips_const(l);
    set_ppc_const(l);
    set_sparc_const(l);
    set_sysz_const(l);
    set_x86_const(l);
    set_xcore_const(l);

    lua_pop(l, 1);
    return 1;
}
