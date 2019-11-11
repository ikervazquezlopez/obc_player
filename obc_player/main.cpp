#include "glew.h"
#include "freeglut.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <stdio.h> 
#include <opencv2/core.hpp>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <math.h>

#include "SpriteManager.h"
#include "Sprite.h"

#define PI 3.1415

SpriteManager* manager;

float m = 0.0f;
float delta = 0.01;

int tw = 660;
int th = 373;
GLubyte* n_tex;


void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	manager->draw_sprites();

	glutSwapBuffers();
}

void idle(void)
{

	Sprite* s = manager->get_sprite(0);
	float r = 0.5;
	float ny = std::sin(m)/2;
	float nx = std::cos(m)/2;
	float x = std::sqrtf(r * r - ny * ny);
	float y = std::sqrtf(r * r - nx * nx);
	float dx = x - s->get_x();
	float dy = y - s->get_y();
	
	if (m > 1.0) {
		manager->update_sprite(s->get_id(), s->get_x() + dx, s->get_y() + dy, s->get_w(), s->get_h(), tw, th, n_tex);
	}
	else {
		manager->update_sprite(s->get_id(), s->get_x() + dx, s->get_y() + dy, s->get_w(), s->get_h(), tw, th, nullptr);
		m += delta;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char c, int x, int y)
{

}



void Init()
{
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 500);
	glutInitWindowSize(800, 600); 
	glutCreateWindow("OBC player");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glewInit();

	Init();

	
	GLubyte * tex = SOIL_load_image("C:\\Users\\ikervazquezlopez\\Pictures\\Saved Pictures\\NationalGeographic.jpg", &tw, &th, 0, SOIL_LOAD_RGB);
	n_tex = SOIL_load_image("C:\\Users\\ikervazquezlopez\\Pictures\\Saved Pictures\\Jump.jpg", &tw, &th, 0, SOIL_LOAD_RGB);
	
	manager = SpriteManager::get_instnce();
	Sprite s;
	Sprite background = manager->create_sprite_background(tw, th, tex);
	s = manager->create_sprite(0, 0.0, 0.0, 0.5, 0.5, tex, tw, th);
	s = manager->create_sprite(1, -1, -1, 0.5, 0.5, tex, tw, th);

	// register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}


