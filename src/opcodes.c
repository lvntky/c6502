//
// Created by levent on 7/1/23.
//

#include "../include/opcodes.h"
#include "../include/addressing_mode.h"
#include "../include/stack.h"
#include "../include/memory.h"
#include <stdio.h>

// IMPLIED
void brk(Cpu* cpu, addressing_mode mode) {
  cpu->flags.break_command = 1;
  cpu->program_counter += 1;
}
void inx(Cpu* cpu, addressing_mode mode) {
  cpu->register_x = (cpu->register_x + 1) & 0xFF; // fix integer overflow
  update_zero_and_negative_flags(cpu, cpu->register_x);
  cpu->program_counter += 1;
}
void iny(Cpu* cpu, addressing_mode mode) {
  cpu->register_y = (cpu->register_y + 1) & 0xFF; // fix integer overflow
  update_zero_and_negative_flags(cpu, cpu->register_y);
  cpu->program_counter += 1;
}
void nop(Cpu* cpu, addressing_mode mode) {
  cpu->program_counter += 1;
}
void pha(Cpu* cpu, addressing_mode mode) {
  push_stack(cpu, cpu->register_a);
  cpu->program_counter += 1;
}
void php(Cpu* cpu, addressing_mode mode) {
  uint8_t status = *((uint8_t*)(&cpu->flags));
  push_stack(cpu, status);
  cpu->program_counter += 1;
}
void pla(Cpu* cpu, addressing_mode mode) {
  cpu->register_a = pop_stack(cpu);
  update_zero_and_negative_flags(cpu, cpu->register_a);
  cpu->program_counter += 1;
}
void plp(Cpu* cpu, addressing_mode mode) {
  uint8_t status = pop_stack(cpu);

  // Update the individual flag bits in the flags field of the Cpu structure
  cpu->flags.carry = (status >> 0) & 1;
  cpu->flags.zero = (status >> 1) & 1;
  cpu->flags.interrupt_disable = (status >> 2) & 1;
  cpu->flags.decimal_mode = (status >> 3) & 1;
  cpu->flags.break_command = (status >> 4) & 1;
  cpu->flags.overflow = (status >> 6) & 1;
  cpu->flags.negative = (status >> 7) & 1;

  // Set the unused flag to 1 (as it is always unused in the 6502)
  cpu->flags.unused = 1;
  cpu->program_counter += 1;
}
void clc(Cpu* cpu, addressing_mode mode) {
  cpu->flags.carry = 0;
  cpu->program_counter += 1;
}
void cld(Cpu* cpu, addressing_mode mode) {
  cpu->flags.decimal_mode = 0;
  cpu->program_counter += 1;
}
void cli(Cpu* cpu, addressing_mode mode) {
  cpu->flags.interrupt_disable = 0;
  cpu->program_counter += 1;
}
void clv(Cpu* cpu, addressing_mode mode) {
  cpu->flags.overflow = 0;
  cpu->program_counter += 1;
}
void dex(Cpu* cpu, addressing_mode mode) {
  cpu->register_x = (cpu->register_x - 1); // int owerflow
  update_zero_and_negative_flags(cpu, cpu->register_x);
  cpu->program_counter += 1;
}
void dey(Cpu* cpu, addressing_mode mode) {
  cpu->register_y = (cpu->register_y - 1); // int owerflow
  update_zero_and_negative_flags(cpu, cpu->register_y);
  cpu->program_counter += 1;
}
void sec(Cpu* cpu, addressing_mode mode) {
  cpu->flags.carry = 1;
  cpu->program_counter += 1;
}
void sed(Cpu* cpu, addressing_mode mode) {
  cpu->flags.decimal_mode = 1;
  cpu->program_counter += 1;
}
void sei(Cpu* cpu, addressing_mode mode) {
  cpu->flags.interrupt_disable = 1;
  cpu->program_counter += 1;
}
void tax(Cpu *cpu, addressing_mode mode) {
  cpu->register_x = cpu->register_a;
  update_zero_and_negative_flags(cpu, cpu->register_x);
  cpu->program_counter += 1;
}
void tay(Cpu* cpu, addressing_mode mode) {
  cpu->register_y = cpu->register_a;
  update_zero_and_negative_flags(cpu, cpu->register_y);
  cpu->program_counter += 1;
}
void tsx(Cpu* cpu, addressing_mode mode) {
  cpu->register_x = cpu->stack_pointer;
  update_zero_and_negative_flags(cpu, cpu->register_x);
  cpu->program_counter += 1;
}
void txa(Cpu* cpu, addressing_mode mode) {
  cpu->register_a = cpu->register_x;
  update_zero_and_negative_flags(cpu, cpu->register_a);
  cpu->program_counter += 1;
}
void txs(Cpu* cpu, addressing_mode mode)
{
  cpu->stack_pointer = cpu->register_x;
  cpu->program_counter += 1;
}
void tya(Cpu* cpu, addressing_mode mode) {
  cpu->register_a = cpu->register_y;
  update_zero_and_negative_flags(cpu, cpu->register_a);
  cpu->program_counter += 1;
}
// IMMEDIATE
void adc(Cpu* cpu, addressing_mode mode) {
  uint16_t address = get_operator_address(cpu, mode);
  uint8_t operand_value = cpu->memory[address + 1];
  uint16_t sum = cpu->register_a + operand_value + cpu->flags.carry;

  cpu->flags.carry = (sum > 0xFF) ? 1 : 0; // Set the carry flag if the sum overflows
  cpu->flags.overflow = (((cpu->register_a ^ operand_value) & 0x80) == 0 && ((cpu->register_a ^ sum) & 0x80) != 0) ? 1 : 0; // Calculate the overflow flag
  cpu->register_a = sum & 0xFF; // Store the result in the accumulator

  update_zero_and_negative_flags(cpu, cpu->register_a); // Update the zero and negative flags
  cpu->program_counter += 2;
}
void and(Cpu* cpu, addressing_mode mode) {
  uint16_t address = get_operator_address(cpu, mode);
  uint8_t operand_value = cpu->memory[address + 1];
  cpu->register_a &= operand_value;

  update_zero_and_negative_flags(cpu, cpu->register_a);
  cpu->program_counter += 2;
}
void cmp(Cpu* cpu, addressing_mode mode) {
  uint16_t address = get_operator_address(cpu, mode);
  uint8_t value = cpu->memory[address + 1];
  uint8_t result = cpu->register_a - value;
  update_zero_and_negative_flags(cpu, result);
  cpu->flags.carry = (cpu->register_a >= value) ? 1 : 0;
  cpu->program_counter += 2;
}
void cpx(Cpu* cpu, addressing_mode mode) {
  uint16_t address = get_operator_address(cpu, mode);
  uint8_t value = cpu->memory[address + 1];
  uint8_t result = cpu->register_x - value;
  update_zero_and_negative_flags(cpu, result);
  cpu->flags.carry = (cpu->register_x >= value) ? 1 : 0;
  cpu->program_counter += 2;
}
void cpy(Cpu* cpu, addressing_mode mode) {
  uint16_t address = get_operator_address(cpu, mode);
  uint8_t value = cpu->memory[address + 1];
  uint8_t result = cpu->register_y - value;
  update_zero_and_negative_flags(cpu, result);
  cpu->flags.carry = (cpu->register_y >= value) ? 1 : 0;
  cpu->program_counter += 2;
}
void ldx(Cpu *cpu, addressing_mode mode) {
  uint16_t address = get_operator_address(cpu, mode);
  unsigned value = read_from_memory(cpu, address + 1); // operand + 1
  cpu->register_x = value;
  update_zero_and_negative_flags(cpu, cpu->register_x);
  cpu->program_counter += 2;
}
void ldy(Cpu *cpu, addressing_mode mode) {
  uint16_t address = get_operator_address(cpu, mode);
  unsigned value = read_from_memory(cpu, address + 1); // operand + 1
  cpu->register_y = value;
  update_zero_and_negative_flags(cpu, cpu->register_y);
  cpu->program_counter += 2;
}
void lda(Cpu *cpu, addressing_mode mode) {
  uint16_t address = get_operator_address(cpu, mode);
  unsigned value = read_from_memory(cpu, address + 1); // operand + 1
  cpu->register_a = value;
  update_zero_and_negative_flags(cpu, cpu->register_a);
  cpu->program_counter += 2;
}
void ora(Cpu* cpu, addressing_mode mode) {
  uint16_t address = get_operator_address(cpu, mode);
  uint8_t value = cpu->memory[address + 1];
  cpu->register_a |= value;
  update_zero_and_negative_flags(cpu, cpu->register_a);
  cpu->program_counter += 2;
}
void sbc(Cpu* cpu, addressing_mode mode);