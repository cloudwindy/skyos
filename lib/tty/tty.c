#include "tty.h"

uint8_t cur_row = 0;
uint8_t cur_col = 0;

static void tty_putchar(char str);
static void tty_return(void);
static void tty_new_line(void);

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
  ssd1306_update();
}

/**
 * 清空虚拟终端
 */
void tty_clear(void)
{
  ssd1306_fill(TTY_BG_COLOR);
  ssd1306_update();
  cur_row = 0;
  cur_col = 0;
}

static void tty_putchar(char str)
{
  switch (str)
  {
  case '\r':
    tty_return();
    break;

  case '\n':
    tty_new_line();
    break;

  default:
    if (cur_col >= TTY_COLUMNS)
      tty_new_line();
    cur_col++;
    ssd1306_write_char(str, TTY_FONT, TTY_COLOR);
    break;
  }
}

/**
 * 回车 '\r'
 *
 * 回到该行的开始位置
 */
static void tty_return(void)
{
  cur_col = 0;
  ssd1306_set_cursor(0, cur_row * TTY_FONT_HEIGHT);
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
    ssd1306_move_up(TTY_FONT_HEIGHT);
  }
  else
  {
    cur_row++;
  }
  tty_return();
}