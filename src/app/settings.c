#include "app.h"
#include "ui.h"

static void settings_update_ui(UI *uu);

void settings_handler(EvType type, void *ev)
{
  switch (type)
  {
  case ev_ui:
    settings_update_ui(ev);
    break;
  default:
    break;
  }
}

static void settings_update_ui(UI *ui)
{
  ui_text(ui, 0, 0, "");
}
