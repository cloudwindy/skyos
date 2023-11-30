#include "app.h"
#include "state.h"

void app_handler(EvType type, void *ev)
{
  switch (g_fun) {
  case fun_home:
    home_handler(type, ev);
    break;
  default:
    break;
  }
}

