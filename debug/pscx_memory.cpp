#include "pscx_memory.h"

#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"

uint32_t pscx_memory::maskRegion(uint32_t addr)
{
	// Index address space in 512MB chunks
	size_t index = (addr >> 29);

	return addr & pscx_memory::REGION_MASK[index];
}
