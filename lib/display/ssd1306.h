/**
 * This Library was originally written by Olivier Van den Eede (4ilo) in 2016.
 * Some refactoring was done and SPI support was added by Aleksander Alekseev (afiskon) in 2018.
 *
 * https://github.com/afiskon/stm32-ssd1306
 */

#ifndef SSD1306_H
#define SSD1306_H

#include <libopencm3/stm32/spi.h>

#include "ssd1306_conf.h"

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

// SSD1306 OLED height in pixels
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT 64
#endif

// SSD1306 width in pixels
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH 128
#endif

void setup_ssd1306(void);

/**
 * @brief Sets the contrast of the display.
 * @param[in] value contrast to set.
 * @note Contrast increases as the value increases.
 * @note RESET = 7Fh.
 */
void ssd1306_set_contrast(const uint8_t value);

/**
 * @brief Set Display ON/OFF.
 * @param[in] on 0 for OFF, any for ON.
 */
void ssd1306_set_display_on(bool on);

void ssd1306_update(uint8_t *buf);

#endif // __SSD1306_H__