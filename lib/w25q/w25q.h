#ifndef W25Q_H
#define W25Q_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <stddef.h>

#define W25Q_SPI SPI1

#define W25Q_BANK_CS GPIOB
#define W25Q_CS GPIO1

void w25q_sleep(void);
void w25q_wakeup(void);
uint16_t w25q_read_id(void);
void w25q_read(uint8_t *buf, uint32_t addr, size_t size);

#endif
