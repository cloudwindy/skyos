#ifndef RTOS_H
#define RTOS_H

#include <stdint.h>

typedef void task(void*);

void os_start(void);
void os_exec(const char *name, task *task_p, void *const args, uint32_t priority);
void os_exit(void);
void os_mutex_lock(void);
void os_mutex_unlock(void);

#endif