#include "ring.h"

/******************************************************************************
 * Simple ringbuffer implementation from open-bldc's libgovernor that
 * you can find at:
 * https://github.com/open-bldc/open-bldc/tree/master/source/libgovernor
 *****************************************************************************/

void ring_init(Ring *ring, uint8_t *buf, ring_size_t size)
{
  ring->data = buf;
  ring->size = size;
  ring->begin = 0;
  ring->end = 0;
}

int ring_write_ch(Ring *ring, uint8_t ch)
{
  if (((ring->end + 1) % ring->size) != ring->begin)
  {
    ring->data[ring->end++] = ch;
    ring->end %= ring->size;
    return (int)ch;
  }

  return -1;
}

int ring_write(Ring *ring, uint8_t *data, ring_size_t size)
{
  int i;

  for (i = 0; i < size; i++)
  {
    if (ring_write_ch(ring, data[i]) < 0)
      return -i;
  }

  return i;
}

int ring_read_ch(Ring *ring, uint8_t *ch)
{
  int ret = -1;

  if (ring->begin != ring->end)
  {
    ret = ring->data[ring->begin++];
    ring->begin %= ring->size;
    if (ch)
      *ch = ret;
  }

  return ret;
}
