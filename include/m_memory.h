#ifndef __M_MEMORY_H__
#define __M_MEMORY_H__

#include <stdint.h>

#define M_MEMORY_SIZE (1 << 16)

typedef struct m_memory
{
    uint16_t mem[M_MEMORY_SIZE];
} m_memory_t;

void m_reset(m_memory_t *mem);

#endif //__M_MEMORY_H__
