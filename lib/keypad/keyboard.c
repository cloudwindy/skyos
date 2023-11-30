#include "keyboard.h"

static char select(char key, uint8_t i);

void kbd_init(Keyboard *kbd, char *buf, size_t len)
{
  kbd->key = '\0';
  kbd->ch = '\0';
  kbd->hit = 0;
  kbd->buf = buf;
  kbd->buf_cur = 0;
  kbd->buf_len = len;
}

char kbd_input(Keyboard *kbd, char key)
{
  if (key != kbd->key)
  { /* Input new character. */
    kbd_confirm(kbd);
    kbd->ch = select(key, 0);
    kbd->key = key;
    kbd->hit = 0;
  }
  else
  { /* Select character. */
    kbd->hit++;
    kbd->ch = select(key, kbd->hit);
  }
  return kbd->ch;
}

/* Save previous character. */
void kbd_confirm(Keyboard *kbd)
{
  if (kbd->ch != '\0')
  {
    kbd->buf[kbd->buf_cur] = kbd->ch;
    kbd->buf_cur++;
    kbd->hit = 0;
    kbd->key = '\0';
    kbd->ch = '\0';
  }
}

static char select(char key, uint8_t i)
{
  switch (key) {
  case '2':
    /* abc */
    return 'a' + i % 3;
  case '3':
    /* def */
    return 'd' + i % 3;
  case '4':
    /* ghi */
    return 'g' + i % 3;
  case '5':
    /* jkl */
    return 'j' + i % 3;
  case '6':
    /* mno */
    return 'm' + i % 3;
  case '7':
    /* pqrs */
    return 'p' + i % 4;
  case '8':
    /* tuv */
    return 't' + i % 3;
  case '9':
    /* wxyz */
    return 'w' + i % 4;
  }
  return '\0';
}
