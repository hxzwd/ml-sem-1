 #include "pscx_bios.h"

#include <fstream>
#include <iterator>
#include "stdint.h"

#include "stdlib.h"
#include "stdio.h"

#define D_BIOS_DEBUG


Bios::BiosState Bios::loadBios(std::string path)
{

	FILE *file;
	if((file  = fopen(path.c_str(), "rb")) == NULL)
	{
		return BIOS_STATE_INCORRECT_FILENAME;
	}

	size_t bios_size = 512 * 1024;
	size_t bytes_readed;
	m_data.resize(bios_size);

	bytes_readed = fread(m_data.data(), sizeof(uint8_t), bios_size, file);

	fclose(file);


#ifdef D_BIOS_DEBUG

	printBiosDebugInfo(std::string(""), std::string("dump 128 0 16 4"));

#endif

	if(bytes_readed != bios_size)
		return BIOS_STATE_INVALID_BIOS_SIZE;

	return BIOS_STATE_SUCCESS;

}

uint32_t Bios::load32(uint32_t offset)
{
	uint32_t b0 = m_data[offset + 0];
	uint32_t b1 = m_data[offset + 1];
	uint32_t b2 = m_data[offset + 2];
	uint32_t b3 = m_data[offset + 3];

	return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}


void Bios::printBiosDebugInfo(std::string msg, std::string info)
{
	std::vector<std::string> params;
	std::string param_buf;

	std::stringstream params_stream(info);

	while(params_stream >> param_buf)
	{
		params.push_back(param_buf);
	}

	if(params[0] == std::string("dump"))
	{
		
		size_t bytes_to_print;
		size_t bytes_offset = 0;
		size_t format1 = 8;
		size_t format2 = 4;
		if(params.size() < 2)
			bytes_to_print = 33;
		else
			bytes_to_print = (size_t)strtoul(params[1].c_str(), (char **)NULL, 10) + 1;

		if(params.size() >= 3)
			bytes_offset = (size_t)strtoul(params[2].c_str(), (char **)NULL, 10);

		if(params.size() >= 4)
			format1 = (size_t)strtoul(params[3].c_str(), (char **)NULL, 10);

		if(params.size() >= 5)
			format2 = (size_t)strtoul(params[4].c_str(), (char **)NULL, 10);

		printf("BIOS DATA[bytes: %d; bytes_offset: %d; format1,2: %d,%d]:\n", bytes_to_print - 1, bytes_offset, format1, format2);
		for(int i = 1; i < bytes_to_print; i++)
		{
			if(i == 1)
				printf("%d\t0x", bytes_offset);
			printf("%.2X", m_data[i - 1 + bytes_offset]);
			if(i % format1 == 0 && i != bytes_to_print - 1)
			{
				printf("\n%d\t0x", i + bytes_offset);
				continue;
			}
			if(i % format2 == 0 && i != bytes_to_print - 1)
				printf("\t0x");

		}
		printf("\n\n");
	
	}


}