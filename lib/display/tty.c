#include "tty.h"

#include "ssd1306.h"
#include "canvas.h"

#define TTY_FONT (Font)

#define TTY_COLOR (White)
#define TTY_BG_COLOR (Black)
#define TTY_FONT_WIDTH (TTY_FONT.width)
#define TTY_FONT_HEIGHT (TTY_FONT.height)
#define TTY_COLUMNS (SSD1306_WIDTH / TTY_FONT_WIDTH)
#define TTY_ROWS (SSD1306_HEIGHT / TTY_FONT_HEIGHT)

Canvas canvas;
uint8_t cur_row = 0;
uint8_t cur_col = 0;

static void tty_putchar(char ch);
static void tty_return(void);
static void tty_new_line(void);

void setup_tty(void)
{
  canvas_init(&canvas, SSD1306_WIDTH, SSD1306_HEIGHT);
  ssd1306_update(canvas.buf);
}

/**
 * 在虚拟终端上打印字符
 */
void tty_print(char *str, int len)
{
  for (int i = 0; i < len; i++)
    tty_putchar(str[i]);
}

/**
 * 刷新虚拟终端
 */
void tty_flush(void)
{
  ssd1306_update(canvas.buf);
}

/**
 * 清空虚拟终端
 */
void tty_clear(void)
{
  canvas_fill(&canvas, TTY_BG_COLOR);
  ssd1306_update(canvas.buf);
  cur_row = 0;
  cur_col = 0;
}

static void tty_putchar(char ch)
{
  if (ch == '\r')
  {
    tty_return();
    return;
  }
  if (ch == '\n')
  {
    tty_new_line();
    return;
  }
  if (ch < 32 || ch > 126)
  {
    return;
  }
  if (cur_col >= TTY_COLUMNS)
  {
    tty_new_line();
  }
  cur_col++;
  canvas_write_char(&canvas, ch, TTY_FONT, TTY_COLOR);
}

/**
 * 回车 '\r'
 *
 * 回到该行的开始位置
 */
static void tty_return(void)
{
  cur_col = 0;
  canvas_set_cursor(&canvas, 0, cur_row * TTY_FONT_HEIGHT);
}

/**
 * 换行 '\n'
 *
 * 进入下一行的开始位置
 */
static void tty_new_line(void)
{
  if (cur_row + 1 >= TTY_ROWS)
  {
    canvas_move_up(&canvas, TTY_FONT_HEIGHT);
  }
  else
  {
    cur_row++;
  }
  tty_return();
}