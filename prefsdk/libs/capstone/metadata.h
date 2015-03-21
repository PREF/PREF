#ifndef METADATA_H
#define METADATA_H

#include <capstone.h>
#include "reflection.h"

/* Arm64 Metadata */
DeclareStructureInfo(arm64_op_mem)
DeclareStructureInfo(cs_arm64_op)
DeclareStructureInfo(cs_arm64)

/* Arm Metadata */
DeclareStructureInfo(arm_op_mem)
DeclareStructureInfo(cs_arm_op)
DeclareStructureInfo(cs_arm)

/* Mips Metadata */
DeclareStructureInfo(mips_op_mem)
DeclareStructureInfo(cs_mips_op)
DeclareStructureInfo(cs_mips)

/* PPC Metaata */
DeclareStructureInfo(ppc_op_crx)
DeclareStructureInfo(ppc_op_mem)
DeclareStructureInfo(cs_ppc_op)
DeclareStructureInfo(cs_ppc)

/* SPARC Metadata */
DeclareStructureInfo(sparc_op_mem)
DeclareStructureInfo(cs_sparc_op)
DeclareStructureInfo(cs_sparc)

/* SyzZ Metadata */
DeclareStructureInfo(sysz_op_mem)
DeclareStructureInfo(cs_sysz_op)
DeclareStructureInfo(cs_sysz)

/* x86 Metadata */
DeclareStructureInfo(x86_op_mem)
DeclareStructureInfo(cs_x86_op)
DeclareStructureInfo(cs_x86)

/* XCore Metadata */
DeclareStructureInfo(xcore_op_mem)
DeclareStructureInfo(cs_xcore_op)
DeclareStructureInfo(cs_xcore)

/* Capstone Metadata */
DeclareStructureInfo(cs_detail)
DeclareStructureInfo(cs_insn)

#endif // METADATA_H
