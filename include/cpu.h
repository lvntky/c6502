#ifndef C6502_CPU_H
#define C6502_CPU_H
#define MEMORY_SIZE (0XFFFF + 1)
#define STACK_PAGE_START 0x100

#include <stdint.h>

typedef struct Cpu {
    uint8_t register_a;
    uint8_t register_x;
    uint8_t register_y;
    uint16_t program_counter;
    uint8_t memory[0XFFFF + 1]; //64kB
    uint8_t stack_pointer;
    struct {
      uint8_t carry : 1;
      uint8_t zero : 1;
      uint8_t interrupt_disable : 1;
      uint8_t decimal_mode : 1;
      uint8_t break_command : 1;
      uint8_t overflow : 1;
      uint8_t negative : 1;
      uint8_t unused : 1;
    } flags;
}Cpu;

Cpu reset_cpu (Cpu *cpu);
void update_zero_and_negative_flags(Cpu *cpu, uint8_t result);
void update_flags_by_stack_status(Cpu* cpu, uint8_t status);
void run(Cpu* cpu, const unsigned char* program, int program_size);
void print_cpu_flags(const Cpu* cpu);
#endif //C6502_CPU_H