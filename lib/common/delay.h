#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

typedef void(*interval_cb_func) (void);

void delay(uint32_t ms);
void sleep(uint32_t ms);
void usleep(uint32_t us);

#endif