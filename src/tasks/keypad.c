#include "tasks.h"
#include "keypad.h"
#include "printf.h"

#include <stdbool.h>

/* Keyboard scan routine. */
void task_keyboard(void *args __attribute__((unused)))
{
  char last_char = 0;
  while (true)
  {
    char new_char = keypad_getchar();
    if (new_char != last_char && new_char != 0)
    {
      printf("%c", new_char);
    }
    last_char = new_char;
    os_delay(100);
  }
}