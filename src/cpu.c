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


void run(Cpu* cpu, const unsigned char* program, int program_size) {
  load_program_to_memory(cpu, program, program_size);
  while (cpu->program_counter < program_size) {
    unsigned char opcode = read_from_memory(cpu, cpu->program_counter);
    switch (opcode) {
      // Implied
      case 0x00:
        cpu->program_counter += 1;
        cpu->flags.break_command = 1;
        printf("Instruction: BRK\n");
        return;
      case 0x18:
        clc(cpu);
        cpu->program_counter += 1;
        printf("Instruction: CLC\n");
        break;
      case 0xD8:
        cld(cpu);
        cpu->program_counter += 1;
        printf("Instruction: CLD\n");
        break;
      case 0x58:
        cli(cpu);
        cpu->program_counter += 1;
        printf("Instruction: CLI\n");
        break;
      case 0xB8:
        clv(cpu);
        cpu->program_counter += 1;
        printf("Instruction: CLV\n");
        break;
      case 0xCA:
        dex(cpu);
        printf("Instruction: DEX\n");
        cpu->program_counter += 1;
        break;
      case 0x88:
        dey(cpu);
        printf("Instruction: DEY\n");
        cpu->program_counter += 1;
        break;
      case 0xE8:
        inx(cpu);
        cpu->program_counter += 1;
        printf("Instruction: INX\n");
        break;
      case 0xC8:
        iny(cpu);
        cpu->program_counter += 1;
        printf("Instruction: INY\n");
        break;
      case 0xEA:
        cpu->program_counter += 1;
        printf("Instruction: NOP\n");
        break;
      case 0x48:
        pha(cpu);
        cpu->program_counter += 1;
        printf("Instruction: PHA\n");
        break;
      case 0x08:
        php(cpu);
        cpu->program_counter += 1;
        printf("Instruction: PHP\n");
        break;
      case 0x68:
        pla(cpu);
        cpu->program_counter += 1;
        printf("Instruction: PLA\n");
        break;
      case 0x28:
        plp(cpu);
        cpu->program_counter += 1;
        printf("Instruction: PLP\n");
        break;
      case 0x38:
        sec(cpu);
        cpu->program_counter += 1;
        printf("Instruction: SEC\n");
        break;
      case 0xF8:
        sed(cpu);
        cpu->program_counter += 1;
        printf("Instruction: SED\n");
        break;
      case 0x78:
        sei(cpu);
        cpu->program_counter += 1;
        printf("Instruction: SEI\n");
        break;
      case 0xAA:
        tax(cpu);
        cpu->program_counter += 1;
        printf("Instruction: TAX\n");
        break;
      case 0xA8:
        tay(cpu);
        cpu->program_counter += 1;
        printf("Instruction: TAY\n");
        break;
      case 0xBA:
        tsx(cpu);
        cpu->program_counter += 1;
        printf("Instruction: TSX\n");
        break;
      case 0x8A:
        txa(cpu);
        cpu->program_counter += 1;
        printf("Instruction: TXA\n");
        break;
      case 0x9A:
        txs(cpu);
        cpu->program_counter += 1;
        printf("Instruction: TXS\n");
        break;
      case 0x98:
        tya(cpu);
        cpu->program_counter += 1;
        printf("Instruction: TYA\n");
        break;
        // Immediate
      case 0x69:
        adc(cpu, IMMEDIATE);
        cpu->program_counter += 2;
        printf("Instruction: ADC\n");
        break;
      case 0x29:
        and(cpu, IMMEDIATE);
        cpu->program_counter += 2;
        printf("Instruction: AND\n");
        break;
      case 0xC9:
        cmp(cpu, IMMEDIATE);
        cpu->program_counter += 2;
        printf("Instruction: CMP\n");
        break;
      case 0xE0:
        cpx(cpu, IMMEDIATE);
        cpu->program_counter += 2;
        printf("Instruction: CPX\n");
        break;
      case 0xC0:
        cpy(cpu, IMMEDIATE);
        cpu->program_counter += 2;
        printf("Instruction: CPY\n");
        break;
      case 0xA9:
        assert(cpu->program_counter + 1 < MEMORY_SIZE);
        addressing_mode mode = IMMEDIATE;
        lda(cpu, mode);
        cpu->program_counter += 2;
        printf("Instruction: LDA\n");
        break;
      case 0xA2:
        assert(cpu->program_counter + 1 < MEMORY_SIZE);
        ldx(cpu, IMMEDIATE);
        cpu->program_counter += 2;
        printf("Instruction: LDX\n");
        break;
      case 0xA0:
        assert(cpu->program_counter + 1 < MEMORY_SIZE);
        ldy(cpu, IMMEDIATE);
        cpu->program_counter += 2;
        printf("Instruction: LDY\n");
        break;
      case 0x09:
        ora(cpu, IMMEDIATE);
        cpu->program_counter += 2;
        printf("Instruction: ORA\n");
        break;
      default:
        printf("Unknown opcode: %02X\n", opcode);
        assert(0 && "Unknown opcode");
    }
  }
}