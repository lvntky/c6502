#include <stdio.h>
#include "../include/cpu.h"

int main(int argc, char** argv)
{
  Cpu testCpu;
  reset_cpu(&testCpu);
  unsigned char program[] = {0xA9, 0x1F,0xA9, 0x1C, 0xAA, 0xE8};
  int program_size = sizeof(program) / sizeof(program[0]);
  run(&testCpu, program, program_size);
  printf("register x : %d\n", testCpu.register_x);
  printf("register a : %d\n", testCpu.register_a);
  printf("pc %d\n", testCpu.program_counter);
  printf("memory size: %ld\n", sizeof(testCpu.memory));
  return 0;
}