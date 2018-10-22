#include "pscx_cpu.h"
#include "pscx_disasm.h"

#include <iostream>
#include <cassert>
#include <climits>

#define assert(message, ...) do { \
    if(!(__VA_ARGS__)) { \
        /*error code*/ \
    } \
} while(0)


static void branch(uint32_t offset, uint32_t& pc);

const uint32_t* Cpu::getRegistersPtr() const
{
	return m_regs;
}

const std::vector<uint32_t>& Cpu::getInstructionsDump() const
{
	return m_debugInstructions;
}

Instruction Cpu::load32(uint32_t addr) const
{
	return m_inter.load32(addr);
}

Instruction Cpu::load8(uint32_t addr) const
{
	return m_inter.load8(addr);
}

void Cpu::store32(uint32_t addr, uint32_t value)
{
	m_inter.store32(addr, value);
}

void Cpu::store16(uint32_t addr, uint16_t value)
{
	m_inter.store16(addr, value);
}

void Cpu::store8(uint32_t addr, uint8_t value)
{
	m_inter.store8(addr, value);
}

// TODO: take a look how to get back error messages.
Cpu::InstructionType Cpu::decodeAndExecute(const Instruction& instruction)
{
	InstructionType instructionType = INSTRUCTION_TYPE_UNKNOWN;

	switch (instruction.getInstructionCode())
	{
	case 0b000000:
		// 0b000000 can introduce a number of instructions
		// To figure out which one we need, we should read bits [5:0]
		switch (instruction.getSubfunctionInstructionCode())
		{
		case /*SLL*/0b000000:
			instructionType = opcodeSLL(instruction);
			break;
		case /*OR*/0b100101:
			instructionType = opcodeOR(instruction);
			break;
		case/*SLTU*/0b101011:
			instructionType = opcodeSLTU(instruction);
			break;
		case/*ADDU*/0b100001:
			instructionType = opcodeADDU(instruction);
			break;
		case/*JR*/0b001000:
			instructionType = opcodeJR(instruction);
			break;
		default:
			LOG("Unhandled sub instruction 0x" << std::hex << instruction.getSubfunctionInstructionCode());
		}
		break;
	case /*LUI*/0b001111:
		//---------------------------------
		// TODO : call LUI instruction.
		// Rust:
		// 0b001111 => self.op_lui(instruction)
		//---------------------------------
		instructionType = opcodeLUI(instruction);
		break;
	case /*ORI*/0b001101:
		instructionType = opcodeORI(instruction);
		break;
	case /*SW*/0b101011:
		instructionType = opcodeSW(instruction);
		break;
	case /*ADDIU*/0b001001:
		instructionType = opcodeADDIU(instruction);
		break;
	case/*J*/0b000010:
		instructionType = opcodeJ(instruction);
		break;
	case/*COP0*/0b010000:
		instructionType = opcodeCOP0(instruction);
		break;
	case/*BNE*/0b000101:
		instructionType = opcodeBNE(instruction);
		break;
	case /*ADDI*/0b001000:
		instructionType = opcodeADDI(instruction);
		break;
	case/*LW*/0b100011:
		instructionType = opcodeLW(instruction);
		break;
	case/*SH*/0b101001:
		instructionType = opcodeSH(instruction);
		break;
	case/*JAL*/0b000011:
		instructionType = opcodeJAL(instruction);
		break;
	case/*ANDI*/0b001100:
		instructionType = opcodeANDI(instruction);
		break;
	case/*SB*/0b101000:
		instructionType = opcodeSB(instruction);
		break;
	case/*LB*/0b100000:
		instructionType = opcodeLB(instruction);
		break;
	case/*BEQ*/0b000100:
		instructionType = opcodeBEQ(instruction);
		break;
	default:
		LOG("Unhandled instruction 0x" << std::hex << instruction.getInstructionCode());
	}

	m_debugInstructions.push_back(instruction.getInstructionOpcode());
	return instructionType;
}

Cpu::InstructionType Cpu::runNextInstruction()
{
	Instruction instruction = m_nextInstruction;

	m_nextInstruction = load32(m_pc);

	Instruction::InstructionStatus status = m_nextInstruction.getInstructionStatus();

	if(status == Instruction::INSTRUCTION_STATUS_UNALIGNED_ACCESS || status == Instruction::INSTRUCTION_STATUS_UNHANDLED_FETCH)
	{
		return INSTRUCTION_TYPE_UNKNOWN;
	}

	m_disasm.print_decode(m_nextInstruction);
		

	m_pc += 4;

	if(status == Instruction::INSTRUCTION_STATUS_NOT_IMPLEMENTED)
	{
		return INSTRUCTION_TYPE_NOT_IMPLEMENTED;
	}


	RegisterIndex reg_index = m_load.m_registerIndex;
	uint32_t reg_value = m_load.m_registerValue;

	setRegisterValue(reg_index, reg_value);

	m_load = RegisterData(RegisterIndex(0), 0);

	InstructionType run_result = decodeAndExecute(instruction);

	memcpy(m_regs, m_outRegs, sizeof(m_regs));


	return run_result;
}

