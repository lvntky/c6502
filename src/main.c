#include <raylib.h>
#include "../include/c_cpu.h"

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
        DrawText("c6502 Emulator and Debugger", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
