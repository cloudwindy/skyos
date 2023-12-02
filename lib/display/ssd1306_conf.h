/**
 * Private configuration file for the SSD1306 library.
 * This example is configured for STM32F0, I2C and including all fonts.
 */

#ifndef SSD1306_CONF_H
#define SSD1306_CONF_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

// Mirror the screen if needed
// #define SSD1306_MIRROR_VERT
// #define SSD1306_MIRROR_HORIZ

// Set inverse color if needed
// # define SSD1306_INVERSE_COLOR

// The width of the screen can be set using this
// define. The default value is 128.
// #define SSD1306_WIDTH           64

// If your screen horizontal axis does not start
// in column 0 you can use this define to
// adjust the horizontal offset
// #define SSD1306_X_OFFSET 0

// The height can be changed as well if necessary.
// It can be 32, 64 or 128. The default value is 64.
// #define SSD1306_HEIGHT          64

#ifdef SSD1306_X_OFFSET
#define SSD1306_X_OFFSET_LOWER (SSD1306_X_OFFSET & 0x0F)
#define SSD1306_X_OFFSET_UPPER ((SSD1306_X_OFFSET >> 4) & 0x07)
#else
#define SSD1306_X_OFFSET_LOWER 0
#define SSD1306_X_OFFSET_UPPER 0
#endif

/* vvv SPI config vvv */

#ifndef SSD1306_SPI
#define SSD1306_SPI SPI1
#endif

#ifndef SSD1306_BANK_MOSI
#define SSD1306_BANK_MOSI GPIO_BANK_SPI1_MOSI
#endif
#ifndef SSD1306_MOSI
#define SSD1306_MOSI GPIO_SPI1_MOSI
#endif

#ifndef SSD1306_BANK_CS
#define SSD1306_BANK_CS GPIOB
#endif
#ifndef SSD1306_CS
#define SSD1306_CS GPIO10
#endif

#ifndef SSD1306_BANK_DC
#define SSD1306_BANK_DC GPIOB
#endif
#ifndef SSD1306_DC
#define SSD1306_DC GPIO14
#endif

#ifndef SSD1306_BANK_RES
#define SSD1306_BANK_RES GPIOA
#endif
#ifndef SSD1306_RES
#define SSD1306_RES GPIO8
#endif

/* ^^^ SPI config ^^^ */

#endif /* __SSD1306_CONF_H__ */