#include <render.h>
#include <stdlib.h>
#include <stdio.h>

void Render(SDL_Surface *surface)
{
    int* x = interpolate(5, 12, 2, 1);
    if (!x)
        return (void)0;

    size_t x_length = sizeof(x);
    size_t length = x_length / sizeof(int);

    for (size_t i = 0; i < length; i++)
    {
        printf("%i\n", x[i]);
    }

    free(x);
}

/*-----------Utils------------*/

int* interpolate_int(int i0, int d0, int i1, int d1)
{
    int i_diff = abs(i1 - i0);
    if (!i_diff)
    {
        int* temp = malloc(sizeof(int));
        if (!temp)
            return NULL;
        temp[0] = d0;
        return temp;
    }
    int *value = calloc(sizeof(int), i_diff);
    if (!value)
        return NULL;
    
    float a = (float)(d1 - d0) / (i1 - i0);
    int d = d0;

    for (int i = 0; i < i_diff; i++)
    {
        value[i] = d;
        d += a;
    }
    return value;
}


/*-----------2D--------------*/

void clear(SDL_Surface *surface, const uint32_t color)
{
    SDL_ClearSurface(surface, (float)((color >> 16) & 0xff)/255, (float)((color >> 8) & 0xff)/255, (float)(color & 0xff)/255, 1);
}

void putPixel(SDL_Surface *surface, int x, int y, const uint32_t color)
{
    uint32_t new_x = surface->w/2 + x;
    uint32_t new_y = surface->h/2 - y;
    SDL_WriteSurfacePixel(surface, new_x, new_y, (uint8_t)((color >> 16) & 0xff), (uint8_t)((color >> 8) & 0xff), (uint8_t)(color & 0xff), 255);
}

void drawLine(SDL_Surface *surface, pixel_t p0, pixel_t p1, uint32_t color)
{
    int dx = abs(p1.x - p0.x),
        dy = -abs(p1.y - p0.y),
        sx = p0.x < p1.x ? 1 : -1,
        sy = p0.y < p1.y ? 1 : -1,
        x = p0.x,
        y = p0.y,
        error = dx + dy;
    
    while(true){
        putPixel(surface, x, y, color);
        if(error*2 >= dy){
            if(x == p1.x)
                break;
            error += dy;
            x += sx;
        }
        if(error*2 <= dx){
            if(y == p1.y)
                break;
            error += dx;
            y += sy;
        }
    }
}

void drawTriangle(SDL_Surface *surface, pixel_t p0, pixel_t p1, pixel_t p2, uint32_t color)
{

}