#include "glew.h"
#include "freeglut.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <stdio.h> 
#include <opencv2/core.hpp>
#include <SOIL.h>

#include "SpriteManager.h"
#include "Sprite.h"


SpriteManager* manager;


void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	manager->draw_sprites();

	glutSwapBuffers();
}

void idle(void)
{
	glutPostRedisplay();
}

void keyboard(unsigned char c, int x, int y)
{
	Sprite* s = manager->get_sprite(1);
	manager->update_sprite((*s).get_id(), (*s).get_x() + 0.1, (*s).get_y(), (*s).get_w(), (*s).get_h(), nullptr);
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

	int w = 660;
	int h = 373;
	GLubyte * tex = SOIL_load_image("C:\\Users\\ikervazquezlopez\\Pictures\\Saved Pictures\\NationalGeographic.jpg", &w, &h, 0, SOIL_LOAD_RGB);
	
	manager = SpriteManager::get_instnce();
	manager->create_sprite_background(w, h, tex);
	manager->create_sprite(0, 0.0, 0.0, 0.5, 0.5, tex, w, h);
	manager->create_sprite(1, -1, -1, 0.5, 0.5, tex, w, h);

	// register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}


