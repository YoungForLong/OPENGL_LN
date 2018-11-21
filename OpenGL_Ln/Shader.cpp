#include "Shader.h"
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>


OPENGL_LN::Shader::Shader(const char * vertexShaderFile, const char * fragmentShaderFile):
	_shaderProgram(-1),
	_source(NULL)
{
	const GLchar* vertexShaderSourec;
	const GLchar* fragmentShaderSource;
	vertexShaderSourec = getFileData(vertexShaderFile);
	fragmentShaderSource = getFileData(fragmentShaderFile);

	GLint succ;

	// 创建顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourec, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		char info[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << "Err: SHADER::VERTEX::COMPILATION_FAILED\n" << info << std::endl;
	}

	// 创建片元着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		char info[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		std::cout << "Err: SHADER::FRAGMENT::COMPILATION_FAILED\n" << info << std::endl;
	}

	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);
	glLinkProgram(_shaderProgram);
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &succ);
	if (!succ)
	{
		char info[512];
		glGetProgramInfoLog(_shaderProgram, 512, NULL, info);
		std::cout << "Err: PROGRAM::SHADER::LINK_FAILED\n" << info << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

OPENGL_LN::Shader::~Shader()
{
	if (_source)
		delete(_source);
}

void OPENGL_LN::Shader::setVal(const std::string & name, float value) const
{
	SET_VAL(1f)(glGetUniformLocation(_shaderProgram, name.c_str()), value);
}

void OPENGL_LN::Shader::setVal(const std::string & name, int value) const
{
	SET_VAL(1i)(glGetUniformLocation(_shaderProgram, name.c_str()), value);
}

void OPENGL_LN::Shader::setVal(const std::string & name, glm::vec3 value) const
{
	SET_VAL(3f)(glGetUniformLocation(_shaderProgram, name.c_str()), value.x, value.y, value.z);
}

void OPENGL_LN::Shader::setVal(const std::string & name, GLsizei count, const float * argv) const
{
	SET_VAL(1fv)(glGetUniformLocation(_shaderProgram, name.c_str()), count, argv);
}



void OPENGL_LN::Shader::setTrans(const std::string & name, const float * matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, name.c_str()), 1, GL_FALSE, matrix);
}

void OPENGL_LN::Shader::use()
{
	glUseProgram(_shaderProgram);
}

const char * OPENGL_LN::Shader::getFileData(const char * path)
{
	char real_path[MAX_PATH] = { 0 };
	strcpy_s(real_path, path);
#ifdef WIN32
	memset(real_path, 0, MAX_PATH);
	const char* prefix = "../Resources/";
	strcpy_s(real_path, prefix);
	strcat_s(real_path, path);
#endif
	FILE* infile;
	fopen_s(&infile, real_path, "rb");
	if (!infile)
	{
		return NULL;
	}
	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	_source = new GLchar[len + 1];
	fread(_source, 1, len, infile);
	fclose(infile);
	_source[len] = 0;
	return const_cast<const GLchar *>(_source);
}