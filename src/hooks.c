#include "tty.h"
#include "serial.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  // for EIO
#include <unistd.h> // for STDOUT_* macros

#include <libopencm3/stm32/rtc.h>
#include <libopencm3/cm3/nvic.h>

int _write(int file, char *ptr, int len);

int _write(int file, char *ptr, int len)
{
  int i = 0;
  if (file == STDOUT_FILENO || file == STDERR_FILENO)
  {
    tty_print(ptr, len);
    tty_update();
    return i;
  }
  errno = EIO;
  return -1;
}

#if (configCHECK_FOR_STACK_OVERFLOW == 1)
void vApplicationStackOverflowHook(TaskHandle_t task, char *task_name)
{
  (void)task;
  printf("%s: stack overflow\n", task_name);
  /* Wait for IWDG reset. */
  for (;;)
    ;
}
#endif

#if (configUSE_MALLOC_FAILED_HOOK == 1)
void vApplicationMallocFailedHook(void)
{
  (void)task;
  printf("%s: malloc failed\n", task_name);
  /* Wait for IWDG reset. */
  for (;;)
    ;
}
#endif

/**
 * USART 端口数据中断
 */
void usart1_isr(void)
{
  serial_handler();
}

void hard_fault_handler(void)
{
  printf("hard fault\n");
  /* Wait for IWDG reset. */
  for (;;)
    ;
}

void bus_fault_handler(void)
{
  printf("bus fault\n");
  /* Wait for IWDG reset. */
  for (;;)
    ;
}

void usage_fault_handler(void)
{
  printf("usage fault\n");
  /* Wait for IWDG reset. */
  for (;;)
    ;
}