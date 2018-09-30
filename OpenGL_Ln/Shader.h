#pragma once

#include <glad\glad.h>
#include <string>

#define DEFAULT_V_SHADER "default_vertex_shader.glsl"
#define DEFAULT_F_SHADER "default_fragment_shader.glsl"
#define INVALID_SHADER_PROGRAM_ID -1
#define SET_VAL(a) (glUniform##a)

class Shader
{
public:
	Shader(const char* vertexShaderFile = DEFAULT_V_SHADER, const char* fragmentShaderFile = DEFAULT_F_SHADER);

	void setVal(const std::string& name, float value = 0.0f) const;
	void setVal(const std::string& name, int value = 0) const;
	void setVal(const std::string& name, GLsizei count, const float* argv) const;

	
private:
	const char* getFileData(const char * path);
	
	int _shaderProgram;
};