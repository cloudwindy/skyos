#ifndef TASKS_H
#define TASKS_H

#include "os.h"

void task_init(void *args);
Task task_ui,
     task_keypad,
     task_remote,
     task_tick;

#endif

