#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

typedef uint16_t Key;

char keypad_getchar(void);
Key keypad_scan(void);

#endif
