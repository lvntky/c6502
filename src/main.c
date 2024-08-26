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

	m_load_bin(&memory, argv[1]);

	InitWindow(U_SCREEN_WIDTH, U_SCREEN_HEIGHT, "C6502");

	g_button_t start_button = { .bounds = { 125, 470, 200, 25 },
				    .text = "Start",
				    .color = GRAY,
				    .hover_color = LIGHTGRAY,
				    .text_color = BLACK };

	g_button_t stop_button = { .bounds = { 125, 530, 200, 25 },
				   .text = "Stop",
				   .color = GRAY,
				   .hover_color = LIGHTGRAY,
				   .text_color = BLACK };

	g_button_t step_button = { .bounds = { 125, 590, 200, 25 },
				   .text = "Step",
				   .color = GRAY,
				   .hover_color = LIGHTGRAY,
				   .text_color = BLACK };

	bool is_running = false; // To control execution state

	// Main emulation loop
	while (!WindowShouldClose()) {
		// Draw
		g_render_control_panel();
		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangle(0, 695, 1280, 25, LIME);
		DrawText(U_PROGRAM_NAME, 0, 700, 20, WHITE);

		if (g_render_button(&start_button)) {
			is_running = true; // Start continuous execution
		}

		if (g_render_button(&stop_button)) {
			is_running = false; // Stop continuous execution
		}

		if (g_render_button(&step_button)) {
			c_execute(
				&cpu,
				&memory); // Execute one opcode if Step button is clicked
		}

		// Execute continuously if running
		if (is_running) {
			c_execute(&cpu, &memory);
		}

		g_render_register_status(&cpu);
		g_render_memory(&memory);
		g_render_virtual_interface();
		g_render_disassembly(&cpu, &memory);

		EndDrawing();
	}

	// De-Initialization
	CloseWindow();

	return 0;
}
