#ifndef APP_H
#define APP_H

#include "ui.h"

typedef enum
{
  ev_ui,
  ev_key,
  ev_tick,
} EvType;

typedef enum
{
  kp_short_press_released,
  kp_long_press_holding,
  kp_long_press_released
} KeyPress;

typedef struct
{
  char key;
  KeyPress key_press;
  uint32_t hold_time;
} EvKey;

typedef UI EvUI;

typedef void EventHandler(EvType type, void *event);

EventHandler app_handler,
             home_handler;

#endif
