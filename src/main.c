#include "os.h"
#include "printf.h"
#include "setup.h"
#include "tasks.h"

int main(void)
{
  setup();

  /**
   * This task won't not run immediately because RTOS
   * isn't started yet.
   */
  os_exec("task", task_init, NULL, 0);
  /**
   * Start up RTOS.
   * It does the SysTick configuring by itself.
   */
  os_start();
  printf("os shut down.\n");
  /* Wait for IWDG reset. */
  for (;;)
    ;
  return 0;
}
