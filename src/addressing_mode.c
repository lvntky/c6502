//
// Created by levent on 7/2/23.
//

#include "../include/addressing_mode.h"
#include <stdio.h>

uint16_t get_operator_address(Cpu *cpu, addressing_mode mode) {
  switch (mode) {
    case IMMEDIATE:
      return cpu->program_counter;
    case ZERO_PAGE:
      break;
    case ABSOLUTE:
      break;
    case ZERO_PAGE_X:
      break;
    case ZERO_PAGE_Y:
      break;
    case ABSOLUTE_X:
      break;
    case ABSOLUTE_Y:
      break;
    case INDIRECT_X:
      break;
    case INDIRECT_Y:
      break;
    default:
      break;
  }
}