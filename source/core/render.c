#include <render.h>
#include <stdlib.h>

void Render(SDL_Surface *surface){
        
        clear(surface, 0);
        pixel_t p0 = {0, 0};
        pixel_t p1 = {50, 100};
        //putPixel(surface, 4, 5, 0xffff0000);
        drawLine(surface, p0, p1, 0xff00ff00);

}

void clear(SDL_Surface *surface, const uint32_t color){
        SDL_ClearSurface(surface, (float)((color >> 16) & 0xff)/255, (float)((color >> 8) & 0xff)/255, (float)(color & 0xff)/255, 1);
}

void putPixel(SDL_Surface *surface, int x, int y, const uint32_t color){

        uint32_t new_x = surface->w/2 + x;
        uint32_t new_y = surface->h/2 - y;
        SDL_WriteSurfacePixel(surface, new_x, new_y, (uint8_t)((color >> 16) & 0xff), (uint8_t)((color >> 8) & 0xff), (uint8_t)(color & 0xff), 255);
}

void drawLine(SDL_Surface *surface, pixel_t p0, pixel_t p1, uint32_t color){
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