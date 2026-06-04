#include <math_b.h>

int32_t edge_cross(vec2_t a, vec2_t b, vec2_t c)
{
    vec2_t ab = { b.x - a.x, b.y - a.y };
    vec2_t ac = { c.x - a.x, c.y - a.y };
    return ab.x * ac.y - ac.x * ab.y;
}