#include "metadata.h"

/* Arm64 Metadata */
BeginStructureInfo(arm64_op_mem)
    StructureField(arm64_op_mem, unsigned int, base),
    StructureField(arm64_op_mem, unsigned int, index),
    StructureField(arm64_op_mem, int32_t, disp)
EndStructureInfo(arm64_op_mem)

BeginStructureInfo(cs_arm64_op)
    StructureField(cs_arm64_op, int, vector_index),
    StructureField(cs_arm64_op, arm64_vas, vas),
    StructureField(cs_arm64_op, arm64_vess, vess),
    StructureField(cs_arm64_op, arm64_shifter, shift.type),
    StructureField(cs_arm64_op, unsigned int, shift.value),
    StructureField(cs_arm64_op, unsigned int, reg),
    StructureField(cs_arm64_op, int64_t, imm),
    StructureField(cs_arm64_op, arm64_op_mem, mem),
    StructureField(cs_arm64_op, arm64_pstate, pstate),
    StructureField(cs_arm64_op, unsigned int, sys),
    StructureField(cs_arm64_op, arm64_prefetch_op, prefetch),
    StructureField(cs_arm64_op, arm64_barrier_op, barrier)
EndStructureInfo(cs_arm64_op)

BeginStructureInfo(cs_arm64)
    StructureField(cs_arm64, arm64_cc, cc),
    StructureField(cs_arm64, bool, update_flags),
    StructureField(cs_arm64, bool, writeback),
    StructureField(cs_arm64, uint8_t, op_count),
    StructureField(cs_arm64, cs_arm_op[8], operands)
EndStructureInfo(cs_arm64)

/* ARM Metadata */
BeginStructureInfo(arm_op_mem)
    StructureField(arm_op_mem, unsigned int, base),
    StructureField(arm_op_mem, unsigned int, index),
    StructureField(arm_op_mem, int, scale),
    StructureField(arm_op_mem, int, disp)
EndStructureInfo(arm_op_mem)

BeginStructureInfo(cs_arm_op)
    StructureField(cs_arm_op, int, vector_index),
    StructureField(cs_arm_op, arm_shifter, shift.type),
    StructureField(cs_arm_op, unsigned int, shift.value),
    StructureField(cs_arm_op, arm_op_type, type),
    StructureField(cs_arm_op, unsigned int, reg),
    StructureField(cs_arm_op, int32_t, imm),
    StructureField(cs_arm_op, double, fp),
    StructureField(cs_arm_op, arm_op_mem, mem),
    StructureField(cs_arm_op, arm_setend_type, setend),
    StructureField(cs_arm_op, bool, subtracted)
EndStructureInfo(cs_arm_op)

BeginStructureInfo(cs_arm)
    StructureField(cs_arm, bool, usermode),
    StructureField(cs_arm, int, vector_size),
    StructureField(cs_arm, arm_vectordata_type, vector_data),
    StructureField(cs_arm, arm_cpsmode_type, cps_mode),
    StructureField(cs_arm, arm_cpsflag_type, cps_flag),
    StructureField(cs_arm, arm_cc, cc),
    StructureField(cs_arm, bool, update_flags),
    StructureField(cs_arm, bool, writeback),
    StructureField(cs_arm, arm_mem_barrier, mem_barrier),
    StructureField(cs_arm, uint8_t, op_count),
    StructureField(cs_arm, cs_arm_op[36], operands),
EndStructureInfo(cs_arm)

/* Mips Metadata */
BeginStructureInfo(mips_op_mem)
    StructureField(mips_op_mem, unsigned int, base),
    StructureField(mips_op_mem, int64_t, disp)
EndStructureInfo(mips_op_mem)

BeginStructureInfo(cs_mips_op)
    StructureField(cs_mips_op, mips_op_type, type),
    StructureField(cs_mips_op, unsigned int, reg),
    StructureField(cs_mips_op, int64_t, imm),
    StructureField(cs_mips_op, mips_op_mem, mem)
EndStructureInfo(cs_mips_op)

