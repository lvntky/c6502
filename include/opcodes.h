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
void adc(Cpu* cpu, addressing_mode mode);
void and(Cpu* cpu, addressing_mode mode);
void cmp(Cpu* cpu, addressing_mode mode);
void cpx(Cpu* cpu, addressing_mode mode);
void cpy(Cpu* cpu, addressing_mode mode);
void lda(Cpu* cpu, addressing_mode mode);
void ldx(Cpu* cpu, addressing_mode mode);
void ldy(Cpu* cpu, addressing_mode mode);
void ora(Cpu* cpu, addressing_mode mode);
void sbc(Cpu* cpu, addressing_mode mode);


#endif //C6502_OPCODES_H
