#include <glew.h>
#include <freeglut.h>
#include <opencv2/core.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	glm::mat4 t = glm::translate(glm::vec3(x - sprite->get_x(), y - sprite->get_y(), 0.0f));
	glm::mat4 s = glm::scale(glm::vec3(w / sprite->get_w(), h / sprite->get_h(), 1.0f));
	sprite->set_M_t(t);
	sprite->set_M_s(s);
	sprite->set_position(x, y);
	sprite->set_dimensions(w, h);
	sprite->set_current_matrix(t * s * sprite->get_current_matrix());
	//sprite.set_texture(tex);
}

void SpriteManager::draw_sprites()
{
	unsigned int M_t_location, M_s_location, current_matrix_location;
	glUseProgram(sprite_display_program);
	glActiveTexture(GL_TEXTURE0);
	unsigned int textureLocation = glGetUniformLocation(this->sprite_display_program, "in_texture");
	glUniform1i(textureLocation, 0);

	M_t_location = glGetUniformLocation(this->sprite_display_program, "M_t");
	M_s_location = glGetUniformLocation(this->sprite_display_program, "M_s");
	current_matrix_location = glGetUniformLocation(this->sprite_display_program, "current_matrix");

	// Draw background
	glBindVertexArray(this->background.get_vao());
	glBindTexture(GL_TEXTURE_2D, this->background.get_texture());
	glDrawArrays(GL_QUADS, 0, 4);
	
	for (std::pair<int, Sprite> p : this->sprites) {
		glBindVertexArray(p.second.get_vao());

		glm::mat4 mt = p.second.get_M_t();
		glm::mat4 ms = p.second.get_M_s();
		glm::mat4 m = p.second.get_current_matrix();
		glUniformMatrix4fv(M_t_location, 1, GL_FALSE, glm::value_ptr(mt));
		glUniformMatrix4fv(M_s_location, 1, GL_FALSE, glm::value_ptr(ms));
		glUniformMatrix4fv(current_matrix_location, 1, GL_FALSE, glm::value_ptr(m));


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
