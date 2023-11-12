#include "command.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libopencm3/stm32/rtc.h>
#include <libopencm3/cm3/scb.h>

typedef int(CommandHandler)(char *resp, int argc, char **argv);

typedef struct command
{
  const char *name;
  CommandHandler *handler;
} Command;

static CommandHandler
    get_time,
    set_time, reset;

Command cmd_list[] = {
    {"get-time", get_time},
    {"set-time", set_time},
    {"reset", reset}};

int remote_exec(char *resp, const char *statement)
{
  int argc = 0;
  char **argv = NULL;
  char *str = strdup(statement);
  str = strtok(str, " ");
  char *name = strdup(str);
  if (name[0] == '\0')
  {
    resp[0] = '\n';

    free(name);
    return 0;
  }
  while (str != NULL)
  {
    void *p = realloc(argv, argc * sizeof(char));
    if (p == NULL)
    {
      sprintf(resp, "out of memory\n");

      free(name);
      free(argv);
      return 0;
    }
    argv = p;
    argv[argc] = strdup(str);
    str = strtok(NULL, " ");
    argc++;
  }
  for (size_t i = 0; i < sizeof(cmd_list) / sizeof(Command); i++)
    if (strcmp(name, cmd_list[i].name) == 0)
    {
      int ret = cmd_list[i].handler(resp, argc, argv);

      free(name);
      free(argv);
      return ret;
    }
  sprintf(resp, "unrecognized command: '%s'\n", name);

  free(name);
  free(argv);
  return -3;
}

static int get_time(char *resp, int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  sprintf(resp, "%lu\n", rtc_get_counter_val());
  return 0;
}

static int set_time(char *resp, int argc, char *argv[])
{
  if (argc == 1)
  {
    sprintf(resp, "not enough arguments (expected 1, got %d)\n", argc - 1);
    return -2;
  }
  uint32_t val = strtoul(argv[1], NULL, 10);
  rtc_set_counter_val(val);
  sprintf(resp, "time set to %lu\n", val);
  return 0;
}

static int reset(char *resp, int argc, char *argv[])
{
  (void)resp;
  (void)argc;
  (void)argv;

  scb_reset_system();
}