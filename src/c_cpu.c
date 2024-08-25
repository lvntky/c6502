#include "../include/c_cpu.h"

void c_reset(c_cpu_t *cpu)
{
	cpu->reg.acc ^= cpu->reg.acc;
	cpu->reg.pc = 0xFFFC;
	cpu->reg.sp = 0x0100;
	cpu->reg.x ^= cpu->reg.x;
	cpu->reg.y ^= cpu->reg.y;

	// reset flags
	CLEAR_FLAG(cpu->reg, FLAG_BREAK);
	CLEAR_FLAG(cpu->reg, FLAG_CARRY);
	CLEAR_FLAG(cpu->reg, FLAG_DECIMAL);
	CLEAR_FLAG(cpu->reg, FLAG_INTERRUPT);
	CLEAR_FLAG(cpu->reg, FLAG_NEGATIVE);
	CLEAR_FLAG(cpu->reg, FLAG_OVERFLOW);
	SET_FLAG(cpu->reg, FLAG_UNUSED);
	CLEAR_FLAG(cpu->reg, FLAG_ZERO);
}

static uint16_t immediate_address_mode(c_cpu_t *cpu)
{
	return cpu->reg.pc++;
}

void lda_handler(c_cpu_t *cpu, uint16_t address)
{
	// Do stuff
}

static const c_instruction_t instruction_set[] = {
	{ 0xA9, ADDR_MODE_IMMEDIATE, immediate_address_mode, lda_handler, 2 }
};
