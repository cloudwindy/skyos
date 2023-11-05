#include <stdint.h>

#ifndef SSD1306_FONTS_H
#define SSD1306_FONTS_H

#include "ssd1306_conf.h"

typedef struct
{
  const uint8_t width; /*!< Font width in pixels */
  const uint8_t height;      /*!< Font height in pixels */
  const uint8_t *data;    /*!< Pointer to data font data array */
} FontDef;

extern FontDef Font_6x8;
extern FontDef Font_7x10;
extern FontDef Font_8x16;
#endif // __SSD1306_FONTS_H__