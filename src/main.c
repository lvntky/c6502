#include <stdio.h>
#include "../include/cpu.h"

int main(int argc, char** argv)
{
  Cpu testCpu;
  reset_cpu(&testCpu);
  testCpu.flags.carry = 1;
  printf("before: %d", testCpu.flags.carry);
  unsigned char program[] = {0xA9, 0x1F, 0x18};
  int program_size = sizeof(program) / sizeof(program[0]);
  run(&testCpu, program, program_size);
  printf("after: %d", testCpu.flags.carry);
  printf("register x : %d\n", testCpu.register_x);
  printf("register a : %d\n", testCpu.register_a);
  printf("pc %d\n", testCpu.program_counter);
  return 0;
}