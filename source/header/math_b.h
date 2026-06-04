#pragma once
#include <stdint.h>

typedef struct vector2
{
	float x, y;
} vec2_t;

typedef struct vector3
{
	float x, y, z;
} vec3_t;

typedef struct vector4
{
	float x, y, z, w;
} vec4_t;

float edge_cross(vec2_t a, vec2_t b, vec2_t c);
vec2_t vec2_rotate(vec2_t v, vec2_t center, float angle);