#include "remote.h"
#include "mem.h"
#include "printf.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/rtc.h>

typedef int(ComponentHandler)(char *resp, int argc, char **argv);

typedef struct command
{
  const char *name;
  ComponentHandler *handler;
} Component;

static ComponentHandler com_time, com_sys;

static void error_arg(char *target, int expected, int got);
static void error_action(char *target, char *action);

Component cmd_list[] = {{"time", com_time}, {"sys", com_sys}};

int remote_exec(char *resp, char *stmt, size_t len)
{
  if (stmt[0] == '\0')
  {
    resp[0] = '\n';
    resp[1] = '\0';
    return 0;
  }

  int argc = 1;
  for (size_t i = 0; i < len; i++)
    if (stmt[i] == ' ')
      argc++;

  char **argv = memalloc(argc);

  char *save;
  stmt = strtok_r(stmt, " ", &save);
  for (size_t i = 0; stmt != NULL; i++)
  {
    argv[i] = stmt;
    stmt = strtok_r(NULL, " ", &save);
  }

  for (size_t i = 0; i < sizeof(cmd_list) / sizeof(Component); i++)
  {
    if (strcmp(argv[0], cmd_list[i].name) == 0)
    {
      int ret = cmd_list[i].handler(resp, argc, argv);
      memfree(argv);
      return ret;
    }
  }
  memfree(argv);
  sprintf(resp, "unrecognized component: '%s'\n", argv[0]);
  return -3;
}

static int com_time(char *resp, int argc, char *argv[])
{
  if (argc == 1)
  {
    sprintf(resp, "time <get|set> [set:now]\n");
  }
  else if (strcmp(argv[1], "get") == 0)
  {
    sprintf(resp, "%" PRIu32 "\n", rtc_get_counter_val());
  }
  else if (strcmp(argv[1], "set") == 0)
  {
    if (argc == 2)
    {
      error_arg(resp, 1, argc - 1);
      return -2;
    }
    uint32_t val = strtoul(argv[2], NULL, 10);
    rtc_set_counter_val(val);
    sprintf(resp, "time set to %" PRIu32 "\n", val);
  }
  else
  {
    error_action(resp, argv[1]);
  }

  return 0;
}

static int com_sys(char *resp, int argc, char *argv[])
{
  if (argc == 1)
  {
    sprintf(resp, "sys <free|reset>\n");
  }
  else if (strcmp(argv[1], "free") == 0)
  {
    MemInfo info = meminfo();
    sprintf(resp, "total=%d free=%d\n", info.total, info.free);
  }
  else if (strcmp(argv[1], "reset") == 0)
  {
    scb_reset_system();
  }
  else
  {
    error_action(resp, argv[1]);
  }
  return 0;
}

static void error_arg(char *target, int expected, int got)
{
  sprintf(target, "not enough arguments (expected %d, got %d)\n", expected,
          got);
}

static void error_action(char *target, char *action)
{
  sprintf(target, "unknown action '%s'\n", action);
}
