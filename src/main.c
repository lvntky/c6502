#include "../include/c_cpu.h"
#include "../include/g_gui.h"
#include <raylib.h>

static c_cpu_t cpu;

int main(void) {
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    c_reset(&cpu);

    InitWindow(screenWidth, screenHeight, "C6502");

    // Main emulation loop
    while (!WindowShouldClose()) {
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("c6502 Emulator and Debugger", 0, 0, 20, LIGHTGRAY);
        EndDrawing();

        if(cpu.reg.pc != 1000000000) {
            cpu.reg.pc++;
        }

        render_register_status(&cpu);
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
