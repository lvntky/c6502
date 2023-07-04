#include <stdio.h>
#include "../include/cpu.h"
#include "../include/opcodes.h"

// DEBUG PURPOSE ONLY, DELETE IT.
int main(int argc, char** argv)
{
  Cpu testCpu;
  reset_cpu(&testCpu);

  const unsigned char program[] = {
      0x18,
      0xD8,
      0x58,
      0xB8,
      0xCA,
      0x88,
      0xE8,
      0xC8,
      0xEA,
      0x48,
      0x08,
      0x28,
      0x68,
      0x38,
      0xF8,
      0x78,
      0xAA,
      0xA8,
      0x8A,
      0x98,
      0x9A,
      0x69, 0x10,
      0x29, 0x10,
      0xC9, 0x10,
      0xE0, 0x10,
      0xC0, 0x10,
      0xA9, 0x10,
      0xA2, 0x10,
      0xA0, 0x10,
      0x09, 0x10,
      0x00
  };

  int program_size = sizeof(program) / sizeof(program[0]);
  run(&testCpu, program, program_size);
  printf("accumulator : %d\n", testCpu.register_a);
  printf("x register : %d\n", testCpu.register_x);
  printf("y register : %d\n", testCpu.register_y);
  printf("pc %d\n", testCpu.program_counter);
  printf("sp: %d\n", STACK_PAGE_START + testCpu.stack_pointer);
  for (int i = 510; i < 512; i++) {
    printf("STACK (%d) - %d\n", (i), testCpu.memory[i]);
  }

  print_cpu_flags(&testCpu);
  return 0;
}