Cpu::InstructionType Cpu::opcodeLUI(const Instruction& instruction)
{
	uint32_t imm_value = instruction.getImmediateValue();
	RegisterIndex target_reg = instruction.getRegisterTargetIndex();

	uint32_t value = imm_value << 16;

	setRegisterValue(target_reg, value);


	return INSTRUCTION_TYPE_LUI;
}

Cpu::InstructionType Cpu::opcodeORI(const Instruction& instruction)
{
	setRegisterValue(instruction.getRegisterTargetIndex(), instruction.getImmediateValue() | getRegisterValue(instruction.getRegisterSourceIndex()));
	return INSTRUCTION_TYPE_ORI;
}

Cpu::InstructionType Cpu::opcodeSW(const Instruction& instruction)
{
	if (m_sr & 0x10000)
	{
		// Cache is isolated, ignore write
		LOG("Ignoring store while cache is isolated");
		return INSTRUCTION_TYPE_CACHE_ISOLATED;
	}

	RegisterIndex registerSourceIndex = instruction.getRegisterSourceIndex();
	RegisterIndex registerTargetIndex = instruction.getRegisterTargetIndex();
	store32(getRegisterValue(registerSourceIndex) + instruction.getSignExtendedImmediateValue(), getRegisterValue(registerTargetIndex));
	return INSTRUCTION_TYPE_SW;
}

Cpu::InstructionType Cpu::opcodeSLL(const Instruction& instruction)
{
	uint32_t imm_val = instruction.getShiftImmediateValue();
	RegisterIndex target_reg = instruction.getRegisterTargetIndex();
	RegisterIndex dest_reg = instruction.getRegisterDestinationIndex();

	uint32_t value = getRegisterValue(target_reg) << imm_val;

	setRegisterValue(target_reg, value);

	return INSTRUCTION_TYPE_SLL;

}

Cpu::InstructionType Cpu::opcodeADDIU(const Instruction& instruction)
{
	uint32_t imm_val = instruction.getSignExtendedImmediateValue();
	RegisterIndex target_reg = instruction.getRegisterTargetIndex();
	RegisterIndex src_reg = instruction.getRegisterSourceIndex();

	uint32_t value = getRegisterValue(src_reg) + imm_val;

	setRegisterValue(target_reg, value);

	return INSTRUCTION_TYPE_ADDIU;
}

Cpu::InstructionType Cpu::opcodeJ(const Instruction& instruction)
{
	uint32_t imm_jump = instruction.getJumpTargetValue();

	m_pc = (m_pc & 0xf0000000) | (imm_jump << 2);

	return INSTRUCTION_TYPE_J;
}

Cpu::InstructionType Cpu::opcodeOR(const Instruction& instruction)
{
	setRegisterValue(instruction.getRegisterDestinationIndex(), getRegisterValue(instruction.getRegisterSourceIndex()) | getRegisterValue(instruction.getRegisterTargetIndex()));
	return INSTRUCTION_TYPE_OR;
}

Cpu::InstructionType Cpu::opcodeCOP0(const Instruction& instruction)
{
	InstructionType instructionType = INSTRUCTION_TYPE_UNKNOWN;

	switch (instruction.getCopOpcodeValue())
	{
	case 0b00100:
		instructionType = opcodeMTC0(instruction);
		break;
	default:
		LOG("Unhandled instruction 0x" << std::hex << instruction.getCopOpcodeValue());
	}

	return instructionType;
}

Cpu::InstructionType Cpu::opcodeMTC0(const Instruction& instruction)
{
	uint32_t cop0Register = instruction.getRegisterDestinationIndex().m_index;
	uint32_t cop0RegisterValue = getRegisterValue(cop0Register);

	switch (cop0Register)
	{
	// $cop0_3 BPC, used to generate a breakpoint exception when the PC takes the given value
	case 3:
	// $cop0_5 BDA, the data breakpoint. It breaks when a certain address is accessed on a data load/store instead of a PC value
	case 5:
	// $cop0_6
	case 6:
	// $cop0_7 DCIC, used to enable and disable the various hardware breakpoints
	case 7:
	// $cop0_9 BDAM, it's a bitmask applied when testing BDA above
	case 9:
	// $cop0_11 BPCM, like BDAM but for masking the BPC breakpoint
	case 11:
		// Breakpoints registers
		if (cop0RegisterValue)
			LOG("Unhandled write to cop0 register 0x" << std::hex << cop0Register);
		break;
	// $cop0_12
	case 12:
		// Status register, it's used to query and mask the exceptions and controlling the cache behaviour
		m_sr = getRegisterValue(instruction.getRegisterTargetIndex());
		break;
	// $cop0_13 CAUSE, which contains mostly read-only data describing the cause of an exception
	case 13:
		// Cause register
		if (cop0RegisterValue)
			LOG("Unhandled write to CAUSE register " << std::hex << cop0Register);
		break;
	default:
		LOG("Unhandled cop0 register 0x" << std::hex << cop0Register);
	}

	return INSTRUCTION_TYPE_MTC0;
}

