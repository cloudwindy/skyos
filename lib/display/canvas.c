#include "canvas.h"
#include <math.h>
#include <stdlib.h>

#define CIRCLE_APPROXIMATION_SEGMENTS (36)

#ifdef ENABLE_CANVAS_DRAW_ARC
static float deg_to_rad(float par_deg);
static uint16_t normalize_to0_360(uint16_t par_deg);
#endif

void canvas_init(Canvas *canvas, uint8_t *buf, uint8_t width, uint8_t height)
{
  canvas->buf = buf;
  canvas->cur_x = 0;
  canvas->cur_y = 0;
  canvas->width = width;
  canvas->height = height;
  canvas->len = width * ((height + 7) / 8);
}

/* Fill the whole screen with the given color */
void canvas_fill(Canvas *canvas, Color color)
{
  uint32_t i;

  for (i = 0; i < canvas->len; i++)
  {
    canvas->buf[i] = (color == Black) ? 0x00 : 0xFF;
  }
}

/*
 * Draw one pixel in the screenbuffer
 * X => X Coordinate
 * Y => Y Coordinate
 * color => Pixel color
 */
void canvas_set_pix(Canvas *canvas, uint8_t x, uint8_t y, Color color)
{
  if (x >= canvas->width || y >= canvas->height)
  {
    // Don't write outside the buffer
    return;
  }

  // Draw in the right color
  if (color == White)
  {
    canvas->buf[x + (y / 8) * canvas->width] |= 1 << (y % 8);
  }
  else
  {
    canvas->buf[x + (y / 8) * canvas->width] &= ~(1 << (y % 8));
  }
}

Color canvas_get_pix(Canvas *canvas, uint8_t x, uint8_t y)
{
  return (canvas->buf[x + (y / 8) * canvas->width] & (1 << (y % 8))) ? White
                                                                     : Black;
}

void canvas_move_up(Canvas *canvas, uint8_t height)
{
  if (height >= canvas->height)
  {
    return;
  }
  for (uint8_t x = 0; x < canvas->width; x++)
  {
    for (uint8_t y = 0; y < height; y++)
      canvas_set_pix(canvas, x, y, Black);
    for (uint8_t y = 0; y < canvas->height - height; y++)
      canvas_set_pix(canvas, x, y, canvas_get_pix(canvas, x, y + height));
    for (uint8_t y = canvas->height; y > canvas->height - height; y--)
      canvas_set_pix(canvas, x, y, Black);
  }
}

/*
 * Draw 1 char to the screen buffer
 * ch        => character to be written
 * font      => Font used
 * color     => Black or White
 * overwrite => Overwrite existing character
 */
char canvas_write_char(Canvas *canvas, char ch, FontDef font, Color color,
                       bool overwrite)
{
  uint32_t i, b, j;

  // Check if character is valid
  if (ch < 32 || ch > 126)
    return 0;

  // Check remaining space on current line
  if (canvas->width < (canvas->cur_x + font.width) ||
      canvas->height < (canvas->cur_y + font.height))
  {
    // Not enough space on current line
    return 0;
  }

  // Use the font to write
  for (i = 0; i < font.height; i++)
  {
    b = (uint32_t)font.data[(ch - 32) * font.height + i] << 4;
    for (j = 0; j < font.width; j++)
    {
      if ((b << j) & 0x0800)
      {
        canvas_set_pix(canvas, canvas->cur_x + j, canvas->cur_y + i,
                       (Color)color);
      }
      else if (overwrite)
      {
        canvas_set_pix(canvas, canvas->cur_x + j, canvas->cur_y + i,
                       (Color)!color);
      }
    }
  }

  // The current space is now taken
  canvas->cur_x += font.width;

  // Return written char for validation
  return ch;
}

/* Write full string to screenbuffer */
char canvas_write_string(Canvas *canvas, const char *str, FontDef font,
                         Color color, bool overwrite)
{
  while (*str)
  {
    if (canvas_write_char(canvas, *str, font, color, overwrite) != *str)
    {
      // Char could not be written
      return *str;
    }
    str++;
  }

  // Everything ok
  return '\0';
}

/* Position the cursor */
void canvas_set_cursor(Canvas *canvas, uint8_t x, uint8_t y)
{
  canvas->cur_x = x;
  canvas->cur_y = y;
}

