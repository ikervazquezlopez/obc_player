#pragma once
#include <unordered_map>
#include <opencv2/core.hpp>
#include <glew.h>
#include <freeglut.h>

#include "Sprite.h"

class SpriteManager
{
public:
	static SpriteManager* get_instnce();
	Sprite create_sprite(int id, float x, float y, float w, float h, GLubyte* tex, int tw, int th);
	Sprite* get_sprite(int id);
	void update_sprite(int id, float x, float y, float w, float h, float tw, float th, GLubyte* tex);
	void draw_sprites();
	Sprite create_sprite_background(float w, float h, GLubyte* tex);

private:
	SpriteManager();
	~SpriteManager();
	std::unordered_map<int, Sprite> sprites;
	static SpriteManager* s_instance;
	GLuint sprite_display_program;
	Sprite background;
};

