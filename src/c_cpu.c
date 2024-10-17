#include "../include/c_cpu.h"
#include "../include/u_util.h"
#include <stdio.h>
#include <stdlib.h>

static bool exec_status = true;

void c_reset(c_cpu_t *cpu)
{
	cpu->reg.acc ^= cpu->reg.acc;
	cpu->reg.pc = 0x0000;
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

static uint16_t immediate_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	UNUSED(mem);
	// current opcode + 1;
	return cpu->reg.pc + 1;
}

static uint16_t absoulute_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	uint16_t address =
		((mem->mem[cpu->reg.pc + 2] << 8) | mem->mem[cpu->reg.pc + 1]);
	printf("absolute address mode returned : 0x%04x\n", address);
	return address;
}

static uint16_t absolute_x_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	uint16_t address =
		((mem->mem[cpu->reg.pc + 2] << 8) | mem->mem[cpu->reg.pc + 1]) +
		cpu->reg.x;
	return address;
}

static uint16_t absolute_y_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	uint16_t address =
		((mem->mem[cpu->reg.pc + 2] << 8) | mem->mem[cpu->reg.pc + 1]) +
		cpu->reg.y;
	return address;
}

static uint16_t zero_page_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	uint16_t address = mem->mem[cpu->reg.pc + 1];
	if (address < 0x00 || address > 0xff) {
		printf("zero page addressing out of bounds: 0x%d\n", address);
		exit(EXIT_FAILURE);
	}

	return address;
}

static uint16_t zero_page_x_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	uint16_t address = (mem->mem[cpu->reg.pc + 1] + cpu->reg.x) & 0xFF;
	return address;
}

static uint16_t zero_page_y_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	uint16_t address = (mem->mem[cpu->reg.pc + 1] + cpu->reg.y) & 0xFF;
	return address;
}

static uint16_t implied_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	UNUSED(cpu);
	UNUSED(mem);
	return 0; // No address needed
}

static uint16_t indirect_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	uint16_t pointer = (mem->mem[cpu->reg.pc + 2] << 8) |
			   mem->mem[cpu->reg.pc + 1];
	uint16_t address = (mem->mem[pointer + 1] << 8) | mem->mem[pointer];
	return address;
}

static uint16_t indirect_x_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	uint8_t pointer = (mem->mem[cpu->reg.pc + 1] + cpu->reg.x) & 0xFF;
	uint16_t address = (mem->mem[pointer + 1] << 8) | mem->mem[pointer];
	return address;
}

static uint16_t indirect_y_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	uint8_t pointer = mem->mem[cpu->reg.pc + 1];
	uint16_t address =
		((mem->mem[pointer + 1] << 8) | mem->mem[pointer]) + cpu->reg.y;
	return address;
}

// Im not sure of that
static uint16_t relative_address_mode(c_cpu_t *cpu, m_memory_t *mem)
{
	int8_t offset = mem->mem[cpu->reg.pc + 1];
	return cpu->reg.pc + 2 + offset;
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

void jmp_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	UNUSED(mem);
	cpu->reg.pc = address;
}

void sta_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	mem->mem[address] = cpu->reg.acc;
}

static c_instruction_t instruction_set[] = {
	{ 0xA9, ADDR_MODE_IMMEDIATE, immediate_address_mode, lda_handler,
	  2 }, // LDA Immediate
	{ 0x4C, ADDR_MODE_ABSOLUTE, absoulute_address_mode, jmp_handler,
	  3 }, // JMP Absolute
	{ 0x8D, ADDR_MODE_ABSOLUTE, absoulute_address_mode, sta_handler,
	  3 } // STA Absolute
};

void c_execute(c_cpu_t *cpu, m_memory_t *memory)
{
	if (exec_status) {
		// Fetch the opcode from memory using the program counter
		uint8_t opcode = memory->mem[cpu->reg.pc];
		printf("PC: 0x%04x, Opcode: 0x%02x\n", cpu->reg.pc,
		       opcode); // Log PC and opcode

		c_instruction_t *instruction = NULL;
		size_t set_size =
			sizeof(instruction_set) / sizeof(c_instruction_t);

		// Find the corresponding instruction in the instruction set
		for (size_t i = 0; i < set_size; i++) {
			if (opcode == instruction_set[i].opcode) {
				instruction = &instruction_set[i];
				break;
			}
		}

		if (instruction) {
			// Log the found instruction and addressing mode
			printf("Instruction found: Opcode: 0x%02x, Cycles: %d, Addressing Mode Handler: %p\n",
			       instruction->opcode, instruction->cycle,
			       (void *)instruction->add_mode_handler);

			// Execute the addressing mode handler and get the memory address
			uint16_t address =
				instruction->add_mode_handler(cpu, memory);
			printf("Calculated Address: 0x%04x\n",
			       address); // Log the calculated address

			// Update the program counter based on the cycle count
			cpu->reg.pc += instruction->cycle;
			printf("Updated PC: 0x%04x\n",
			       cpu->reg.pc); // Log the updated program counter

			// Execute the opcode handler
			instruction->opcode_handler(cpu, memory, address);
		} else {
			// Log unhandled opcode
			printf("UNHANDLED OPCODE 0x%04x at PC: 0x%04x\n",
			       opcode, cpu->reg.pc);

			// Increment the program counter to skip over the unhandled opcode
			cpu->reg.pc++;
		}
	}
}
