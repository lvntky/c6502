#include <stdio.h>
#include "../include/cpu.h"
#include "../include/opcodes.h"

// DEBUG PURPOSE ONLY, DELETE IT.
int main(int argc, char** argv)
{
  Cpu testCpu;
  reset_cpu(&testCpu);
  testCpu.flags.interrupt_disable = 1;
  unsigned char program[] = {0x08};
  int program_size = sizeof(program) / sizeof(program[0]);
  run(&testCpu, program, program_size);
  testCpu.flags.interrupt_disable = 0;
  unsigned char program2[] = {0x28};
  int program_size2 = sizeof(program2) / sizeof(program2[0]);
  run(&testCpu, program2, program_size2);
  printf("register x : %d\n", testCpu.register_x);
  printf("register a : %d\n", testCpu.register_a);
  printf("pc %d\n", testCpu.program_counter);
  printf("sp: %d\n", STACK_PAGE_START + testCpu.stack_pointer);
  for (int i = 256; i < 512; i++) {
    printf("STACK (%d) - %d\n", (i), testCpu.memory[i]);
  }
  plp(&testCpu);

  printf("flag interrupt disable : %d\n", testCpu.flags.interrupt_disable);
  return 0;
}