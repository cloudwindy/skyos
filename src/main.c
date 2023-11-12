#include "setup.h"
#include "led.h"
#include "delay.h"
#include "keypad.h"
#include "serial.h"
#include "command.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libopencm3/stm32/iwdg.h>

int main(void)
{
  setup();
  const char hello_msg[] = "UART command line utility\n";
  serial_send(hello_msg, sizeof(hello_msg)-1);

  char last_char = 0;

  char *serial_buf;
  serial_buf = malloc(SERIAL_RX_BUFFER_SIZE);

  char *cmd_stmt = NULL;
  int cmd_stmt_cur = 0;

  while (true)
  {
    /**
     * 主循环
     */
    char new_char = keypad_getchar();
    if (new_char != last_char && new_char != 0)
    {
      printf("%c", new_char);
    }
    int recvlen = serial_recv(serial_buf);
    if (recvlen > 0)
    {
      led_blink();
      for (int i = 0; i < recvlen; i++)
      {
        char ch = serial_buf[i];

        if (cmd_stmt == NULL)
          cmd_stmt = calloc(1, SERIAL_RX_BUFFER_SIZE);

        if (ch == '\r')
          continue;

        if (ch == '\n')
        {
          char *msg = malloc(SERIAL_TX_BUFFER_SIZE);

          int ret = remote_exec(msg, cmd_stmt);
          int msglen = strlen(msg);

          if (ret < 0)
            printf("remote_exec: return %d\n", ret);

          if (msglen > 0)
            serial_send(msg, msglen);

          free(msg);
          free(cmd_stmt);
          cmd_stmt = NULL;
          cmd_stmt_cur = 0;

          continue;
        }
        cmd_stmt[cmd_stmt_cur] = ch;
        cmd_stmt_cur++;
      }
      memset(serial_buf, 0, SERIAL_RX_BUFFER_SIZE);
    }
    last_char = new_char;
    fflush(stdout);
    iwdg_reset();
    delay(10);
  }
}
