#include <glew.h>
#include <freeglut.h>
#include <opencv2/core.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SpriteManager.h"
#include "ShaderLoader.h"

SpriteManager* SpriteManager::s_instance = NULL;

SpriteManager *SpriteManager::get_instance()
{
	if (!s_instance) {
		s_instance = new SpriteManager;	
	}
	return s_instance;;
}

Sprite SpriteManager::create_sprite(Sprite::Sprite_Data data)
{
	Sprite sprite = Sprite(data);
	if (data.id >= 0) 
	{
		std::pair<int, Sprite> el(data.id, sprite);
		sprites.insert(el);
	}

	return sprite;
}

Sprite* SpriteManager::get_sprite(int id)
{
	return &sprites.at(id);
}

std::unordered_map<int, Sprite> SpriteManager::get_sprites()
{
	return this->sprites;
}

void SpriteManager::update_sprite(Sprite::Sprite_Data data)
{
	if (!contains_sprite(data.id)) {
		Sprite s = create_sprite(data);
	}
	Sprite* sprite = get_sprite(data.id);

	glm::mat4 t = glm::translate(glm::vec3(data.x - sprite->get_x(), data.y - sprite->get_y(), 0.0f));
	glm::mat4 s = glm::scale(glm::vec3(data.w / sprite->get_w(), data.h / sprite->get_h(), 1.0f));
	sprite->set_M_t(t);
	sprite->set_M_s(s);
	sprite->set_position(data.x, data.y);
	sprite->set_dimensions(data.w, data.h);
	sprite->set_current_matrix(t * s * sprite->get_current_matrix());
	sprite->set_texture(data.tx, data.ty, data.tw, data.th);
}

void SpriteManager::draw_sprites(GLuint program)
{
	unsigned int M_t_location, M_s_location, current_matrix_location;
	
	glActiveTexture(GL_TEXTURE0);
	unsigned int textureLocation = glGetUniformLocation(program, "in_texture");
	glUniform1i(textureLocation, 0);

	M_t_location = glGetUniformLocation(program, "M_t");
	M_s_location = glGetUniformLocation(program, "M_s");
	current_matrix_location = glGetUniformLocation(program, "current_matrix");
	
	for (std::pair<int, Sprite> p : this->sprites) {
		glBindVertexArray(p.second.get_vao());

		glm::mat4 mt = p.second.get_M_t();
		glm::mat4 ms = p.second.get_M_s();
		glm::mat4 m = p.second.get_current_matrix();
		glUniformMatrix4fv(M_t_location, 1, GL_FALSE, glm::value_ptr(mt));
		glUniformMatrix4fv(M_s_location, 1, GL_FALSE, glm::value_ptr(ms));
		glUniformMatrix4fv(current_matrix_location, 1, GL_FALSE, glm::value_ptr(m));

		glDrawArrays(GL_QUADS, 0, 4);

		
	}
	
}

SpriteManager::SpriteManager()
{
	
}

SpriteManager::~SpriteManager()
{

}

bool SpriteManager::contains_sprite(int id)
{
	return this->sprites.count(id) > 0;
}
