
#include "pscx_disasm.h"




int MipsDisasm::disasm_main(uint32_t pc_reg_value)
{

//	PC_REG = pc_reg_value;
	return 0;

}

uint32_t MipsDisasm::InstructionDisasm::function()
{
	return m_data >> 26;
}

uint32_t MipsDisasm::InstructionDisasm::subfunction()
{
	return m_data & 0x3f;
}

uint32_t MipsDisasm::InstructionDisasm::cop_opcode()
{
	return (m_data >> 21) & 0x1f;
}

RegisterIndex MipsDisasm::InstructionDisasm::s()
{
	return RegisterIndex((m_data >> 21) & 0x1f);
}

RegisterIndex MipsDisasm::InstructionDisasm::t()
{
	return RegisterIndex((m_data >> 16) & 0x1f);
}

RegisterIndex MipsDisasm::InstructionDisasm::d()
{
	return RegisterIndex((m_data >> 11) & 0x1f);
}

uint32_t MipsDisasm::InstructionDisasm::imm()
{
	return m_data & 0xffff;
}

uint32_t MipsDisasm::InstructionDisasm::imm_se()
{
	uint16_t value = (uint16_t)(m_data & 0xffff);
	return (uint32_t)value;
}

uint32_t MipsDisasm::InstructionDisasm::shift()
{
	return (m_data >> 6) & 0x1f;
}

uint32_t MipsDisasm::InstructionDisasm::imm_jump()
{
	return m_data & 0x3ffffff;
}


std::string MipsDisasm::InstructionDisasm::i2s()
{
	char buffer[1024] = { 0 };
	sprintf(buffer, "%08X", m_data);
	std::string hexstr(buffer);
	return hexstr;
}

uint32_t MipsDisasm::u32_from_str_prefix(char * str)
{
	int str_len = strlen(str);
	char *number;
	uint32_t base;
	switch(str_len)
	{
		case 0:
		case 1:
			base = 10;
			number = str;
			break;
		default:
			if(str[0] == '0')
			{
				switch(str[1])
				{
					case 'x':
						base = 16;
						number = str + 2;
						break;
					case 'o':
						base = 8;
						number = str + 2;
						break;
					case 'b':
						base = 2;
						number = str + 2;
						break;
					default:
						base = 10;
						number = str;
				}
			}
			else
			{
				base = 10;
				number = str;
			}
	}
	return (uint32_t)strtoul(number, (char **)NULL, base);

}


