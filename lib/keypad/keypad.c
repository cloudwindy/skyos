#include "keypad.h"
#include "delay.h"

#include <stdio.h>

#define BITMASK_SWITCH(x)                    \
  for (uint64_t bit = 1; x >= bit; bit *= 2) \
    if (x & bit)                             \
      switch (bit)

static uint8_t keypad_get_col(void);
static void keypad_set_col(uint8_t col);
static void keypad_clear_cols(void);

char keypad_getchar(void)
{
  uint16_t keys = keypad_scan();
  BITMASK_SWITCH(keys)
  {
  case key_d:
    return 'd';
  case key_c:
    return 'c';
  case key_b:
    return 'b';
  case key_a:
    return 'a';
  case key_hash:
    return '#';
  case key_9:
    return '9';
  case key_6:
    return '6';
  case key_3:
    return '3';
  case key_0:
    return '0';
  case key_8:
    return '8';
  case key_5:
    return '5';
  case key_2:
    return '2';
  case key_star:
    return '*';
  case key_7:
    return '7';
  case key_4:
    return '4';
  case key_1:
    return '1';
  }
  return '\0';
}

/**
 * 扫描矩阵键盘
 */
uint16_t keypad_scan(void)
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
  uint16_t row0 = gpio_get(GPIOB, GPIO6);
  uint16_t row1 = gpio_get(GPIOB, GPIO7);
  uint16_t row2 = gpio_get(GPIOB, GPIO8);
  uint16_t row3 = gpio_get(GPIOB, GPIO9);
  uint8_t row = 0;
  row |= (row0 > 0) << 0; // B6
  row |= (row1 > 0) << 1; // B7
  row |= (row2 > 0) << 2; // B8
  row |= (row3 > 0) << 3; // B9
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