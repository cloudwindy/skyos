#ifndef RING_H
#define RING_H

#include <stdint.h>

typedef uint8_t ring_size_t;

typedef struct ring
{
  uint8_t *data;
  ring_size_t size;
  uint32_t begin;
  uint32_t end;
} Ring;

#define RING_SIZE(RING) ((RING)->size - 1)
#define RING_DATA(RING) (RING)->data
#define RING_EMPTY(RING) ((RING)->begin == (RING)->end)

void ring_init(Ring *ring, uint8_t *buf, ring_size_t size);
int ring_write_ch(Ring *ring, uint8_t ch);
int ring_write(Ring *ring, uint8_t *data, ring_size_t size);
int ring_read_ch(Ring *ring, uint8_t *ch);

#endif