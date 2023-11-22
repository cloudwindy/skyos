#include "app.h"

#include "delay.h"
#include "mem.h"
#include "os.h"

#include "led.h"
#include "keypad.h"
#include "serial.h"
#include "remote.h"
#include "ssd1306.h"
#include "tty.h"
#include "ui.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/iwdg.h>

static void task_ui(void *);
static void task_keyboard(void *);
static void task_remote(void *);
static void task_stayin_alive(void *);

static void strip_crlf(char *str, size_t *len_p);

void task_init(void *args __attribute__((unused)))
{
  printf("skyOS started\n");
  os_exec("task_ui", task_ui, NULL, 0);
  os_exec("task_keyboard", task_keyboard, NULL, 0);
  os_exec("task_remote", task_remote, NULL, 0);
  os_exec("task_stayin_alive", task_stayin_alive, NULL, 1);

  os_exit();
}

static void task_ui(void *args __attribute__((unused)))
{
  UI *ui = memalloc(sizeof(UI));
  ui_init(ui);
  ui_text(ui, 0, 0, "skyOS");
  ui_line_break(ui, 17);

  time_t rawtime;
  struct tm info;
  char timetext[9] = {0};

  while (true)
  {
    rawtime = rtc_get_counter_val();
    gmtime_r(&rawtime, &info);
    snprintf(timetext, sizeof(timetext), "%02d:%02d %s",
             info.tm_hour == 12 ? 12 : info.tm_hour % 12,
             info.tm_min,
             info.tm_hour <= 12 ? "AM" : "PM");
    ui_text(ui, sizeof(timetext) - 1, 0, timetext);
    ui_update(ui);
    os_delay(20);
  }
  os_exit();
}

/* Keyboard scan routine. */
static void task_keyboard(void *args __attribute__((unused)))
{
  char last_char = 0;
  while (true)
  {
    char new_char = keypad_getchar();
    if (new_char != last_char && new_char != 0)
    {
      printf("%c", new_char);
      fflush(stdout);
    }
    last_char = new_char;
    os_delay(100);
  }
}

static void task_remote(void *args __attribute__((unused)))
{
  /* Allocate receive buffer. */
  char *recvbuf = memalloc(2);
  size_t recvbuf_size = 2;
  /* Indicate how much buffer is used. */
  size_t recvcur = 0;
  /* Indicate how much data is incoming. */
  size_t recvlen = 0;

  while (true)
  { /* Main loop, check for incoming data. */
    recvlen = serial_recvlen();
    if (recvlen > 0)
    { /* Check if our buffer is sufficient for the incoming data. */
      if (recvbuf_size < recvcur + recvlen)
      { /* Re-allocate the old buffer with more memory. */
        char *recvbuf_old = recvbuf;
        /* TODO: we should implement realloc(). */
        recvbuf = memalloc(recvbuf_size * 2);
        memcpy(recvbuf, recvbuf_old, recvbuf_size);
        memfree(recvbuf_old);
        recvbuf_size *= 2;
      }

      /**
       * serial_recv:
       * 1. Skip the used parts of the buffer. (+ recvcur)
       * 2. Prevent overflowing. (- recvcur)
       * 3. Write to recvbuf.
       */
      serial_recv(recvbuf + recvcur, recvbuf_size - recvcur);
      /* Indicate more buffer is used. */
      recvcur += recvlen;
      /* Terminate the string. */
      recvbuf[recvcur] = '\0';
      /* The start of data block. */
      char *start_p = recvbuf;
      /* The end of data block. */
      char *end_p = strchr(start_p, REMOTE_DATA_DELIMITER);

      /* Search for data blocks as long as it's not ended. */
      while (end_p != NULL)
      { /* Data end. */
        size_t block_size = end_p - start_p + 1;
        /* Remove crlf and again terminate the string. */
        strip_crlf(start_p, &block_size);

        /* Execute command in the data block. */
        char *resp = memalloc(REMOTE_MAX_RESPONSE_SIZE);
        int ret = remote_exec(resp, start_p, block_size);
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
      { /* We just had at least one block. */
        led_blink();
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
    os_delay(50);
  }
}

static void task_stayin_alive(void *args __attribute__((unused)))
{
  while (true)
  { /* Life goin' nowhere, somebody help me. */
    iwdg_reset();
    os_delay(50);
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
