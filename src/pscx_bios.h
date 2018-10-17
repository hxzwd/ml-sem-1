#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "stdint.h"

struct Range
{
	Range(uint32_t start, uint32_t length) :
		m_start(start),
		m_length(length)
	{
	}

	bool contains(uint32_t addr, uint32_t &offset)
	{
		
		
		if(addr >= m_start && addr < m_start + m_length)
		{
			offset = addr - m_start;
			return true;
		}

		return false;
	}

	uint32_t m_start, m_length;
};

// BIOS image
struct Bios
{
	// BIOS images are always 512KB in length
	Bios() : m_range(0xbfc00000, 512 * 1024)
	{
	}

	enum BiosState
	{
		BIOS_STATE_SUCCESS,
		BIOS_STATE_INCORRECT_FILENAME,
		BIOS_STATE_INVALID_BIOS_SIZE
	};

	// BIOS memory
	std::vector<uint8_t> m_data;
	Range m_range;

	// Load a BIOS image from the file that is located in 'path'
	BiosState loadBios(std::string path);

	// Fetch the 32 bit little endian word at 'offset'
	uint32_t load32(uint32_t offset);



	void printBiosDebugInfo(std::string msg, std::string info);



};
