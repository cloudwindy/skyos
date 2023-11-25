#include "ui.h"
#include "mem.h"

#include "ssd1306.h"

void ui_init(UI *ui)
{
  static uint8_t vbuf[SSD1306_HEIGHT * SSD1306_WIDTH / 8];
  ui->vbuf = vbuf;
  canvas_init(&ui->v, ui->vbuf, SSD1306_WIDTH, SSD1306_HEIGHT);
}

void ui_clear(UI *ui)
{
  canvas_fill(&ui->v, Black);
}

void ui_line_break(UI *ui, uint8_t y)
{
  canvas_line(&ui->v, 0, y, ui->v.width, y, White);
}

void ui_text(UI *ui, uint8_t row, uint8_t col, const char *str)
{
  canvas_set_cursor(&ui->v, row * Font.width, col * Font.height);
  canvas_write_string(&ui->v, str, Font, White, false);
}

void ui_text_clear(UI *ui, uint8_t row, uint8_t col, uint8_t spaces)
{
  uint8_t x1 = row * Font.width;
  uint8_t y1 = col * Font.height;
  canvas_fill_rectangle(&ui->v, x1, y1, x1 + spaces * Font.width, y1 + Font.height, Black);
}

void ui_update(UI *ui)
{
  ssd1306_update(ui->vbuf);
}
