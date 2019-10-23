#include "Coder.h"

Object_data Coder::fill_object_data(uint32_t id, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t tx, uint32_t ty, uint32_t tw, uint32_t th)
{
	Quad_data quad;
	quad.x = x;
	quad.y = y;
	quad.w = w;
	quad.h = h;

	Texture_data texture;
	texture.x = tx;
	texture.y = ty;
	texture.w = tw;
	texture.h = th;

	Object_data obj;
	obj.id = id;
	obj.quad = quad;
	obj.texture = texture;

	return obj;
}
