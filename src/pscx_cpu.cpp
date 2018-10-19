#include "pscx_cpu.h"

#include "disasm/disasm_op.h"

#include <iostream>
#include <cassert>
#include "assert.h"
#include "stdint.h"

#define assert(message, ...) do { \
    if(!(__VA_ARGS__)) { \
        /*error code*/ \
    } \
} while(0)

#define D_CPU_DEBUG

std::vector<uint32_t> Cpu::dumpCpuRegs()
{
	std::vector<uint32_t> dump_regs;
	for(int i = 0; i < 32; i++)
	{
		dump_regs.push_back(m_regs[i]);
	}
	dump_regs.push_back(m_pc);

	return dump_regs;
}


uint32_t Cpu::load32(uint32_t addr)
{
	return m_inter.load32(addr);
}

void Cpu::store32(uint32_t addr, uint32_t value)
{
	m_inter.store32(addr, value);
}

Cpu::InstructionType Cpu::decodeAndExecute(Instruction instruction)
{
	InstructionType instructionType = INSTRUCTION_TYPE_UNKNOWN;

	std::vector<uint32_t> old_regs = dumpCpuRegs();

	switch (instruction.getInstructionCode())
	{
			case 0b000000:
				switch(instruction.subfunction())
				{
/*SLL*/				case 0b000000: instructionType = opcodeSLL(instruction); break;
/*SRL*/				case 0b000010: instructionType = opcodeSRL(instruction); break;
/*SRA*/				case 0b000011: instructionType = opcodeSRA(instruction); break;
/*SLLV*/			case 0b000100: instructionType = opcodeSLLV(instruction); break;
/*SRLV*/			case 0b000110: instructionType = opcodeSRLV(instruction); break;
/*SRAV*/			case 0b000111: instructionType = opcodeSRAV(instruction); break;
/*JR*/				case 0b001000: instructionType = opcodeJR(instruction); break;
/*JALR*/			case 0b001001: instructionType = opcodeJALR(instruction); break;
/*SYSCALL*/			case 0b001100: instructionType = opcodeSYSCALL(instruction); break;
/*BREAK*/			case 0b001101: instructionType = opcodeBREAK(instruction); break;
/*MFHI*/			case 0b010000: instructionType = opcodeMFHI(instruction); break;
/*MTHI*/			case 0b010001: instructionType = opcodeMTHI(instruction); break;
/*MFLO*/			case 0b010010: instructionType = opcodeMFLO(instruction); break;
/*MTLO*/			case 0b010011: instructionType = opcodeMTLO(instruction); break;
/*MULT*/			case 0b011000: instructionType = opcodeMULT(instruction); break;
/*MULTU*/			case 0b011001: instructionType = opcodeMULTU(instruction); break;
/*DIV*/				case 0b011010: instructionType = opcodeDIV(instruction); break;
/*DIVU*/			case 0b011011: instructionType = opcodeDIVU(instruction); break;
/*ADD*/				case 0b100000: instructionType = opcodeADD(instruction); break;
/*ADDU*/			case 0b100001: instructionType = opcodeADDU(instruction); break;
/*SUB*/				case 0b100010: instructionType = opcodeSUB(instruction); break;
/*SUBU*/			case 0b100011: instructionType = opcodeSUBU(instruction); break;
/*AND*/				case 0b100100: instructionType = opcodeAND(instruction); break;
/*OR*/				case 0b100101: instructionType = opcodeOR(instruction); break;
/*XOR*/				case 0b100110: instructionType = opcodeXOR(instruction); break;
/*NOR*/				case 0b100111: instructionType = opcodeNOR(instruction); break;
/*SLT*/				case 0b101010: instructionType = opcodeSLT(instruction); break;
/*SLTU*/			case 0b101011: instructionType = opcodeSLTU(instruction); break;
					default:
						std::cout << "unhandled instruction 0x" << std::hex << instruction.getInstructionCode() << std::endl;
				}
				break;
/*BXX*/ 	case 0b000001: instructionType = opcodeBXX(instruction); break;
/*J*/ 		case 0b000010: instructionType = opcodeJ(instruction); break;
/*JAL*/ 	case 0b000011: instructionType = opcodeJAL(instruction); break;
/*BEQ*/ 	case 0b000100: instructionType = opcodeBEQ(instruction); break;
/*BNE*/ 	case 0b000101: instructionType = opcodeBNE(instruction); break;
/*BLEZ*/ 	case 0b000110: instructionType = opcodeBLEZ(instruction); break;
/*BGTZ*/ 	case 0b000111: instructionType = opcodeBGTZ(instruction); break;
/*ADDI*/ 	case 0b001000: instructionType = opcodeADDI(instruction); break;
/*ADDIU*/ 	case 0b001001: instructionType = opcodeADDIU(instruction); break;
/*SLTI*/ 	case 0b001010: instructionType = opcodeSLTI(instruction); break;
/*SLTIU*/ 	case 0b001011: instructionType = opcodeSLTIU(instruction); break;
/*ANDI*/ 	case 0b001100: instructionType = opcodeANDI(instruction); break;
/*ORI*/ 	case 0b001101: instructionType = opcodeORI(instruction); break;
/*XORI*/ 	case 0b001110: instructionType = opcodeXORI(instruction); break;
/*LUI*/ 	case 0b001111: instructionType = opcodeLUI(instruction); break;
/*COP0*/ 	case 0b010000: instructionType = opcodeCOP0(instruction); break;
/*COP1*/ 	case 0b010001: instructionType = opcodeCOP1(instruction); break;
/*COP2*/ 	case 0b010010: instructionType = opcodeCOP2(instruction); break;
/*COP3*/ 	case 0b010011: instructionType = opcodeCOP3(instruction); break;
/*LB*/ 		case 0b100000: instructionType = opcodeLB(instruction); break;
/*LH*/ 		case 0b100001: instructionType = opcodeLH(instruction); break;
/*LWL*/ 	case 0b100010: instructionType = opcodeLWL(instruction); break;
/*LW*/ 		case 0b100011: instructionType = opcodeLW(instruction); break;
/*LBU*/ 	case 0b100100: instructionType = opcodeLBU(instruction); break;
/*LHU*/ 	case 0b100101: instructionType = opcodeLHU(instruction); break;
/*LWR*/ 	case 0b100110: instructionType = opcodeLWR(instruction); break;
/*SB*/ 		case 0b101000: instructionType = opcodeSB(instruction); break;
/*SH*/ 		case 0b101001: instructionType = opcodeSH(instruction); break;
/*SWL*/ 	case 0b101010: instructionType = opcodeSWL(instruction); break;
/*SW*/ 		case 0b101011: instructionType = opcodeSW(instruction); break;
/*SWR*/ 	case 0b101110: instructionType = opcodeSWR(instruction); break;
/*LWC0*/ 	case 0b110000: instructionType = opcodeLWC0(instruction); break;
/*LWC1*/ 	case 0b110001: instructionType = opcodeLWC1(instruction); break;
/*LWC2*/ 	case 0b110010: instructionType = opcodeLWC2(instruction); break;
/*LWC3*/ 	case 0b110011: instructionType = opcodeLWC3(instruction); break;
/*SWC0*/ 	case 0b111000: instructionType = opcodeSWC0(instruction); break;
/*SWC1*/ 	case 0b111001: instructionType = opcodeSWC1(instruction); break;
/*SWC2*/ 	case 0b111010: instructionType = opcodeSWC2(instruction); break;
/*SWC3*/ 	case 0b111011: instructionType = opcodeSWC3(instruction); break;
		default:
			std::cout << "unhandled instruction 0x" << std::hex << instruction.getInstructionCode() << std::endl;
	}

#ifdef D_CPU_DEBUG
	decodeAndExecuteDebug(old_regs, instruction, instructionType);
//	storeDebugInfo(sdecodeAndExecuteDebug(old_regs, instruction, instructionType));
#endif

	return instructionType;
}

