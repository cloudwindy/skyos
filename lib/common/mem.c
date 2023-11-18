#include "mem.h"

#include "FreeRTOS.h"
#include "FreeRTOS/task.h"

/**
 * Wrapper of FreeRTOS pvPortMalloc.
 */
void *memalloc(size_t size)
{
  return pvPortMalloc(size);
}

/**
 * Wrapper of FreeRTOS vPortFree.
 */
void memfree(void *ptr)
{
  vPortFree(ptr);
}

/**
 * Wrapper of FreeRTOS xPortGetFreeHeapSize.
 */
MemInfo meminfo(void)
{
  MemInfo info = {
      .total = configTOTAL_HEAP_SIZE,
      .free = xPortGetFreeHeapSize()};
  return info;
}