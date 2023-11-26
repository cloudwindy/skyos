#include "app.h"
#include "tasks.h"
#include "os.h"
#include "keypad.h"
#include "printf.h"

#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/iwdg.h>

static task task_stayin_alive;

void task_init(void *args __attribute__((unused)))
{
  printf("skyOS started\n");
  os_exec("task_ui", task_ui, NULL, 0);
  os_exec("task_keyboard", task_keyboard, NULL, 0);
  os_exec("task_remote", task_remote, NULL, 0);
  os_exec("task_stayin_alive", task_stayin_alive, NULL, 1);

  os_exit();
}

static void task_stayin_alive(void *args __attribute__((unused)))
{
  while (true)
  { /* Life goin' nowhere, somebody help me. */
    iwdg_reset();
    os_delay(50);
  }
}
