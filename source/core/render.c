#include <app.h>
#include <render.h>
#include <stdlib.h>

extern handler_t handler;

void render()
{
    pixel_t p0 = { .pos.x = 0, .pos.y = 0, .color = RED };
    pixel_t p1 = { .pos.x = 100, .pos.y = 100, .color = GREEN };
    pixel_t p2 = { .pos.x = 100, .pos.y = -100, .color = BLUE };
    clear(BLACK);
    draw_triangle(&p0, &p2, &p1);
    SDL_UpdateWindowSurface(handler.window);
}

/*-----------Utils------------*/

float edge_cross(vec2_t* a, vec2_t* b, vec2_t* c)
{
    vec2_t ab = { b->x - a->x, b->y - a->x };
    vec2_t ac = { c->x - a->x, c->y - a->y };
    return ab.x * ac.y - ac.x * ab.y;
}

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
        put_pixel(x, y, p0->color);
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
    float min_y = MIN(MIN(p0->pos.y, p1->pos.y), p2->pos.y);
    float min_x = MIN(MIN(p0->pos.x, p1->pos.x), p2->pos.x);
    float max_y = MAX(MAX(p0->pos.y, p1->pos.y), p2->pos.y);
    float max_x = MAX(MAX(p0->pos.x, p1->pos.x), p2->pos.x);

    uint8_t inside = TRUE;

    for (float y = min_y; y < max_y; y++)
    {
        for (float x = min_x; x < max_x; x++)
        {
            vec2_t p = { x, y };

            float w0 = edge_cross(&p0->pos, &p1->pos, &p);
            float w1 = edge_cross(&p1->pos, &p2->pos, &p);
            float w2 = edge_cross(&p2->pos, &p0->pos, &p);

            inside = (w0 >= 0) && (w1 >= 0) && (w2 >= 0);

            if (inside)
            {
                put_pixel(x, y, WHITE);
            }
        }
    }

    draw_line(p0, p1);
    draw_line(p1, p2);
    draw_line(p2, p0);
}