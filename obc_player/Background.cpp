#include <iostream>

#include "Background.h"

Background::Background()
{
}

Background::Background(int tw_pix, int th_pix, unsigned char* texture)
{

	Sprite::Sprite_Data data;
	data.id = -1;
	data.x = data.y = -1;
	data.tx = 0;
	data.ty = 0;
	data.w = 2; 
	data.h = 2;
	data.th = 1;
	data.tw = 1;

	float z = 0.1;
	// Vertex data => (x, y, z, tx, ty)
	float vertices[] = {
		data.x,	data.y,	0 + z,					data.tx, data.ty + data.th,		//0, 1, // bottom left
		data.x + data.w,data.y,	0 + z,			data.tx + data.tw, data.ty + data.th,	//1, 1, // bottom right
		data.x + data.w,data.y + data.h,0 + z,	data.tx + data.tw, data.ty,		//1, 0, // top right
		data.x, data.y + data.h,0 + z,			data.tx, data.ty			//0, 0  // top left
	};

	int vertex_size = 5;

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	// Create Vertex buffer object in GPU
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_size * 4, vertices, GL_DYNAMIC_DRAW); //  4 == number of vertices


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)(3 * sizeof(float)));
	
	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw_pix, th_pix, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	
}

Background::~Background()
{
}

GLuint Background::get_vao()
{
	return this->vao;
}

GLuint Background::get_texture()
{
	return this->texture;
}

void Background::draw()
{
	glBindVertexArray(this->vao);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glDrawArrays(GL_QUADS, 0, 4);
}
