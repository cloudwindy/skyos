#include "app.h"
#include "tasks.h"
#include "state.h"
#include "os.h"
#include "printf.h"

#include <libopencm3/stm32/iwdg.h>

static task task_stayin_alive;

void task_init(void *args __attribute__((unused)))
{
  printf("skyOS started\n");

  /* TODO: implement settings */
  State *st = state();
  st->vfo_freq = 438'500'000;
  st->freq_step = 25'000;
  st->fs_mode = fs_vfo;
  st->ui.hold_delay = 500;

  os_exec("task_ui", task_ui, NULL, 0);
  os_exec("task_keypad", task_keypad, NULL, 0);
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
