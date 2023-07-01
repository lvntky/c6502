cmake_minimum_required(VERSION 3.22.1)
project(c6502)
add_executable(c6502 src/main.c src/cpu.c src/opcodes.c include/cpu.h include/opcodes.h)