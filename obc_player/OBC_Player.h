#pragma once
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
#include "Background.h"
#include "ShaderLoader.h"
#include "Coder.h"
#include "Decoder.h"


bool play_flag;

GLuint shader_program;

GLint M_t_location;
GLint M_s_location;
GLint current_matrix_location;
GLint texture_location;

GLuint texture_atlas;

SpriteManager* manager;

Background* background;

int tw = 660;
int th = 373;

Coder *coder;
Decoder *decoder;


void _render_scene(void);
void _idle(void);
void _keyboard(unsigned char c, int x, int y);
void _init();

void initialize(int width, int heigth, int argc, char** argv);


void play();
void stop();

void draw_background();
void draw_sprite(Sprite s);

Background * get_background();
void set_background(Background * b);
void update_texture_atlas(unsigned char* texture_atlas, int tw, int th);





