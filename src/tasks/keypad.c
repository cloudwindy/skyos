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
  char holding_ch = '\0';
  uint32_t hold_time = 0;
  uint32_t hold_repeat_time = 0;
  while (true)
  {
    char ch = keypad_getchar();
    if (ch != '\0' && ch == holding_ch)
    { /* Key is held. */
      hold_time += KEYPAD_TICK;
      if (hold_time > g_st.ui.hold_delay)
      { /* This is a long press holding. */
        hold_repeat_time += KEYPAD_TICK;
        if (hold_repeat_time > KEYPAD_REPEAT_INTERVAL)
        {
          EvKey ev = {
            .key = ch,
            .key_press = kp_long_press_holding,
            .hold_time = hold_time,
          };
          app_handler(ev_key, &ev);
          hold_repeat_time = 0;
        }
      }
    }
    else if (ch != '\0')
    { /* A new key is pressed. */
      holding_ch = ch;
      hold_time += KEYPAD_TICK;
    }
    else if (hold_time > 0)
    { /* Key is released. */
      EvKey ev = {
        .key = holding_ch,
        .hold_time = hold_time,
      };
      if (hold_time > g_st.ui.hold_delay)
      { /* This was a long press. */
        ev.key_press = kp_long_press_released;
      }
      else
      { /* This was a short press. */
        ev.key_press = kp_short_press_released;
      }
      app_handler(ev_key, &ev);
      holding_ch = '\0';
      hold_time = 0;
      hold_repeat_time = 0;
    }
    os_delay(KEYPAD_TICK);
  }
}
