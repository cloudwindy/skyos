#include "command.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libopencm3/stm32/rtc.h>

typedef int(CommandHandler)(char *response, int argc, char **argv);

typedef struct command
{
  const char *name;
  CommandHandler *handler;
} Command;

static CommandHandler gettime;

Command cmd_list[] = {
    {"gettime", gettime},
};

int remote_exec(char *response, const char *statement)
{
  int argc = 0;
  char **argv = NULL;
  char *str = strdup(statement);
  str = strtok(str, " ");
  char *name = strdup(str);
  if (name[0] == '\0')
  {
    sprintf(response, "\n");

    free(name);
    return 0;
  }
  while (str != NULL)
  {
    void *p = realloc(argv, argc * sizeof(char));
    if (p == NULL) {
      sprintf(response, "out of memory\n");

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
      int ret = cmd_list[i].handler(response, argc, argv);

      free(name);
      free(argv);
      return ret;
    }
  sprintf(response, "unrecognized command: '%s'\n", name);

  free(name);
  free(argv);
  return -3;
}

static int gettime(char *response, int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  sprintf(response, "%lu\n", rtc_get_counter_val());
  return 0;
}