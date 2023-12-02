#include "keypad.h"
#include "delay.h"

#define BITMASK_SWITCH(x)                                                      \
  for (uint64_t bit = 1; x >= bit; bit *= 2)                                   \
    if (x & bit)                                                               \
      switch (bit)

static uint8_t keypad_get_col(void);
static void keypad_set_col(uint8_t col);
static void keypad_clear_cols(void);

char keypad_getchar(void)
{
  Key keys = keypad_scan();
  BITMASK_SWITCH(keys)
  {
    case 0:
      return '\0';
    case BIT15:
      return '1';
    case BIT14:
      return '2';
    case BIT13:
      return '3';
    case BIT12:
      return 'a';
    case BIT11:
      return '4';
    case BIT10:
      return '5';
    case BIT9:
      return '6';
    case BIT8:
      return 'b';
    case BIT7:
      return '7';
    case BIT6:
      return '8';
    case BIT5:
      return '9';
    case BIT4:
      return 'c';
    case BIT3:
      return '*';
    case BIT2:
      return '0';
    case BIT1:
      return '#';
    case BIT0:
      return 'd';
    default:
      return '\0';
  }
  return '\0';
}

/**
 * Scan matrix keypad.
 */
Key keypad_scan(void)
{
  uint16_t keys = 0;
  uint8_t i = 4;
  while (i--)
  { /* Scan from 1 to 4 */
    keypad_clear_cols();
    keypad_set_col(i);
    usleep(1);
    keys |= keypad_get_col() << i * 4;
  }
  keypad_clear_cols();
  return keys;
}

/**
 * 4 bits of current row.
 */
static uint8_t keypad_get_col(void)
{
  uint8_t row = 0;
  row |= (gpio_get(GPIOB, GPIO6) > 0) << 0;
  row |= (gpio_get(GPIOB, GPIO7) > 0) << 1;
  row |= (gpio_get(GPIOB, GPIO8) > 0) << 2;
  row |= (gpio_get(GPIOB, GPIO9) > 0) << 3;
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
