#include "capstone_const.h"

void set_capstone_const(lua_State *l)
{
    /* All type of errors encountered by Capstone API. */

    lua_pushinteger(l, CS_ERR_OK);
    lua_setfield(l, -2, "CS_ERR_OK");
    lua_pushinteger(l, CS_ERR_MEM);
    lua_setfield(l, -2, "CS_ERR_MEM");
    lua_pushinteger(l, CS_ERR_ARCH);
    lua_setfield(l, -2, "CS_ERR_ARCH");
    lua_pushinteger(l, CS_ERR_HANDLE);
    lua_setfield(l, -2, "CS_ERR_HANDLE");
    lua_pushinteger(l,CS_ERR_CSH);
    lua_setfield(l, -2, "CS_ERR_CSH");
    lua_pushinteger(l, CS_ERR_MODE);
    lua_setfield(l, -2, "CS_ERR_MODE");
    lua_pushinteger(l, CS_ERR_OPTION);
    lua_setfield(l, -2, "CS_ERR_OPTION");
    lua_pushinteger(l, CS_ERR_DETAIL);
    lua_setfield(l, -2, "CS_ERR_DETAIL");
    lua_pushinteger(l, CS_ERR_MEMSETUP);
    lua_setfield(l, -2, "CS_ERR_MEMSETUP");
    lua_pushinteger(l, CS_ERR_VERSION);
    lua_setfield(l, -2, "CS_ERR_VERSION");
    lua_pushinteger(l, CS_ERR_DIET);
    lua_setfield(l, -2, "CS_ERR_DIET");
    lua_pushinteger(l, CS_ERR_SKIPDATA);
    lua_setfield(l, -2, "CS_ERR_SKIPDATA");
    lua_pushinteger(l, CS_ERR_X86_ATT);
    lua_setfield(l, -2, "CS_ERR_X86_ATT");
    lua_pushinteger(l, CS_ERR_X86_INTEL);
    lua_setfield(l, -2, "CS_ERR_X86_INTEL");

    /* Architecture type */

    lua_pushinteger(l, CS_ARCH_ARM);
    lua_setfield(l, -2, "CS_ARCH_ARM");
    lua_pushinteger(l, CS_ARCH_ARM64);
    lua_setfield(l, -2, "CS_ARCH_ARM64");
    lua_pushinteger(l, CS_ARCH_MIPS);
    lua_setfield(l, -2, "CS_ARCH_MIPS");
    lua_pushinteger(l, CS_ARCH_X86);
    lua_setfield(l, -2, "CS_ARCH_X86");
    lua_pushinteger(l, CS_ARCH_PPC);
    lua_setfield(l, -2, "CS_ARCH_PPC");
    lua_pushinteger(l, CS_ARCH_SPARC);
    lua_setfield(l, -2, "CS_ARCH_SPARC");
    lua_pushinteger(l, CS_ARCH_SYSZ);
    lua_setfield(l, -2, "CS_ARCH_SYSZ");
    lua_pushinteger(l, CS_ARCH_XCORE);
    lua_setfield(l, -2, "CS_ARCH_XCORE");
    lua_pushinteger(l, CS_ARCH_MAX);
    lua_setfield(l, -2, "CS_ARCH_MAX");
    lua_pushinteger(l, CS_ARCH_ALL);
    lua_setfield(l, -2, "CS_ARCH_ALL");

    /* Mode type */

    lua_pushinteger(l, CS_MODE_LITTLE_ENDIAN);
    lua_setfield(l, -2, "CS_MODE_LITTLE_ENDIAN");
    lua_pushinteger(l, CS_MODE_ARM);
    lua_setfield(l, -2, "CS_MODE_ARM");
    lua_pushinteger(l, CS_MODE_16);
    lua_setfield(l, -2, "CS_MODE_16");
    lua_pushinteger(l, CS_MODE_32);
    lua_setfield(l, -2, "CS_MODE_32");
    lua_pushinteger(l, CS_MODE_64);
    lua_setfield(l, -2, "CS_MODE_64");
    lua_pushinteger(l, CS_MODE_THUMB);
    lua_setfield(l, -2, "CS_MODE_THUMB");
    lua_pushinteger(l, CS_MODE_MCLASS);
    lua_setfield(l, -2, "CS_MODE_MCLASS");
    lua_pushinteger(l, CS_MODE_V8);
    lua_setfield(l, -2, "CS_MODE_V8");
    lua_pushinteger(l, CS_MODE_MICRO);
    lua_setfield(l, -2, "CS_MODE_MICRO");
    lua_pushinteger(l, CS_MODE_MIPS3);
    lua_setfield(l, -2, "CS_MODE_MIPS3");
    lua_pushinteger(l, CS_MODE_MIPS32R6);
    lua_setfield(l, -2, "CS_MODE_MIPS32R6");
    lua_pushinteger(l, CS_MODE_MIPSGP64);
    lua_setfield(l, -2, "CS_MODE_MIPSGP64");
    lua_pushinteger(l, CS_MODE_V9);
    lua_setfield(l, -2, "CS_MODE_V9");
    lua_pushinteger(l, CS_MODE_BIG_ENDIAN);
    lua_setfield(l, -2, "CS_MODE_BIG_ENDIAN");
    lua_pushinteger(l, CS_MODE_MIPS32);
    lua_setfield(l, -2, "CS_MODE_MIPS32");
    lua_pushinteger(l, CS_MODE_MIPS64);
    lua_setfield(l, -2, "CS_MODE_MIPS64");

    /* Runtime option for the disassembled engine */

    lua_pushinteger(l, CS_OPT_SYNTAX);
    lua_setfield(l, -2, "CS_OPT_SYNTAX");
    lua_pushinteger(l, CS_OPT_DETAIL);
    lua_setfield(l, -2, "CS_OPT_DETAIL");
    lua_pushinteger(l, CS_OPT_MODE);
    lua_setfield(l, -2, "CS_OPT_MODE");
    lua_pushinteger(l, CS_OPT_MEM);
    lua_setfield(l, -2, "CS_OPT_MEM");
    lua_pushinteger(l, CS_OPT_SKIPDATA);
    lua_setfield(l, -2, "CS_OPT_SKIPDATA");
    lua_pushinteger(l, CS_OPT_SKIPDATA_SETUP);
    lua_setfield(l, -2, "CS_OPT_SKIPDATA_SETUP");

    /* Runtime option value (associated with option type above) */

    lua_pushinteger(l, CS_OPT_OFF);
    lua_setfield(l, -2, "CS_OPT_OFF");
    lua_pushinteger(l, CS_OPT_ON);
    lua_setfield(l, -2, "CS_OPT_ON");
    lua_pushinteger(l, CS_OPT_SYNTAX_DEFAULT);
    lua_setfield(l, -2, "CS_OPT_SYNTAX_DEFAULT");
    lua_pushinteger(l, CS_OPT_SYNTAX_INTEL);
    lua_setfield(l, -2, "CS_OPT_SYNTAX_INTEL");
    lua_pushinteger(l, CS_OPT_SYNTAX_ATT);
    lua_setfield(l, -2, "CS_OPT_SYNTAX_ATT");
    lua_pushinteger(l, CS_OPT_SYNTAX_NOREGNAME);
    lua_setfield(l, -2, "CS_OPT_SYNTAX_NOREGNAME");

    /* Common instruction operand types - to be consistent across all architectures. */

    lua_pushinteger(l, CS_OP_INVALID);
    lua_setfield(l, -2, "CS_OP_INVALID");
    lua_pushinteger(l, CS_OP_REG);
    lua_setfield(l, -2, "CS_OP_REG");
    lua_pushinteger(l, CS_OP_IMM);
    lua_setfield(l, -2, "CS_OP_IMM");
    lua_pushinteger(l, CS_OP_MEM);
    lua_setfield(l, -2, "CS_OP_MEM");
    lua_pushinteger(l, CS_OP_FP);
    lua_setfield(l, -2, "CS_OP_FP");

    /* Common instruction groups - to be consistent across all architectures. */

    lua_pushinteger(l, CS_GRP_INVALID);
    lua_setfield(l, -2, "CS_GRP_INVALID");
    lua_pushinteger(l, CS_GRP_JUMP);
    lua_setfield(l, -2, "CS_GRP_JUMP");
    lua_pushinteger(l, CS_GRP_CALL);
    lua_setfield(l, -2, "CS_GRP_CALL");
    lua_pushinteger(l, CS_GRP_RET);
    lua_setfield(l, -2, "CS_GRP_RET");
    lua_pushinteger(l, CS_GRP_INT);
    lua_setfield(l, -2, "CS_GRP_INT");
    lua_pushinteger(l, CS_GRP_IRET);
    lua_setfield(l, -2, "CS_GRP_IRET");
}
