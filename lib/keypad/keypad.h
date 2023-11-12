#ifndef SKYRADIO_KEYPAD_H
#define SKYRADIO_KEYPAD_H

#include <libopencm3/stm32/gpio.h>

typedef enum key
{
  key_not_pressed = 0,

  key_1 = 1 << 0,
  key_4 = 1 << 1,
  key_7 = 1 << 2,
  key_star = 1 << 3,

  key_2 = 1 << 4,
  key_5 = 1 << 5,
  key_8 = 1 << 6,
  key_0 = 1 << 7,

  key_3 = 1 << 8,
  key_6 = 1 << 9,
  key_9 = 1 << 10,
  key_hash = 1 << 11,

  key_a = 1 << 12,
  key_b = 1 << 13,
  key_c = 1 << 14,
  key_d = 1 << 15,
} Key;

void setup_keypad(void);
char keypad_getchar(void);
uint16_t keypad_scan(void);

#endif