#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>

#define SERIAL_TX_BUFFER_SIZE (512)
#define SERIAL_RX_BUFFER_SIZE (512)

int serial_recv(char *buf, size_t len);
int serial_send(const char *s, size_t len);
void serial_handler(void);

#endif