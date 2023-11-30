#include "tasks.h"

#include "app.h"
#include "state.h"
#include "keypad.h"
#include "printf.h"

#define KEYPAD_TICK 10
#define KEYPAD_REPEAT_INTERVAL 100

/* Keypad scan routine. */
void task_keypad(void *args __attribute__((unused)))
{
  State *st = state();
  char holding_ch = '\0';
  uint32_t hold_time = 0;
  uint32_t hold_repeat_time = 0;
  while (true)
  {
    char ch = keypad_getchar();
    if (ch != '\0')
    { /* Key is pressed. */
      if (ch == holding_ch)
      { /* Key is being hold. */
        hold_time += KEYPAD_TICK;
        if (hold_time > st->ui.hold_delay)
        { /* This is a long press holding. */
          hold_repeat_time += KEYPAD_TICK;
          if (hold_repeat_time > KEYPAD_REPEAT_INTERVAL)
          {
            EvKey event = {
              .key = ch,
              .key_press = kp_long_press_holding,
              .hold_time = hold_time,
            };
            app_handler(ev_key, &event);
            hold_repeat_time = 0;
          }
        }
      }
      else
      { /**
         * Key changes during pressing.
         * This is perhaps an error, so we're going to ignore it.
         */
        holding_ch = ch;
        hold_time = 0;
        hold_repeat_time = 0;
      }
    }
    else
    { /* Key is not pressed. */
      if (hold_time > st->ui.hold_delay)
      { /* This is a long press released. */
        EvKey event = {
          .key = ch,
          .key_press = kp_long_press_released,
          .hold_time = hold_time,
        };
        app_handler(ev_key, &event);
      }
      else if (hold_time > 0)
      { /* This is a short press released. */
        EvKey event = {
          .key = ch,
          .key_press = kp_short_press_released,
          .hold_time = hold_time,
        };
        app_handler(ev_key, &event);
      }
      holding_ch = '\0';
      hold_time = 0;
      hold_repeat_time = 0;
    }
    os_delay(KEYPAD_TICK);
  }
}
