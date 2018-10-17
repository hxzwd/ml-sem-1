
#pragma once


#include "disasm_main.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <iostream>
#include <string>
#include <vector>


extern int64_t PC_REG;


std::string decode(InstructionDisasm instruction);
std::vector<uint32_t> get_used_regs(InstructionDisasm instruction); 


char * reg(RegisterIndex idx);


char * op_sll(InstructionDisasm instruction);
char * op_srl(InstructionDisasm instruction);
char * op_sra(InstructionDisasm instruction);
char * op_sllv(InstructionDisasm instruction);
char * op_srlv(InstructionDisasm instruction);
char * op_srav(InstructionDisasm instruction);
char * op_jr(InstructionDisasm instruction);
char * op_jalr(InstructionDisasm instruction);
char * op_syscall(InstructionDisasm instruction);
char * op_break(InstructionDisasm instruction);
char * op_mfhi(InstructionDisasm instruction);
char * op_mthi(InstructionDisasm instruction);
char * op_mflo(InstructionDisasm instruction);
char * op_mtlo(InstructionDisasm instruction);
char * op_mult(InstructionDisasm instruction);
char * op_multu(InstructionDisasm instruction);
char * op_div(InstructionDisasm instruction);
char * op_divu(InstructionDisasm instruction);
char * op_add(InstructionDisasm instruction);
char * op_addu(InstructionDisasm instruction);
char * op_sub(InstructionDisasm instruction);
char * op_subu(InstructionDisasm instruction);
char * op_and(InstructionDisasm instruction);
char * op_or(InstructionDisasm instruction);
char * op_xor(InstructionDisasm instruction);
char * op_nor(InstructionDisasm instruction);
char * op_slt(InstructionDisasm instruction);
char * op_sltu(InstructionDisasm instruction);
char * op_bxx(InstructionDisasm instruction);
char * op_j(InstructionDisasm instruction);
char * op_jal(InstructionDisasm instruction);
char * op_beq(InstructionDisasm instruction);
char * op_bne(InstructionDisasm instruction);
char * op_blez(InstructionDisasm instruction);
char * op_bgtz(InstructionDisasm instruction);
char * op_addi(InstructionDisasm instruction);
char * op_addiu(InstructionDisasm instruction);
char * op_slti(InstructionDisasm instruction);
char * op_sltiu(InstructionDisasm instruction);
char * op_andi(InstructionDisasm instruction);
char * op_ori(InstructionDisasm instruction);
char * op_xori(InstructionDisasm instruction);
char * op_lui(InstructionDisasm instruction);
char * op_cop0(InstructionDisasm instruction);
char * op_cop1(InstructionDisasm instruction);
char * op_cop2(InstructionDisasm instruction);
char * op_cop3(InstructionDisasm instruction);
char * op_lb(InstructionDisasm instruction);
char * op_lh(InstructionDisasm instruction);
char * op_lwl(InstructionDisasm instruction);
char * op_lw(InstructionDisasm instruction);
char * op_lbu(InstructionDisasm instruction);
char * op_lhu(InstructionDisasm instruction);
char * op_lwr(InstructionDisasm instruction);
char * op_sb(InstructionDisasm instruction);
char * op_sh(InstructionDisasm instruction);
char * op_swl(InstructionDisasm instruction);
char * op_sw(InstructionDisasm instruction);
char * op_swr(InstructionDisasm instruction);
char * op_lwc0(InstructionDisasm instruction);
char * op_lwc1(InstructionDisasm instruction);
char * op_lwc2(InstructionDisasm instruction);
char * op_lwc3(InstructionDisasm instruction);
char * op_swc0(InstructionDisasm instruction);
char * op_swc1(InstructionDisasm instruction);
char * op_swc2(InstructionDisasm instruction);
char * op_swc3(InstructionDisasm instruction);


char * op_mfc0(InstructionDisasm instruction);
char * op_mtc0(InstructionDisasm instruction);
char * op_rfe(InstructionDisasm instruction);



/*

char * op_sll
char * op_srl
char * op_sra
char * op_sllv
char * op_srlv
char * op_srav
char * op_jr
char * op_jalr
char * op_syscall
char * op_break
char * op_mfhi
char * op_mthi
char * op_mflo
char * op_mtlo
char * op_mult
char * op_multu
char * op_div
char * op_divu
char * op_add
char * op_addu
char * op_sub
char * op_subu
char * op_and
char * op_or
char * op_xor
char * op_nor
char * op_slt
char * op_sltu
char * op_bxx
char * op_j
char * op_jal
char * op_beq
char * op_bne
char * op_blez
char * op_bgtz
char * op_addi
char * op_addiu
char * op_slti
char * op_sltiu
char * op_andi
char * op_ori
char * op_xori
char * op_lui
char * op_cop0
char * op_cop1
char * op_cop2
char * op_cop3
char * op_lb
char * op_lh
char * op_lwl
char * op_lw
char * op_lbu
char * op_lhu
char * op_lwr
char * op_sb
char * op_sh
char * op_swl
char * op_sw
char * op_swr
char * op_lwc0
char * op_lwc1
char * op_lwc2
char * op_lwc3
char * op_swc0
char * op_swc1
char * op_swc2
char * op_swc3

*/
