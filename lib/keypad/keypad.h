#ifndef KEYPAD_H
#define KEYPAD_H

#include <libopencm3/stm32/gpio.h>

typedef enum key
{
  key_not_pressed = 0,

  key_1 = BIT0,
  key_4 = BIT1,
  key_7 = BIT2,
  key_star = BIT3,

  key_2 = BIT4,
  key_5 = BIT5,
  key_8 = BIT6,
  key_0 = BIT7,

  key_3 = BIT8,
  key_6 = BIT9,
  key_9 = BIT10,
  key_hash = BIT11,

  key_a = BIT12,
  key_b = BIT13,
  key_c = BIT14,
  key_d = BIT15,
} Key;

char keypad_getchar(void);
uint16_t keypad_scan(void);

#endif