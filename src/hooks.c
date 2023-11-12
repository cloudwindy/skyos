#include "tty.h"
#include "serial.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <libopencm3/stm32/rtc.h>
#include <libopencm3/cm3/nvic.h>

int _write(int file, char *ptr, int len);

int _write(int file, char *ptr, int len)
{
  int i = 0;
  if (file == STDOUT_FILENO || file == STDERR_FILENO)
  {
    tty_print(ptr, len);
    tty_flush();
    return i;
  }
  errno = EIO;
  return -1;
}

/**
 * RTC 计时器中断
 *
 * 每秒执行一次
 */
void rtc_isr(void)
{
  rtc_clear_flag(RTC_SEC);
  fflush(stdout);
}

/**
 * USART 端口数据中断
 */
void usart1_isr(void)
{
  serial_handler();
}