#include "app.h"
#include "state.h"
#include "printf.h"
#include "ui.h"

#include <time.h>
#include <libopencm3/stm32/rtc.h>

static void ui_status_bar(UI *ui);

void app_update_ui(UI *ui)
{
  switch (g_fun)
  {
  case fun_home:
    ui_status_bar(ui);
    home_update_ui(ui);
    break;
  case fun_fmradio:
    ui_text(ui, 0, 0, "FM");
    ui_status_bar(ui);
    home_update_ui(ui);
    break;
  default:
    break;
  }
}

void app_process_key(char key, KeyPress kp, uint32_t hold_time)
{
  switch (g_fun)
  {
  case fun_home:
    home_process_key(key, kp, hold_time);
    break;
  default:
    break;
  }
}

static void ui_status_bar(UI *ui)
{
  static time_t rawtime;
  static struct tm info;
  static char timetext[9] = {0};
  ui_line_break(ui, 16);
  /* Update time. */
  rawtime = rtc_get_counter_val();
  gmtime_r(&rawtime, &info);
  snprintf(timetext, sizeof(timetext), "%02d:%02d %s",
           info.tm_hour == 12 ? 12 : info.tm_hour % 12,
           info.tm_min,
           info.tm_hour <= 12 ? "AM" : "PM");
  ui_text(ui, sizeof(timetext) - 1, 0, timetext);
}
