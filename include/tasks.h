#ifndef SKYRADIO_TASKS_H
#define SKYRADIO_TASKS_H

#include "os.h"

void task_init(void *args);
task task_ui, task_keypad, task_remote;

#endif
