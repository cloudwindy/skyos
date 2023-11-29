#include "w25q.h"
#include "w25q_opcodes.h"

#define WRITE(data) spi_send(W25Q_SPI, (uint16_t)(data))
#define READ() spi_read(W25Q_SPI)

static void write(uint8_t *data, size_t size);
static void select(void);
static void deselect(void);

uint16_t w25q_read_id(void)
{
  select();

  WRITE(OP_READ_MANUFACTURER_DEVICE_ID);
  WRITE(0);
  WRITE(0);
  WRITE(0);
  uint16_t temp = READ() << 8;
  temp |= READ();

  deselect();
  return temp;
}

void w25q_read(uint8_t *buf, uint32_t addr, size_t size)
{
  select();

  WRITE(OP_READ_DATA);
  WRITE((uint8_t)addr << 16);
  WRITE((uint8_t)addr << 8);
  WRITE((uint8_t)addr);
  for (size_t i = 0; i < size; i++)
  {
    buf[i] = READ();
  }

  deselect();
}

void w25q_sleep(void)
{
  select();

  WRITE(OP_POWER_DOWN);

  deselect();
}

void w25q_wakeup(void)
{
  select();

  WRITE(OP_RELEASE_POWER_DOWN_DEVICE_ID);

  deselect();
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
