#ifndef SKYRADIO_UI_H
#define SKYRADIO_UI_H

#include "canvas.h"

#include <stdint.h>

typedef struct ui
{
  Canvas v;
  uint8_t *vbuf; /* VRAM */
} UI;

void ui_init(UI *ui);
void ui_line_break(UI *ui, uint8_t y);
void ui_text(UI *ui, uint8_t x, uint8_t y, const char *str);
void ui_update(UI *ui);

#endif