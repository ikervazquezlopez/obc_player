#pragma once


#include "glew.h"
#include "freeglut.h"

#include <iostream>

class ShaderLoader
{
	private:
		std::string ReadShader(const char* filename);
		GLuint CreateShader(GLenum shaderType, std::string source, const char* shaderName);

	public:
		ShaderLoader(void);
		~ShaderLoader(void);
		GLuint CreateProgram(const char* vertex_shader_filename, const char* fragment_shader_filename);
};