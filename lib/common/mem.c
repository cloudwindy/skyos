#include "mem.h"

#include "FreeRTOS.h"
#include "task.h"

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