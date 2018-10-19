#pragma once

#include "pscx_interconnect.h"
#include "pscx_instruction.h"

//#include "mips_disasm/disasm_op.h"

#include "string.h"
#include "stdio.h"
#include <iostream>
#include <vector>

// CPU state
struct Cpu
{
	// 0xbfc00000 PC reset value at the beginning of the BIOS
	Cpu(Interconnect inter) :
		m_pc(0xbfc00000),
		m_inter(inter)
	{
		// Reset registers values to 0xdeadbeef
		memset(m_regs, 0xdeadbeef, sizeof(uint32_t) * 32);

		// $zero is hardwired to 0
		m_regs[0] = 0x0;
	}

	// enum InstructionType
	// {
	// 	INSTRUCTION_TYPE_LUI,
	// 	INSTRUCTION_TYPE_ORI,
	// 	INSTRUCTION_TYPE_SW,
	// 	INSTRUCTION_TYPE_UNKNOWN
	// };

	enum InstructionType
	{
		INSTRUCTION_TYPE_LUI,
		INSTRUCTION_TYPE_ORI,
		INSTRUCTION_TYPE_SW,
		INSTRUCTION_TYPE_SLL,
		INSTRUCTION_TYPE_ADDIU,
		INSTRUCTION_TYPE_J,
		INSTRUCTION_TYPE_OR,
		INSTRUCTION_TYPE_COP0,
		INSTRUCTION_TYPE_BNE,
		INSTRUCTION_TYPE_ADDI,
		INSTRUCTION_TYPE_LW,
		INSTRUCTION_TYPE_SRL,
		INSTRUCTION_TYPE_SRA,
		INSTRUCTION_TYPE_SLLV,
		INSTRUCTION_TYPE_SRLV,
		INSTRUCTION_TYPE_SRAV,
		INSTRUCTION_TYPE_JR,
		INSTRUCTION_TYPE_JALR,
		INSTRUCTION_TYPE_SYSCALL,
		INSTRUCTION_TYPE_BREAK,
		INSTRUCTION_TYPE_MFHI,
		INSTRUCTION_TYPE_MTHI,
		INSTRUCTION_TYPE_MFLO,
		INSTRUCTION_TYPE_MTLO,
		INSTRUCTION_TYPE_MULT,
		INSTRUCTION_TYPE_MULTU,
		INSTRUCTION_TYPE_DIV,
		INSTRUCTION_TYPE_DIVU,
		INSTRUCTION_TYPE_ADD,
		INSTRUCTION_TYPE_ADDU,
		INSTRUCTION_TYPE_SUB,
		INSTRUCTION_TYPE_SUBU,
		INSTRUCTION_TYPE_AND,
		INSTRUCTION_TYPE_XOR,
		INSTRUCTION_TYPE_NOR,
		INSTRUCTION_TYPE_SLT,
		INSTRUCTION_TYPE_SLTU,
		INSTRUCTION_TYPE_BXX,
		INSTRUCTION_TYPE_JAL,
		INSTRUCTION_TYPE_BEQ,
		INSTRUCTION_TYPE_BLEZ,
		INSTRUCTION_TYPE_BGTZ,
		INSTRUCTION_TYPE_SLTI,
		INSTRUCTION_TYPE_SLTIU,
		INSTRUCTION_TYPE_ANDI,
		INSTRUCTION_TYPE_XORI,
		INSTRUCTION_TYPE_COP1,
		INSTRUCTION_TYPE_COP2,
		INSTRUCTION_TYPE_COP3,
		INSTRUCTION_TYPE_LB,
		INSTRUCTION_TYPE_LH,
		INSTRUCTION_TYPE_LWL,
		INSTRUCTION_TYPE_LBU,
		INSTRUCTION_TYPE_LHU,
		INSTRUCTION_TYPE_LWR,
		INSTRUCTION_TYPE_SB,
		INSTRUCTION_TYPE_SH,
		INSTRUCTION_TYPE_SWL,
		INSTRUCTION_TYPE_SWR,
		INSTRUCTION_TYPE_LWC0,
		INSTRUCTION_TYPE_LWC1,
		INSTRUCTION_TYPE_LWC2,
		INSTRUCTION_TYPE_LWC3,
		INSTRUCTION_TYPE_SWC0,
		INSTRUCTION_TYPE_SWC1,
		INSTRUCTION_TYPE_SWC2,
		INSTRUCTION_TYPE_SWC3,
		INSTRUCTION_TYPE_UNKNOWN
	};

