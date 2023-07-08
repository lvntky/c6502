#include "../include/cpu.h"
#include "../include/opcodes.h"
#include "../include/addressing_mode.h"
#include "../include/memory.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

Cpu reset_cpu (Cpu *cpu) {
    cpu->register_a = 0;
    cpu->program_counter = 0;
    cpu->register_x = 0;
    cpu->register_y = 0;
    memset(cpu->memory, 0, sizeof(cpu->memory));
    cpu->stack_pointer = 255;
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

void update_flags_by_stack_status(Cpu* cpu, uint8_t status) {
  cpu->flags.carry = (status >> 0) & 0x01;
  cpu->flags.zero = (status >> 1) & 0x01;
  cpu->flags.interrupt_disable = (status >> 2) & 0x01;
  cpu->flags.decimal_mode = (status >> 3) & 0x01;
  cpu->flags.break_command = (status >> 4) & 0x01;
  cpu->flags.unused = 1; // Set unused flag to 1
  cpu->flags.overflow = (status >> 6) & 0x01;
  cpu->flags.negative = (status >> 7) & 0x01;
}

void print_cpu_flags(const Cpu* cpu) {
  printf("========================================\n");
  printf("Carry: %d\n", cpu->flags.carry);
  printf("Zero: %d\n", cpu->flags.zero);
  printf("Interrupt Disable: %d\n", cpu->flags.interrupt_disable);
  printf("Decimal Mode: %d\n", cpu->flags.decimal_mode);
  printf("Break Command: %d\n", cpu->flags.break_command);
  printf("Overflow: %d\n", cpu->flags.overflow);
  printf("Negative: %d\n", cpu->flags.negative);
  printf("Unused: %d\n", cpu->flags.unused);
  printf("========================================\n");
}

const struct OpcodeEntry opcode_table[] = {
    // Implied
    {0x00, brk, NONE_ADDRESSING, "BRK"},
    {0x18, clc, NONE_ADDRESSING, "CLC"},
    {0xD8, cld, NONE_ADDRESSING, "CLD"},
    {0x58, cli, NONE_ADDRESSING, "CLI"},
    {0xB8, clv, NONE_ADDRESSING, "CLV"},
    {0xCA, dex, NONE_ADDRESSING, "DEX"},
    {0x88, dey, NONE_ADDRESSING, "DEY"},
    {0xE8, inx, NONE_ADDRESSING, "INX"},
    {0xC8, iny, NONE_ADDRESSING, "INY"},
    {0xEA, nop, NONE_ADDRESSING, "NOP"},
    {0x48, pha, NONE_ADDRESSING, "PHA"},
    {0x08, php, NONE_ADDRESSING, "PHP"},
    {0x68, pla, NONE_ADDRESSING, "PLA"},
    {0x28, plp, NONE_ADDRESSING, "PLP"},
    {0x38, sec, NONE_ADDRESSING, "SEC"},
    {0xF8, sed, NONE_ADDRESSING, "SED"},
    {0x78, sei, NONE_ADDRESSING, "SEI"},
    {0xAA, tax, NONE_ADDRESSING, "TAX"},
    {0xA8, tay, NONE_ADDRESSING, "TAY"},
    {0xBA, tsx, NONE_ADDRESSING, "TSX"},
    {0x8A, txa, NONE_ADDRESSING, "TXA"},
    {0x9A, txs, NONE_ADDRESSING, "TXS"},
    {0x98, tya, NONE_ADDRESSING ,"TYA"},
    // Immediate
    {0x69, adc, IMMEDIATE, "ADC"},
    {0x29, and, IMMEDIATE, "AND"},
    {0xC9, cmp, IMMEDIATE, "CMP"},
    {0xE0, cpx, IMMEDIATE, "CPX"},
    {0xC0, cpy, IMMEDIATE, "CPY"},
    {0xA9, lda, IMMEDIATE, "LDA"},
    {0xA2, ldx, IMMEDIATE, "LDX"},
    {0xA0, ldy, IMMEDIATE, "LDY"},
    {0x09, ora, IMMEDIATE, "ORA"}
};


void run(Cpu* cpu, const unsigned char* program, int program_size) {
  load_program_to_memory(cpu, program, program_size);

  while (cpu->program_counter < program_size) {
    unsigned char opcode = read_from_memory(cpu, cpu->program_counter);

    // Search for the opcode in the lookup table
    int numOpcodes = sizeof(opcode_table) / sizeof(opcode_table[0]);
    int opcodeFound = 0;
    for (int i = 0; i < numOpcodes; i++) {
      if (opcode_table[i].opcode == opcode) {
        opcodeFound = 1;
        opcode_table[i].func(cpu, opcode_table[i].mode);
        printf("Instruction: %s\n", opcode_table[i].name);
        break;
      }
    }

    if (!opcodeFound) {
      printf("Unknown opcode: %02X\n", opcode);
      assert(0 && "Unknown opcode");
    }
  }
}