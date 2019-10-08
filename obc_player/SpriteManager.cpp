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

int SpriteManager::create_sprite(int id, float x, float y, float w, float h/*, unsigned char* tex, int tw, int th*/)
{
	Sprite sprite = Sprite(id, x, y, w, h/*, tex, tw, th*/);
	std::pair<int, Sprite> el(id, sprite);
	sprites.insert(el);

	return id;
}

Sprite SpriteManager::get_sprite(int id)
{
	return sprites.at(id);
}

void SpriteManager::update_sprite(int id, float x, float y, float w, float h/*, unsigned char* tex*/)
{
	Sprite sprite = get_sprite(id);
	sprite.set_position(x, y);
	sprite.set_dimensions(w, h);
	//sprite.set_texture(tex);
}

void SpriteManager::draw_sprites()
{
	glUseProgram(sprite_display_program);
	for (std::pair<int, Sprite> p : this->sprites) {
		glBindVertexArray(p.second.get_vao());
		
		//glBindTexture(GL_TEXTURE_2D, p.second.get_texture());
		glDrawArrays(GL_QUADS, 0, 4);
	}
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
