//
// Created by levent on 7/4/23.
//

#ifndef C6502_SRC_STACK_H_
#define C6502_SRC_STACK_H_
#include "./cpu.h"

void push_stack(Cpu* cpu, uint8_t value);
uint8_t pop_stack(Cpu* cpu);

#endif //C6502_SRC_STACK_H_
