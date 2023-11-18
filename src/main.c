#include "setup.h"
#include "os.h"

#include "app.h"

#include <stddef.h>

int main(void)
{
  setup();

  os_exec("task_init", task_init, NULL, 0);
  /**
   * Start up RTOS.
   *
   * It will configure the SysTick by itself.
   */
  os_start();
  /* Wait for IWDG reset. */
  for (;;)
    ;
  return 0;
}