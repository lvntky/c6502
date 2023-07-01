#ifndef C6502_CPU_H
#define C6502_CPU_H

#include <stdint.h>

typedef struct Cpu {
    uint8_t register_a;
    uint8_t register_x;
    uint8_t status;
    uint16_t program_counter;
}Cpu;

Cpu reset_cpu (Cpu *cpu);
void lda(Cpu *cpu, uint8_t value);
void tax(Cpu *cpu);
void update_zero_and_negative_flags(Cpu *cpu, uint8_t result);
void inx(Cpu* cpu);
void interpret(Cpu *cpu, unsigned char* program, int program_size);

#endif //C6502_CPU_H
