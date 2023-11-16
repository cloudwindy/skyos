#ifndef SKYRADIO_COMMAND_H
#define SKYRADIO_COMMAND_H

#include <stddef.h>

#define REMOTE_BUFFER_SIZE (128)

int remote_exec(char *response, char *statement, size_t len);

#endif