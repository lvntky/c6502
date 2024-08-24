#ifndef __C_CPU_H__
#define __C_CPU_H__

#include "c_register.h"

typedef struct c_cpu
{
    c_register_t reg;

} c_cpu_t;

void c_reset(c_cpu_t *cpu);

#endif //__C_CPU_H__