std::string MipsDisasm::decode_(Instruction i_instruction)
{
	InstructionDisasm instruction(i_instruction.getInstructionOpcode());
	char *opcode_str = NULL;
	std::string decode_error("UNKNOWN OR ERROR");

	switch(instruction.function())
	{
		case 0b000000:
			switch(instruction.subfunction())
			{
				case 0b000000: opcode_str = op_sll(instruction); break;
				case 0b000010: opcode_str = op_srl(instruction); break;
				case 0b000011: opcode_str = op_sra(instruction); break;
				case 0b000100: opcode_str = op_sllv(instruction); break;
				case 0b000110: opcode_str = op_srlv(instruction); break;
				case 0b000111: opcode_str = op_srav(instruction); break;
				case 0b001000: opcode_str = op_jr(instruction); break;
				case 0b001001: opcode_str = op_jalr(instruction); break;
				case 0b001100: opcode_str = op_syscall(instruction); break;
				case 0b001101: opcode_str = op_break(instruction); break;
				case 0b010000: opcode_str = op_mfhi(instruction); break;
				case 0b010001: opcode_str = op_mthi(instruction); break;
				case 0b010010: opcode_str = op_mflo(instruction); break;
				case 0b010011: opcode_str = op_mtlo(instruction); break;
				case 0b011000: opcode_str = op_mult(instruction); break;
				case 0b011001: opcode_str = op_multu(instruction); break;
				case 0b011010: opcode_str = op_div(instruction); break;
				case 0b011011: opcode_str = op_divu(instruction); break;
				case 0b100000: opcode_str = op_add(instruction); break;
				case 0b100001: opcode_str = op_addu(instruction); break;
				case 0b100010: opcode_str = op_sub(instruction); break;
				case 0b100011: opcode_str = op_subu(instruction); break;
				case 0b100100: opcode_str = op_and(instruction); break;
				case 0b100101: opcode_str = op_or(instruction); break;
				case 0b100110: opcode_str = op_xor(instruction); break;
				case 0b100111: opcode_str = op_nor(instruction); break;
				case 0b101010: opcode_str = op_slt(instruction); break;
				case 0b101011: opcode_str = op_sltu(instruction); break;
				default:
					return decode_error;
			}
			break;
		case 0b000001: opcode_str = op_bxx(instruction); break;
		case 0b000010: opcode_str = op_j(instruction); break;
		case 0b000011: opcode_str = op_jal(instruction); break;
		case 0b000100: opcode_str = op_beq(instruction); break;
		case 0b000101: opcode_str = op_bne(instruction); break;
		case 0b000110: opcode_str = op_blez(instruction); break;
		case 0b000111: opcode_str = op_bgtz(instruction); break;
		case 0b001000: opcode_str = op_addi(instruction); break;
		case 0b001001: opcode_str = op_addiu(instruction); break;
		case 0b001010: opcode_str = op_slti(instruction); break;
		case 0b001011: opcode_str = op_sltiu(instruction); break;
		case 0b001100: opcode_str = op_andi(instruction); break;
		case 0b001101: opcode_str = op_ori(instruction); break;
		case 0b001110: opcode_str = op_xori(instruction); break;
		case 0b001111: opcode_str = op_lui(instruction); break;
		case 0b010000: opcode_str = op_cop0(instruction); break;
		case 0b010001: opcode_str = op_cop1(instruction); break;
		case 0b010010: opcode_str = op_cop2(instruction); break;
		case 0b010011: opcode_str = op_cop3(instruction); break;
		case 0b100000: opcode_str = op_lb(instruction); break;
		case 0b100001: opcode_str = op_lh(instruction); break;
		case 0b100010: opcode_str = op_lwl(instruction); break;
		case 0b100011: opcode_str = op_lw(instruction); break;
		case 0b100100: opcode_str = op_lbu(instruction); break;
		case 0b100101: opcode_str = op_lhu(instruction); break;
		case 0b100110: opcode_str = op_lwr(instruction); break;
		case 0b101000: opcode_str = op_sb(instruction); break;
		case 0b101001: opcode_str = op_sh(instruction); break;
		case 0b101010: opcode_str = op_swl(instruction); break;
		case 0b101011: opcode_str = op_sw(instruction); break;
		case 0b101110: opcode_str = op_swr(instruction); break;
		case 0b110000: opcode_str = op_lwc0(instruction); break;
		case 0b110001: opcode_str = op_lwc1(instruction); break;
		case 0b110010: opcode_str = op_lwc2(instruction); break;
		case 0b110011: opcode_str = op_lwc3(instruction); break;
		case 0b111000: opcode_str = op_swc0(instruction); break;
		case 0b111001: opcode_str = op_swc1(instruction); break;
		case 0b111010: opcode_str = op_swc2(instruction); break;
		case 0b111011: opcode_str = op_swc3(instruction); break;
		default:
			return decode_error;
	}
	if(opcode_str == NULL)
	{
		printf("CAN NOT MALLOC MEMORY!!!!!\nNULL RETURNED\n");
		return decode_error;
	}

	std::string disasm_command(opcode_str);
	free(opcode_str);


	return disasm_command;
}

std::vector<uint32_t> MipsDisasm::get_used_regs(MipsDisasm::InstructionDisasm instruction)
{
	std::vector<uint32_t> v_regs;
	std::string str_decoded = decode(Instruction(instruction.m_data));
	char buffer[1024] = { 0 };
	char buffer1[3] = { 0 };
	sprintf(buffer, "%s", str_decoded.c_str());
	uint32_t str_len = strlen(buffer);
	for(int i = 0; i < str_len; i++)
	{
		if(buffer[i] == '$')
		{
			buffer1[0] = buffer[i + 2];
			buffer1[1] = buffer[i + 3];
			buffer1[2] = '\0';
			uint32_t tmp_index = strtoul(buffer1, (char **)NULL, 10);
			v_regs.push_back(tmp_index);
			buffer1[0] = 0;
			buffer1[1] = 0;
			buffer1[2] = 0;
		}
	}

	return v_regs;

}

