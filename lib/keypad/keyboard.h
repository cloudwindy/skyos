#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stddef.h>
#include <stdint.h>

typedef struct keyboard
{
  /* Previous numeric key. */
  char key;
  /* Previous character. */
  char ch;
  /* How many times numeric key is hit. */
  uint8_t hit;
  /* Input string. */
  char *buf;
  /* How much the buffer is used. */
  uint8_t buf_cur;
  /* How big the buffer is. */
  size_t buf_len;
} Keyboard;

void kbd_init(Keyboard *kbd, char *buf, size_t size);
char kbd_input(Keyboard *kbd, char ch);
void kbd_confirm(Keyboard *kbd);
void kbd_read(Keyboard *kbd, char *buf);

#endif
