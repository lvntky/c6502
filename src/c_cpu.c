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
	int8_t offset = mem->mem[cpu->reg.pc + 1]; // Offset is signed
	return cpu->reg.pc + 2 +
	       offset; // PC is incremented by 2 (opcode + operand)
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

// Load X register
void ldx_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	cpu->reg.x = mem->mem[address];
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

// Load Y register
void ldy_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	cpu->reg.y = mem->mem[address];
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

void txa_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	UNUSED(mem);
	UNUSED(address);
	cpu->reg.acc = cpu->reg.x;

	// Update zero flag
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

void pha_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	UNUSED(address);
	mem->mem[0x0100 + cpu->reg.sp] =
		cpu->reg.acc; // Store accumulator in stack
	cpu->reg.sp--; // Decrement stack pointer
}

void pla_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	UNUSED(address);
	cpu->reg.acc = mem->mem[++cpu->reg.sp];
}

void decx_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	UNUSED(mem); // DECX doesn't use memory
	UNUSED(address); // DECX doesn't use memory

	cpu->reg.x--; // Decrement X register

	// Update Zero flag
	if (cpu->reg.x == 0) {
		SET_FLAG(cpu->reg, FLAG_ZERO);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_ZERO);
	}

	// Update Negative flag
	if (cpu->reg.x & 0x80) { // Check if the sign bit is set (bit 7)
		SET_FLAG(cpu->reg, FLAG_NEGATIVE);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_NEGATIVE);
	}
}

void inx_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	UNUSED(mem);
	UNUSED(address);
	cpu->reg.x++;

	// Update zero flag
	if (cpu->reg.x == 0) {
		SET_FLAG(cpu->reg, FLAG_ZERO);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_ZERO);
	}

	// Update negative flag
	if (cpu->reg.x & 0x80) {
		SET_FLAG(cpu->reg, FLAG_NEGATIVE);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_NEGATIVE);
	}
}

void iny_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	UNUSED(mem);
	UNUSED(address);
	cpu->reg.y++;

	// Update zero flag
	if (cpu->reg.y == 0) {
		SET_FLAG(cpu->reg, FLAG_ZERO);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_ZERO);
	}

	// Update negative flag
	if (cpu->reg.y & 0x80) {
		SET_FLAG(cpu->reg, FLAG_NEGATIVE);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_NEGATIVE);
	}
}

void cpy_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	uint8_t value = mem->mem[address];
	uint16_t result = cpu->reg.y - value;

	// Zero flag
	if (result == 0) {
		SET_FLAG(cpu->reg, FLAG_ZERO);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_ZERO);
	}

	// Carry flag
	if (cpu->reg.y >= value) {
		SET_FLAG(cpu->reg, FLAG_CARRY);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_CARRY);
	}

	// Negative flag
	if (result & 0x80) {
		SET_FLAG(cpu->reg, FLAG_NEGATIVE);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_NEGATIVE);
	}
}

void bne_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	UNUSED(address); // The address is not used directly by BNE

	// Get the signed offset from the instruction
	int8_t offset = (int8_t)mem->mem[cpu->reg.pc + 1];

	// Log the Zero flag and the offset
	printf("BNE: Zero flag is %d, Offset is %d\n",
	       (cpu->reg.status & FLAG_ZERO) != 0, offset);

	// Branch if the Zero flag is NOT set
	if ((cpu->reg.status & FLAG_ZERO) == 0) {
		// Apply the relative branch by adding the offset to the PC
		cpu->reg.pc +=
			offset; // Include the size of the instruction (2 bytes)
		printf("BNE Taken: PC set to 0x%04X\n", cpu->reg.pc);
	} else {
		// If Zero flag is set, move to the next instruction
		cpu->reg.pc +=
			2; // Just increment PC by 2 bytes (BNE instruction length)
		printf("BNE Not Taken: PC set to 0x%04X\n", cpu->reg.pc);
	}
}

void brk_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	UNUSED(cpu);
	UNUSED(mem);
	UNUSED(address);
	/*
	while (1) {
		//pseudo halt
	}
	*/
}

