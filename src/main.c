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
	c_reset(&cpu);
	m_reset(&memory);

	InitWindow(U_SCREEN_WIDTH, U_SCREEN_HEIGHT, "C6502");

	// Main emulation loop
	while (!WindowShouldClose()) {
		// Draw
		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangle(0, 695, 1280, 25, LIME);
		DrawText(U_PROGRAM_NAME, 0, 700, 20, WHITE);

		g_render_register_status(&cpu);
		g_render_memory(&memory);

		EndDrawing();
	}

	// De-Initialization
	CloseWindow();

	return 0;
}
