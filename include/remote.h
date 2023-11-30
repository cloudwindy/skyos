#ifndef COMMAND_H
#define COMMAND_H

#include <stddef.h>

#define REMOTE_MAX_RESPONSE_SIZE (32)
#define REMOTE_DATA_DELIMITER ('\n')

int remote_exec(char *response, char *statement, size_t len);

#endif