BeginStructureInfo(cs_mips)
    StructureField(cs_mips, uint8_t, op_count),
    StructureField(cs_mips, cs_mips_op[8], operands)
EndStructureInfo(cs_mips)

/* PPC Metadata */
BeginStructureInfo(ppc_op_crx)
    StructureField(ppc_op_crx, unsigned int, scale),
    StructureField(ppc_op_crx, unsigned int, reg),
    StructureField(ppc_op_crx, ppc_bc, cond)
EndStructureInfo(ppc_op_crx)

BeginStructureInfo(ppc_op_mem)
    StructureField(ppc_op_mem, unsigned int, base),
    StructureField(ppc_op_mem, int32_t, disp)
EndStructureInfo(ppc_op_mem)

BeginStructureInfo(cs_ppc_op)
    StructureField(cs_ppc_op, ppc_op_type, type),
    StructureField(cs_ppc_op, unsigned int, reg),
    StructureField(cs_ppc_op, int32_t, imm),
    StructureField(cs_ppc_op, ppc_op_mem, mem),
    StructureField(cs_ppc_op, ppc_op_crx, crx)
EndStructureInfo(cs_ppc_op)

BeginStructureInfo(cs_ppc)
    StructureField(cs_ppc, ppc_bc, bc),
    StructureField(cs_ppc, ppc_bh, bh),
    StructureField(cs_ppc, bool, update_cr0),
    StructureField(cs_ppc, uint8_t, op_count),
    StructureField(cs_ppc, cs_ppc_op[8], operands)
EndStructureInfo(cs_ppc)

/* SPARC Metadata */
BeginStructureInfo(sparc_op_mem)
    StructureField(sparc_op_mem, uint8_t, base),
    StructureField(sparc_op_mem, uint8_t, index),
    StructureField(sparc_op_mem, int32_t, disp)
EndStructureInfo(sparc_op_mem)

BeginStructureInfo(cs_sparc_op)
    StructureField(cs_sparc_op, sparc_op_type, type),
    StructureField(cs_sparc_op, unsigned int, reg),
    StructureField(cs_sparc_op, int32_t, imm),
    StructureField(cs_sparc_op, sparc_op_mem, mem)
EndStructureInfo(cs_sparc_op)

BeginStructureInfo(cs_sparc)
    StructureField(cs_sparc, sparc_cc, cc),
    StructureField(cs_sparc, sparc_hint, hint),
    StructureField(cs_sparc, uint8_t, op_count),
    StructureField(cs_sparc, cs_sparc_op[4], operands)
EndStructureInfo(cs_sparc)

/* SysZ Metadata */
BeginStructureInfo(sysz_op_mem)
    StructureField(sysz_op_mem, uint8_t, base),
    StructureField(sysz_op_mem, uint8_t, index),
    StructureField(sysz_op_mem, uint64_t, length),
    StructureField(sysz_op_mem, int64_t, disp)
EndStructureInfo(sysz_op_mem)

BeginStructureInfo(cs_sysz_op)
    StructureField(cs_sysz_op, sysz_op_type, type),
    StructureField(cs_sysz_op, unsigned int, reg),
    StructureField(cs_sysz_op, int64_t, imm),
    StructureField(cs_sysz_op, sysz_op_mem, mem)
EndStructureInfo(cs_sysz_op)

BeginStructureInfo(cs_sysz)
    StructureField(cs_sysz, sysz_cc, cc),
    StructureField(cs_sysz, uint8_t, op_count),
    StructureField(cs_sysz, cs_sysz_op[6], operands)
EndStructureInfo(cs_sysz)

/* x86 Metadata */
BeginStructureInfo(x86_op_mem)
    StructureField(x86_op_mem, unsigned int, segment),
    StructureField(x86_op_mem, unsigned int, base),
    StructureField(x86_op_mem, unsigned int, index),
    StructureField(x86_op_mem, int, scale),
    StructureField(x86_op_mem, int64_t, disp)
EndStructureInfo(x86_op_mem)

