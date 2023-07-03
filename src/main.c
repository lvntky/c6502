#include <stdio.h>
#include "../include/cpu.h"

int main(int argc, char** argv)
{
  Cpu testCpu;
  reset_cpu(&testCpu);
  testCpu.flags.carry = 1;
  unsigned char program[] = {0xA9, 0x1F, 0x48, 0xA9, 0x10, 0x48};
  int program_size = sizeof(program) / sizeof(program[0]);
  run(&testCpu, program, program_size);
  printf("register x : %d\n", testCpu.register_x);
  printf("register a : %d\n", testCpu.register_a);
  printf("pc %d\n", testCpu.program_counter);
  int i = 0;
  while (testCpu.stack[i] != 0) {
    printf("stack: %d\n", testCpu.stack[i]);
    i++;
  }
  return 0;
}