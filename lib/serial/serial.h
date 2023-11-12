#ifndef SERIAL_H
#define SERIAL_H

void setup_serial(void);
int serial_recv(char *buf);
int serial_send(const char *s, int len);
void serial_handler(void);

#endif