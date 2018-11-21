#pragma once
#include <glm\glm.hpp>
#include <string>
#define DEFAULT_AMBIENT (0.1f)

namespace OPENGL_LN 
{
	class Shader;
	enum LIGHT_TYPE
	{
		SPOT_LIGHT = 0x0001, // 聚光灯
		DIRECTIONAL_LIGHT = 0x0002, // 平行光
		POINT_LIGHT = 0x0003 // 点光
	};
	class Light
	{
	public:
		// direction light ctor
		Light(const std::string& refName, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);

		// point light ctor
		Light(const std::string& refName, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos, float constant, float linear, float quadratic);

		// spot light ctor
		Light(const std::string& refName, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos, glm::vec3 direction, float cutOff, float outerCutOff);

		void applyLight(Shader& compiler);
	public:
		static unsigned int rgba2hex(const glm::vec4 argbColor);
		static glm::vec4 hex2rgba(const unsigned int hexColor);

	private:
#pragma region common
		LIGHT_TYPE _lightType;
		glm::vec3 _ambient;
		glm::vec3 _diffuse;
		glm::vec3 _specular;
		std::string _refName;
		glm::vec3 _pos;
		glm::vec3 _direction;
#pragma endregion
		
		
#pragma region point_light_only
		float _constant;
		float _linear;
		float _quadratic;
#pragma endregion


#pragma region directional_light_only
		
#pragma endregion


#pragma region spot_light_only
		float _cutOff;
		float _outerCutOff;
#pragma endregion


	};
}