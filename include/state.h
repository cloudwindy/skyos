#ifndef SKYRADIO_STATE_H
#define SKYRADIO_STATE_H

#include <stdint.h>

typedef enum freq_selector_mode
{
  fs_vfo,
  fs_mr
} FSMode;

typedef struct state
{
  uint32_t vfo_freq;
  FSMode mode;
} State;

State *state(void);

#endif