void Cpu::decodeAndExecuteDebug(std::vector<uint32_t> old_regs, Instruction instruction, InstructionType instructionType)
{
	if(instructionType == INSTRUCTION_TYPE_UNKNOWN)
	{
		printDebugInfo("***NEXT INSTRUCTION IS UNKNOWN***", "\n");
	}
	InstructionDisasm tmp_instruction(instruction.m_instruction);
	std::string str_decoded = decode(tmp_instruction);
	std::vector<uint32_t> v_used_regs = get_used_regs(tmp_instruction);

	char tmp_buf[1024] = { 0 };//pnryabov
	sprintf(tmp_buf, "%08X", instruction.m_instruction);

	printDebugInfo("0x" + std::string(tmp_buf), std::string("[INSTRUCTION]:"));
	printDebugInfo(str_decoded, "[DISASM]:");
	sprintf(tmp_buf, "%08X\t\t[PREV $PC: 0x%08X]", m_pc, old_regs[old_regs.size() - 1]);
	printDebugInfo("REG $PC: 0x" + std::string(tmp_buf), std::string("[PC REG]:"));

	for(uint32_t i = 0; i != v_used_regs.size(); i++)
	{
		sprintf(tmp_buf, "REG $%d: 0x%08X\t\t[PREV $%d: 0x%08X]", v_used_regs[i], m_regs[v_used_regs[i]], v_used_regs[i], old_regs[v_used_regs[i]]);
		printDebugInfo(tmp_buf, std::string("[USED REGS]:"));
	}
}

