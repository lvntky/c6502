//
// Created by levent on 7/2/23.
//

#include "../include/addressing_mode.h"
#include <stdio.h>

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