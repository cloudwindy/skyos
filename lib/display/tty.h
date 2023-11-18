#ifndef TTY_H
#define TTY_H

void setup_tty(void);

#ifdef ENABLE_TTY
void tty_print(char *str, int len);
void tty_update(void);
void tty_clear(void);

#endif // defined(ENABLE_TTY)

#endif