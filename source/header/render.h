#pragma once
#include <SDL3/SDL.h>

typedef struct pixel{
        int x, y;
} pixel_t;

void Render(SDL_Surface *surface);
void putPixel(SDL_Surface *surface, int x, int y, uint32_t color);
void clear(SDL_Surface *surface, uint32_t color);
void drawLine(SDL_Surface *surface, pixel_t p0, pixel_t p1, uint32_t color);