#include "app.h"

#include "delay.h"
#include "mem.h"

#include "led.h"
#include "keypad.h"
#include "serial.h"
#include "remote.h"
#include "ssd1306.h"
#include "tty.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libopencm3/stm32/iwdg.h>

static void strip_crlf(char *str, size_t *len_p);

void app(void *args)
{
  (void)args;

  static const char hello_msg[] = "UART command line utility\n";
  int ret = serial_send(hello_msg, sizeof(hello_msg) - 1);
  if (ret < 0)
  {
    printf("serial_send: %d\n", ret);
  }

  char last_char = 0;

  while (true)
  {
    /* Keyboard scan routine. */
    char new_char = keypad_getchar();
    if (new_char != last_char && new_char != 0)
    {
      printf("%c", new_char);
    }
    last_char = new_char;

    /* Receive buffer. */
    static char recvbuf[REMOTE_BUFFER_SIZE];
    /* Indicate how much buffer is used. */
    static int recvcur = 0;

    /**
     * serial_recv:
     * 1. Skip the used parts of the buffer. (+ recvcur)
     * 2. Prevent overflowing. (- recvcur)
     * 3. Write to recvbuf.
     */
    int recvlen = serial_recv(recvbuf + recvcur, REMOTE_BUFFER_SIZE - recvcur);
    /* Indicate more buffer is used. */
    recvcur += recvlen;
    if (recvlen > 0)
    { /* Data RX. */
      led_blink();
      /* Terminate the string. */
      recvbuf[recvcur] = '\0';
      /* The start of data block. */
      char *start_p = recvbuf;
      /* The end of data block. */
      char *end_p = strchr(start_p, REMOTE_DATA_DELIMITER);

      while (end_p != NULL)
      { /* Data end. */
        size_t block_size = end_p - start_p + 1;
        /* Remove crlf and again terminate the string. */
        strip_crlf(start_p, &block_size);

        /* Execute command in the data block. */
        char *resp = memalloc(REMOTE_MAX_RESPONSE_SIZE);
        ret = remote_exec(resp, start_p, block_size);
        size_t resp_len = strlen(resp);

        if (ret < 0)
        { /* Failed. */
          printf("remote_exec: %d\n", ret);
        }

        if (resp_len > 0)
        { /* We have something to reply. */
          ret = serial_send(resp, resp_len);
          if (ret < 0)
          { /* ...but we couldn't. */
            printf("serial_send: %d\n", ret);
          }
        }

        memfree(resp);
        /* Move to the start of next data block. */
        start_p = end_p + 1;
        if (start_p[0] != '\0')
        { /* Find the end of next data block, if there is. */
          end_p = strchr(start_p, REMOTE_DATA_DELIMITER);
        }
        else
        {
          break;
        }
      }
      if (start_p != recvbuf)
      { /* We just had RX. */
        if (start_p[0] != '\0')
        { /* ...but the last block didn't end properly. */
          size_t block_size = strlen(start_p);
          /* Move it to the start of buffer. */
          memmove(recvbuf, start_p, block_size);
          recvcur = block_size;
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