/**
 * This Library was originally written by Olivier Van den Eede (4ilo) in 2016.
 * Some refactoring was done and SPI support was added by Aleksander Alekseev (afiskon) in 2018.
 *
 * https://github.com/afiskon/stm32-ssd1306
 */

#ifndef SSD1306_H
#define SSD1306_H

#include <stddef.h>
#include <_ansi.h>
#include <libopencm3/stm32/spi.h>

_BEGIN_STD_C

#include "ssd1306_conf.h"

#ifdef SSD1306_X_OFFSET
#define SSD1306_X_OFFSET_LOWER (SSD1306_X_OFFSET & 0x0F)
#define SSD1306_X_OFFSET_UPPER ((SSD1306_X_OFFSET >> 4) & 0x07)
#else
#define SSD1306_X_OFFSET_LOWER 0
#define SSD1306_X_OFFSET_UPPER 0
#endif

#include "ssd1306_fonts.h"

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

#ifndef SSD1306_BUFFER_SIZE
#define SSD1306_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT / 8)
#endif

// Enumeration for screen colors
typedef enum
{
  Black = 0x00, // Black color, no pixel
  White = 0x01  // Pixel is set. Color depends on OLED
} SSD1306_Color;

typedef enum
{
  SSD1306_OK = 0x00,
  SSD1306_ERR = 0x01 // Generic error.
} SSD1306_Error_t;

// Struct to store transformations
typedef struct
{
  uint16_t CurrentX;
  uint16_t CurrentY;
  uint8_t Initialized;
  uint8_t DisplayOn;
} SSD1306_t;

typedef struct
{
  uint8_t x;
  uint8_t y;
} SSD1306_VERTEX;

// Procedure definitions
void setup_ssd1306(void);
void ssd1306_fill(SSD1306_Color color);
void ssd1306_update(void);
SSD1306_Color ssd1306_get_pixel(uint8_t x, uint8_t y);
void ssd1306_draw_pixel(uint8_t x, uint8_t y, SSD1306_Color color);
char ssd1306_write_char(char ch, FontDef Font, SSD1306_Color color);
char ssd1306_write_string(char *str, FontDef Font, SSD1306_Color color);
void ssd1306_set_cursor(uint8_t x, uint8_t y);
void ssd1306_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color);
void ssd1306_draw_arc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_Color color);
void ssd1306_draw_arc_with_radius_line(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_Color color);
void ssd1306_draw_circle(uint8_t par_x, uint8_t par_y, uint8_t par_r, SSD1306_Color color);
void ssd1306_fill_circle(uint8_t par_x, uint8_t par_y, uint8_t par_r, SSD1306_Color par_color);
void ssd1306_polyline(const SSD1306_VERTEX *par_vertex, uint16_t par_size, SSD1306_Color color);
void ssd1306_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color);
void ssd1306_fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color);
void ssd1306_draw_bitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, SSD1306_Color color);

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
void ssd1306_set_display_on(const uint8_t on);

/**
 * @brief Reads DisplayOn state.
 * @return  0: OFF.
 *          1: ON.
 */
uint8_t ssd1306_get_display_on(void);

// Low-level procedures
void ssd1306_reset(void);
void ssd1306_write_command(uint8_t byte);
void ssd1306_write_data(uint8_t *buffer, size_t buff_size);
void ssd1306_fill_buffer(uint8_t *buf, uint32_t len);
void ssd1306_move_up(uint8_t height);

_END_STD_C

#endif // __SSD1306_H__