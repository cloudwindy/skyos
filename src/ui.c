#include "ui.h"
#include "mem.h"

#include "ssd1306.h"

void ui_init(UI *ui)
{
  static uint8_t vbuf[SSD1306_HEIGHT * SSD1306_WIDTH / 8] = {0};
  ui->vbuf = vbuf;
  canvas_init(&ui->v, ui->vbuf, SSD1306_WIDTH, SSD1306_HEIGHT);
}

void ui_line_break(UI *ui, uint8_t y)
{
  canvas_line(&ui->v, 0, y, ui->v.width, y, White);
}

void ui_text(UI *ui, uint8_t row, uint8_t col, const char *str)
{
  ui->v.cur_x = row * Font.width;
  ui->v.cur_y = col * Font.height;
  canvas_write_string(&ui->v, str, Font, White);
}

void ui_update(UI *ui)
{
  ssd1306_update(ui->vbuf);
}
