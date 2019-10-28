#include "Sprite.h"
#include "glm/glm.hpp"

#include <opencv2/core.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


Sprite::Sprite()
{
}

Sprite::Sprite(int id, float x, float y, float w, float h, GLubyte* tex, int tw, int th)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->M_t = glm::mat4(1.0f);
	this->M_s = glm::mat4(1.0f);
	this->current_matrix = glm::mat4(1.0f);

	float z = 0;
	if (id == -1)
		z = 0.1;
	
	float vertices[] = {
		x,	y,		0+z,			0, 1, // bottom left
		x + w,	y,	0+z,			1, 1, // bottom right
		x + w,	y + h,	0+z,		1, 0, // top right
		x, y + h,	0+z,			0, 0  // top left
	};
	
	int vertex_size = 5;

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	// Create Vertex buffer object in GPU
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_size * 4, vertices, GL_DYNAMIC_DRAW); //  4 == number of vertices


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size*sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_size*sizeof(float), (void*)(3 * sizeof(float)));

	// Create texture
	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
	
}

Sprite::~Sprite()
{
	glDeleteBuffers(1, &this->vbo);
}

void Sprite::set_position(float x, float y)
{
	this->x = x;
	this->y = y;
	//update_buffer();
}

void Sprite::set_dimensions(float w, float h)
{
	this->w = w;
	this->h = h;
	//update_buffer();
}

void Sprite::set_current_matrix(glm::mat4 m)
{
	this->current_matrix = m;
}

void Sprite::set_M_t(glm::mat4 m)
{
	this->M_t = m;
}

void Sprite::set_M_s(glm::mat4 m)
{
	this->M_s = m;
}

void Sprite::update_buffer() {
	float p0[] = { this->x, this->y };
	float p1[] = { this->x+this->w, this->y };
	float p2[] = { this->x+this->w, this->y+this->h };
	float p3[] = { this->x, this->y + this->h };

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(float), p0);
	glBufferSubData(GL_ARRAY_BUFFER, 1 * 5 * sizeof(float), 2 * sizeof(float), p1);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * 5 * sizeof(float), 2 * sizeof(float), p2);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(float), 2 * sizeof(float), p3);

}

int Sprite::get_id()
{
	return this->id;
}

float Sprite::get_x()
{
	return this->x;
}

float Sprite::get_y()
{
	return this->y;
}

float Sprite::get_w()
{
	return this->w;
}

float Sprite::get_h()
{
	return this->h;
}

GLuint Sprite::get_texture()
{
	return this->texture;
}

GLuint Sprite::get_vao()
{
	return this->vao;
}

glm::mat4 Sprite::get_current_matrix()
{
	return this->current_matrix;
}

glm::mat4 Sprite::get_M_t()
{
	return this->M_t;
}

glm::mat4 Sprite::get_M_s()
{
	return this->M_s;
}

