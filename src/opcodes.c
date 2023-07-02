//
// Created by levent on 7/1/23.
//

#include "../include/opcodes.h"
#include <stdio.h>

void lda(Cpu *cpu, addressing_mode mode) {
  uint16_t address = get_operand_address(cpu, mode);
  printf("address : %d\n", address);
  unsigned value = read_from_memory(cpu, address + 1); // operand + 1
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