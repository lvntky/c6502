#include "../include/cpu.h"
#include <stdio.h>
#include <assert.h>

Cpu reset_cpu (Cpu *cpu) {
    cpu->status = 0;
    cpu->register_a = 0;
    cpu->program_counter = 0;
    cpu->register_x = 0;
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

void lda(Cpu *cpu, uint8_t value) {
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

void interpret(Cpu* cpu, unsigned char* program, int program_size) {
  while (cpu->program_counter < program_size) {
    unsigned char opcode = program[cpu->program_counter];
    switch (opcode) {
      case 0xA9:
        assert(cpu->program_counter + 1 < program_size);
        unsigned char value = program[cpu->program_counter + 1];
        lda(cpu, value);
        cpu->program_counter += 2;
        printf("Instruction: LDA, Value: %02X\n", value);
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
        printf("%d\n", value);
        assert(0 && "Unknown opcode");
    }
  }
}