
#include "disasm_main.h"
#include "disasm_op.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"

#include <iostream>
#include <string>




int disasm_main(uint32_t pc_reg_value)
{

	PC_REG = pc_reg_value;
	return 0;

}


uint32_t InstructionDisasm::function()
{
	return m_data >> 26;
}

uint32_t InstructionDisasm::subfunction()
{
	return m_data & 0x3f;
}

uint32_t InstructionDisasm::cop_opcode()
{
	return (m_data >> 21) & 0x1f;
}

RegisterIndex InstructionDisasm::s()
{
	return RegisterIndex((m_data >> 21) & 0x1f);
}

RegisterIndex InstructionDisasm::t()
{
	return RegisterIndex((m_data >> 16) & 0x1f);
}

RegisterIndex InstructionDisasm::d()
{
	return RegisterIndex((m_data >> 11) & 0x1f);
}

uint32_t InstructionDisasm::imm()
{
	return m_data & 0xffff;
}

uint32_t InstructionDisasm::imm_se()
{
	uint16_t value = (uint16_t)(m_data & 0xffff);
	return (uint32_t)value;
}

uint32_t InstructionDisasm::shift()
{
	return (m_data >> 6) & 0x1f;
}

uint32_t InstructionDisasm::imm_jump()
{
	return m_data & 0x3ffffff;
}


std::string InstructionDisasm::i2s()
{
	char buffer[1024] = { 0 };
	sprintf(buffer, "%08X", m_data);
	std::string hexstr(buffer);
	return hexstr;
}

uint32_t u32_from_str_prefix(char * str)
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
