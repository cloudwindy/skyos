#include "state.h"
#include "mem.h"

State g_state;

State *state(void)
{
  return &g_state;
}