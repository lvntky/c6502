#include "../include/g_gui.h"
#include <stdio.h>
#include <raylib.h>

void render_register_status(c_cpu_t *cpu) {
char buffer[128];

    // Print the Program Counter (PC)
    snprintf(buffer, sizeof(buffer), "PC: 0x%04X", cpu->reg.pc);
    DrawText(buffer, 50, 250, 20, LIGHTGRAY);

    // Print the Stack Pointer (SP)
    snprintf(buffer, sizeof(buffer), "SP: 0x%02X", cpu->reg.sp);
    DrawText(buffer, 50, 280, 20, LIGHTGRAY);

    // Print the Accumulator (A)
    snprintf(buffer, sizeof(buffer), "A:  0x%02X", cpu->reg.acc);
    DrawText(buffer, 50, 310, 20, LIGHTGRAY);

    // Print the X Register
    snprintf(buffer, sizeof(buffer), "X:  0x%02X", cpu->reg.x);
    DrawText(buffer, 50, 340, 20, LIGHTGRAY);

    // Print the Y Register
    snprintf(buffer, sizeof(buffer), "Y:  0x%02X", cpu->reg.y);
    DrawText(buffer, 50, 370, 20, LIGHTGRAY);

    // Print the Status Register
    snprintf(buffer, sizeof(buffer), "Status: 0x%02X", cpu->reg.status);
    DrawText(buffer, 50, 400, 20, LIGHTGRAY);

    // Optionally, you can also break down the status register flags:
    snprintf(buffer, sizeof(buffer), "Flags: C:%d Z:%d I:%d D:%d B:%d O:%d N:%d",
        IS_FLAG_SET(cpu->reg, FLAG_CARRY),
        IS_FLAG_SET(cpu->reg, FLAG_ZERO),
        IS_FLAG_SET(cpu->reg, FLAG_INTERRUPT),
        IS_FLAG_SET(cpu->reg, FLAG_DECIMAL),
        IS_FLAG_SET(cpu->reg, FLAG_BREAK),
        IS_FLAG_SET(cpu->reg, FLAG_OVERFLOW),
        IS_FLAG_SET(cpu->reg, FLAG_NEGATIVE)
    );
    DrawText(buffer, 50, 430, 20, LIGHTGRAY);

}
