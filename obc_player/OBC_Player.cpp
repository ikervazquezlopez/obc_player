#include <iostream>
#include "SpriteManager.h"
#include "Sprite.h"
#include "Coder.h"
#include "Decoder.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "OBC_Player.h"



void _render_scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	if (background != nullptr) {
		draw_background();
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_atlas);
	for (std::pair<int, Sprite> p : SpriteManager::get_instance()->get_sprites()) {
		draw_sprite(p.second);
	}

	glutSwapBuffers();
}

void _idle(void)
{
	Sprite* s = SpriteManager::get_instance()->get_sprite(0);
	Sprite::Sprite_Data data = s->get_data();
	data = { s->get_id(), s->get_x(), s->get_y(), s->get_w(), s->get_h(),
						s->get_tx(), s->get_ty(), s->get_tw(), s->get_th() };
	SpriteManager::get_instance()->update_sprite(data);
	glutPostRedisplay();
}

void _keyboard(unsigned char c, int x, int y)
{
}



void initialize(int width, int heigth, int argc, char** argv)
{
	play_flag = false;
	//manager = SpriteManager::get_instance();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, heigth);
	glutCreateWindow("OBC player");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glewInit();

	_init();

	// Create program
	ShaderLoader shader_loader;
	std::string vertexShader_filename = "SpriteShader.glsl";
	std::string fragmentShader_filename = "FragmentShader.glsl";
	shader_program = shader_loader.CreateProgram(vertexShader_filename.c_str(),
		fragmentShader_filename.c_str());
	glUseProgram(shader_program);

	// Get uniform locations in the shader
	M_t_location = glGetUniformLocation(shader_program, "M_t");
	M_s_location = glGetUniformLocation(shader_program, "M_s");
	current_matrix_location = glGetUniformLocation(shader_program, "current_matrix");
	texture_location = glGetUniformLocation(shader_program, "in_texture");


	// Create texture atlas
	glGenTextures(1, &texture_atlas);
	glBindTexture(GL_TEXTURE_2D, texture_atlas);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
}

// WARNING!!! CHANGE tw and th DEFAULT VALUES FOR 4K VALUES!!!
void update_texture_atlas(unsigned char* atlas, int tw = 660, int th = 373)
{
	glBindTexture(GL_TEXTURE_2D, texture_atlas);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, GL_RGB, GL_UNSIGNED_BYTE, atlas);
}

void play()
{
	play_flag = true;
}

void stop()
{
	play_flag = false;
}

Background * get_background()
{
	return background;
}

void set_background(Background *b)
{
	background = b;
}

void draw_background()
{
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(background->get_vao());

	glm::mat4 m = glm::mat4(1.0f);
	glUniformMatrix4fv(M_t_location, 1, GL_FALSE, glm::value_ptr(m));
	glUniformMatrix4fv(M_s_location, 1, GL_FALSE, glm::value_ptr(m));
	glUniformMatrix4fv(current_matrix_location, 1, GL_FALSE, glm::value_ptr(m));

	glBindTexture(GL_TEXTURE_2D, background->get_texture());
	glDrawArrays(GL_QUADS, 0, 4);
}

void draw_sprite(Sprite s)
{
	glBindVertexArray(s.get_vao());

	glUniformMatrix4fv(M_t_location, 1, GL_FALSE, glm::value_ptr(s.get_M_t()));
	glUniformMatrix4fv(M_s_location, 1, GL_FALSE, glm::value_ptr(s.get_M_s()));
	glUniformMatrix4fv(current_matrix_location, 1, GL_FALSE, glm::value_ptr(s.get_current_matrix()));

	glDrawArrays(GL_QUADS, 0, 4);
}

void _init()
{
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void code_video() {
	coder->open_file("test_video.16kc");
	coder->write_header(3, 30, 400, 400);
	GLubyte* background_img = SOIL_load_image("C:\\Users\\ikervazquezlopez\\Pictures\\Saved Pictures\\background.jpg", &tw, &th, 0, SOIL_LOAD_RGB);
	
	coder->write_background(background_img, tw * th * 3);
	coder->close_file();
}



int main(int argc, char** argv) {

	coder = new Coder();
	//code_video();
	decoder = new Decoder();
	decoder->open_file("test_video.16kc");
	decoder->read_header();
	GLubyte* background_img = decoder->read_background();
	
	background = nullptr;

	GLubyte* tex = SOIL_load_image("C:\\Users\\ikervazquezlopez\\Pictures\\Saved Pictures\\NationalGeographic.jpg", &tw, &th, 0, SOIL_LOAD_RGB);
	GLubyte* atlas = SOIL_load_image("C:\\Users\\ikervazquezlopez\\Documents\\Python Scripts\\2DbinPacking\\test_texture.jpg", &tw, &th, 0, SOIL_LOAD_RGB);


	initialize(660, 373, argc, argv);
	update_texture_atlas(atlas, 256, 256);

	background = new Background(660, 373, background_img);
	
	SpriteManager* manager = SpriteManager::get_instance();

	Sprite::Sprite_Data data;
	data = { 0, 0, 0, 0.5, 0.5, 0, 0, 1, 1 };
	Sprite s = manager->create_sprite(data);

	data = { 1, -1, -1, 0.5, 0.5, 0.25, 0.25, 0.75, 0.75 };
	s = manager->create_sprite(data);

	glutDisplayFunc(_render_scene);
	glutIdleFunc(_idle);
	glutKeyboardFunc(_keyboard);
	glutMainLoop();

	return 0;

}