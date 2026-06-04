#pragma once
#include <stdint.h>

typedef struct vector2
{
	int32_t x, y;
} vec2_t;

typedef struct vector3
{
	int32_t x, y, z;
} vec3_t;

typedef struct vector4
{
	int32_t x, y, z, w;
} vec4_t;

int32_t edge_cross(vec2_t a, vec2_t b, vec2_t c);