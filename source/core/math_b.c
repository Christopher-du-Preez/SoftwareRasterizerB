#include <math_b.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

float edge_cross(vec3_t a, vec3_t b, vec3_t c)
{
    vec2_t ab = { b.x - a.x, b.y - a.y };
    vec2_t ac = { c.x - a.x, c.y - a.y };
    return ab.x * ac.y - ab.y * ac.x;
}

vec2_t vec2_rotate(vec2_t v, vec2_t center, float angle)
{
    vec2_t rot;
    v.x -= center.x;
    v.y -= center.y;
    rot.x = v.x * cos(angle) - v.y * sin(angle);
    rot.y = v.x * sin(angle) + v.y * cos(angle);
    rot.x += center.x;
    rot.y += center.y;
    return rot;
}