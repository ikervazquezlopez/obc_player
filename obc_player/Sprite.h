#pragma once

#include <opencv2/core.hpp>
#include <glew.h>
#include <freeglut.h>
#include <glm/glm.hpp>


class Sprite
{

	

public:
	struct Sprite_Data {
		int id;

		// Quad data
		float x;
		float y;
		float w;
		float h;

		// Texture data
		float tx;
		float ty;
		float tw;
		float th;
	};

	Sprite();
	Sprite(Sprite::Sprite_Data data);
	~Sprite();

	void set_position(float x, float y);
	void set_dimensions(float w, float h);
	void set_id(int i);
	void set_texture(float tx, float ty, float tw, float th);
	void set_current_matrix(glm::mat4 m);
	void set_M_t(glm::mat4 m);
	void set_M_s(glm::mat4 m);

	int get_id();
	float get_x();
	float get_y();
	float get_w();
	float get_h();
	float get_tx();
	float get_ty();
	float get_tw();
	float get_th();
	Sprite_Data get_data();

	GLuint get_vao();
	glm::mat4 get_current_matrix();
	glm::mat4 get_M_t();
	glm::mat4 get_M_s();

private:
	float x;
	float y;
	float w;
	float h;
	float tx;	// Texture x coordinate [0,1]
	float ty;	// Texture y coordinate [0,1]
	float tw;	// Texture width [0,1]
	float th;	// Texture heigth [0,1]
	int id;
	GLuint vbo;
	GLuint vao;
	glm::mat4 current_matrix;
	glm::mat4 M_t;
	glm::mat4 M_s;

	void update_buffer();
};

