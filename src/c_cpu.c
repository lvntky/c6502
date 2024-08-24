#include "../include/c_cpu.h"

void c_reset(c_cpu_t *cpu)
{
	cpu->reg.acc ^= cpu->reg.acc;
	cpu->reg.pc = 0x600;
	cpu->reg.sp ^= cpu->reg.sp;
	cpu->reg.x ^= cpu->reg.x;
	cpu->reg.y ^= cpu->reg.y;

	// reset flags
	CLEAR_FLAG(cpu->reg, FLAG_BREAK);
	CLEAR_FLAG(cpu->reg, FLAG_CARRY);
	CLEAR_FLAG(cpu->reg, FLAG_DECIMAL);
	CLEAR_FLAG(cpu->reg, FLAG_INTERRUPT);
	CLEAR_FLAG(cpu->reg, FLAG_NEGATIVE);
	CLEAR_FLAG(cpu->reg, FLAG_OVERFLOW);
	CLEAR_FLAG(cpu->reg, FLAG_UNUSED);
	CLEAR_FLAG(cpu->reg, FLAG_ZERO);
}
