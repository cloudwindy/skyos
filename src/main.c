#include "setup.h"

#include "FreeRTOS.h"
#include "task.h"

#include "app.h"

int main(void)
{
  setup();

  xTaskCreate(app, "APP", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
  /**
   * Start RTOS task scheduler.
   * 
   * SysTick will be 
   */
  vTaskStartScheduler();
  /* Wait for IWDG reset. */
  for (;;)
    ;
  return 0;
}