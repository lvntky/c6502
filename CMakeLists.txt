cmake_minimum_required(VERSION 3.22.1)
project(c6502)
add_executable(c6502 src/main.c src/cpu.c src/opcodes.c src/addressing_mode.c include/cpu.h include/opcodes.h include/addressing_mode.h)