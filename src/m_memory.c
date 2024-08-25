#include "../include/m_memory.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

//TODO: write bound check macro

#define CHECK_MEMORY(mem)    \
	assert(mem != NULL); \
	assert(mem->mem != NULL);

void m_reset(m_memory_t *mem)
{
	memset(mem->mem, 0, sizeof(mem->mem));
}

uint8_t m_read_byte(m_memory_t *mem, uint16_t addr)
{
	CHECK_MEMORY(mem)
	return mem->mem[addr];
}

void m_write_byte(m_memory_t *mem, uint16_t addr, uint8_t data)
{
	CHECK_MEMORY(mem)
	mem->mem[addr] = data;
}