BeginStructureInfo(cs_x86_op)
    StructureField(cs_x86_op, x86_op_type, type),
    StructureField(cs_x86_op, unsigned int, reg),
    StructureField(cs_x86_op, int64_t, imm),
    StructureField(cs_x86_op, double, fp),
    StructureField(cs_x86_op, x86_op_mem, mem),
    StructureField(cs_x86_op, uint8_t, size),
    StructureField(cs_x86_op, x86_avx_bcast, avx_bcast),
    StructureField(cs_x86_op, bool, avx_zero_opmask)
EndStructureInfo(cs_x86_op)

BeginStructureInfo(cs_x86)
    StructureField(cs_x86, uint8_t[4], prefix),
    StructureField(cs_x86, uint8_t[4], opcode),
    StructureField(cs_x86, uint8_t, rex),
    StructureField(cs_x86, uint8_t, addr_size),
    StructureField(cs_x86, uint8_t, modrm),
    StructureField(cs_x86, uint8_t, sib),
    StructureField(cs_x86, int32_t, disp),
    StructureField(cs_x86, x86_reg, sib_index),
    StructureField(cs_x86, int8_t, sib_scale),
    StructureField(cs_x86, x86_reg, sib_base),
    StructureField(cs_x86, x86_sse_cc, sse_cc),
    StructureField(cs_x86, x86_avx_cc, sse_cc),
    StructureField(cs_x86, bool, avx_sae),
    StructureField(cs_x86, x86_avx_rm, avx_rm),
    StructureField(cs_x86, uint8_t, op_count),
    StructureField(cs_x86, cs_x86_op[8], operands)
EndStructureInfo(cs_x86)

/* XCore Metadata */
BeginStructureInfo(xcore_op_mem)
    StructureField(xcore_op_mem, uint8_t, base),
    StructureField(xcore_op_mem, uint8_t, index),
    StructureField(xcore_op_mem, int32_t, disp),
    StructureField(xcore_op_mem, int, direct)
EndStructureInfo(xcore_op_mem)

BeginStructureInfo(cs_xcore_op)
    StructureField(cs_xcore_op, xcore_op_type, type),
    StructureField(cs_xcore_op, unsigned int, reg),
    StructureField(cs_xcore_op, int32_t, imm),
    StructureField(cs_xcore_op, xcore_op_mem, mem)
EndStructureInfo(cs_xcore_op)

BeginStructureInfo(cs_xcore)
    StructureField(cs_xcore, uint8_t, op_count),
    StructureField(cs_xcore, cs_xcore_op[8], operands)
EndStructureInfo(cs_xcore)

/* Capstone Metadata */
BeginStructureInfo(cs_detail)
    StructureField(cs_detail, uint8_t[12], regs_read),
    StructureField(cs_detail, uint8_t, regs_read_count),
    StructureField(cs_detail, uint8_t[20], regs_write),
    StructureField(cs_detail, uint8_t, regs_write_count),
    StructureField(cs_detail, uint8_t[8], groups),
    StructureField(cs_detail, uint8_t, groups_count),
    StructureField(cs_detail, cs_x86, x86),
    StructureField(cs_detail, cs_arm64, arm64),
    StructureField(cs_detail, cs_arm, arm),
    StructureField(cs_detail, cs_mips, mips),
    StructureField(cs_detail, cs_ppc, ppc),
    StructureField(cs_detail, cs_sparc, sparc),
    StructureField(cs_detail, cs_sysz, sysz),
    StructureField(cs_detail, cs_xcore, xcore)
EndStructureInfo(cs_detail)

BeginStructureInfo(cs_insn)
    StructureField(cs_insn, unsigned int, id),
    StructureField(cs_insn, uint64_t, address),
    StructureField(cs_insn, uint16_t, size),
    StructureField(cs_insn, uint8_t[16], bytes),
    StructureField(cs_insn, char[32], mnemonic),
    StructureField(cs_insn, char[160], op_str),
    StructureField(cs_insn, cs_detail*, detail)
EndStructureInfo(cs_insn)