void Cpu::printDebugInfo(std::string message)
{

	printf("[DEBUG INFO]:\t%s\n", message.c_str());

}

void Cpu::printDebugInfo(std::string message, std::string prefix)
{

	printf("%s\t%s\n", prefix.c_str(), message.c_str());

}


std::string Cpu::sdecodeAndExecuteDebug(std::vector<uint32_t> old_regs, Instruction instruction, InstructionType instructionType)
{
	std::string debug_info("");
	if(instructionType == INSTRUCTION_TYPE_UNKNOWN)
	{
		debug_info += sprintDebugInfo1("***NEXT INSTRUCTION IS UNKNOWN***", "\n");
	}
	InstructionDisasm tmp_instruction(instruction.m_instruction);
	std::string str_decoded = decode(tmp_instruction);
	std::vector<uint32_t> v_used_regs = get_used_regs(tmp_instruction);

	char tmp_buf[1024] = { 0 };//pnryabov
	sprintf(tmp_buf, "%08X", instruction.m_instruction);

	debug_info += sprintDebugInfo1("0x" + std::string(tmp_buf), std::string("[INSTRUCTION]:"));
	debug_info += sprintDebugInfo1(str_decoded, "[DISASM]:");
	debug_info += sprintf(tmp_buf, "%08X\t\t[PREV $PC: 0x%08X]", m_pc, old_regs[old_regs.size() - 1]);
	debug_info += sprintDebugInfo1("REG $PC: 0x" + std::string(tmp_buf), std::string("[PC REG]:"));

	for(uint32_t i = 0; i != v_used_regs.size(); i++)
	{
		sprintf(tmp_buf, "REG $%d: 0x%08X\t\t[PREV $%d: 0x%08X]", v_used_regs[i], m_regs[v_used_regs[i]], v_used_regs[i], old_regs[v_used_regs[i]]);
		debug_info += sprintDebugInfo1(tmp_buf, std::string("[USED REGS]:"));
	}

	return debug_info;
}


std::string Cpu::sprintDebugInfo0(std::string message)
{
	char buffer[1024] = { 0 };
	sprintf(buffer, "[DEBUG INFO]:\t%s\n", message.c_str());
	return std::string(buffer);
}

std::string Cpu::sprintDebugInfo1(std::string message, std::string prefix)
{

	char buffer[1024] = { 0 };
	sprintf(buffer, "%s\t%s\n", prefix.c_str(), message.c_str());
	return std::string(buffer);
}

void Cpu::storeDebugInfo(std::string debug_info)
{
	uint32_t current_size = m_debug_info.size();
	if(current_size == m_debug_size)
	{
		for(int i = 0; i < m_debug_size - 1; i++)
		{
			m_debug_info[i] = m_debug_info[i + 1];
		}
		m_debug_info[m_debug_size - 1] = debug_info;
	}
	else
	{
		m_debug_info[current_size] = debug_info;
	}
}

Cpu::InstructionType Cpu::runNextInstruction()
{

	uint32_t pc = m_pc;

	Instruction instruction = load32(pc);

	m_pc = pc + 4;

	return decodeAndExecute(instruction);

//	return INSTRUCTION_TYPE_UNKNOWN;
}





Cpu::InstructionType Cpu::opcodeLUI(Instruction instruction)
{
	uint32_t imm_value = instruction.getImmediateValue();
	uint32_t target_reg = instruction.getRegisterTargetIndex();

	uint32_t value = imm_value << 16;

	setRegisterValue(target_reg, value);


	return INSTRUCTION_TYPE_LUI;
}

Cpu::InstructionType Cpu::opcodeORI(Instruction instruction)
{
	setRegisterValue(instruction.getRegisterTargetIndex(), instruction.getImmediateValue() | getRegisterValue(instruction.getRegisterSourceIndex()));
	return INSTRUCTION_TYPE_ORI;
}

Cpu::InstructionType Cpu::opcodeSW(Instruction instruction)
{
	uint32_t registerSourceIndex = instruction.getRegisterSourceIndex();
	uint32_t registerTargetIndex = instruction.getRegisterTargetIndex();
	store32(getRegisterValue(registerSourceIndex) + instruction.getSignExtendedImmediateValue(), getRegisterValue(registerTargetIndex));
	return INSTRUCTION_TYPE_SW;
}

uint32_t Cpu::getRegisterValue(uint32_t index) const
{
	assert(index < _countof(m_regs), "Index out of boundaries");
	return m_regs[index];
}

