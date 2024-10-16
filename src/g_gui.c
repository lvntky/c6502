#include "../include/g_gui.h"
#include <stdio.h>
#include <string.h>
#include "../include/u_config.h"
#include "../include/u_util.h"

#define MEMORY_DISPLAY_ROWS 10 // Number of memory rows to display
#define MEMORY_DISPLAY_COLS 2 // Number of memory columns to display
#define MEMORY_CELL_WIDTH 70 // Width of each memory cell
#define MEMORY_CELL_HEIGHT 25 // Height of each memory cell
#define MEMORY_DISPLAY_SIZE (MEMORY_DISPLAY_ROWS * MEMORY_DISPLAY_COLS)

#define DISASSEMBLY_DISPLAY_ROWS 20 // Number of disassembly rows to display
#define DISASSEMBLY_DISPLAY_SIZE (DISASSEMBLY_DISPLAY_ROWS * 16)

#define MAX_INPUT_CHARS 32

#define VIRTUAL_INTERFACE_START_ADDRESS 0x0200
#define VIRTUAL_INTERFACE_END_ADDRESS 0x05FF

static int scrollOffset = 0x0000;

void g_render_register_status(c_cpu_t *cpu)
{
	char buffer[128];

	// Define the position and size of the box
	int boxWidth = 350;
	int boxHeight = 255;
	int boxX = U_SCREEN_WIDTH - (boxWidth + 50);
	int boxY = 50;

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

	int boxWidth = 350;
	int boxHeight = 255;
	int boxX = U_SCREEN_WIDTH - (boxWidth + 50);
	int boxY = 370;

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

bool g_render_button(g_button_t *button)
{
	Vector2 mousePoint = GetMousePosition();
	bool clicked = false;

	if (CheckCollisionPointRec(mousePoint, button->bounds)) {
		DrawRectangleRec(button->bounds, button->hover_color);
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			clicked = true;
		}
	} else {
		DrawRectangleRec(button->bounds, button->color);
	}

	// Draw button text centered
	int textWidth =
		MeasureText(button->text, 20); // Assuming font size is 20
	int textHeight = 20; // Assuming font size is 20
	DrawText(button->text,
		 button->bounds.x + (button->bounds.width / 2 - textWidth / 2),
		 button->bounds.y +
			 (button->bounds.height / 2 - textHeight / 2),
		 20, button->text_color);

	return clicked;
}

void g_render_control_panel()
{
	int boxWidth = 350;
	int boxHeight = 165;
	int boxX = 50;
	int boxY = 460;

	// Draw the box
	DrawRectangle(boxX, boxY, boxWidth, boxHeight, DARKGRAY);
	DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, LIGHTGRAY);

	DrawText("Control Panel", boxX, boxY - 25, 22, RAYWHITE);
}

void g_render_virtual_interface(m_memory_t *memory)
{
	int boxWidth = 320;
	int boxHeight = 320;
	int boxX = 50;
	int boxY = 50;
	int screenWidth = 32; // Width in "pixels" for the virtual screen
	int screenHeight = 32; // Height in "pixels" for the virtual screen
	int pixelSize =
		boxWidth / screenWidth; // Size of each pixel in the interface

	// Draw the virtual interface box
	DrawRectangle(boxX, boxY, boxWidth, boxHeight, RAYWHITE);
	DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, ORANGE);

	// Draw the title
	DrawText("Virtual Interface", boxX, boxY - 25, 22, RAYWHITE);

	// Render pixels inside the virtual interface area
	for (int addr = 0x0200; addr <= 0x05FF; addr++) {
		// Calculate pixel position
		int pixelX = boxX + ((addr - 0x0200) % screenWidth) * pixelSize;
		int pixelY = boxY + ((addr - 0x0200) / screenWidth) * pixelSize;

		// Get the memory value and map it to a color
		uint8_t pixelColor = memory->mem[addr];
		Color color;

		// Map memory values to screen colors
		switch (pixelColor) {
		case 0x0:
			color = BLACK;
			break;
		case 0x1:
			color = WHITE;
			break;
		case 0x2:
			color = RED;
			break;
		case 0x3:
			color = (Color){ 0, 255, 255, 255 };
			break; // Cyan
		case 0x4:
			color = PURPLE;
			break;
		case 0x5:
			color = GREEN;
			break;
		case 0x6:
			color = BLUE;
			break;
		case 0x7:
			color = YELLOW;
			break;
		case 0x8:
			color = ORANGE;
			break;
		case 0x9:
			color = BROWN;
			break;
		case 0xA:
			color = (Color){ 255, 102, 102, 255 };
			break; // Light red
		case 0xB:
			color = DARKGRAY;
			break;
		case 0xC:
			color = GRAY;
			break;
		case 0xD:
			color = LIME;
			break;
		case 0xE:
			color = SKYBLUE;
			break;
		case 0xF:
			color = LIGHTGRAY;
			break;
		default:
			color = BLACK;
			break; // Default to black for unknown values
		}

		// Draw each pixel
		DrawRectangle(pixelX, pixelY, pixelSize, pixelSize, color);
	}
}

void g_render_disassembly(c_cpu_t *cpu, m_memory_t *memory)
{
	UNUSED(cpu);
	UNUSED(memory);

	int boxWidth = 320;
	int boxHeight = 575;
	int boxX = 475;
	int boxY = 50;

	// Draw the box
	DrawRectangle(boxX, boxY, boxWidth, boxHeight, DARKGRAY);
	DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, LIGHTGRAY);

	// Draw the title
	DrawText("Disassembler", boxX, boxY - 25, 22, RAYWHITE);

	// Parameters for disassembly display
	int textX = boxX + 10;
	int textY = boxY + 10;
	int lineHeight = 20;

	// Iterate over memory and disassemble instructions
	for (int i = 0; i < 10; i += 1) {
		// Disassemble the instruction at current memory location
		char disassembled_instruction[128]; // Adjust size accordingly
		//

		// Draw the disassembled instruction
		DrawText(disassembled_instruction, textX, textY, 18, RAYWHITE);

		// Move to the next line
		textY += lineHeight;

		// Stop rendering if we exceed the box height
		if (textY >= boxY + boxHeight - lineHeight) {
			break;
		}
	}
}
