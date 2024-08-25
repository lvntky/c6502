#include "../include/c_cpu.h"
#include "../include/g_gui.h"
#include "../include/m_memory.h"
#include "../include/u_config.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>

static c_cpu_t cpu;
static m_memory_t memory;

int main(void)
{
	// Initialization
	const int screenWidth = 1280;
	const int screenHeight = 720;

	c_reset(&cpu);
	m_reset(&memory);

	m_write_byte(&memory, 0x600, 0x10);
	printf("data: %x\n", memory.mem[cpu.reg.pc]);

	InitWindow(screenWidth, screenHeight, "C6502");

	// Main emulation loop
	while (!WindowShouldClose()) {
		// Draw
		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangle(0, 695, 1280, 25, LIME);
		DrawText(U_PROGRAM_NAME, 0, 700, 20, WHITE);
		EndDrawing();

		g_render_register_status(&cpu);
		g_render_memory(&memory);
	}

	// De-Initialization
	CloseWindow();

	return 0;
}