std::vector<uint32_t> MipsDisasm::touched_regs(Instruction instruction)
{
	std::vector<uint32_t> v_regs;
	std::string str_decoded = decode(instruction);
	char buffer[1024] = { 0 };
	char buffer1[3] = { 0 };
	sprintf(buffer, "%s", str_decoded.c_str());
	uint32_t str_len = strlen(buffer);
	for(int i = 0; i < str_len; i++)
	{
		if(buffer[i] == '$')
		{
			buffer1[0] = buffer[i + 2];
			buffer1[1] = buffer[i + 3];
			buffer1[2] = '\0';
			uint32_t tmp_index = strtoul(buffer1, (char **)NULL, 10);
			v_regs.push_back(tmp_index);
			buffer1[0] = 0;
			buffer1[1] = 0;
			buffer1[2] = 0;
		}
	}

	return v_regs;

}

std::map<std::string, std::string> MipsDisasm::get_instruction(Instruction instruction)
{

	std::string decoded = decode(instruction);
	std::vector<uint32_t> regs_info = touched_regs(instruction);

	uint32_t opcode = instruction.getInstructionOpcode();
	uint32_t pc = get_pc();

	std::map<std::string, std::string> res;
	std::stringstream tmp_str;
	tmp_str.str("");
	tmp_str << HEXW(opcode);
	res.insert(std::pair<std::string, std::string>("opcode", tmp_str.str()));
	res.insert(std::pair<std::string, std::string>("decode", decoded));
	tmp_str.str("");
	tmp_str << HEXW(pc);
	res.insert(std::pair<std::string, std::string>("pc", tmp_str.str()));

	tmp_str.str("");
	if(regs_info.size() != 0)
	{
		for(int i  = 0; i < regs_info.size() - 1; i++)
		{
			tmp_str.str("");
			tmp_str << "$R" << std::dec << regs_info[i] << " ";
		}
		tmp_str << "$R" << std::dec << regs_info[regs_info.size() - 1];
	}
	res.insert(std::pair<std::string, std::string>("regs", tmp_str.str()));

	return res;

}

void MipsDisasm::print_decode(Instruction instruction)
{
	if(!is_on())
		return;
	if(m_format == "default")
	{
		std::cout << decode(instruction) << std::endl;
	}
	if(m_format == "full")
	{
		std::map<std::string, std::string> info = get_instruction(instruction);
		for(std::map<std::string, std::string>::iterator it = info.begin(); it != info.end(); ++it)
		{
			std::cout << it->first << ":\t" << it->second << std::endl;
		}

	}
	if(m_format == "single-line")
	{
		std::map<std::string, std::string> info = get_instruction(instruction);
		std::cout << m_index << ": " << info["opcode"] << "\t" << info["decode"] << "\t" << " ## " << info["regs"] << std::endl;
	}
}



void MipsDisasm::force_get_instruction(Instruction instruction)
{
	std::map<std::string, std::string> info = get_instruction(instruction);
	std::stringstream tmp_str;
	tmp_str.str("");
	for(std::map<std::string, std::string>::iterator it = info.begin(); it != info.end(); ++it)
	{
		tmp_str << it->first << ":\t" << it->second << std::endl;
	}

	memset(force_info_buffer, 0, force_info_buffer_size);
	sprintf(force_info_buffer, "%s\n", tmp_str.str().c_str());

}



char * MipsDisasm::reg(RegisterIndex idx)
{
	char *mnemonics[] = {\
		"$R00", "$R01", "$R02", "$R03", "$R04",
		"$R05", "$R06", "$R07", "$R08", "$R09",
		"$R10", "$R11", "$R12", "$R13", "$R14",
		"$R15", "$R16", "$R17", "$R18", "$R19",
		"$R20", "$R21", "$R22", "$R23", "$R24",
		"$R25", "$R26", "$R27", "$R28", "$R29",
		"$R30", "$R31" };
	return mnemonics[(size_t)idx.m_index];
}


char * MipsDisasm::op_sll(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	uint32_t i = instruction.shift();
	sprintf(buffer, "sll %s, %s << %d", reg(d), reg(t), i);
	return buffer;
}

char * MipsDisasm::op_srl(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	uint32_t i = instruction.shift();
	sprintf(buffer, "srl %s, %s >> %d", reg(d), reg(t), i);
	return buffer;
}

char * MipsDisasm::op_sra(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	uint32_t i = instruction.shift();
	sprintf(buffer, "sra %s, %s >> %d", reg(d), reg(t), i);
	return buffer;
}

char * MipsDisasm::op_sllv(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "sllv %s, %s << %s", reg(d), reg(t), reg(s));
	return buffer;
}

char * MipsDisasm::op_srlv(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "srlv %s, %s >> %s", reg(d), reg(t), reg(s));
	return buffer;
}

char * MipsDisasm::op_srav(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "srav  %s, %s >> %s", reg(d), reg(t), reg(s));
	return buffer;
}

