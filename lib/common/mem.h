#ifndef MEM_H
#define MEM_H

#include <stddef.h>

void *memalloc(size_t size);
void memfree(void *ptr);

#endif