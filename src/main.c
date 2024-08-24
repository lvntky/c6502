#include "../include/c_cpu.h"
#include "../include/g_gui.h"
#include "../include/m_memory.h"
#include <raylib.h>
#include <stdio.h>

static c_cpu_t cpu;
static m_memory_t memory;

int main(void)
{
	// Initialization
	const int screenWidth = 1280;
	const int screenHeight = 720;

	c_reset(&cpu);
	m_reset(&memory);

	InitWindow(screenWidth, screenHeight, "C6502");

	// Main emulation loop
	while (!WindowShouldClose()) {
		// Draw
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText("c6502 Emulator & Debugger", 0, 700, 20, LIGHTGRAY);
		EndDrawing();

		g_render_register_status(&cpu);
		g_render_memory(&memory);
	}

	// De-Initialization
	CloseWindow();

	return 0;
}
