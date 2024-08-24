#include "../include/g_gui.h"
#include <stdio.h>
#include <raylib.h>

#define MEMORY_DISPLAY_SIZE 256  // Number of memory locations to display
#define MEMORY_COLUMNS 16        // Number of memory columns in the display grid


void g_render_register_status(c_cpu_t *cpu) {
    char buffer[128];

    // Define the position and size of the box
    int boxX = 50;
    int boxY = 230;
    int boxWidth = 350;
    int boxHeight = 250;

    // Draw the box
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, DARKGRAY);
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, LIGHTGRAY);

    // Set starting coordinates for the text inside the box
    int textX = boxX + 10;
    int textY = boxY + 20;
    int textSpacing = 30;

    // Print the Program Counter (PC)
    snprintf(buffer, sizeof(buffer), "PC: 0x%04X", cpu->reg.pc);
    DrawText(buffer, textX, textY, 20, RAYWHITE);

    // Print the Stack Pointer (SP)
    textY += textSpacing;
    snprintf(buffer, sizeof(buffer), "SP: 0x%02X", cpu->reg.sp);
    DrawText(buffer, textX, textY, 20, RAYWHITE);

    // Print the Accumulator (A)
    textY += textSpacing;
    snprintf(buffer, sizeof(buffer), "A:  0x%02X", cpu->reg.acc);
    DrawText(buffer, textX, textY, 20, RAYWHITE);

    // Print the X Register
    textY += textSpacing;
    snprintf(buffer, sizeof(buffer), "X:  0x%02X", cpu->reg.x);
    DrawText(buffer, textX, textY, 20, RAYWHITE);

    // Print the Y Register
    textY += textSpacing;
    snprintf(buffer, sizeof(buffer), "Y:  0x%02X", cpu->reg.y);
    DrawText(buffer, textX, textY, 20, RAYWHITE);

    // Print the Status Register
    textY += textSpacing;
    snprintf(buffer, sizeof(buffer), "Status: 0x%02X", cpu->reg.status);
    DrawText(buffer, textX, textY, 20, RAYWHITE);

    // Optionally, you can also break down the status register flags
    textY += textSpacing;
    snprintf(buffer, sizeof(buffer), "Flags: C:%d Z:%d I:%d D:%d B:%d O:%d N:%d",
        IS_FLAG_SET(cpu->reg, FLAG_CARRY),
        IS_FLAG_SET(cpu->reg, FLAG_ZERO),
        IS_FLAG_SET(cpu->reg, FLAG_INTERRUPT),
        IS_FLAG_SET(cpu->reg, FLAG_DECIMAL),
        IS_FLAG_SET(cpu->reg, FLAG_BREAK),
        IS_FLAG_SET(cpu->reg, FLAG_OVERFLOW),
        IS_FLAG_SET(cpu->reg, FLAG_NEGATIVE)
    );
    DrawText(buffer, textX, textY, 20, RAYWHITE);
}

/*
void g_render_memory(m_memory_t *memory) {
    char buffer[128];
    int startX = 350;  // Starting X position for the memory display
    int startY = 250;  // Starting Y position for the memory display
    int cellWidth = 80; // Width of each memory cell
    int cellHeight = 20; // Height of each memory cell

    for (int i = 0; i < MEMORY_DISPLAY_SIZE; i++) {
        // Calculate row and column for the current memory cell
        int row = i / MEMORY_COLUMNS;
        int col = i % MEMORY_COLUMNS;

        // Calculate the position to render the memory value
        int xPos = startX + col * cellWidth;
        int yPos = startY + row * cellHeight;

        // Format and draw the memory value
        snprintf(buffer, sizeof(buffer), "0x%04X: 0x%02X", i, memory->mem[i]);
        DrawText(buffer, xPos, yPos, 20, LIGHTGRAY);
    }
}
*/
