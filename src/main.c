#include <stdio.h>
#include "../include/cpu.h"
#include "../include/opcodes.h"

// DEBUG PURPOSE ONLY, DELETE IT.
int main(int argc, char** argv)
{
  Cpu testCpu;
  reset_cpu(&testCpu);

  unsigned char program[] = {0x29, 0x10};
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