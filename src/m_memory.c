#include "../include/m_memory.h"
#include <string.h>

void m_reset(m_memory_t *mem) {
    memset(mem->mem, 0, sizeof(mem->mem));
}
