#ifndef TASKS_H
#define TASKS_H

#define KEYPAD_TICK 50
#define KEYPAD_REPEAT_INTERVAL 100

typedef void Task(void *args);

Task task_init,
     task_ui,
     task_keypad,
     task_remote,
     task_tick;

#endif

