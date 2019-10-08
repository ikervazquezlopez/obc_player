#pragma once

#include <opencv2/core.hpp>
#include <glew.h>
#include <freeglut.h>


class Sprite
{


public:
	Sprite();
	Sprite(int id, float x, float y, float w, float h, unsigned char* tex, int tw, int th);
	~Sprite();

	void set_position(float x, float y);
	void set_dimensions(float w, float h);
	void set_id(int i);
	void set_texture(unsigned char* tex);
	GLuint get_texture();
	GLuint get_vao();

private:
	GLuint texture;
	float x;
	float y;
	float w;
	float h;
	int id;
	GLuint vbo;
	GLuint vao;
};

