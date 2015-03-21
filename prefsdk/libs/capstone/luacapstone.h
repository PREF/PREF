#ifndef LUACAPSTONE_H
#define LUACAPSTONE_H

#include <lua.hpp>
#include <capstone.h>
#include "tables/capstone_const.h"
#include "tables/arm64_const.h"
#include "tables/arm_const.h"
#include "tables/mips_const.h"
#include "tables/ppc_const.h"
#include "tables/sparc_const.h"
#include "tables/sysz_const.h"
#include "tables/x86_const.h"
#include "tables/xcore_const.h"
#include "luacapstonemacros.h"
#include "metadata.h"

typedef struct CapstoneIterator
{
    cs_insn* insn;
    const uint8_t* code;
    uint64_t codesize;
    uint64_t address;

    /* Special Type */
    int __codetype;
} CapstoneIterator;

DeclareStructureInfo(CapstoneIterator)

int luaopen_capstone(lua_State* l);

#endif // LUACAPSTONE_H

