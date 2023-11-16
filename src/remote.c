#include "remote.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libopencm3/stm32/rtc.h>
#include <libopencm3/cm3/scb.h>

typedef int(ComponentHandler)(char *resp, int argc, char **argv);

typedef struct command
{
  const char *name;
  ComponentHandler *handler;
} Component;

static ComponentHandler
    time_cmd,
    reset;

static void error_arg(char *target, int expected, int got);

Component cmd_list[] = {
    {"time", time_cmd},
    {"reset", reset}};

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

  char **argv = malloc(argc);

  stmt = strtok(stmt, " ");
  char *name = stmt;
  for (size_t i = 0; stmt != NULL; i++)
  {
    argv[i] = stmt;
    stmt = strtok(NULL, " ");
  }

  for (size_t i = 0; i < sizeof(cmd_list) / sizeof(Component); i++)
    if (strcmp(name, cmd_list[i].name) == 0)
      return cmd_list[i].handler(resp, argc, argv);

  sprintf(resp, "unrecognized component: '%s'\n", name);
  return -3;
}

static int time_cmd(char *resp, int argc, char *argv[])
{
  if (argc == 1)
  {
    sprintf(resp, "time <get|set> [set:now]\n");
  }
  else if (strcmp(argv[1], "get") == 0)
  {
    sprintf(resp, "%lu\n", rtc_get_counter_val());
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
    sprintf(resp, "time set to %lu\n", val);
  }
  else
  {
    sprintf(resp, "unknown action '%s'\n", argv[1]);
  }

  return 0;
}

static int reset(char *resp, int argc, char *argv[])
{
  (void)resp;
  (void)argc;
  (void)argv;

  scb_reset_system();
}

static void error_arg(char *target, int expected, int got)
{
  sprintf(target, "not enough arguments (expected %d, got %d)\n", expected, got);
}