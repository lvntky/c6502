#include "../include/cpu.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

Cpu reset_cpu (Cpu *cpu) {
    cpu->status = 0;
    cpu->register_a = 0;
    cpu->program_counter = 0;
    cpu->register_x = 0;
  uint8_t register_y = 0;
    memset(cpu->memory, 0, sizeof(cpu->memory));
}

void update_zero_and_negative_flags(Cpu *cpu, uint8_t result) {
    if (result == 0) {
        cpu->status = 0b00000010;
    } else {
        cpu->status = cpu->status & 0b11111101;
    }

    if (result & 0b10000000 != 0) {
        cpu->status = cpu->status | 0b10000000;
    } else {
        cpu->status = cpu->status & 0b01111111;
    }
}

uint8_t read_from_memory(Cpu *cpu, uint16_t address) {
  uint8_t data = cpu->memory[address];
  return data;
}

uint16_t read_from_memory_u16(Cpu *cpu, uint16_t address) {
  uint16_t lo = (uint16_t)cpu->memory[address];
  uint16_t hi = (uint16_t)cpu->memory[address + 1];
  return (hi << 8) | lo;
}

void write_to_memory(Cpu* cpu, uint16_t address, uint8_t data) {
  cpu->memory[address] = data;
}

void write_to_memory_u16(Cpu* cpu, uint16_t address, uint16_t data) {
  uint8_t hi = (uint8_t)(data >> 8);
  uint8_t lo = (uint8_t)(data & 0xff);
  write_to_memory(cpu, address, lo);
  write_to_memory(cpu, (address + 1), hi);
}

void load_program_to_memory(Cpu* cpu, const unsigned char* program, int program_size) {
  for (int i = 0; i < program_size; i++) {
    write_to_memory(cpu, i, program[i]);
  }
}

uint16_t get_operand_address(Cpu *cpu, addressing_mode mode) {
  switch (mode) {
    case IMMEDIATE:
      return cpu->program_counter;
    case ZERO_PAGE:
      return read_from_memory(cpu, cpu->program_counter);
    case ABSOLUTE:
      return read_from_memory_u16(cpu, cpu->program_counter);
    case ZERO_PAGE_X:
      return read_from_memory(cpu, cpu->program_counter) + cpu->register_x;
    case ZERO_PAGE_Y:
      return read_from_memory(cpu, cpu->program_counter) + cpu->register_y;
    case ABSOLUTE_X: {
      uint16_t base = read_from_memory_u16(cpu, cpu->program_counter);
      uint16_t address = base + (uint16_t) cpu->register_x;
      return address;
    }
    case ABSOLUTE_Y: {
      uint16_t base = read_from_memory_u16(cpu, cpu->program_counter);
      uint16_t address = base + (uint16_t) cpu->register_y;
      return address;
    }
    case INDIRECT_X: {
      uint8_t base = read_from_memory(cpu, cpu->program_counter);

      uint8_t ptr = base + cpu->register_x;
      uint8_t lo = read_from_memory(cpu, ptr);
      uint8_t hi = read_from_memory(cpu, ptr + 1);
      return (hi << 8) | lo;
    }
      case INDIRECT_Y: {
      uint8_t base = read_from_memory(cpu, cpu->program_counter);

      uint8_t lo = read_from_memory(cpu, base);
      uint8_t hi = read_from_memory(cpu, (base + 1) & 0xFF);
      uint16_t deref_base = (hi << 8) | lo;
      uint16_t deref = deref_base + cpu->register_y;
      return deref;
    }
    case NONE_ADDRESSING:
      printf("mode %x unknown.", mode);
      break;
    default:
      break;
  }
}

void lda(Cpu *cpu, addressing_mode mode) {
  uint16_t address = get_operand_address(cpu, mode);
  printf("address : %d\n", address);
  unsigned value = read_from_memory(cpu, address + 1); // operand + 1
  cpu->register_a = value;
  update_zero_and_negative_flags(cpu, cpu->register_a);
}
void tax(Cpu *cpu) {
  cpu->register_x = cpu->register_a;
  update_zero_and_negative_flags(cpu, cpu->register_x);
}
void inx(Cpu* cpu) {
  cpu->register_x = (cpu->register_x + 1) & 0xFF; // fix integer overflow
  update_zero_and_negative_flags(cpu, cpu->register_x);
}

void run(Cpu* cpu, const unsigned char* program, int program_size) {
  load_program_to_memory(cpu, program, program_size);
  while (cpu->program_counter < program_size) {
    unsigned char opcode = read_from_memory(cpu, cpu->program_counter);
    switch (opcode) {
      case 0xA9:
        assert(cpu->program_counter + 1 < MEMORY_SIZE);
        addressing_mode mode = IMMEDIATE;
        lda(cpu, mode);
        cpu->program_counter += 2;
        break;
      case 0xAA:
        tax(cpu);
        cpu->program_counter += 1;
        printf("Instruction: TAX\n");
        break;
      case 0xE8:
        inx(cpu);
        cpu->program_counter += 1;
        printf("Instruction: INX\n");
        break;
      default:
        printf("Unknown opcode: %02X\n", opcode);
        assert(0 && "Unknown opcode");
    }
  }
}