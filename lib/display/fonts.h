#ifndef FONTS_H
#define FONTS_H

#include "ssd1306_conf.h"
#include <stdint.h>

typedef struct font
{
  const uint8_t width;
  const uint8_t height;
  const uint8_t *data;
} FontDef;

extern const FontDef Font;

#endif