/* Draw line by Bresenhem's algorithm */
void canvas_line(Canvas *canvas, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                 Color color)
{
  int32_t deltaX = abs(x2 - x1);
  int32_t deltaY = abs(y2 - y1);
  int32_t signX = ((x1 < x2) ? 1 : -1);
  int32_t signY = ((y1 < y2) ? 1 : -1);
  int32_t error = deltaX - deltaY;
  int32_t error2;

  canvas_set_pix(canvas, x2, y2, color);

  while ((x1 != x2) || (y1 != y2))
  {
    canvas_set_pix(canvas, x1, y1, color);
    error2 = error * 2;
    if (error2 > -deltaY)
    {
      error -= deltaY;
      x1 += signX;
    }

    if (error2 < deltaX)
    {
      error += deltaX;
      y1 += signY;
    }
  }
  return;
}

/* Draw polyline */
void canvas_polyline(Canvas *canvas, Vertex *par_vertex, uint16_t par_size,
                     Color color)
{
  uint16_t i;
  if (par_vertex == NULL)
  {
    return;
  }

  for (i = 1; i < par_size; i++)
  {
    canvas_line(canvas, par_vertex[i - 1].x, par_vertex[i - 1].y,
                par_vertex[i].x, par_vertex[i].y, color);
  }

  return;
}

#ifdef ENABLE_CANVAS_DRAW_ARC
/*
 * DrawArc. Draw angle is beginning from 4 quart of trigonometric circle (3pi/2)
 * start_angle in degree
 * sweep in degree
 */
