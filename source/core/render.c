#include <app.h>
#include <render.h>
#include <math_b.h>

extern handler_t handler;

color_t red = { .r = 255, .g = 0,   .b = 0,   .a = 255 };
color_t green = { .r = 0,   .g = 255, .b = 0,   .a = 255 };
color_t blue = { .r = 0,   .g = 0,   .b = 255, .a = 255 };
color_t white = { .r = 255, .g = 255, .b = 255, .a = 255 };
color_t black = { .r = 0,   .g = 0,   .b = 0,   .a = 255 };

void render()
{
    pixel_t p0 = { .x = 0, .y = 0, .color = red };
    pixel_t p1 = { .x = 100, .y = 100, .color = green };
    pixel_t p2 = { .x = 100, .y = -100, .color = blue };
    clear(black);
    draw_triangle(&p0, &p2, &p1);
}

/*-----------Utils------------*/

uint32_t color_to_int(color_t color)
{
    uint32_t new_color = 0;
    new_color = new_color | color.a;
    new_color = (new_color << 8) | color.r;
    new_color = (new_color << 8) | color.g;
    new_color = (new_color << 8) | color.b;
    return new_color;
}

/*-----------2D--------------*/

void clear(color_t color)
{
    size_t length = WIDTH * HEIGHT;
    for (size_t i = 0; i < length; i++)
    {
        handler.frame_buffer[i] = color_to_int(color);
    }
}

void put_pixel(int32_t x, int32_t y, color_t color)
{
    uint32_t new_x = WIDTH/2 + x;
    uint32_t new_y = HEIGHT/2 - y;
    if(new_x > WIDTH || new_y > HEIGHT)
        return;
    handler.frame_buffer[(WIDTH * new_y) + new_x] = color_to_int(color);
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

uint8_t is_top_left(vec2_t a, vec2_t b)
{
    vec2_t edge = { b.x - a.x, b.y - a.y };
    return (edge.y == 0 && edge.x > 0) || edge.y < 0;
}

void draw_triangle(pixel_t *p0, pixel_t *p1, pixel_t *p2)
{
    vec2_t v0 = { p0->x, p0->y };
    vec2_t v1 = { p1->x, p1->y };
    vec2_t v2 = { p2->x, p2->y };

    int32_t area = edge_cross(v0, v1, v2);

    int32_t min_y = MIN(MIN(p0->y, p1->y), p2->y);
    int32_t min_x = MIN(MIN(p0->x, p1->x), p2->x);
    int32_t max_y = MAX(MAX(p0->y, p1->y), p2->y);
    int32_t max_x = MAX(MAX(p0->x, p1->x), p2->x);

    int8_t bias0 = is_top_left(v0, v1) ? 0 : -1;
    int8_t bias1 = is_top_left(v1, v2) ? 0 : -1;
    int8_t bias2 = is_top_left(v2, v0) ? 0 : -1;

    uint8_t inside = TRUE;

    for (int32_t y = min_y; y < max_y; y++)
    {
        for (int32_t x = min_x; x < max_x; x++)
        {
            vec2_t p = { x, y };

            int32_t w0 = edge_cross(v0, v1, p) + bias0;
            int32_t w1 = edge_cross(v1, v2, p) + bias1;
            int32_t w2 = edge_cross(v2, v0, p) + bias2;

            inside = (w0 >= 0) && (w1 >= 0) && (w2 >= 0);

            if (inside)
            {
                color_t p_color = red;
                float gamma = w0 / (float)area;
                float beta = w1 / (float)area;
                float alpha = w2 / (float)area;

                p_color.r = p0->color.r * gamma + p1->color.r * beta + p2->color.r * alpha;
                p_color.g = p0->color.g * gamma + p1->color.g * beta + p2->color.g * alpha;
                p_color.b = p0->color.b * gamma + p1->color.b * beta + p2->color.b * alpha;

                put_pixel(x, y, p_color);
            }
        }
    }
}