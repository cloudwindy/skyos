#include "app.h"
#include "tasks.h"
#include "ui.h"

void boot_handler(EvType type, void *ev)
{
  if (type == ev_ui)
  {
    UI *ui = ev;
    ui_text(ui, 0, 0, "skyOS");
    ui_line_break(ui, 16);
  }
}