char * MipsDisasm::op_jr(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex s = instruction.s();
	sprintf(buffer, "jr %s", reg(s));
	return buffer;
}

char * MipsDisasm::op_jalr(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "jalr  %s, %s", reg(d), reg(s));
	return buffer;
}

char * MipsDisasm::op_syscall(InstructionDisasm instruction)
{
	uint32_t param = (instruction.m_data >> 6) & 0xfffff;
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	sprintf(buffer, "syscall  %x", param);
	return buffer;
}

char * MipsDisasm::op_break(InstructionDisasm instruction)
{
	uint32_t param = (instruction.m_data >> 6) & 0xfffff;
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	sprintf(buffer, "break  %x", param);
	return buffer;
}

char * MipsDisasm::op_mfhi(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex d = instruction.d();
	sprintf(buffer, "mfhi %s", reg(d));
	return buffer;
}

char * MipsDisasm::op_mthi(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex s = instruction.s();
	sprintf(buffer, "mthi %s", reg(s));
	return buffer;
}

char * MipsDisasm::op_mflo(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex d = instruction.d();
	sprintf(buffer, "mflo %s", reg(d));
	return buffer;
}

char * MipsDisasm::op_mtlo(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex s = instruction.s();
	sprintf(buffer, "mtlo %s", reg(s));
	return buffer;
}