void canvas_draw_arc(Canvas *canvas, uint8_t x, uint8_t y, uint8_t radius,
                     uint16_t start_angle, uint16_t sweep, Color color)
{
  float approx_degree;
  uint32_t approx_segments;
  uint8_t xp1, xp2;
  uint8_t yp1, yp2;
  uint32_t count = 0;
  uint32_t loc_sweep = 0;
  float rad;

  loc_sweep = normalize_to0_360(sweep);

  count =
    (normalize_to0_360(start_angle) * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
  approx_segments = (loc_sweep * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
  approx_degree = loc_sweep / (float)approx_segments;
  while (count < approx_segments)
  {
    rad = deg_to_rad(count * approx_degree);
    xp1 = x + (int8_t)(sin(rad) * radius);
    yp1 = y + (int8_t)(cos(rad) * radius);
    count++;
    if (count != approx_segments)
    {
      rad = deg_to_rad(count * approx_degree);
    }
    else
    {
      rad = deg_to_rad(loc_sweep);
    }
    xp2 = x + (int8_t)(sin(rad) * radius);
    yp2 = y + (int8_t)(cos(rad) * radius);
    canvas_line(canvas, xp1, yp1, xp2, yp2, color);
  }

  return;
}

/*
 * Draw arc with radius line
 * Angle is beginning from 4 quart of trigonometric circle (3pi/2)
 * start_angle: start angle in degree
 * sweep: finish angle in degree
 */
void canvas_draw_arc_with_radius_line(Canvas *canvas, uint8_t x, uint8_t y,
                                      uint8_t radius, uint16_t start_angle,
                                      uint16_t sweep, Color color)
{
  float approx_degree;
  uint32_t approx_segments;
  uint8_t xp1 = 0;
  uint8_t xp2 = 0;
  uint8_t yp1 = 0;
  uint8_t yp2 = 0;
  uint32_t count = 0;
  uint32_t loc_sweep = 0;
  float rad;

  loc_sweep = normalize_to0_360(sweep);

  count =
    (normalize_to0_360(start_angle) * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
  approx_segments = (loc_sweep * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
  approx_degree = loc_sweep / (float)approx_segments;

  rad = deg_to_rad(count * approx_degree);
  uint8_t first_point_x = x + (int8_t)(sin(rad) * radius);
  uint8_t first_point_y = y + (int8_t)(cos(rad) * radius);
  while (count < approx_segments)
  {
    rad = deg_to_rad(count * approx_degree);
    xp1 = x + (int8_t)(sin(rad) * radius);
    yp1 = y + (int8_t)(cos(rad) * radius);
    count++;
    if (count != approx_segments)
    {
      rad = deg_to_rad(count * approx_degree);
    }
    else
    {
      rad = deg_to_rad(loc_sweep);
    }
    xp2 = x + (int8_t)(sin(rad) * radius);
    yp2 = y + (int8_t)(cos(rad) * radius);
    canvas_line(canvas, xp1, yp1, xp2, yp2, color);
  }

  // Radius line
  canvas_line(canvas, x, y, first_point_x, first_point_y, color);
  canvas_line(canvas, x, y, xp2, yp2, color);
  return;
}
#endif

/* Draw circle by Bresenhem's algorithm */
void canvas_draw_circle(Canvas *canvas, uint8_t par_x, uint8_t par_y,
                        uint8_t par_r, Color par_color)
{
  int32_t x = -par_r;
  int32_t y = 0;
  int32_t err = 2 - 2 * par_r;
  int32_t e2;

  if (par_x >= canvas->width || par_y >= canvas->height)
  {
    return;
  }

  do
  {
    canvas_set_pix(canvas, par_x - x, par_y + y, par_color);
    canvas_set_pix(canvas, par_x + x, par_y + y, par_color);
    canvas_set_pix(canvas, par_x + x, par_y - y, par_color);
    canvas_set_pix(canvas, par_x - x, par_y - y, par_color);
    e2 = err;

    if (e2 <= y)
    {
      y++;
      err = err + (y * 2 + 1);
      if (-x == y && e2 <= x)
      {
        e2 = 0;
      }
    }

    if (e2 > x)
    {
      x++;
      err = err + (x * 2 + 1);
    }
  } while (x <= 0);

  return;
}

/* Draw filled circle. Pixel positions calculated using Bresenham's algorithm */
void canvas_fill_circle(Canvas *canvas, uint8_t par_x, uint8_t par_y,
                        uint8_t par_r, Color par_color)
{
  int32_t x = -par_r;
  int32_t y = 0;
  int32_t err = 2 - 2 * par_r;
  int32_t e2;

  if (par_x >= canvas->width || par_y >= canvas->height)
  {
    return;
  }

  do
  {
    for (uint8_t _y = (par_y + y); _y >= (par_y - y); _y--)
    {
      for (uint8_t _x = (par_x - x); _x >= (par_x + x); _x--)
      {
        canvas_set_pix(canvas, _x, _y, par_color);
      }
    }

    e2 = err;
    if (e2 <= y)
    {
      y++;
      err = err + (y * 2 + 1);
      if (-x == y && e2 <= x)
      {
        e2 = 0;
      }
    }

    if (e2 > x)
    {
      x++;
      err = err + (x * 2 + 1);
    }
  } while (x <= 0);

  return;
}

/* Draw a rectangle */
void canvas_draw_rectangle(Canvas *canvas, uint8_t x1, uint8_t y1, uint8_t x2,
                           uint8_t y2, Color color)
{
  canvas_line(canvas, x1, y1, x2, y1, color);
  canvas_line(canvas, x2, y1, x2, y2, color);
  canvas_line(canvas, x2, y2, x1, y2, color);
  canvas_line(canvas, x1, y2, x1, y1, color);

  return;
}

/* Draw a filled rectangle */
void canvas_fill_rectangle(Canvas *canvas, uint8_t x1, uint8_t y1, uint8_t x2,
                           uint8_t y2, Color color)
{
  uint8_t x_start = ((x1 <= x2) ? x1 : x2);
  uint8_t x_end = ((x1 <= x2) ? x2 : x1);
  uint8_t y_start = ((y1 <= y2) ? y1 : y2);
  uint8_t y_end = ((y1 <= y2) ? y2 : y1);

  for (uint8_t y = y_start; (y <= y_end) && (y < canvas->height); y++)
  {
    for (uint8_t x = x_start; (x <= x_end) && (x < canvas->width); x++)
    {
      canvas_set_pix(canvas, x, y, color);
    }
  }
  return;
}

/* Draw a bitmap */
void canvas_draw_bitmap(Canvas *canvas, uint8_t x, uint8_t y,
                        const uint8_t *bitmap, uint8_t w, uint8_t h,
                        Color color)
{
  int16_t byte_width = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  if (x >= canvas->width || y >= canvas->height)
  {
    return;
  }

  for (uint8_t j = 0; j < h; j++, y++)
  {
    for (uint8_t i = 0; i < w; i++)
    {
      if (i & 7)
      {
        byte <<= 1;
      }
      else
      {
        byte = (*(uint8_t *)(&bitmap[j * byte_width + i / 8]));
      }

      if (byte & 0x80)
      {
        canvas_set_pix(canvas, x + i, y, color);
      }
    }
  }
  return;
}

#ifdef ENABLE_CANVAS_DRAW_ARC
/* Convert Degrees to Radians */
static float deg_to_rad(float par_deg) { return par_deg * 3.14 / 180.0; }

/* Normalize degree to [0;360] */
static uint16_t normalize_to0_360(uint16_t par_deg)
{
  uint16_t loc_angle;
  if (par_deg <= 360)
  {
    loc_angle = par_deg;
  }
  else
  {
    loc_angle = par_deg % 360;
    loc_angle = ((par_deg != 0) ? par_deg : 360);
  }
  return loc_angle;
}
#endif
