#include <app.h>
#include <render.h>
#include <math_b.h>
#include <math.h>
#include <stdio.h>

extern handler_t handler;

void render()
{
    clear(BLACK);
    
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

    float min_y = floor(MIN(MIN(p0->y, p1->y), p2->y));
    float min_x = floor(MIN(MIN(p0->x, p1->x), p2->x));
    float max_y = ceil(MAX(MAX(p0->y, p1->y), p2->y));
    float max_x = ceil(MAX(MAX(p0->x, p1->x), p2->x));

    float area = edge_cross(v0, v1, v2);

    float delta_w0_col = v0.y - v1.y;
    float delta_w1_col = v1.y - v2.y;
    float delta_w2_col = v2.y - v0.y;

    float delta_w0_row = v1.x - v0.x;
    float delta_w1_row = v2.x - v1.x;
    float delta_w2_row = v0.x - v2.x;

    float bias0 = is_top_left(v0, v1) ? 0 : -0.5f;
    float bias1 = is_top_left(v1, v2) ? 0 : -0.5f;
    float bias2 = is_top_left(v2, v0) ? 0 : -0.5f;

    vec2_t p = { min_x + 0.5f, min_y + 0.5f};

    float w0_row = edge_cross(v0, v1, p) + bias0;
    float w1_row = edge_cross(v1, v2, p) + bias1;
    float w2_row = edge_cross(v2, v0, p) + bias2;

    for (int32_t y = min_y; y < max_y; y++)
    {
        float w0 = w0_row;
        float w1 = w1_row;
        float w2 = w2_row;
        for (int32_t x = min_x; x < max_x; x++)
        {
            uint8_t inside = (w0 >= 0) && (w1 >= 0) && (w2 >= 0);

            if (inside)
            {
                color_t p_color = { 0 };
                float gamma = w0 / area;
                float beta  = w1 / area;
                float alpha = w2 / area;

                p_color.a = 255;
                p_color.r = p0->color.r * alpha + p1->color.r * beta + p2->color.r * gamma;
                p_color.g = p0->color.g * alpha + p1->color.g * beta + p2->color.g * gamma;
                p_color.b = p0->color.b * alpha + p1->color.b * beta + p2->color.b * gamma;

                put_pixel(x, y, p_color);
            }
            w0 += delta_w0_col;
            w1 += delta_w1_col;
            w2 += delta_w2_col;
        }
        w0_row += delta_w0_row;
        w1_row += delta_w1_row;
        w2_row += delta_w2_row;
    }
}