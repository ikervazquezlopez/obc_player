#pragma once
#include <stdint.h>

struct Quad_data {
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
};

struct Texture_data {
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
};

struct Object_data {
	uint32_t id;
	Quad_data quad;
	Texture_data texture;
};