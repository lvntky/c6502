//
// Created by levent on 7/4/23.
//

#include "../include/memory.h"

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