#ifndef TASKS_H
#define TASKS_H

#include "os.h"

void task_init(void *args);
task task_ui, task_keypad, task_remote;

#endif

