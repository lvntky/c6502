#include "../include/c_cpu.h"
#include "../include/g_gui.h"
#include "../include/m_memory.h"
#include "../include/u_config.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static c_cpu_t cpu;
static m_memory_t memory;

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Please specify file to load.\n");
		exit(EXIT_FAILURE);
	}

	c_reset(&cpu);
	m_reset(&memory);

	//m_load_bin(&memory, argv[1]);
	memory.mem[0xFFFC] = 0xA9;
	memory.mem[0xFFFD] = 0x00;

	InitWindow(U_SCREEN_WIDTH, U_SCREEN_HEIGHT, "C6502");

	// Main emulation loop
	while (!WindowShouldClose()) {
		// Draw
		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangle(0, 695, 1280, 25, LIME);
		DrawText(U_PROGRAM_NAME, 0, 700, 20, WHITE);

		c_execute(&cpu, &memory);

		g_render_register_status(&cpu);
		g_render_memory(&memory);

		EndDrawing();
	}

	// De-Initialization
	CloseWindow();

	return 0;
}
