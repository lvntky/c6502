#include "../include/cpu.h"
#include "../include/opcodes.h"
#include "../include/addressing_mode.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

Cpu reset_cpu (Cpu *cpu) {
    cpu->register_a = 0;
    cpu->program_counter = 0;
    cpu->register_x = 0;
    cpu->register_y = 0;
    memset(cpu->memory, 0, sizeof(cpu->memory));
    memset(cpu->stack, 0, sizeof(cpu->stack));
    cpu->stack_pointer = 0; // top of the stack
    return *cpu;
}

void update_zero_and_negative_flags(Cpu *cpu, uint8_t result) {
  if (result == 0) {
    cpu->flags.zero = 1;
  } else {
    cpu->flags.zero = 0;
  }

  if (result & 0b10000000) {
    cpu->flags.negative = 1;
  } else {
    cpu->flags.negative = 0;
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
void push_stack(Cpu* cpu, uint8_t value) {
  cpu->stack[cpu->stack_pointer] = value;
  cpu->stack_pointer += 1;
}
uint8_t pop_stack(Cpu* cpu) {
  uint8_t value = cpu->stack[0x100 | cpu->stack_pointer];
  cpu->stack_pointer -= 1;
  return value;
}

void run(Cpu* cpu, const unsigned char* program, int program_size) {
  load_program_to_memory(cpu, program, program_size);
  while (cpu->program_counter < program_size) {
    unsigned char opcode = read_from_memory(cpu, cpu->program_counter);
    switch (opcode) {
      // Implied
      case 0x18:
        clc(cpu);
        cpu->program_counter += 1;
        break;
      case 0xD8:
        cld(cpu);
        cpu->program_counter += 1;
        break;
      case 0x58:
        cli(cpu);
        cpu->program_counter += 1;
        break;
      case 0xB8:
        clv(cpu);
        cpu->program_counter += 1;
        break;
      case 0xCA:
        dex(cpu);
        break;
      case 0x88:
        dey(cpu);
        break;
      case 0xE8:
        inx(cpu);
        cpu->program_counter += 1;
        printf("Instruction: INX\n");
        break;
      case 0xC8:
        iny(cpu);
        cpu->program_counter += 1;
        break;
      case 0xEA:
        cpu->program_counter += 1;
        break;
      case 0x48:
        // will implement pha
        cpu->program_counter += 1;
        break;
      case 0x08:
        // will implement php
        cpu->program_counter += 1;
        break;
      case 0x68:
        // wil implement pla
        cpu->program_counter += 1;
        break;
      case 0x28:
        // will implement plp
        cpu->program_counter += 1;
        break;
      case 0x38:
        sec(cpu);
        cpu->program_counter += 1;
        break;
      case 0xF8:
        sed(cpu);
        break;
      case 0x78:
        sei(cpu);
        break;
      case 0xAA:
        tax(cpu);
        cpu->program_counter += 1;
        printf("Instruction: TAX\n");
        break;
      case 0xA8:
        tay(cpu);
        cpu->program_counter += 1;
        break;
      case 0xBA:
        tsx(cpu);
        cpu->program_counter += 1;
        break;
      case 0x8A:
        txa(cpu);
        cpu->program_counter += 1;
        break;
      case 0x9A:
        txs(cpu);
        cpu->program_counter += 1;
        break;
      case 0x98:
        tya(cpu);
        cpu->program_counter += 1;
        break;  
      case 0xA9:
        assert(cpu->program_counter + 1 < MEMORY_SIZE);
        addressing_mode mode = IMMEDIATE;
        lda(cpu, mode);
        cpu->program_counter += 2;
        break;
      default:
        printf("Unknown opcode: %02X\n", opcode);
        assert(0 && "Unknown opcode");
    }
  }
}