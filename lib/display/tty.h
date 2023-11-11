#ifndef SKYRADIO_TTY_H
#define SKYRADIO_TTY_H

void setup_tty(void);
void tty_print(char *str, int len);
void tty_flush(void);
void tty_clear(void);

#endif