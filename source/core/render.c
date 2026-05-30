#include <app.h>
#include <render.h>

extern handler_t handler;

void render()
{
    pixel_t p0 = { .x = 0, .y = 0, .color = RED };
    pixel_t p1 = { .x = 100, .y = 100, .color = GREEN };
    pixel_t p2 = { .x = 100, .y = -100, .color = BLUE };
    clear(BLACK);
    draw_triangle(&p0, &p2, &p1);
}

/*-----------Utils------------*/

int32_t edge_cross(vec2_t a, vec2_t b, vec2_t c)
{
    vec2_t ab = { b.x - a.x, b.y - a.x };
    vec2_t ac = { c.x - a.x, c.y - a.y };
    return ab.x * ac.y - ac.x * ab.y;
}

/*-----------2D--------------*/

void clear(uint32_t color)
{
    size_t length = WIDTH * HEIGHT;
    for (size_t i = 0; i < length; i++)
    {
        handler.frame_buffer[i] = color;
    }
}

void put_pixel(int32_t x, int32_t y, uint32_t color)
{
    uint32_t new_x = WIDTH/2 + x;
    uint32_t new_y = HEIGHT/2 - y;
    if(new_x > WIDTH || new_y > HEIGHT)
        return;
    handler.frame_buffer[(WIDTH * new_y) + new_x] = color;
}

void draw_line(pixel_t *p0, pixel_t *p1)
{
    int32_t dx = abs(p1->x - p0->x),
        dy = -abs(p1->y - p0->y),
        sx = p0->x < p1->x ? 1 : -1,
        sy = p0->y < p1->y ? 1 : -1,
        x = p0->x,
        y = p0->y,
        error = dx + dy;
    
    while(TRUE){
        put_pixel(x, y, p0->color);
        if(error*2 >= dy){
            if(x == p1->x)
                break;
            error += dy;
            x += sx;
        }
        if(error*2 <= dx){
            if(y == p1->y)
                break;
            error += dx;
            y += sy;
        }
    }
}

void draw_triangle(pixel_t *p0, pixel_t *p1, pixel_t *p2)
{
    vec2_t v0 = { p0->x, p0->y };
    vec2_t v1 = { p1->x, p1->y };
    vec2_t v2 = { p2->x, p2->y };

    int32_t min_y = MIN(MIN(p0->y, p1->y), p2->y);
    int32_t min_x = MIN(MIN(p0->x, p1->x), p2->x);
    int32_t max_y = MAX(MAX(p0->y, p1->y), p2->y);
    int32_t max_x = MAX(MAX(p0->x, p1->x), p2->x);

    uint8_t inside = TRUE;

    for (int32_t y = min_y; y < max_y; y++)
    {
        for (int32_t x = min_x; x < max_x; x++)
        {
            vec2_t p = { x, y };

            int32_t w0 = edge_cross(v0, v1, p);
            int32_t w1 = edge_cross(v1, v2, p);
            int32_t w2 = edge_cross(v2, v0, p);

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