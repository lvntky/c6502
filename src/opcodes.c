//
// Created by levent on 7/1/23.
//

#include "../include/opcodes.h"
#include "../include/addressing_mode.h"
#include "../include/stack.h"
#include "../include/memory.h"
#include <stdio.h>

// IMPLIED
void inx(Cpu* cpu) {
  cpu->register_x = (cpu->register_x + 1) & 0xFF; // fix integer overflow
  update_zero_and_negative_flags(cpu, cpu->register_x);
}
void iny(Cpu* cpu) {
  cpu->register_y = (cpu->register_y + 1) & 0xFF; // fix integer overflow
  update_zero_and_negative_flags(cpu, cpu->register_y);
}
void pha(Cpu* cpu) {
  push_stack(cpu, cpu->register_a);
}
void php(Cpu* cpu) {
  uint8_t status = *((uint8_t*)(&cpu->flags));
  push_stack(cpu, status);
}
void pla(Cpu* cpu) {
  cpu->register_a = pop_stack(cpu);
  update_zero_and_negative_flags(cpu, cpu->register_a);
}
void plp(Cpu* cpu) {
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
}
void clc(Cpu* cpu) {
  cpu->flags.carry = 0;
}
void cld(Cpu* cpu) {
  cpu->flags.decimal_mode = 0;
}
void cli(Cpu* cpu) {
  cpu->flags.interrupt_disable = 0;
}
void clv(Cpu* cpu) {
  cpu->flags.overflow = 0;
}
void dex(Cpu* cpu) {
  cpu->register_x = (cpu->register_x - 1); // int owerflow
  update_zero_and_negative_flags(cpu, cpu->register_x);
}
void dey(Cpu* cpu) {
  cpu->register_y = (cpu->register_y - 1); // int owerflow
  update_zero_and_negative_flags(cpu, cpu->register_y);
}
void sec(Cpu* cpu) {
  cpu->flags.carry = 1;
}
void sed(Cpu* cpu) {
  cpu->flags.decimal_mode = 1;
}
void sei(Cpu* cpu) {
  cpu->flags.interrupt_disable = 1;
}
void tax(Cpu *cpu) {
  cpu->register_x = cpu->register_a;
  update_zero_and_negative_flags(cpu, cpu->register_x);
}
void tay(Cpu* cpu) {
  cpu->register_y = cpu->register_a;
  update_zero_and_negative_flags(cpu, cpu->register_y);
}
void tsx(Cpu* cpu) {
  cpu->register_x = cpu->stack_pointer;
  update_zero_and_negative_flags(cpu, cpu->register_x);
}
void txa(Cpu* cpu) {
  cpu->register_a = cpu->register_x;
  update_zero_and_negative_flags(cpu, cpu->register_a);
}
void txs(Cpu* cpu)
{
  cpu->stack_pointer = cpu->register_x;
}
void tya(Cpu* cpu) {
  cpu->register_a = cpu->register_y;
  update_zero_and_negative_flags(cpu, cpu->register_a);
}
// IMMEDIATE
void lda(Cpu *cpu, addressing_mode mode) {
  uint16_t address = get_operator_address(cpu, mode);
  printf("address : %d\n", address);
  unsigned value = read_from_memory(cpu, address + 1); // operand + 1
  cpu->register_a = value;
  update_zero_and_negative_flags(cpu, cpu->register_a);
}
void adc(Cpu* cpu, addressing_mode mode) {
  uint8_t address = get_operator_address(cpu, mode);
  uint8_t operand_value = cpu->memory[address + 1];
  uint16_t sum = cpu->register_a + operand_value + cpu->flags.carry;

  cpu->flags.carry = (sum > 0xFF) ? 1 : 0; // Set the carry flag if the sum overflows
  cpu->flags.overflow = (((cpu->register_a ^ operand_value) & 0x80) == 0 && ((cpu->register_a ^ sum) & 0x80) != 0) ? 1 : 0; // Calculate the overflow flag
  cpu->register_a = sum & 0xFF; // Store the result in the accumulator

  update_zero_and_negative_flags(cpu, cpu->register_a); // Update the zero and negative flags
}
void and(Cpu* cpu, addressing_mode mode) {
  uint8_t address = get_operator_address(cpu, mode);
  uint8_t operand_value = cpu->memory[address + 1];
  cpu->register_a &= operand_value;

  update_zero_and_negative_flags(cpu, cpu->register_a);
}
