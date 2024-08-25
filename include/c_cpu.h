#ifndef __C_CPU_H__
#define __C_CPU_H__

#include "c_register.h"
#include "m_memory.h"
#include "c_address_mode.h"
#include <stdbool.h>

#define EXECUTE_SUCCESS 0
#define EXECUTE_FAIL 1

typedef struct c_cpu {
	c_register_t reg;
} c_cpu_t;

// Function pointer for address mode handler
typedef uint16_t (*address_mode_handler)(c_cpu_t *cpu, m_memory_t *mem);

typedef struct c_instruction {
	uint8_t opcode;
	address_mode_t address_mode;
	address_mode_handler add_mode_handler;
	void (*opcode_handler)(c_cpu_t *cpu, m_memory_t *mem,
			       uint16_t address); // opcpode handler
	uint8_t cycle;
} c_instruction_t;

/**
 * Core CPU Functions
 */
void c_reset(c_cpu_t *cpu);
void c_execute(c_cpu_t *cpu, m_memory_t *memory);

#endif //__C_CPU_H__