char * MipsDisasm::op_mult(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "mult %s, %s", reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_multu(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "multu %s, %s", reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_div(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "div %s, %s", reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_divu(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "divu %s, %s", reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_add(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "add  %s, %s, %s", reg(d), reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_addu(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "addu  %s, %s, %s", reg(d), reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_sub(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "sub  %s, %s, %s", reg(d), reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_subu(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "subu  %s, %s, %s", reg(d), reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_and(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "and  %s, %s, %s", reg(d), reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_or(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "or  %s, %s, %s", reg(d), reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_xor(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "xor  %s, %s, %s", reg(d), reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_nor(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "nor  %s, %s, %s", reg(d), reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_slt(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "slt  %s, %s, %s", reg(d), reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_sltu(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	RegisterIndex d = instruction.d();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "sltu  %s, %s, %s", reg(d), reg(s), reg(t));
	return buffer;
}

char * MipsDisasm::op_bxx(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	std::string op, al;
	if((instruction.m_data & (1 << 16)) != 0)
		op = "bgez";
	else
		op = "bltz";
	if((instruction.m_data & (1 << 20)) != 0)
		al = "al";
	else
		al = "";
	sprintf(buffer, "%s%s  %s, %d", op.c_str(), al.c_str(), reg(s), (int32_t)(i << 2));
	return buffer;
}

char * MipsDisasm::op_j(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t i = instruction.imm_jump();
	if(PC_REG == -1)
		sprintf(buffer, "j  (PC & 0xf0000000) | 0x%x", i << 2);
	else
	{
		uint32_t pc = (uint32_t)PC_REG;
		uint32_t addr = (pc & 0xf0000000) | (i << 2);
		sprintf(buffer, "j  (0x%08x & 0xf0000000) | 0x%x [0x%08x]", pc, i << 2, addr);
	}
	return buffer;
}

char * MipsDisasm::op_jal(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t i = instruction.imm_jump();
	if(PC_REG == -1)
		sprintf(buffer, "jal  (PC & 0xf0000000) | 0x%x", i << 2);
	else
	{
		uint32_t pc = (uint32_t)PC_REG;
		uint32_t addr = (pc & 0xf0000000) | (i << 2);
		sprintf(buffer, "jal  (0x%08x & 0xf0000000) | 0x%x [0x%08x]", pc, i << 2, addr);
	}
	return buffer;
}

char * MipsDisasm::op_beq(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "beq  %s, %s, %d", reg(s), reg(t), (int32_t)(i << 2));
	return buffer;
}

char * MipsDisasm::op_bne(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "bne  %s, %s, %d", reg(s), reg(t), (int32_t)(i << 2));
	return buffer;
}

char * MipsDisasm::op_blez(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "blez  %s, %d", reg(s), (int32_t)(i << 2));
	return buffer;
}

char * MipsDisasm::op_bgtz(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "bgtz  %s, %d", reg(s), (int32_t)(i << 2));
	return buffer;
}

char * MipsDisasm::op_addi(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "addi  %s, %s, 0x%x", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_addiu(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "addiu  %s, %s, 0x%x", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_slti(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "slti  %s, %s, %d", reg(t), reg(s), (int32_t)i);
	return buffer;
}

char * MipsDisasm::op_sltiu(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "sltiu  %s, %s, 0x%x", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_andi(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "andi  %s, %s, 0x%x", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_ori(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "ori  %s, %s, 0x%x", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_xori(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "xori  %s, %s, 0x%x", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lui(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t i = instruction.imm();
	RegisterIndex t = instruction.t();
	sprintf(buffer, "lui  %s,  0x%x", reg(t), i);
	return buffer;
}

char * MipsDisasm::op_cop0(InstructionDisasm instruction)
{
	char *buffer = NULL;
	switch(instruction.cop_opcode())
	{
		case 0b00000: buffer = op_mfc0(instruction); break;
		case 0b00100: buffer = op_mtc0(instruction); break;
		case 0b10000: buffer = op_rfe(instruction); break;
		default:
			printf("UNKNOWN COP0 INSTRUCTION\n");
			return NULL;
	}
	return buffer;
}

char * MipsDisasm::op_cop1(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	sprintf(buffer, "cop1");;
	return buffer;
}

char * MipsDisasm::op_cop2(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	sprintf(buffer, "cop2 (GTE)");;
	return buffer;
}

char * MipsDisasm::op_cop3(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	sprintf(buffer, "cop3");;
	return buffer;
}

char * MipsDisasm::op_mfc0(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex cpu_r = instruction.t();
	uint32_t cop_r = instruction.d().m_index;
	sprintf(buffer, "mfc0 %s, cop0r%d", reg(cpu_r), cop_r);
	return buffer;
}

char * MipsDisasm::op_mtc0(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex cpu_r = instruction.t();
	uint32_t cop_r = instruction.d().m_index;
	sprintf(buffer, "mtc0 %s, cop0r%d", reg(cpu_r), cop_r);
	return buffer;
}

char * MipsDisasm::op_rfe(InstructionDisasm instruction)
{
	char *buffer = NULL;
	if(instruction.m_data & 0x3f == 0b010000)
	{
		buffer = (char *)malloc(512*sizeof(char));
		if(buffer == NULL)
		{
			return NULL;
		}
		sprintf(buffer, "rfe");
		return buffer;
	}
	else
	{
		printf("INVALID COP0 INSTRUCTION: 0x%s\n", instruction.i2s().c_str());
		return NULL;
	}
}

char * MipsDisasm::op_lb(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lb  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lh(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lh  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lwl(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lwl  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lw(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lw  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lbu(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lbu  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lhu(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lhu  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lwr(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lwr  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_sb(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "sb  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_sh(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "sh  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_swl(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "swl  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_sw(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "sw  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_swr(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	RegisterIndex t = instruction.t();
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "swr  %s, [%s + 0x%x]", reg(t), reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lwc0(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t t = instruction.t().m_index;
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lwc0 cop0r%d, [%s + 0x%x]", t, reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lwc1(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t t = instruction.t().m_index;
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lwc1 cop0r%d, [%s + 0x%x]", t, reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lwc2(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t t = instruction.t().m_index;
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lwc2 cop0r%d, [%s + 0x%x]", t, reg(s), i);
	return buffer;
}

char * MipsDisasm::op_lwc3(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t t = instruction.t().m_index;
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "lwc3 cop0r%d, [%s + 0x%x]", t, reg(s), i);
	return buffer;
}

char * MipsDisasm::op_swc0(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t t = instruction.t().m_index;
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "swc0 cop0r%d, [%s + 0x%x]", t, reg(s), i);
	return buffer;
}

char * MipsDisasm::op_swc1(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t t = instruction.t().m_index;
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "swc1cop0r%d, [%s + 0x%x]", t, reg(s), i);
	return buffer;
}

char * MipsDisasm::op_swc2(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t t = instruction.t().m_index;
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "swc2 cop0r%d, [%s + 0x%x]", t, reg(s), i);
	return buffer;
}

char * MipsDisasm::op_swc3(InstructionDisasm instruction)
{
	char *buffer = NULL;
	buffer = (char *)malloc(512*sizeof(char));
	if(buffer == NULL)
	{
		return NULL;
	}
	uint32_t t = instruction.t().m_index;
	uint32_t i = instruction.imm_se();
	RegisterIndex s = instruction.s();
	sprintf(buffer, "swc3 cop0r%d, [%s + 0x%x]", t, reg(s), i);
	return buffer;
}
