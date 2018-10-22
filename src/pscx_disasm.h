#pragma once

#include "pscx_memory.h"
#include "pscx_instruction.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <map>


#define format_string(F, ...)\
			[]() -> std::string\
			{\
				memset(m_format_buffer, 0, 2048);\
				sprintf(m_format_buffer, F, ##__VA_ARGS__);\
				return std::string(m_format_buffer);\
			}()

#define HEXW(x)\
			"0x" << std::setfill('0') << std::uppercase << std::setw(8) << std::hex << x
#define HEXH(x)\
			"0x" << std::setfill('0') << std::uppercase << std::setw(4) << std::hex << x
#define HEXB(x)\
			"0x" << std::setfill('0') << std::uppercase << std::setw(2) << std::hex << x


using namespace pscx_memory;


struct MipsDisasm 
{

	#ifdef format_string
	char m_format_buffer[2048];
	#endif

 	MipsDisasm(int64_t pc_reg = -1, bool flag_enabled = false, std::string format = std::string("default"), uint32_t index = 0) :
 		PC_REG(pc_reg),
 		is_enabled(flag_enabled),
 		m_format(format),
 		m_index(index)
 	{
 	}

 	int64_t PC_REG;
 	bool is_enabled;
 	std::string m_format;
 	uint32_t m_index;

	struct InstructionDisasm
	{

		InstructionDisasm(uint32_t data) :
			m_data(data)
		{
		}

		uint32_t m_data;

		uint32_t function();
		uint32_t subfunction();
		uint32_t cop_opcode();
		RegisterIndex s();
		RegisterIndex t();
		RegisterIndex d();
		uint32_t imm();
		uint32_t imm_se();
		uint32_t shift();
		uint32_t imm_jump();


		std::string i2s();

	};

	bool is_on() { return is_enabled; }
	void set_state(bool state) { is_enabled = state; }
	void set_format(std::string format) { m_format = format; }
	void set_index(uint32_t index) { m_index = index; }
	void on() { is_enabled = true; }
	void off() { is_enabled = false; }
	void next() { m_index++; if(PC_REG >= 0) PC_REG+=4;  }
	uint32_t get_pc() { return PC_REG; }

	uint32_t u32_from_str_prefix(char *str);
	int disasm_main(uint32_t ps_reg_value);

	std::string decode_(Instruction instruction);
	std::string decode(Instruction instruction) { std::string res = decode_(instruction); next(); return res; }
	std::vector<uint32_t> get_used_regs(InstructionDisasm instruction);
	std::vector<uint32_t> touched_regs(Instruction instruction);

	char * reg(RegisterIndex idx);

	std::map<std::string, std::string> get_instruction(Instruction instruction);
	void print_decode(Instruction instruction);


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

};