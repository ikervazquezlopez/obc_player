#pragma once

#include <opencv2/core.hpp>
#include <glew.h>
#include <freeglut.h>
#include <glm/glm.hpp>


class Sprite
{


public:
	Sprite();
	Sprite(int id, float x, float y, float w, float h, GLubyte* tex, int tw, int th);
	~Sprite();

	void set_position(float x, float y);
	void set_dimensions(float w, float h);
	void set_id(int i);
	void set_texture(float tw, float th, GLubyte* tex);
	void set_current_matrix(glm::mat4 m);
	void set_M_t(glm::mat4 m);
	void set_M_s(glm::mat4 m);

	int get_id();
	float get_x();
	float get_y();
	float get_w();
	float get_h();
	GLuint get_texture();
	GLuint get_vao();
	glm::mat4 get_current_matrix();
	glm::mat4 get_M_t();
	glm::mat4 get_M_s();

private:
	GLuint texture;
	float x;
	float y;
	float w;
	float h;
	int id;
	GLuint vbo;
	GLuint vao;
	glm::mat4 current_matrix;
	glm::mat4 M_t;
	glm::mat4 M_s;

	void update_buffer();
};

