#include "keypad.h"
#include "delay.h"

#define BITMASK_SWITCH(x)                    \
  for (uint64_t bit = 1; x >= bit; bit *= 2) \
    if (x & bit)                             \
      switch (bit)

static uint8_t keypad_get_col(void);
static void keypad_set_col(uint8_t col);
static void keypad_clear_cols(void);

char keypad_getchar(void)
{
  Key keys = keypad_scan();
  BITMASK_SWITCH(keys)
  {
  case KEY_NOT_PRESSED:
    return '\0';
  case KEY_D:
    return 'd';
  case KEY_C:
    return 'c';
  case KEY_B:
    return 'b';
  case KEY_A:
    return 'a';
  case KEY_HASH:
    return '#';
  case KEY_9:
    return '9';
  case KEY_6:
    return '6';
  case KEY_3:
    return '3';
  case KEY_0:
    return '0';
  case KEY_8:
    return '8';
  case KEY_5:
    return '5';
  case KEY_2:
    return '2';
  case KEY_STAR:
    return '*';
  case KEY_7:
    return '7';
  case KEY_4:
    return '4';
  case KEY_1:
    return '1';
  default:
    return '\0';
  }
  return '\0';
}

/**
 * 扫描矩阵键盘
 */
Key keypad_scan(void)
{
  uint16_t keys = 0;
  // 扫描 1 ~ 4 列
  for (uint8_t i = 0; i < 4; i++)
  {
    keypad_clear_cols();
    keypad_set_col(i);
    usleep(1);
    uint8_t row = keypad_get_col();
    keys |= row << i * 4;
  }
  keypad_clear_cols();
  return keys;
}

/**
 * 获取 1 ~ 4 行的按键状态
 */
static uint8_t keypad_get_col(void)
{
  uint8_t row = 0;
  row |= (gpio_get(GPIOB, GPIO6) > 0) << 0; // B6
  row |= (gpio_get(GPIOB, GPIO7) > 0) << 1; // B7
  row |= (gpio_get(GPIOB, GPIO8) > 0) << 2; // B8
  row |= (gpio_get(GPIOB, GPIO9) > 0) << 3; // B9
  return row;
}

static void keypad_set_col(uint8_t col)
{
  switch (col)
  {
  case 0:
    gpio_set(GPIOA, GPIO3);
    break;
  case 1:
    gpio_set(GPIOA, GPIO2);
    break;
  case 2:
    gpio_set(GPIOA, GPIO1);
    break;
  case 3:
    gpio_set(GPIOA, GPIO0);
    break;
  }
}

static void keypad_clear_cols(void)
{
  gpio_clear(GPIOA, GPIO0 | GPIO1 | GPIO2 | GPIO3);
}
