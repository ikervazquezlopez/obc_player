#include <iostream>
#include <fstream>
#include "SpriteManager.h"
#include "Sprite.h"
#include "Coder.h"
#include "Decoder.h"
#include "CodecUtils.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <thread>
#include <chrono>

#include "OBC_Player.h"

int pass = 0;

void _render_scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	if (!decoder->is_finised()) {
		if (background != nullptr) {
			draw_background();
		}
	
		std::pair<std::vector<Object_data>, unsigned char*> p = decoder->read_frame();
		GLubyte* texture_atlas_data = p.second;
		update_texture_atlas(texture_atlas_data, TEXTURE_ATLAS_WIDTH, TEXTURE_ATLAS_HEIGHT);
		
		std::vector<Object_data> objs = p.first;
		std::cout << "=========" << std::endl;
		for (Object_data o : objs) {
			Sprite::Sprite_Data sdata = { o.id, (float) o.quad.x / decoder->get_width(), (float) o.quad.y / decoder->get_height(), 
												(float) o.quad.w / decoder->get_width(), (float) o.quad.h / decoder->get_height(),
												(float) o.texture.x / TEXTURE_ATLAS_WIDTH, (float) o.texture.y / TEXTURE_ATLAS_HEIGHT, 
												(float) o.texture.w / TEXTURE_ATLAS_WIDTH, (float) o.texture.h / TEXTURE_ATLAS_HEIGHT };
			SpriteManager::get_instance()->update_sprite(sdata);
			Sprite::print_sprite_data(sdata);
		}
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_atlas);
		for (std::pair<int, Sprite> p : SpriteManager::get_instance()->get_sprites()) {
			draw_sprite(p.second);
		}

	}
	else {
		std::cout << "I am finished" << std::endl;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(250));
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
	coder->write_header(16, 30, 800, 800);
	GLubyte* background_img = SOIL_load_image("C:\\Users\\ikervazquezlopez\\Pictures\\Saved Pictures\\background.jpg", &tw, &th, 0, SOIL_LOAD_RGB);
	
	coder->write_background(background_img, tw * th * 3);


	Object_data data;
	Quad_data qdata;
	Texture_data tdata;
	std::vector<Object_data> objs;
	qdata = { 0,0, 200, 100 };
	tdata = { 0, 0, 256, 256 };
	data = { 0, qdata, tdata };
	objs.push_back(data);

	qdata = { 200, 100, 200,300 };
	tdata = { 256, 0, 476, 593 };
	data = { 1, qdata, tdata };
	objs.push_back(data);

	qdata = { 500, 150, 500,250 };
	tdata = { 0, 595, 660, 373 };
	data = { 2, qdata, tdata };
	objs.push_back(data);

	int taw = 1024;
	int tah = 1024;
	for (int i = 0; i < 16; i++) {
		std::stringstream filename;
		filename << "texture_atlas_" << i << ".png";
	
		
		GLubyte* atlas = SOIL_load_image(filename.str().c_str(), &taw, &tah, 0, SOIL_LOAD_RGB);
		if (atlas == nullptr)
			continue;
		coder->write_frame_metadata(objs);
		objs.at(0).quad.x += 10;
		objs.at(1).quad.w -= 10;
		objs.at(2).quad.y += 10;
		coder->write_frame_texture_atlas(atlas);
	}


	coder->close_file();
}



int main(int argc, char** argv) {

	coder = new Coder();
	code_video();
	decoder = new Decoder();
	decoder->open_file("test_video.16kc");
	decoder->read_header();
	GLubyte* background_img = decoder->read_background();
	
	background = nullptr;
	/*
	std::pair<std::vector<Object_data>, unsigned char*> p;
	for (int i = 0; i < 17; i++) {
		if (decoder->is_finised()) {
			std::cout << "File reading finshed." << std::endl;
			break;
		}
		p = decoder->read_frame();
	}
*/
	//GLubyte* tex = SOIL_load_image("C:\\Users\\ikervazquezlopez\\Pictures\\Saved Pictures\\NationalGeographic.jpg", &tw, &th, 0, SOIL_LOAD_RGB);
	//GLubyte* atlas = SOIL_load_image("C:\\Users\\ikervazquezlopez\\Documents\\Python Scripts\\2DbinPacking\\test_texture.jpg", &tw, &th, 0, SOIL_LOAD_RGB);


	initialize(decoder->get_width(), decoder->get_width(), argc, argv);
	//update_texture_atlas(atlas, 256, 256);
	
	background = new Background(660, 373, background_img);
	
	
	/*
	SpriteManager* manager = SpriteManager::get_instance();

	Sprite::Sprite_Data data;
	data = { 0, 0, 0, 0.5, 0.5, 0, 0, 1, 1 };
	Sprite s = manager->create_sprite(data);

	data = { 1, -1, -1, 0.5, 0.5, 0.25, 0.25, 0.75, 0.75 };
	s = manager->create_sprite(data);
	*/

	glutDisplayFunc(_render_scene);
	glutIdleFunc(_idle);
	glutKeyboardFunc(_keyboard);
	glutMainLoop();

	return 0;

}