// Branch to immediate value "offset"
void branch(uint32_t offset, uint32_t& pc)
{
	// Offset immediates are always shifted two places to the right
	// since "PC" addresses have to be aligned on 32 bits at all times.
	pc += (offset << 2);

	// We need to compensate for the hardcoded "pc += 4" in the runNextInstruction
	pc -= 4;
}


Cpu::InstructionType Cpu::opcodeBNE(const Instruction& instruction)
{

	uint32_t imm_val = instruction.getSignExtendedImmediateValue();
	RegisterIndex target_reg = instruction.getRegisterTargetIndex();
	RegisterIndex src_reg = instruction.getRegisterSourceIndex();

	if(getRegisterValue(target_reg) != getRegisterValue(src_reg))
	{
		branch(imm_val, m_pc);
	}

	return INSTRUCTION_TYPE_BNE;
}

Cpu::InstructionType Cpu::opcodeADDI(const Instruction& instruction)
{
	int32_t signExtendedImmediateValue = instruction.getSignExtendedImmediateValue();
	RegisterIndex registerSourceIndex  = instruction.getRegisterSourceIndex();

	int32_t registerSourceValue = getRegisterValue(registerSourceIndex);

	assert(!(signExtendedImmediateValue > 0 && registerSourceValue > INT_MAX - signExtendedImmediateValue), "ADDI overflow");
	assert(!(signExtendedImmediateValue < 0 && registerSourceValue < INT_MIN - signExtendedImmediateValue), "ADDI underflow");

	setRegisterValue(instruction.getRegisterTargetIndex(), uint32_t(registerSourceValue + signExtendedImmediateValue));

	return INSTRUCTION_TYPE_ADDI;
}

Cpu::InstructionType Cpu::opcodeLW(const Instruction& instruction)
{
	if(m_sr & 0x10000 != 0)
	{
		// Cache is isolated, ignore write
		LOG("Ignoring load while cache is isolated");
		return INSTRUCTION_TYPE_CACHE_ISOLATED;
	}

	uint32_t imm_val = instruction.getSignExtendedImmediateValue();
	RegisterIndex target_reg = instruction.getRegisterTargetIndex();
	RegisterIndex src_reg = instruction.getRegisterSourceIndex();

	uint32_t addr = getRegisterValue(src_reg) + imm_val;
	Instruction value = load32(addr);
	Instruction::InstructionStatus status = value.getInstructionStatus();
	if(status == Instruction::INSTRUCTION_STATUS_UNALIGNED_ACCESS || status == Instruction::INSTRUCTION_STATUS_UNHANDLED_FETCH)
	{
		return INSTRUCTION_TYPE_UNKNOWN;
	}
	if(status == Instruction::INSTRUCTION_STATUS_NOT_IMPLEMENTED)
	{
		return INSTRUCTION_TYPE_NOT_IMPLEMENTED;
	}

	m_load = RegisterData(target_reg, value.getInstructionOpcode());
	return INSTRUCTION_TYPE_LW;
}

Cpu::InstructionType Cpu::opcodeSLTU(const Instruction& instruction)
{
	RegisterIndex registerSourceIndex = instruction.getRegisterSourceIndex();
	RegisterIndex registerTargetIndex = instruction.getRegisterTargetIndex();

	uint32_t registerValueComparison = getRegisterValue(registerSourceIndex) < getRegisterValue(registerTargetIndex);

	setRegisterValue(instruction.getRegisterDestinationIndex(), registerValueComparison);
	return INSTRUCTION_TYPE_SLTU;
}

Cpu::InstructionType Cpu::opcodeADDU(const Instruction& instruction)
{
	RegisterIndex registerSourceIndex = instruction.getRegisterSourceIndex();
	RegisterIndex registerTargetIndex = instruction.getRegisterTargetIndex();

	setRegisterValue(instruction.getRegisterDestinationIndex(), getRegisterValue(registerSourceIndex) + getRegisterValue(registerTargetIndex));
	return INSTRUCTION_TYPE_ADDU;
}

