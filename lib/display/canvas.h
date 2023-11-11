#ifndef CANVAS_H
#define CANVAS_H

#include <stdint.h>

#include "fonts.h"

typedef enum Color
{
  White,
  Black
} Color;

typedef struct Canvas
{
  uint8_t *buf;
  uint8_t cur_x;
  uint8_t cur_y;
  uint8_t width;
  uint8_t height;
} Canvas;

typedef struct Vertex
{
  uint8_t x;
  uint8_t y;
} Vertex;

void canvas_init(Canvas *canvas, uint8_t width, uint8_t height);
void canvas_fill(Canvas *canvas, Color color);
Color canvas_get_pix(Canvas *canvas, uint8_t x, uint8_t y);
void canvas_set_pix(Canvas *canvas, uint8_t x, uint8_t y, Color color);
void canvas_move_up(Canvas *canvas, uint8_t height);
char canvas_write_char(Canvas *canvas, char ch, FontDef font, Color color);
char canvas_write_string(Canvas *canvas, const char *str, FontDef font, Color color);
void canvas_set_cursor(Canvas *canvas, uint8_t x, uint8_t y);
void canvas_line(Canvas *canvas, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);
void canvas_draw_arc(Canvas *canvas, uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, Color color);
void canvas_draw_arc_with_radius_line(Canvas *canvas, uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, Color color);
void canvas_draw_circle(Canvas *canvas, uint8_t par_x, uint8_t par_y, uint8_t par_r, Color color);
void canvas_fill_circle(Canvas *canvas, uint8_t par_x, uint8_t par_y, uint8_t par_r, Color par_color);
void canvas_polyline(Canvas *canvas, Vertex *par_vertex, uint16_t par_size, Color color);
void canvas_draw_rectangle(Canvas *canvas, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);
void canvas_fill_rectangle(Canvas *canvas, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);
void canvas_draw_bitmap(Canvas *canvas, uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, Color color);

#endif