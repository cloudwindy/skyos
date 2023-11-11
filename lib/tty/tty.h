#ifndef SKYRADIO_TTY_H
#define SKYRADIO_TTY_H

#include "ssd1306.h"

#define TTY_FONT (Font)

#define TTY_COLOR (White)
#define TTY_BG_COLOR (Black)
#define TTY_FONT_WIDTH (TTY_FONT.width)
#define TTY_FONT_HEIGHT (TTY_FONT.height)
#define TTY_COLUMNS (SSD1306_WIDTH / TTY_FONT_WIDTH)
#define TTY_ROWS (SSD1306_HEIGHT / TTY_FONT_HEIGHT)

void tty_print(char *str, int len);
void tty_flush(void);
void tty_clear(void);

#endif