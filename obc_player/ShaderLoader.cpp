#include <iostream>
#include<fstream>

#include "ShaderLoader.h"


std::string ShaderLoader::ReadShader(const char* filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	if (!file.good())
	{
		std::cout << "Cannot read the file " << filename << std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();

	return shaderCode;
}

GLuint ShaderLoader::CreateShader(GLenum shaderType, std::string source, const char* shaderName)
{
	return GLuint();
}
