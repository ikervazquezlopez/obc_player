#pragma once
#include <unordered_map>
#include <opencv2/core.hpp>
#include <glew.h>
#include <freeglut.h>

#include "Sprite.h"

class SpriteManager
{
public:
	static SpriteManager* get_instance();
	Sprite create_sprite(Sprite::Sprite_Data data);
	Sprite* get_sprite(int id);
	std::unordered_map<int, Sprite> get_sprites();
	void update_sprite(Sprite::Sprite_Data data);
	void draw_sprites(GLuint program);

private:
	SpriteManager();
	~SpriteManager();
	std::unordered_map<int, Sprite> sprites;
	bool contains_sprite(int id);
	static SpriteManager* s_instance;
	Sprite background;
};

