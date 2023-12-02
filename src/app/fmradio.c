#include "app.h"
#include "ui.h"

static void update_ui(UI *ui);

void fmradio_handler(EvType type, void *ev)
{
  switch (type)
  {
    case ev_ui:
      update_ui(ev);
      break;
    default:
      break;
  }
}

static void update_ui(UI *ui)
{
  ui_text(ui, 0, 0, "FM");
  ui_time(ui);
}