	std::vector<std::string> ins_type_str =
	{
		"INSTRUCTION_TYPE_LUI",\
		"INSTRUCTION_TYPE_ORI",\
		"INSTRUCTION_TYPE_SW",\
		"INSTRUCTION_TYPE_SLL",\
		"INSTRUCTION_TYPE_ADDIU",\
		"INSTRUCTION_TYPE_J",\
		"INSTRUCTION_TYPE_OR",\
		"INSTRUCTION_TYPE_COP0",\
		"INSTRUCTION_TYPE_BNE",\
		"INSTRUCTION_TYPE_ADDI",\
		"INSTRUCTION_TYPE_LW",\
		"INSTRUCTION_TYPE_UNKNOWN"
	};


	// Special purpose registers
	uint32_t m_pc; // program counter register

	// General purpose registers
	// m_regs[0]                     $zero      Always zero
	// m_regs[1]                     $at        Assembler temporary
	// m_regs[2],      m_regs[3]     $v0,v1     Function return values
	// m_regs[4]  ...  m_regs[7]     $a0...$a3  Function arguments
	// m_regs[8]  ...  m_regs[15]    $t0...$t7  Temporary registers
	// m_regs[16] ...  m_regs[23]    $s0...$s7  Saved registers
	// m_regs[24],     m_regs[25]    $t8,$t9    Temporary registers
	// m_regs[26],     m_regs[27]    $k0,$k1    Kernel reserved registers
	// m_regs[28]                    $gp        Global pointer
	// m_regs[29]                    $sp        Stack pointer
	// m_regs[30]                    $fp        Frame pointer
	// m_regs[31]                    $ra        Function return address
	uint32_t m_regs[32];

	// Memory interface
	Interconnect m_inter;

	// Load 32 bit value from the interconnect
	uint32_t load32(uint32_t addr);

	// Store 32 bit value into the memory
	void store32(uint32_t addr, uint32_t value);

	InstructionType decodeAndExecute(Instruction instruction);
	InstructionType runNextInstruction();

	// Load Upper Immediate
	InstructionType opcodeLUI(Instruction instruction);

	// Bitwise Or Immediate
	InstructionType opcodeORI(Instruction instruction);

	// Store Word
	InstructionType opcodeSW(Instruction instruction);

	uint32_t getRegisterValue(uint32_t index) const;
	void setRegisterValue(uint32_t index, uint32_t value);

	int m_debug_size;
	std::vector<std::string> m_debug_info;
	std::vector<uint32_t> dumpCpuRegs();
	void printDebugInfo(std::string message);
	void printDebugInfo(std::string message, std::string prefix);
	std::string sprintDebugInfo0(std::string message);
	std::string sprintDebugInfo1(std::string message, std::string prefix);
	void decodeAndExecuteDebug(std::vector<uint32_t> old_regs, Instruction instruction, InstructionType InstructionType);
	std::string sdecodeAndExecuteDebug(std::vector<uint32_t> old_regs, Instruction instruction, InstructionType InstructionType);
	void storeDebugInfo(std::string debug_info);


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


	uint32_t m_sr;



	InstructionType opcodeUNKNOWN(Instruction instruction);
	InstructionType opcodeSLL(Instruction instruction);
	InstructionType opcodeADDIU(Instruction instruction);
	InstructionType opcodeJ(Instruction instruction);
	InstructionType opcodeOR(Instruction instruction);
	InstructionType opcodeCOP0(Instruction instruction);
	void opcodeMTC0(Instruction instruction);
	InstructionType opcodeBNE(Instruction instruction);
	InstructionType opcodeADDI(Instruction instruction);
	InstructionType opcodeLW(Instruction instruction);