Cpu::InstructionType Cpu::opcodeSH(const Instruction& instruction)
{
	if(m_sr & 0x10000 != 0)
	{
		// Cache is isolated, ignore write
		LOG("Ignoring store while cache is isolated");
		return INSTRUCTION_TYPE_CACHE_ISOLATED;
	}

	uint32_t imm_val = instruction.getSignExtendedImmediateValue();
	RegisterIndex target_reg = instruction.getRegisterTargetIndex();
	RegisterIndex src_reg = instruction.getRegisterSourceIndex();

	uint32_t addr = getRegisterValue(src_reg) + imm_val;
	uint16_t value = (uint16_t)getRegisterValue(target_reg);

	store16(addr, value);

	return INSTRUCTION_TYPE_SH;
}


Cpu::InstructionType Cpu::opcodeJAL(const Instruction& instruction)
{
	
	uint32_t ret_addr = m_pc;

	setRegisterValue(RegisterIndex(31), ret_addr);

	opcodeJ(instruction);

	return INSTRUCTION_TYPE_JAL;
}

Cpu::InstructionType Cpu::opcodeANDI(const Instruction& instruction)
{
	RegisterIndex registerSourceIndex = instruction.getRegisterSourceIndex();
	RegisterIndex registerTargetIndex = instruction.getRegisterTargetIndex();

	setRegisterValue(registerTargetIndex, getRegisterValue(registerSourceIndex) & instruction.getImmediateValue());
	return INSTRUCTION_TYPE_ANDI;
}

Cpu::InstructionType Cpu::opcodeSB(const Instruction& instruction)
{
	if(m_sr & 0x10000 != 0)
	{
		// Cache is isolated, ignore write
		LOG("Ignoring store while cache is isolated");
		return INSTRUCTION_TYPE_CACHE_ISOLATED;
	}

	uint32_t imm_val = instruction.getSignExtendedImmediateValue();
	RegisterIndex target_reg = instruction.getRegisterTargetIndex();
	RegisterIndex src_reg = instruction.getRegisterSourceIndex();

	uint32_t addr = getRegisterValue(src_reg) + imm_val;
	uint8_t value = (uint16_t)getRegisterValue(target_reg);

	store8(addr, value);

	return INSTRUCTION_TYPE_SB;
}


Cpu::InstructionType Cpu::opcodeJR(const Instruction& instruction)
{
	RegisterIndex src_reg = instruction.getRegisterSourceIndex();

	m_pc = getRegisterValue(src_reg);

	return INSTRUCTION_TYPE_JR;
}

Cpu::InstructionType Cpu::opcodeLB(const Instruction& instruction)
{
	if(m_sr & 0x10000 != 0)
	{
		// Cache is isolated, ignore write
		LOG("Ignoring load while cache is isolated");
		return INSTRUCTION_TYPE_CACHE_ISOLATED;
	}

	uint32_t imm_val = instruction.getSignExtendedImmediateValue();
	RegisterIndex target_reg = instruction.getRegisterTargetIndex();
	RegisterIndex src_reg = instruction.getRegisterSourceIndex();

	uint32_t addr = getRegisterValue(src_reg) + imm_val;
	Instruction value = load8(addr);
	Instruction::InstructionStatus status = value.getInstructionStatus();
	if(status == Instruction::INSTRUCTION_STATUS_UNALIGNED_ACCESS || status == Instruction::INSTRUCTION_STATUS_UNHANDLED_FETCH)
	{
		return INSTRUCTION_TYPE_UNKNOWN;
	}
	if(status == Instruction::INSTRUCTION_STATUS_NOT_IMPLEMENTED)
	{
		return INSTRUCTION_TYPE_NOT_IMPLEMENTED;
	}

	m_load = RegisterData(target_reg, value.getInstructionOpcode());
	return INSTRUCTION_TYPE_LB;
}

Cpu::InstructionType Cpu::opcodeBEQ(const Instruction& instruction)
{
	uint32_t imm_val = instruction.getSignExtendedImmediateValue();
	RegisterIndex target_reg = instruction.getRegisterTargetIndex();
	RegisterIndex src_reg = instruction.getRegisterSourceIndex();

	if(getRegisterValue(target_reg) == getRegisterValue(src_reg))
	{
		branch(imm_val, m_pc);
	}

	return INSTRUCTION_TYPE_BEQ;
}

uint32_t Cpu::getRegisterValue(RegisterIndex registerIndex) const
{
	assert(registerIndex.m_index < _countof(m_regs), "Index out of boundaries");
	return m_regs[registerIndex.m_index];
}

void Cpu::setRegisterValue(RegisterIndex registerIndex, uint32_t value)
{
	assert(registerIndex.m_index < _countof(m_outRegs), "Index out of boundaries");
	if (registerIndex.m_index > 0) m_outRegs[registerIndex.m_index] = value;
}
