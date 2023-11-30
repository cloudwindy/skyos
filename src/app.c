#include "app.h"
#include "state.h"

void app_handler(EvType type, void *ev)
{
  switch (g_fun) {
  case fun_home:
    home_handler(type, ev);
    break;
  case fun_welcome:
    welcome_hanlder(type, ev);
    break;
  default:
    break;
  }
}

