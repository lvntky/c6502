#ifndef __M_MEMORY_H__
#define __M_MEMORY_H__

#include <stdint.h>

#define M_MEMORY_SIZE (1 << 16)

typedef struct m_memory {
	uint8_t mem[M_MEMORY_SIZE];
} m_memory_t;

void m_reset(m_memory_t *mem);
uint8_t m_read_byte(m_memory_t *mem, uint16_t addr);
void m_write_byte(m_memory_t *mem, uint16_t addr, uint8_t data);
void m_load_bin(m_memory_t *mem, const char *filename);

#endif //__M_MEMORY_H__
