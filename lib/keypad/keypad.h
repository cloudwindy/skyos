#ifndef KEYPAD_H
#define KEYPAD_H

#include <libopencm3/stm32/gpio.h>

typedef uint16_t Key;

#define KEY_NOT_PRESSED 0
#define KEY_1 BIT0
#define KEY_4 BIT1
#define KEY_7 BIT2
#define KEY_STAR BIT3
#define KEY_2 BIT4
#define KEY_5 BIT5
#define KEY_8 BIT6
#define KEY_0 BIT7
#define KEY_3 BIT8
#define KEY_6 BIT9
#define KEY_9 BIT10
#define KEY_HASH BIT11
#define KEY_A BIT12
#define KEY_B BIT13
#define KEY_C BIT14
#define KEY_D BIT15

char keypad_getchar(void);
Key keypad_scan(void);

#endif
