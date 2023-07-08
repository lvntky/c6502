//
// Created by levent on 7/1/23.
//

#ifndef C6502_OPCODES_H
#define C6502_OPCODES_H
#include "./cpu.h"
#include "./addressing_mode.h"

typedef void (*OpcodeFunc)(Cpu*, addressing_mode);
struct OpcodeEntry {
  unsigned char opcode;
  OpcodeFunc func;
  addressing_mode mode;
  char *name;
};

// Addressing Mode : IMPLIED
void brk(Cpu* cpu, addressing_mode mode);
void clc(Cpu* cpu, addressing_mode mode);
void cld(Cpu* cpu, addressing_mode mode);
void cli(Cpu* cpu, addressing_mode mode);
void clv(Cpu* cpu, addressing_mode mode);
void dex(Cpu* cpu, addressing_mode mode);
void dey(Cpu* cpu, addressing_mode mode);
void inx(Cpu* cpu, addressing_mode mode);
void iny(Cpu* cpu, addressing_mode mode);
void nop(Cpu* cpu, addressing_mode mode);
void pha(Cpu* cpu, addressing_mode mode);
void php(Cpu* cpu, addressing_mode mode);
void pla(Cpu* cpu, addressing_mode mode);
void plp(Cpu* cpu, addressing_mode mode);
void sec(Cpu* cpu, addressing_mode mode);
void sed(Cpu* cpu, addressing_mode mode);
void sei(Cpu* cpu, addressing_mode mode);
void tax(Cpu* cpu, addressing_mode mode);
void tay(Cpu* cpu, addressing_mode mode);
void tsx(Cpu* cpu, addressing_mode mode);
void txa(Cpu* cpu, addressing_mode mode);
void txs(Cpu* cpu, addressing_mode mode);
void tya(Cpu* cpu, addressing_mode mode);
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
