#pragma once

#include <math_b.h>

#define RED    (color_t){ .r = 255, .g = 0,   .b = 0,   .a = 255, .val = 1.0f }
#define GREEN  (color_t){ .r = 0,   .g = 255, .b = 0,   .a = 255, .val = 1.0f }
#define BLUE   (color_t){ .r = 0,   .g = 0,   .b = 255, .a = 255, .val = 1.0f }
#define WHITE  (color_t){ .r = 255, .g = 255, .b = 255, .a = 255, .val = 1.0f }
#define BLACK  (color_t){ .r = 0,   .g = 0,   .b = 0,   .a = 255, .val = 1.0f }
#define PURPLE (color_t){ .r = 255, .g = 0,   .b = 255, .a = 255, .val = 1.0f }
#define CYAN   (color_t){ .r = 0,   .g = 255, .b = 255, .a = 255, .val = 1.0f }
#define YELLOW (color_t){ .r = 255, .g = 255, .b = 0,   .a = 255, .val = 1.0f }
#define COLOR  (color_t){ .r = 0,   .g = 0,   .b = 0,   .a = 255, .val = 1.0f }

#define MIN(a, b) (( (a) < (b) ) ? a : b)
#define MAX(a, b) (( (a) > (b) ) ? a : b)

#define WIDTH 640
#define HEIGHT 480
typedef struct color
{
    uint8_t r, g, b, a;
    float val;
} color_t;

typedef struct pixel
{
        int32_t x, y;
        color_t color;
} pixel_t;

void render();
void put_pixel(int32_t x, int32_t y, color_t color);
void clear(color_t color);
void draw_line(vec2_t *p0, vec2_t *p1, color_t color);
void draw_triangle(vec2_t point[3], color_t color[3]);