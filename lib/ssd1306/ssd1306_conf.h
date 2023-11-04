/**
 * Private configuration file for the SSD1306 library.
 * This example is configured for STM32F0, I2C and including all fonts.
 */

#ifndef SSD1306_CONF_H
#define SSD1306_CONF_H

#include <libopencm3/stm32/gpio.h>

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
#define SSD1306_X_OFFSET 2

// The height can be changed as well if necessary.
// It can be 32, 64 or 128. The default value is 64.
// #define SSD1306_HEIGHT          64

#endif /* __SSD1306_CONF_H__ */