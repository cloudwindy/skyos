#include "tasks.h"

#include "state.h"
#include "keypad.h"
#include "printf.h"

#include <stdbool.h>

/* Keyboard scan routine. */
void task_keyboard(void *args __attribute__((unused)))
{
  while (true)
  {
    char ch = keypad_getchar();
    switch (ch)
    {
    case 'b':
      state()->vfo_freq += 1000;
      break;
    case 'c':
      State *st = state();
      if (st->vfo_freq > 1000)
        st->vfo_freq -= 1000;
      break;
    }
    os_delay(50);
  }
}