#ifndef C6502_CPU_H
#define C6502_CPU_H
#define MEMORY_SIZE (0XFFFF + 1)

#include <stdint.h>

typedef struct Cpu {
    uint8_t register_a;
    uint8_t register_x;
    uint8_t status;
    uint16_t program_counter;
    uint8_t memory[0XFFFF + 1]; //64kB
}Cpu;

typedef enum addressing_mode {
  ABSOLUTE,
  ABSOLUTE_X,
  ABSOLUTE_Y,
  ZERO_PAGE,
  ZERO_PAGE_X,
  ZERO_PAGE_Y,
  IMMEDIATE,
  INDIRECT_X,
  INDIRECT_Y,
  NONE_ADDRESSING
}addressing_mode;

Cpu reset_cpu (Cpu *cpu);
void lda(Cpu *cpu, uint8_t value);
void tax(Cpu *cpu);
void update_zero_and_negative_flags(Cpu *cpu, uint8_t result);
void inx(Cpu* cpu);
void run(Cpu* cpu, const unsigned char* program, int program_size);
uint8_t read_from_memory(Cpu *cpu, uint16_t address);
void write_to_memory(Cpu* cpu, uint16_t address, uint8_t data);
void load_program_to_memory(Cpu* cpu, const unsigned char* program, int program_size);
uint16_t get_operand_address(Cpu *cpu, addressing_mode mode);

#endif //C6502_CPU_H
