#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

typedef void (*interval_cb_func)(void);

void sleep(uint32_t ms);
void usleep(uint16_t us);

#endif
