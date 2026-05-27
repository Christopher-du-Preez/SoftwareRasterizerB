#include <app.h>
#include <render.h>
#include <stdlib.h>

extern handler_t handler;

pixel_t p0 = {.pos.x=0, .pos.y=0, .color=RED}
pixel_t p1 = {.pos.x=100, .pos.y=100, .color=GREEN}
pixel_t p2 = {.pos.x=100, .pos.y=-100, .color=BLUE}

void render()
{
    clear(BLACK)
    draw_line(&p0, &p1);
    SDL_UpdateWindowSurface(handler.window);
}

/*-----------Utils------------*/

/*-----------2D--------------*/

void clear(color_t color)
{
    SDL_ClearSurface(handler.surface, color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
}

void put_pixel(float x, float y, color_t color)
{
    int new_x = handler.surface->w/2 + x;
    int new_y = handler.surface->h/2 - y;
    if(new_x < 0 || new_x > handler.surface->w || new_y < 0 || new_y > handler.surface->h)
        return;
        
    SDL_WriteSurfacePixel(handler.surface, new_x, new_y, color.r, color.g, color.b, color.a);
}

void draw_line(pixel_t *p0, pixel_t *p1)
{
    float dx = abs(p1->pos.x - p0->pos.x),
        dy = -abs(p1->pos.y - p0->pos.y),
        sx = p0->pos.x < p1->pos.x ? 1 : -1,
        sy = p0->pos.y < p1->pos.y ? 1 : -1,
        x = p0->pos.x,
        y = p0->pos.y,
        error = dx + dy;
    
    while(true){
        put_pixel(x, y, p0.color);
        if(error*2 >= dy){
            if(x == p1->pos.x)
                break;
            error += dy;
            x += sx;
        }
        if(error*2 <= dx){
            if(y == p1->pos.y)
                break;
            error += dx;
            y += sy;
        }
    }
}

void draw_triangle(pixel_t *p0, pixel_t *p1, pixel_t *p2)
{

}