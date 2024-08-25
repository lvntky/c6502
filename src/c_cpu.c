#include "../include/c_cpu.h"
#include <stdio.h>

static bool exec_status = true;

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

void lda_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	cpu->reg.acc = mem->mem[address];
	if (cpu->reg.acc == 0) {
		SET_FLAG(cpu->reg, FLAG_ZERO);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_ZERO);
	}
	// Update negative flag
	if (cpu->reg.acc & 0x80) {
		SET_FLAG(cpu->reg, FLAG_NEGATIVE);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_NEGATIVE);
	}
}

static c_instruction_t instruction_set[] = {
	{ 0xA9, ADDR_MODE_IMMEDIATE, immediate_address_mode, lda_handler, 2 }
};

void c_execute(c_cpu_t *cpu, m_memory_t *memory)
{
	if (exec_status) {
		uint8_t opcode = memory->mem[cpu->reg.pc];
		c_instruction_t *instruction = NULL;

		size_t set_size =
			sizeof(instruction_set) / sizeof(c_instruction_t);

		for (size_t i = 0; i < set_size; i++) {
			if (opcode == instruction_set[i].opcode) {
				instruction = &instruction_set[i];
				break;
			}
		}

		if (instruction) {
			uint16_t address = instruction->add_mode_handler(cpu);
			instruction->opcode_handler(cpu, memory, address);
			cpu->reg.pc += instruction->cycle;
		} else {
			printf("UNHANDLED OPCODE 0x%04x\n", opcode);
			exec_status = false;
		}
	}
}
