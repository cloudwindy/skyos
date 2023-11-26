#ifndef KEYPAD_H
#define KEYPAD_H

#include <libopencm3/stm32/gpio.h>

typedef uint16_t Key;

char keypad_getchar(void);
Key keypad_scan(void);

#endif
