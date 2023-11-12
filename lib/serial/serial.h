#ifndef SERIAL_H
#define SERIAL_H

#define SERIAL_TX_BUFFER_SIZE (128)
#define SERIAL_RX_BUFFER_SIZE (128)

void setup_serial(void);
int serial_recv(char *buf);
int serial_send(const char *s, int len);
void serial_handler(void);

#endif