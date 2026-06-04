#pragma once

#define RED   (color_t){ .r = 255, .g = 0,   .b = 0,   .a = 0 }
#define GREEN (color_t){ .r = 0,   .g = 255, .b = 0,   .a = 255 }
#define BLUE  (color_t){ .r = 0,   .g = 0,   .b = 255, .a = 255 }
#define WHITE (color_t){ .r = 255, .g = 255, .b = 255, .a = 255 }
#define BLACK (color_t){ .r = 0,   .g = 0,   .b = 0,   .a = 255 }
#define PURPLE (color_t){ .r = 255,   .g = 0,   .b = 255,   .a = 255 }
#define CYAN (color_t){ .r = 0,   .g = 255,   .b = 255,   .a = 255 }
#define YELLOW (color_t){ .r = 255,   .g = 255,   .b = 0,   .a = 255 }
#define COLOR (color_t){ .r = 0,   .g = 0,   .b = 0,   .a = 255 }

#define MIN(a, b) (( (a) < (b) ) ? a : b)
#define MAX(a, b) (( (a) > (b) ) ? a : b)

typedef struct color
{
    uint8_t r, g, b, a;
} color_t;

typedef struct pixel
{
        int32_t x, y;
        color_t color;
} pixel_t;

void render();
void put_pixel(int32_t x, int32_t y, color_t color);
void clear(color_t color);
void draw_line(pixel_t *p0, pixel_t *p1);
void draw_triangle(pixel_t *p0, pixel_t *p1, pixel_t *p2);