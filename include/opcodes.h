//
// Created by levent on 7/1/23.
//

#ifndef C6502_OPCODES_H
#define C6502_OPCODES_H
#include "./cpu.h"
#include "./addressing_mode.h"

// Addressing Mode : IMPLIED
void clc(Cpu* cpu);
void cld(Cpu* cpu);
void cli(Cpu* cpu);
void clv(Cpu* cpu);
void dex(Cpu* cpu);
void dey(Cpu* cpu);
void inx(Cpu* cpu);
void iny(Cpu* cpu);
void pha(Cpu* cpu);
void php(Cpu* cpu);
void pla(Cpu* cpu);
void plp(Cpu* cpu);
void sec(Cpu* cpu);
void sed(Cpu* cpu);
void sei(Cpu* cpu);
void tax(Cpu* cpu);
void tay(Cpu* cpu);
void tsx(Cpu* cpu);
void txa(Cpu* cpu);
void txs(Cpu* cpu);
void tya(Cpu* cpu);
// Addressing Mode : IMMEDIATE
void lda(Cpu* cpu, addressing_mode mode);
void adc(Cpu* cpu, addressing_mode mode);
void and(Cpu* cpu, addressing_mode mode);

#endif //C6502_OPCODES_H
