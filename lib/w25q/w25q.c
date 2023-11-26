#include "w25q.h"
#include "w25q_opcodes.h"

#include <stddef.h>
#include <libopencm3/stm32/spi.h>

#define WRITE(data) spi_send(W25Q_SPI, (uint16_t)(data))

static void write(uint8_t *data, size_t size);
static void select(void);
static void deselect(void);

void setup_flash(void)
{
  deselect();
  WRITE(OP_ENABLE_RESET_AND_RESET_DEVICE(0));
  WRITE(OP_ENABLE_RESET_AND_RESET_DEVICE(1));
  
}

static void write(uint8_t *data, size_t size)
{
  while (size--)
  {
    WRITE((uint16_t)*data++);
  }
}

static void select(void)
{
  gpio_clear(W25Q_BANK_CS, W25Q_CS);
}

static void deselect(void)
{
  gpio_set(W25Q_BANK_CS, W25Q_CS);
}