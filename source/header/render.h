#pragma once

#define RED (color_t){.r=255, .g=0, .b=0, .a=255}
#define GREEN (color_t){.r=0, .g=255, .b=0, .a=255}
#define BLUE (color_t){.r=00, .g=0, .b=255, .a=255}
#define WHITE (color_t){.r=255, .g=255, .b=255, .a=255}
#define BLACK (color_t){.r=0, .g=0, .b=0, .a=255}

#define MIN(a, b) (( (a) < (b) ) ? a : b)
#define MAX(a, b) (( (a) > (b) ) ? a : b)

typedef struct color
{
        uint8_t r, g, b, a;
} color_t;

typedef struct vector2
{
        float x, y;
} vec2_t;

typedef struct pixel
{
        vec2_t pos;
        color_t color;
} pixel_t;

void render();
void put_pixel(float x, float y, color_t color);
void clear(color_t color);
void draw_line(pixel_t *p0, pixel_t *p1);
void draw_triangle(pixel_t *p0, pixel_t *p1, pixel_t *p2);