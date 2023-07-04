#include <stdio.h>
#include "../include/cpu.h"

int main(int argc, char** argv)
{
  Cpu testCpu;
  reset_cpu(&testCpu);
  testCpu.flags.carry = 1;
  unsigned char program[] = {0xA9, 0x1F, 0x48, 0xA9, 0x10, 0x48, 0x68};
  int program_size = sizeof(program) / sizeof(program[0]);
  run(&testCpu, program, program_size);
  printf("register x : %d\n", testCpu.register_x);
  printf("register a : %d\n", testCpu.register_a);
  printf("pc %d\n", testCpu.program_counter);
  printf("sp: %d\n", STACK_PAGE_START + testCpu.stack_pointer);
  for (int i = 256 ; i < 512; i++) {
    printf("STACK (%d) - %d\n", (i), testCpu.memory[i]);
  }
  return 0;
}