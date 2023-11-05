#ifndef FONTS_H
#define FONTS_H

#include "ssd1306_conf.h"
#include <stdint.h>

typedef struct
{
  const uint8_t width;  /*!< Font width in pixels */
  const uint8_t height; /*!< Font height in pixels */
  const uint8_t *data;  /*!< Pointer to data font data array */
} FontDef;

extern const FontDef Font;

#endif