//
// Created by levent on 7/1/23.
//

#ifndef C6502_OPCODES_H
#define C6502_OPCODES_H
#include "./cpu.h"
#include "./addressing_mode.h"

void lda(Cpu* cpu, addressing_mode mode);
void tax(Cpu* cpu);
void inx(Cpu* cpu);
void iny(Cpu* cpu);
void clc(Cpu* cpu);
void cld(Cpu* cpu);
void cli(Cpu* cpu);
void clv(Cpu* cpu);
void dex(Cpu* cpu);
void dey(Cpu* cpu);
void pha(Cpu* cpu);

#endif //C6502_OPCODES_H
