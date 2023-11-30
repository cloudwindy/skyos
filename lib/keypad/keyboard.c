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
    char ch = select(key, 0);
    if (kbd->ch != '\0')
    { /* Save previous character. */
      kbd->buf[kbd->buf_cur] = kbd->ch;
      kbd->buf_cur++;
    }
    kbd->key = key;
    kbd->ch = ch;
    kbd->hit = 0;
  }
  else
  { /* Select character. */
    kbd->hit++;
    char ch = select(key, kbd->hit);
  }
  return kbd->ch;
}

static char select(char key, uint8_t i)
{
  char ch;
  switch (key) {
  case '2':
    /* abc */
    ch = 'a' + i % 3;
    break;
  case '3':
    /* def */
    ch = 'd' + i % 3;
    break;
  case '4':
    /* ghi */
    ch = 'g' + i % 3;
    break;
  case '5':
    /* jkl */
    ch = 'j' + i % 3;
    break;
  case '6':
    /* mno */
    ch = 'm' + i % 3;
    break;
  case '7':
    /* pqrs */
    ch = 'p' + i % 4;
    break;
  case '8':
    /* tuv */
    ch = 't' + i % 3;
    break;
  case '9':
    /* wxyz */
    ch = 'w' + i % 4;
    break;
  }
  return ch;
}