void cpx_handler(c_cpu_t *cpu, m_memory_t *mem, uint16_t address)
{
	uint8_t value = mem->mem[address]; // Get the value to compare
	uint16_t result = cpu->reg.x - value;

	// Zero flag: Set if X == value
	if (result == 0) {
		SET_FLAG(cpu->reg, FLAG_ZERO);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_ZERO);
	}

	// Carry flag: Set if X >= value
	if (cpu->reg.x >= value) {
		SET_FLAG(cpu->reg, FLAG_CARRY);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_CARRY);
	}

	// Negative flag: Set if result of X - value is negative (bit 7 is 1)
	if (result & 0x80) {
		SET_FLAG(cpu->reg, FLAG_NEGATIVE);
	} else {
		CLEAR_FLAG(cpu->reg, FLAG_NEGATIVE);
	}

	// Log the flags after the CPX instruction
	printf("CPX Result: X = %02X, Value = %02X, Zero = %d, Carry = %d, Negative = %d\n",
	       cpu->reg.x, value, (cpu->reg.status & FLAG_ZERO) != 0,
	       (cpu->reg.status & FLAG_CARRY) != 0,
	       (cpu->reg.status & FLAG_NEGATIVE) != 0);
}
// Update instruction_set to include an instruction length
static c_instruction_t instruction_set[] = {
	{ 0x00, ADDR_MODE_IMMEDIATE, immediate_address_mode, brk_handler,
	  1 }, // BRK
	{ 0xA9, ADDR_MODE_IMMEDIATE, immediate_address_mode, lda_handler,
	  2 }, // LDA Immediate
	{ 0xA2, ADDR_MODE_IMMEDIATE, immediate_address_mode, ldx_handler,
	  2 }, // LDX Immediate
	{ 0xA0, ADDR_MODE_IMMEDIATE, immediate_address_mode, ldy_handler,
	  2 }, // LDX Immediate
	{ 0x4C, ADDR_MODE_ABSOLUTE, absoulute_address_mode, jmp_handler,
	  3 }, // JMP Absolute
	{ 0x8D, ADDR_MODE_ABSOLUTE, absoulute_address_mode, sta_handler,
	  3 }, // STA Absolute
	{ 0x8A, ADDR_MODE_IMPLIED, implied_address_mode, txa_handler,
	  1 }, // TXA
	{ 0x48, ADDR_MODE_IMPLIED, implied_address_mode, pha_handler,
	  1 }, // PHA
	{ 0x68, ADDR_MODE_IMPLIED, implied_address_mode, pla_handler,
	  1 }, // PLA
	{ 0xE8, ADDR_MODE_IMPLIED, implied_address_mode, inx_handler,
	  1 }, // INX
	{ 0xC8, ADDR_MODE_IMPLIED, implied_address_mode, iny_handler,
	  1 }, // INY
	{ 0xC0, ADDR_MODE_IMMEDIATE, immediate_address_mode, cpy_handler,
	  2 }, // CPY Immediate
	{ 0xD0, ADDR_MODE_RELATIVE, relative_address_mode, bne_handler,
	  2 }, // BNE
	{ 0x99, ADDR_MODE_ABSOLUTE_Y, absolute_y_address_mode, sta_handler,
	  3 }, // STA Absolute,Y
	{ 0xE0, ADDR_MODE_IMMEDIATE, immediate_address_mode, cpx_handler,
	  2 }, // CPX Immediate
	{ 0xCA, ADDR_MODE_IMPLIED, implied_address_mode, decx_handler,
	  1 }, // DECX Implied

};

// Update program counter based on the instruction length
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
			printf("Instruction found: Opcode: 0x%02x, Length: %d\n",
			       instruction->opcode, instruction->cycle);

			// Execute the addressing mode handler and get the memory address
			uint16_t address =
				instruction->add_mode_handler(cpu, memory);
			printf("Calculated Address: 0x%04x\n",
			       address); // Log the calculated address

			// Update the program counter based on the instruction length (not cycles)

			// Execute the opcode handler
			instruction->opcode_handler(cpu, memory, address);

			cpu->reg.pc += instruction->cycle;
			printf("Updated PC: 0x%04x\n",
			       cpu->reg.pc); // Log the updated program counter
		} else {
			// Log unhandled opcode
			printf("UNHANDLED OPCODE 0x%04x at PC: 0x%04x\n",
			       opcode, cpu->reg.pc);

			// Increment the program counter to skip over the unhandled opcode
			cpu->reg.pc++;
		}
	}
}
