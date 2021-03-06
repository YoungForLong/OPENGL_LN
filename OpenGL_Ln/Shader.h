#pragma once

#include <glad\glad.h>
#include <string>
#include <glm\glm.hpp>

#define DEFAULT_V_SHADER "default_vertex_shader.glsl"
#define DEFAULT_F_SHADER "default_fragment_shader.glsl"
#define INVALID_SHADER_PROGRAM_ID -1
#define SET_VAL(a) (glUniform##a)

namespace OPENGL_LN
{
	class Shader
	{
	public:
		Shader(const char* vertexShaderFile = DEFAULT_V_SHADER, const char* fragmentShaderFile = DEFAULT_F_SHADER);
		~Shader();
		void setVal(const std::string& name, float value = 0.0f) const;
		void setVal(const std::string& name, int value = 0) const;
		void setVal(const std::string& name, glm::vec3 value) const;
		void setVal(const std::string& name, GLsizei count, const float* argv) const;
		void setTrans(const std::string& name, const float* matrix);

		void use();

		int getShaderId() const { return _shaderProgram; }
	private:
		const char* getFileData(const char * path);
		GLchar* _source;
		int _shaderProgram;
	};
}