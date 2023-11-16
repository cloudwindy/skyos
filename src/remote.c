#include "remote.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <libopencm3/stm32/rtc.h>
#include <libopencm3/cm3/scb.h>

typedef int(CommandHandler)(char *resp, int argc, char **argv);

typedef struct command
{
  const char *name;
  CommandHandler *handler;
} Command;

static CommandHandler
    time_cmd,
    reset;

static void error_arg(char *target, int expected, int got);

Command cmd_list[] = {
    {"time", time_cmd},
    {"reset", reset}};

int remote_exec(char *resp, char *stmt, size_t len)
{
  if (stmt[len - 3] == ';')
    stmt[len - 3] = '\0';
  else if (stmt[len - 2] == '\r')
    stmt[len - 2] = '\0';
  else if (stmt[len - 1] == '\n')
    stmt[len - 1] = '\0';

  if (stmt[0] == '\0')
  {
    resp[0] = '\n';
    return 0;
  }

  int argc = 0;
  char *argv[REMOTE_MAX_ARGUMENTS] = {NULL};

  stmt = strtok(stmt, " ");
  char *name = stmt;
  while (stmt != NULL)
  {
    argv[argc] = stmt;
    stmt = strtok(NULL, " ");
    argc++;
  }

  for (size_t i = 0; i < sizeof(cmd_list) / sizeof(Command); i++)
    if (strcmp(name, cmd_list[i].name) == 0)
      return cmd_list[i].handler(resp, argc, argv);

  sprintf(resp, "unrecognized command: '%s'\n", name);
  return -3;
}

static int time_cmd(char *resp, int argc, char *argv[])
{
  if (argc == 1)
  {
    sprintf(resp, "time <get|set|format> [set:now]\n");
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
  else if (strcmp(argv[1], "format") == 0)
  {
    time_t rawtime = rtc_get_counter_val();
    struct tm *info = localtime(&rawtime);
    strftime(resp, REMOTE_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S %Z\n", info);
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