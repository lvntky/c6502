#include "../include/m_memory.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

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

void m_load_bin(m_memory_t *mem, const char *filename)
{
	CHECK_MEMORY(mem)

	FILE *file = fopen(filename, "rb");
	if (!file) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	rewind(file);

	if (file_size > M_MEMORY_SIZE) {
		fprintf(stderr, "Error: File size exceeds memory size.\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	size_t bytes_read = fread(mem->mem, 1, file_size, file);
	if (bytes_read != file_size) {
		perror("Failed to read file");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	fclose(file);
}
