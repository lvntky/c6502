//
// Created by levent on 7/4/23.
//

#ifndef C6502_SRC_MEMORY_H_
#define C6502_SRC_MEMORY_H_
#include <stdint.h>
#include "./cpu.h"

uint8_t read_from_memory(Cpu *cpu, uint16_t address);
void write_to_memory(Cpu* cpu, uint16_t address, uint8_t data);
void load_program_to_memory(Cpu* cpu, const unsigned char* program, int program_size);
uint16_t read_from_memory_u16(Cpu *cpu, uint16_t address);
void write_to_memory_u16(Cpu* cpu, uint16_t address, uint16_t data);

#endif //C6502_SRC_MEMORY_H_
