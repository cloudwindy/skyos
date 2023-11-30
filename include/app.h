#ifndef APP_H
#define APP_H

#include "ui.h"

#include <stdbool.h>

typedef enum key_press
{
  kp_short_press_released,
  kp_long_press_holding,
  kp_long_press_released
} KeyPress;

typedef void KeyHandler(char key, KeyPress kp, uint32_t hold_time);
typedef void UIHandler(UI *ui);

KeyHandler app_process_key, home_process_key;
UIHandler app_update_ui, home_update_ui;

#endif
