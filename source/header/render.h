#pragma once

#define RED   0xffff0000
#define GREEN 0xff00ff00
#define BLUE  0xff0000ff
#define WHITE 0xffffffff
#define BLACK 0xff000000

#define MIN(a, b) (( (a) < (b) ) ? a : b)
#define MAX(a, b) (( (a) > (b) ) ? a : b)

typedef struct vector2
{
        int32_t x, y;
} vec2_t;

typedef struct pixel
{
        int32_t x, y;
        uint32_t color;
} pixel_t;

void render();
void put_pixel(int32_t x, int32_t y, uint32_t color);
void clear(uint32_t color);
void draw_line(pixel_t *p0, pixel_t *p1);
void draw_triangle(pixel_t *p0, pixel_t *p1, pixel_t *p2);
int32_t edge_cross(vec2_t a, vec2_t b, vec2_t c);