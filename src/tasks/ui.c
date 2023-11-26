#include "tasks.h"

#include "mem.h"
#include "ui.h"
#include "state.h"
#include "printf.h"

#include <time.h>
#include <string.h>
#include <libopencm3/stm32/rtc.h>

void task_ui(void *args __attribute__((unused)))
{
  UI ui;
  ui_init(&ui);
  ui_text(&ui, 0, 0, "skyOS");
  ui_line_break(&ui, 16);
  ui_update(&ui);
  os_delay(1000);
  ui_text_clear(&ui, 0, 0, 5);

  time_t rawtime;
  struct tm info;
  char timetext[9] = {0};

  while (true)
  {
    ui_clear(&ui);
    ui_line_break(&ui, 16);
    { /* Update frequency. */
      char freq_str[16 + 1];
      State *st = state();
      switch (st->fs_mode)
      {
      case fs_vfo:
        snprintf(freq_str, sizeof(freq_str),
                 " VFO %3d.%06d", st->vfo_freq / 1000000,
                 st->vfo_freq % 1000000);
        break;
      case fs_mr:
        strcpy(freq_str, " MR ");
        break;
      }
      ui_text(&ui, 0, 2, freq_str);
    }
    { /* Update time. */
      rawtime = rtc_get_counter_val();
      gmtime_r(&rawtime, &info);
      snprintf(timetext, sizeof(timetext), "%02d:%02d %s",
               info.tm_hour == 12 ? 12 : info.tm_hour % 12,
               info.tm_min,
               info.tm_hour <= 12 ? "AM" : "PM");
      ui_text(&ui, sizeof(timetext) - 1, 0, timetext);
    }
    ui_update(&ui);
    os_delay(50);
  }
  os_exit();
}