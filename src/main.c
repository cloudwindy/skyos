#include "setup.h"
#include "led.h"
#include "delay.h"
#include "keypad.h"
#include "serial.h"
#include "remote.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libopencm3/stm32/iwdg.h>

int main(void)
{
  setup();
  const char hello_msg[] = "UART command line utility\n";
  serial_send(hello_msg, sizeof(hello_msg) - 1);

  char last_char = 0;

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
    last_char = new_char;

    static char recvbuf[REMOTE_BUFFER_SIZE];
    static int recvcur = 0;

    int recvlen = serial_recv(recvbuf + recvcur, REMOTE_BUFFER_SIZE - recvcur);
    recvcur += recvlen;
    if (recvlen > 0)
    {
      /* DATA RX */
      led_blink();
      recvbuf[recvcur] = '\0';
      char *recvnext = recvbuf;
      for (char *end = strchr(recvnext, '\n'); end != NULL; end = strchr(recvnext, '\n'))
      {
        /* DATA EOF */
        char *msg = malloc(REMOTE_BUFFER_SIZE);

        int ret = remote_exec(msg, recvnext, end - recvnext + 1);
        int msglen = strlen(msg);

        if (ret < 0)
          printf("remote_exec: return %d\n", ret);

        if (msglen > 0)
          serial_send(msg, msglen);

        free(msg);
        recvcur = 0;
        recvnext = end + 1;
        break;
      }
    }

    fflush(stdout);
    iwdg_reset();
    delay(10);
  }
}