	//InstructionType opcodeSLL(Instruction instruction);
	InstructionType opcodeSRL(Instruction instruction);
	InstructionType opcodeSRA(Instruction instruction);
	InstructionType opcodeSLLV(Instruction instruction);
	InstructionType opcodeSRLV(Instruction instruction);
	InstructionType opcodeSRAV(Instruction instruction);
	InstructionType opcodeJR(Instruction instruction);
	InstructionType opcodeJALR(Instruction instruction);
	InstructionType opcodeSYSCALL(Instruction instruction);
	InstructionType opcodeBREAK(Instruction instruction);
	InstructionType opcodeMFHI(Instruction instruction);
	InstructionType opcodeMTHI(Instruction instruction);
	InstructionType opcodeMFLO(Instruction instruction);
	InstructionType opcodeMTLO(Instruction instruction);
	InstructionType opcodeMULT(Instruction instruction);
	InstructionType opcodeMULTU(Instruction instruction);
	InstructionType opcodeDIV(Instruction instruction);
	InstructionType opcodeDIVU(Instruction instruction);
	InstructionType opcodeADD(Instruction instruction);
	InstructionType opcodeADDU(Instruction instruction);
	InstructionType opcodeSUB(Instruction instruction);
	InstructionType opcodeSUBU(Instruction instruction);
	InstructionType opcodeAND(Instruction instruction);
	//InstructionType opcodeOR(Instruction instruction);
	InstructionType opcodeXOR(Instruction instruction);
	InstructionType opcodeNOR(Instruction instruction);
	InstructionType opcodeSLT(Instruction instruction);
	InstructionType opcodeSLTU(Instruction instruction);


	InstructionType opcodeBXX(Instruction instruction);
//	InstructionType opcodeJ(Instruction instruction);
	InstructionType opcodeJAL(Instruction instruction);
	InstructionType opcodeBEQ(Instruction instruction);
//	InstructionType opcodeBNE(Instruction instruction);
	InstructionType opcodeBLEZ(Instruction instruction);
	InstructionType opcodeBGTZ(Instruction instruction);
//	InstructionType opcodeADDI(Instruction instruction);
//	InstructionType opcodeADDIU(Instruction instruction);
	InstructionType opcodeSLTI(Instruction instruction);
	InstructionType opcodeSLTIU(Instruction instruction);
	InstructionType opcodeANDI(Instruction instruction);
//	InstructionType opcodeORI(Instruction instruction);
	InstructionType opcodeXORI(Instruction instruction);
//	InstructionType opcodeLUI(Instruction instruction);
//	InstructionType opcodeCOP0(Instruction instruction);
	InstructionType opcodeCOP1(Instruction instruction);
	InstructionType opcodeCOP2(Instruction instruction);
	InstructionType opcodeCOP3(Instruction instruction);
	InstructionType opcodeLB(Instruction instruction);
	InstructionType opcodeLH(Instruction instruction);
	InstructionType opcodeLWL(Instruction instruction);
//	InstructionType opcodeLW(Instruction instruction);
	InstructionType opcodeLBU(Instruction instruction);
	InstructionType opcodeLHU(Instruction instruction);
	InstructionType opcodeLWR(Instruction instruction);
	InstructionType opcodeSB(Instruction instruction);
	InstructionType opcodeSH(Instruction instruction);
	InstructionType opcodeSWL(Instruction instruction);
//	InstructionType opcodeSW(Instruction instruction);
	InstructionType opcodeSWR(Instruction instruction);
	InstructionType opcodeLWC0(Instruction instruction);
	InstructionType opcodeLWC1(Instruction instruction);
	InstructionType opcodeLWC2(Instruction instruction);
	InstructionType opcodeLWC3(Instruction instruction);
	InstructionType opcodeSWC0(Instruction instruction);
	InstructionType opcodeSWC1(Instruction instruction);
	InstructionType opcodeSWC2(Instruction instruction);
	InstructionType opcodeSWC3(Instruction instruction);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


};