void Cpu::setRegisterValue(uint32_t index, uint32_t value)
{
	assert(index < _countof(m_regs), "Index out of boundaries");
	if (index > 0) m_regs[index] = value;
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Cpu::InstructionType Cpu::opcodeUNKNOWN(Instruction instruction)
{
	return INSTRUCTION_TYPE_UNKNOWN;
}

Cpu::InstructionType Cpu::opcodeSLL(Instruction instruction)
{
	uint32_t imm_val = instruction.shift();
	uint32_t target_reg = instruction.getRegisterTargetIndex();
	uint32_t dest_reg = instruction.getRegisterDestinationIndex();

	uint32_t value = getRegisterValue(target_reg) << imm_val;

	setRegisterValue(target_reg, value);

	return INSTRUCTION_TYPE_SLL;
}

Cpu::InstructionType Cpu::opcodeADDIU(Instruction instruction)
{
	uint32_t imm_val = instruction.getSignExtendedImmediateValue();
	uint32_t target_reg = instruction.getRegisterTargetIndex();
	uint32_t src_reg = instruction.getRegisterSourceIndex();

	uint32_t value = getRegisterValue(src_reg) + imm_val;

	setRegisterValue(target_reg, value);

	return INSTRUCTION_TYPE_ADDIU;

}

Cpu::InstructionType Cpu::opcodeJ(Instruction instruction)
{
	uint32_t imm_jump = instruction.imm_jump();

	m_pc = (m_pc & 0xf0000000) | (imm_jump << 2);


	return INSTRUCTION_TYPE_J;

}

Cpu::InstructionType Cpu::opcodeOR(Instruction instruction)
{

	uint32_t dest_reg = instruction.getRegisterDestinationIndex();
	uint32_t src_reg = instruction.getRegisterSourceIndex();
	uint32_t target_reg = instruction.getRegisterTargetIndex();

	uint32_t value = getRegisterValue(src_reg) | getRegisterValue(target_reg);

	setRegisterValue(dest_reg, value);

	return INSTRUCTION_TYPE_OR;

}


Cpu::InstructionType Cpu::opcodeCOP0(Instruction instruction)
{

	InstructionType result = INSTRUCTION_TYPE_COP0;

	switch(instruction.cop_opcode())
	{
///*mfc0*/ 	case /*mfc0*/ 0b00000: break;
/*mtc0*/ 	case 0b00100: opcodeMTC0(instruction); break;
// /*rfe*/	case /*rfe*/ 0b10000: break;
			default:
				std::cout << "unhandled cop0 instruction 0x" << std::hex << instruction.getInstructionCode() << std::endl;
	}

	return result;
}

void Cpu::opcodeMTC0(Instruction instruction)
{

	uint32_t cpu_r = instruction.getRegisterTargetIndex();
	uint32_t cop_r = instruction.getRegisterDestinationIndex();

	uint32_t value = getRegisterValue(cpu_r);

	switch(cop_r)
	{
		case 12:
			m_sr = value;
			break;
		default:
			std::cout << "unhandled cop0 register 0x" << std::hex << cop_r << std::endl;
	}

}

Cpu::InstructionType Cpu::opcodeBNE(Instruction instruction) { }
Cpu::InstructionType Cpu::opcodeADDI(Instruction instruction) { }
Cpu::InstructionType Cpu::opcodeLW(Instruction instruction) { }



//Cpu::InstructionType Cpu::opcodeSLL(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSRL(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSRA(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSLLV(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSRLV(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSRAV(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeJR(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeJALR(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSYSCALL(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeBREAK(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeMFHI(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeMTHI(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeMFLO(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeMTLO(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeMULT(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeMULTU(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeDIV(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeDIVU(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeADD(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeADDU(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSUB(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSUBU(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeAND(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
//Cpu::InstructionType Cpu::opcodeOR(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeXOR(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeNOR(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSLT(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSLTU(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }




Cpu::InstructionType Cpu::opcodeBXX(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
//Cpu::InstructionType Cpu::opcodeJ(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeJAL(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeBEQ(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
//Cpu::InstructionType Cpu::opcodeBNE(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeBLEZ(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeBGTZ(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
//Cpu::InstructionType Cpu::opcodeADDI(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
//Cpu::InstructionType Cpu::opcodeADDIU(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSLTI(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSLTIU(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeANDI(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
//Cpu::InstructionType Cpu::opcodeORI(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeXORI(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
//Cpu::InstructionType Cpu::opcodeLUI(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
//Cpu::InstructionType Cpu::opcodeCOP0(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeCOP1(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeCOP2(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeCOP3(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeLB(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeLH(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeLWL(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
//Cpu::InstructionType Cpu::opcodeLW(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeLBU(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeLHU(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeLWR(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSB(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSH(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSWL(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
//Cpu::InstructionType Cpu::opcodeSW(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSWR(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeLWC0(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeLWC1(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeLWC2(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeLWC3(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSWC0(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSWC1(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSWC2(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }
Cpu::InstructionType Cpu::opcodeSWC3(Instruction instruction) { return INSTRUCTION_TYPE_UNKNOWN; }


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
