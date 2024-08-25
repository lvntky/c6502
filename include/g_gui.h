#ifndef __G_GUI_H__
#define __G_GUI_H__

#include "c_cpu.h"
#include "m_memory.h"
#include <raylib.h>
#include <stdbool.h>

typedef struct g_button {
	Rectangle bounds;
	const char *text;
	Color color;
	Color hover_color;
	Color text_color;
} g_button_t;

void g_render_register_status(c_cpu_t *cpu);
void g_render_memory(m_memory_t *mem);
void g_render_disassembly(c_cpu_t *cpu, m_memory_t *memory);
bool g_render_button(g_button_t *button);

#endif //__G_GUI_H__
