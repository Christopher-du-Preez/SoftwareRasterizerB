#include <render.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t* frame_buffer = NULL;
static uint32_t width = WIDTH;
static uint32_t height = HEIGHT;
cube_t cube1;
cube_t cube2;
color_t color[3];

void make_cube(cube_t *cube)
{
    cube->vertices[0] = (vec3_t){ 1.0f,  1.0f,  1.0f };
    cube->vertices[1] = (vec3_t){ -1.0f,  1.0f,  1.0f };
    cube->vertices[2] = (vec3_t){ -1.0f, -1.0f,  1.0f };
    cube->vertices[3] = (vec3_t){ 1.0f, -1.0f,  1.0f };

    cube->vertices[4] = (vec3_t){ 1.0f,  1.0f, -1.0f };
    cube->vertices[5] = (vec3_t){ -1.0f,  1.0f, -1.0f };
    cube->vertices[6] = (vec3_t){ -1.0f, -1.0f, -1.0f };
    cube->vertices[7] = (vec3_t){ 1.0f, -1.0f, -1.0f };

    cube->faces[0]  = 0; cube->faces[1]  = 1; cube->faces[2]  = 2;
    cube->faces[3]  = 0; cube->faces[4]  = 2; cube->faces[5]  = 3;
    cube->faces[6]  = 4; cube->faces[7]  = 0; cube->faces[8]  = 3;
    cube->faces[9]  = 4; cube->faces[10] = 3; cube->faces[11] = 7;
    cube->faces[12] = 5; cube->faces[13] = 4; cube->faces[14] = 7;
    cube->faces[15] = 5; cube->faces[16] = 7; cube->faces[17] = 6;
    cube->faces[18] = 1; cube->faces[19] = 5; cube->faces[20] = 6;
    cube->faces[21] = 1; cube->faces[22] = 6; cube->faces[23] = 2;
    cube->faces[24] = 4; cube->faces[25] = 5; cube->faces[26] = 1;
    cube->faces[27] = 4; cube->faces[28] = 1; cube->faces[29] = 0;
    cube->faces[30] = 2; cube->faces[31] = 6; cube->faces[32] = 7;
    cube->faces[33] = 2; cube->faces[34] = 7; cube->faces[35] = 3;
}

void render_init()
{
    

    cube1.transform = (vec3_t){ 1.5f, 0.0f, 5.0f };
    make_cube(&cube1);

    cube2.transform = (vec3_t){ -2.5f, 0.0f, 10.0f };
    make_cube(&cube2);

    color[0] = RED;
    color[1] = GREEN;
    color[2] = BLUE;
}

void wire(vec3_t p0, vec3_t p1, vec3_t p2, color_t color)
{
    draw_line(p0, p1, color);
    draw_line(p1, p2, color);
    draw_line(p2, p0, color);
}

void render_cube(cube_t *model)
{
    vec3_t vertices[8];
    memcpy(vertices, model->vertices, 8*sizeof(vec3_t));
    vec3_t projected[8];
    for (size_t i = 0; i < 8; i++)
    {
        vertices[i].x += model->transform.x;
        vertices[i].y += model->transform.y;
        vertices[i].z += model->transform.z;

        projected[i] = project(vertices[i]);
    }
    for (size_t i = 0; i < 34; i+=3)
    {
        draw_triangle(projected[model->faces[i]], projected[model->faces[i+1]], projected[model->faces[i+2]], color);//wire(projected[model->faces[i]], projected[model->faces[i+1]], projected[model->faces[i+2]], RED);
    }
}

void render()
{
    clear(BLACK);
    render_cube(&cube1);
    render_cube(&cube2);
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

vec3_t project(vec3_t v)
{
    vec3_t new_pos;
    float d = 1;
    if (v.z <= 0)
        v.z = d;
    new_pos.x = (v.x * d / v.z) * width;
    new_pos.y = (v.y * d / v.z) * height;
    new_pos.z = d;

    return new_pos;
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
    if(new_x >= width || new_y >= height)
        return;
    frame_buffer[(width * new_y) + new_x] = color_to_int(color);
}

void draw_line(vec3_t p0, vec3_t p1, color_t color)
{
    int32_t dx = round(abs(p1.x - p0.x)),
        dy = -round(abs(p1.y - p0.y)),
        sx = p0.x < p1.x ? 1 : -1,
        sy = p0.y < p1.y ? 1 : -1,
        x = round(p0.x),
        y = round(p0.y),
        end_x = round(p1.x),
        end_y = round(p1.y),
        error = dx + dy;
    
    while(1){
        put_pixel(x, y, color);
        if(error*2 >= dy){
            if(x == end_x)
                break;
            error += dy;
            x += sx;
        }
        if(error*2 <= dx){
            if(y == end_y)
                break;
            error += dx;
            y += sy;
        }
    }
}

uint8_t is_top_left(vec3_t a, vec3_t b)
{
    vec2_t edge = { b.x - a.x, b.y - a.y };
    return (edge.y == 0 && edge.x > 0) || edge.y < 0;
}

void draw_triangle(vec3_t v0, vec3_t v1, vec3_t v2, color_t color[3])
{
    float min_y = floor(MIN(MIN(v0.y, v1.y), v2.y));
    float min_x = floor(MIN(MIN(v0.x, v1.x), v2.x));
    float max_y =  ceil(MAX(MAX(v0.y, v1.y), v2.y));
    float max_x =  ceil(MAX(MAX(v0.x, v1.x), v2.x));

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

    vec3_t p = { min_x + 0.5f, min_y + 0.5f, 0};

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
            uint8_t inside = (w0 <= 0) && (w1 <= 0) && (w2 <= 0);

            if (inside)
            {
                color_t p_color = { 0 };
                float gamma = w0 / area;
                float beta  = w1 / area;
                float alpha = w2 / area;

                p_color.val =              color[0].val * gamma + color[1].val * beta + color[2].val * alpha;
                p_color.r = p_color.val * (color[0].r   * gamma + color[1].r   * beta + color[2].r   * alpha);
                p_color.g = p_color.val * (color[0].g   * gamma + color[1].g   * beta + color[2].g   * alpha);
                p_color.b = p_color.val * (color[0].b   * gamma + color[1].b   * beta + color[2].b   * alpha);

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