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

static void strip_crlf(char *str, size_t *len_p);

int main(void)
{
  setup();

  int ret;

  static const char hello_msg[] = "UART command line utility\n";
  ret = serial_send(hello_msg, sizeof(hello_msg) - 1);
  if (ret < 0)
  {
    printf("serial_send: %d\n", ret);
  }

  char last_char = 0;

  while (true)
  {
    /* keyboard scan routine */
    char new_char = keypad_getchar();
    if (new_char != last_char && new_char != 0)
    {
      printf("%c", new_char);
    }
    last_char = new_char;

    /* complete buffer */
    static char recvbuf[REMOTE_BUFFER_SIZE];
    /* how much buffer is used */
    static int recvcur = 0;

    /**
     * serial_recv:
     * 1. skip the used parts of the buffer (+ recvcur)
     * 2. prevent overflowing (- recvcur)
     * 3. write to recvbuf
     */
    int recvlen = serial_recv(recvbuf + recvcur, REMOTE_BUFFER_SIZE - recvcur);
    /* indicate more buffer is used */
    recvcur += recvlen;
    if (recvlen > 0)
    { /* data RX */
      led_blink();
      /* terminate the string */
      recvbuf[recvcur] = '\0';
      /* the start of data block */
      char *start_p = recvbuf;
      /* the end of data block */
      char *end_p = strchr(start_p, REMOTE_DATA_DELIMITER);

      while (end_p != NULL)
      { /* data end */
        size_t block_len = end_p - start_p + 1;
        /* strip crlf and again terminate the string */
        strip_crlf(start_p, &block_len);

        char *resp = malloc(REMOTE_MAX_RESPONSE_SIZE);
        ret = remote_exec(resp, start_p, block_len);
        size_t resp_len = strlen(resp);

        if (ret < 0)
        { /* failed */
          printf("remote_exec: %d\n", ret);
        }

        if (resp_len > 0)
        { /* we have something to reply */
          ret = serial_send(resp, resp_len);
          if (ret < 0)
          { /* ...but we couldn't */
            printf("serial_send: %d\n", ret);
          }
        }

        free(resp);
        /* move to the start of next data block */
        start_p = end_p + 1;
        if (start_p[0] != '\0')
        { /* find the end of next data block, if there is */
          end_p = strchr(start_p, REMOTE_DATA_DELIMITER);
        }
        else
        {
          break;
        }
      }
      if (start_p != recvbuf)
      { /* we just had RX */
        if (start_p[0] != '\0')
        { /* but the last block didn't end properly */
          size_t block_len = strlen(start_p);
          /* move it to the start of buffer */
          memmove(recvbuf, start_p, block_len);
          recvcur = block_len;
        }
        else
        {
          recvcur = 0;
        }
      }
    }

    fflush(stdout);
    iwdg_reset();
    delay(10);
  }
}

static void strip_crlf(char *str, size_t *len_p)
{
  int len = *len_p;
  if (str[len - 2] == '\r')
  {
    str[len - 2] = '\0';
    *len_p -= 2;
  }
  else if (str[len - 1] == '\n')
  {
    str[len - 1] = '\0';
    *len_p -= 1;
  }
  else
  {
    str[len] = '\0';
  }
}