//
// Created by levent on 7/4/23.
//

#include "../include/stack.h"

void push_stack(Cpu* cpu, uint8_t value) {
  cpu->memory[STACK_PAGE_START + cpu->stack_pointer] = value;
  cpu->stack_pointer--;
}
uint8_t pop_stack(Cpu* cpu) {
  uint8_t value = cpu->memory[STACK_PAGE_START + cpu->stack_pointer + 1];
  cpu->memory[STACK_PAGE_START + cpu->stack_pointer + 1] = 0;
  cpu->stack_pointer++;
  return value;
}
