#include "Sprite.h"
#include "glm/glm.hpp"

#include <opencv2/core.hpp>
#include <vector>


Sprite::Sprite()
{
}

Sprite::Sprite(int id, float x, float y, float w, float h, unsigned char* tex, int tw, int th)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	float vertices[] = {
		x,	y + h,		0,//		0, 1, // top left
		x,	y,			0,//		0, 0, // bottom left
		x + w,	y,		0,//		1, 0, // bottom right
		x + w, y + h,	0,//		1, 1  // top right
	};

	int vertex_size = 3;

	/*
	// Create the quad vertices
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3( x, y + h, 0 ));
	vertices.push_back(glm::vec3(x, y, 0 ));
	vertices.push_back(glm::vec3(x + w, y, 0 ));
	vertices.push_back(glm::vec3(x + w, y + h, 0 ));
	*/

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	// Create Vertex buffer object in GPU
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_size * 4, &vertices[0], GL_STATIC_DRAW); //  4 == number of vertices

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size*sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_size, (void*)(3 * sizeof(float)));

	// Create texture
	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
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
}

void Sprite::set_dimensions(float w, float h)
{
	this->w = w;
	this->h = h;
}

GLuint Sprite::get_texture()
{
	return this->texture;
}

GLuint Sprite::get_vao()
{
	return this->vao;
}

