#ifndef W25Q_H
#define W25Q_H

#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>

#define W25Q_SPI SPI1

#define W25Q_BANK_CS GPIOB
#define W25Q_CS GPIO1

#endif