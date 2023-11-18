#ifndef MEM_H
#define MEM_H

#include <stddef.h>

typedef struct mem_info
{
  size_t total;
  size_t free;
} MemInfo;

void *memalloc(size_t size);
void *memcalloc(size_t num, size_t size);
void memfree(void *ptr);
MemInfo meminfo(void);

#endif