#include "ShaderLoader.h"
#include <iostream>
#include <fstream>
#include <vector>


ShaderLoader::ShaderLoader(void)
{
};

ShaderLoader::~ShaderLoader(void)
{
};


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

	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char* shaderCode_ptr = source.c_str();
	const int shaderCode_size = source.size();

	glShaderSource(shader, 1, &shaderCode_ptr, &shaderCode_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	// check for errors
	if (compile_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		std::cout << "ERROR compiling the shader: " << shaderName << std::endl << &shader_log[0] << std::endl;

		return 0;
	}

	return shader;

}


GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);

	std::string vertexShader_name = "vertex shader";
	std::string fragmentShader_name = "fragment shader";

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, vertexShader_name.c_str());
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, fragmentShader_name.c_str());

	int link_result = 0;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);

	if (link_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;

		return 0;
	}

	return program;
}






