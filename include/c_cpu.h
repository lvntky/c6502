#ifndef __C_CPU_H__
#define __C_CPU_H__

#include "c_register.h"
#include "c_address_mode.h"

typedef struct c_cpu {
	c_register_t reg;
} c_cpu_t;

// Function pointer for address mode handler
typedef uint16_t (*address_mode_handler)(c_cpu_t *cpu);

typedef struct c_instruction {
	uint8_t opcode;
	address_mode_t address_mode;
	address_mode_handler add_mode_handler;
	void (*handler)(c_cpu_t *cpu, uint16_t address); // opcpode handler
	uint8_t cycle;
} c_instruction_t;

void c_reset(c_cpu_t *cpu);

#endif //__C_CPU_H__
