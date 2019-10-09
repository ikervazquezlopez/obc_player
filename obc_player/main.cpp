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



void Init()
{
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 500);//optional
	glutInitWindowSize(800, 600); //optional
	glutCreateWindow("OBC player");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glewInit();

	Init();

	//cv::Mat tex;
	int w = 256;
	int h = 256;
	//unsigned char* tex = SOIL_load_image("C:\\Users\\ikervazquezlopez\\Pictures\\Saved Pictures\\NationalGeographic_1.jpg", &w, &h, 0, SOIL_LOAD_RGB);
	GLubyte tex[] = { 255, 0, 0,
						0, 255, 0,
						0, 0, 255,
						255, 255, 255
							};
	manager = SpriteManager::get_instnce();
	manager->create_sprite(0, 0.0, 0.0, 0.5, 0.5, tex, 2, 2);

	// register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}


