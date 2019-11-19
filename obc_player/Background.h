#pragma once
#include <glew.h>
#include <freeglut.h>
#include <opencv2/core.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sprite.h"

class Background
{


public:
	Background();
	Background(int tw_pix, int th_pix, unsigned char* texture);
	~Background();

	GLuint get_vao();
	GLuint get_texture();
	void draw();

private:

	const Sprite::Sprite_Data s_data = { -1, 
		-1, -1,
		2, 2,
		0, 0,
		1, 1};

	GLuint texture;
	Sprite s;

	GLuint vao;
	GLuint vbo;
};

