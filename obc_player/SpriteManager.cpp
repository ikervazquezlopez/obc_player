#include <glew.h>
#include <freeglut.h>
#include <opencv2/core.hpp>

#include "SpriteManager.h"
#include "ShaderLoader.h"

SpriteManager* SpriteManager::s_instance = NULL;

SpriteManager *SpriteManager::get_instnce()
{
	if (!s_instance) {
		s_instance = new SpriteManager;	
	}
	return s_instance;;
}

Sprite SpriteManager::create_sprite(int id, float x, float y, float w, float h, GLubyte* tex, int tw, int th)
{
	Sprite sprite = Sprite(id, x, y, w, h, tex, tw, th);
	if (id >= 0) 
	{
		std::pair<int, Sprite> el(id, sprite);
		sprites.insert(el);
	}

	return sprite;
}

Sprite* SpriteManager::get_sprite(int id)
{
	return &sprites.at(id);
}

void SpriteManager::update_sprite(int id, float x, float y, float w, float h, GLubyte* tex)
{
	Sprite* sprite = get_sprite(id);
	(*sprite).set_position(x, y);
	(*sprite).set_dimensions(w, h);
	//sprite.set_texture(tex);
}

void SpriteManager::draw_sprites()
{
	glUseProgram(sprite_display_program);
	glActiveTexture(GL_TEXTURE0);
	unsigned int textureLocation = glGetUniformLocation(this->sprite_display_program, "in_texture");
	glUniform1i(textureLocation, 0);

	// Draw background
	glBindVertexArray(this->background.get_vao());
	glBindTexture(GL_TEXTURE_2D, this->background.get_texture());
	glDrawArrays(GL_QUADS, 0, 4);
	
	for (std::pair<int, Sprite> p : this->sprites) {
		glBindVertexArray(p.second.get_vao());
		glBindTexture(GL_TEXTURE_2D, p.second.get_texture());
		glDrawArrays(GL_QUADS, 0, 4);
	}
	
}

Sprite SpriteManager::create_sprite_background(float w, float h, GLubyte* tex)
{
	this->background = create_sprite(-1, -1.0, -1.0, 2.0, 2.0, tex, w, h);
	return this->background;
}

SpriteManager::SpriteManager()
{
	ShaderLoader shader_loader;
	std::string vertexShader_filename = "SpriteShader.glsl";
	std::string fragmentShader_filename = "FragmentShader.glsl";
	this->sprite_display_program = shader_loader.CreateProgram(vertexShader_filename.c_str(),
																fragmentShader_filename.c_str());
}

SpriteManager::~SpriteManager()
{

}
