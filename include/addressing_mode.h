//
// Created by levent on 7/2/23.
//

#ifndef C6502_SRC_ADDRESSING_MODE_H_
#define C6502_SRC_ADDRESSING_MODE_H_
#include <stdint.h>
#include "./cpu.h"

typedef enum addressing_mode {
  ABSOLUTE,
  ABSOLUTE_X,
  ABSOLUTE_Y,
  ZERO_PAGE,
  ZERO_PAGE_X,
  ZERO_PAGE_Y,
  IMMEDIATE,
  INDIRECT_X,
  INDIRECT_Y,
  NONE_ADDRESSING
}addressing_mode;

uint16_t get_operand_address(Cpu *cpu, addressing_mode mode);

#endif //C6502_SRC_ADDRESSING_MODE_H_
