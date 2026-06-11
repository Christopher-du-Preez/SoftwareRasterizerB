#pragma once
#include <stdint.h>

enum types
{
	VECTOR2 = 8,
	VECTOR3 = 12,
	VECTOR4 = 16,
	INDEX = 8
};

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

typedef struct array
{
	void * data;
	uint32_t size;
} arr_t;

float edge_cross(vec3_t a, vec3_t b, vec3_t c);
vec2_t vec2_rotate(vec2_t v, vec2_t center, float angle);