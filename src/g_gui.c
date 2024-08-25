#include "../include/g_gui.h"
#include <stdio.h>
#include <raylib.h>
#include <string.h>

#define MEMORY_DISPLAY_ROWS 10 // Number of memory rows to display
#define MEMORY_DISPLAY_COLS 2 // Number of memory columns to display
#define MEMORY_CELL_WIDTH 70 // Width of each memory cell
#define MEMORY_CELL_HEIGHT 25 // Height of each memory cell
#define MEMORY_DISPLAY_SIZE (MEMORY_DISPLAY_ROWS * MEMORY_DISPLAY_COLS)

#define DISASSEMBLY_DISPLAY_ROWS 20 // Number of disassembly rows to display
#define DISASSEMBLY_DISPLAY_SIZE (DISASSEMBLY_DISPLAY_ROWS * 16)

#define MAX_INPUT_CHARS 32

static int scrollOffset = 0x0000;

void g_render_register_status(c_cpu_t *cpu)
{
	char buffer[128];

	// Define the position and size of the box
	int boxX = 50;
	int boxY = 50;
	int boxWidth = 350;
	int boxHeight = 255;

	// Draw the box
	DrawRectangle(boxX, boxY, boxWidth, boxHeight, DARKGRAY);
	DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, LIGHTGRAY);

	// Set starting coordinates for the text inside the box
	int textX = boxX + 10;
	int textY = boxY + 20;
	int textSpacing = 30;

	DrawText("Register Status", boxX, boxY - 25, 22, RAYWHITE);

	// Print the Program Counter (PC)
	snprintf(buffer, sizeof(buffer), "PC: 0x%04X", cpu->reg.pc);
	DrawText(buffer, textX, textY, 20, RAYWHITE);

	// Print the Stack Pointer (SP)
	textY += textSpacing;
	snprintf(buffer, sizeof(buffer), "SP: 0x%04X", cpu->reg.sp);
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
	snprintf(buffer, sizeof(buffer),
		 "Flags: C:%d Z:%d I:%d D:%d B:%d O:%d N:%d",
		 IS_FLAG_SET(cpu->reg, FLAG_CARRY),
		 IS_FLAG_SET(cpu->reg, FLAG_ZERO),
		 IS_FLAG_SET(cpu->reg, FLAG_INTERRUPT),
		 IS_FLAG_SET(cpu->reg, FLAG_DECIMAL),
		 IS_FLAG_SET(cpu->reg, FLAG_BREAK),
		 IS_FLAG_SET(cpu->reg, FLAG_OVERFLOW),
		 IS_FLAG_SET(cpu->reg, FLAG_NEGATIVE));
	DrawText(buffer, textX, textY, 20, RAYWHITE);
}

void g_render_memory(m_memory_t *memory)
{
	char buffer[128];

	// Define the position and size of the memory display box
	int boxX = 50;
	int boxY = 370;
	int boxWidth = 350;
	int boxHeight = 255;

	// Draw the memory display box
	DrawRectangle(boxX, boxY, boxWidth, boxHeight, DARKGRAY);
	DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, LIGHTGRAY);

	// Set the starting coordinates for the memory content
	int textX = boxX + 10;
	int textY = boxY + 10;

	DrawText("Memory Status", boxX, boxY - 25, 22, RAYWHITE);

	// Display memory cells
	for (int row = 0; row < MEMORY_DISPLAY_ROWS; row++) {
		snprintf(buffer, sizeof(buffer),
			 "0x%04X:", scrollOffset + (row * MEMORY_DISPLAY_COLS));
		DrawText(buffer, textX, textY + row * MEMORY_CELL_HEIGHT, 20,
			 RAYWHITE);

		for (int col = 0; col < MEMORY_DISPLAY_COLS; col++) {
			int memIndex = scrollOffset +
				       (row * MEMORY_DISPLAY_COLS) + col;
			if (memIndex >= M_MEMORY_SIZE)
				break;

			snprintf(buffer, sizeof(buffer), "0x%02X",
				 memory->mem[memIndex]);
			DrawText(buffer, textX + 150 + col * MEMORY_CELL_WIDTH,
				 textY + row * MEMORY_CELL_HEIGHT, 20,
				 RAYWHITE);
		}
	}

	int mouseX = GetMouseX();
	int mouseY = GetMouseY();

	// Check if the mouse is within the memory display box
	bool isMouseInBox = (mouseX >= boxX && mouseX <= (boxX + boxWidth) &&
			     mouseY >= boxY && mouseY <= (boxY + boxHeight));

	if (isMouseInBox) {
		int scroll_action = (int)(GetMouseWheelMove() * 4);

		scrollOffset -= scroll_action;

		// Ensure scrollOffset stays within valid bounds
		if (scrollOffset < 0) {
			scrollOffset = 0;
		} else if (scrollOffset >
			   M_MEMORY_SIZE -
				   MEMORY_DISPLAY_ROWS * MEMORY_DISPLAY_COLS) {
			scrollOffset =
				M_MEMORY_SIZE -
				MEMORY_DISPLAY_ROWS * MEMORY_DISPLAY_COLS;
		}
	}
}
