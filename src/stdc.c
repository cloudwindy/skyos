#include "tty.h"

#include <stdio.h>
#include <errno.h>
#include <time.h>

int _write(int file, char *ptr, int len);

int _write(int file, char *ptr, int len)
{
  int i;
  if (file != 1 && file != 2)
  {
    errno = EIO;
    return -1;
  }
  tty_print(ptr, len);
  return i;
}