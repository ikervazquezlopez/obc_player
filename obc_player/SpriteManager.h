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
	int create_sprite(int id, float x, float y, float w, float h, unsigned char* tex, int tw, int th);
	Sprite get_sprite(int id);
	void update_sprite(int id, float x, float y, float w, float h, unsigned char* tex);
	void draw_sprites();

private:
	SpriteManager();
	~SpriteManager();
	std::unordered_map<int, Sprite> sprites;
	static SpriteManager* s_instance;
	GLuint sprite_display_program;
};

