#include <render.h>
#include <math.h>
#include <stdio.h>

uint32_t* frame_buffer = NULL;
static uint32_t width = WIDTH;
static uint32_t height = HEIGHT;
vec2_t points[3];
color_t colors[3];

void render_init()
{
    points[0] = (vec2_t){0, 0 };
    points[1] = (vec2_t){100, -100 };
    points[2] = (vec2_t){ 100, 100 };

    colors[0] = RED;
    colors[0].val = 0.1f;
    colors[1] = GREEN;
    colors[1].val = 0.5f;
    colors[2] = BLUE;
}

void render()
{
    clear(BLACK);
    draw_triangle(points, colors);
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
    size_t length = width * height;
    for (size_t i = 0; i < length; i++)
    {
        frame_buffer[i] = color_to_int(color);
    }
}

void put_pixel(int32_t x, int32_t y, color_t color)
{
    uint32_t new_x = width/2 + x;
    uint32_t new_y = height/2 - y;
    if(new_x > width || new_y > height)
        return;
    frame_buffer[(WIDTH * new_y) + new_x] = color_to_int(color);
}

void draw_line(vec2_t *p0, vec2_t *p1, color_t color)
{
    int32_t dx = abs(p1->x - p0->x),
        dy = -abs(p1->y - p0->y),
        sx = p0->x < p1->x ? 1 : -1,
        sy = p0->y < p1->y ? 1 : -1,
        x = p0->x,
        y = p0->y,
        error = dx + dy;
    
    while(1){
        put_pixel(x, y, color);
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

void draw_triangle(vec2_t point[3], color_t color[3])
{
    float min_y = floor(MIN(MIN(point[0].y, point[1].y), point[2].y));
    float min_x = floor(MIN(MIN(point[0].x, point[1].x), point[2].x));
    float max_y =  ceil(MAX(MAX(point[0].y, point[1].y), point[2].y));
    float max_x =  ceil(MAX(MAX(point[0].x, point[1].x), point[2].x));

    float area = edge_cross(point[0], point[1], point[2]);

    float delta_w0_col = point[0].y - point[1].y;
    float delta_w1_col = point[1].y - point[2].y;
    float delta_w2_col = point[2].y - point[0].y;

    float delta_w0_row = point[1].x - point[0].x;
    float delta_w1_row = point[2].x - point[1].x;
    float delta_w2_row = point[0].x - point[2].x;

    float bias0 = is_top_left(point[0], point[1]) ? 0 : -0.5f;
    float bias1 = is_top_left(point[1], point[2]) ? 0 : -0.5f;
    float bias2 = is_top_left(point[2], point[0]) ? 0 : -0.5f;

    vec2_t p = { min_x + 0.5f, min_y + 0.5f};

    float w0_row = edge_cross(point[0], point[1], p) + bias0;
    float w1_row = edge_cross(point[1], point[2], p) + bias1;
    float w2_row = edge_cross(point[2], point[0], p) + bias2;

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

                p_color.val =              color[0].val * beta + color[1].val * alpha + color[2].val * gamma;
                p_color.r = p_color.val * (color[0].r   * beta + color[1].r   * alpha + color[2].r   * gamma);
                p_color.g = p_color.val * (color[0].g   * beta + color[1].g   * alpha + color[2].g   * gamma);
                p_color.b = p_color.val * (color[0].b   * beta + color[1].b   * alpha + color[2].b   * gamma);

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