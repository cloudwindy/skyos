#include "tty.h"
#include "serial.h"

#include "FreeRTOS.h"
#include "FreeRTOS/task.h"

#include <stdlib.h>
#include <string.h>

#include <libopencm3/stm32/rtc.h>
#include <libopencm3/cm3/nvic.h>

#ifndef ENANLE_SERIAL_PRINTF
#include "printf.h"
#include "serial.h"
void _putchar(char ch)
{
  serial_send(&ch, 1);
}
#endif

#if (configCHECK_FOR_STACK_OVERFLOW == 1)
void vApplicationStackOverflowHook(TaskHandle_t task __attribute__((unused)), char *task_name)
{
  printf("%s: stack overflow\n", task_name);
  /* Wait for IWDG reset. */
  for (;;)
    ;
}
#endif

#if (configUSE_MALLOC_FAILED_HOOK == 1)
void vApplicationMallocFailedHook(void)
{
  printf("malloc failed\n");
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
