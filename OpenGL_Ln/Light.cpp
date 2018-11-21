#include "Light.h"
#include <iostream>
#include "LerpFuncs.h"
#include "Shader.h"


OPENGL_LN::Light::Light(const std::string& refName, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction):
	_refName(refName),
	_ambient(ambient),
	_diffuse(diffuse),
	_specular(specular),
	_direction(direction),
	_lightType(LIGHT_TYPE::DIRECTIONAL_LIGHT)
{
}

OPENGL_LN::Light::Light(const std::string & refName, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos, float constant, float linear, float quadratic):
	_refName(refName),
	_ambient(ambient),
	_diffuse(diffuse),
	_specular(specular),
	_pos(pos),
	_constant(constant),
	_linear(linear),
	_quadratic(quadratic),
	_lightType(LIGHT_TYPE::POINT_LIGHT)
{
}

OPENGL_LN::Light::Light(const std::string & refName, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos, glm::vec3 direction, float cutOff, float outerCutOff):
	_refName(refName),
	_ambient(ambient),
	_diffuse(diffuse),
	_specular(specular),
	_pos(pos),
	_direction(direction),
	_cutOff(cutOff),
	_outerCutOff(outerCutOff),
	_lightType(LIGHT_TYPE::SPOT_LIGHT)
{
}

void OPENGL_LN::Light::applyLight(Shader & compiler)
{
	compiler.setVal(_refName + ".ambient", _ambient);
	compiler.setVal(_refName + ".diffuse", _diffuse);
	compiler.setVal(_refName + ".specular", _specular);

	if(LIGHT_TYPE::DIRECTIONAL_LIGHT == _lightType)
	{
		compiler.setVal(_refName + ".direction", _direction);
	}
	else if(LIGHT_TYPE::POINT_LIGHT == _lightType)
	{
		compiler.setVal(_refName + ".position", _pos);
		compiler.setVal(_refName + ".constant", _constant);
		compiler.setVal(_refName + ".linear", _linear);
		compiler.setVal(_refName + ".quadratic", _quadratic);
	}
	else if (LIGHT_TYPE::SPOT_LIGHT == _lightType)
	{
		compiler.setVal(_refName + ".position", _pos);
		compiler.setVal(_refName + ".direction", _direction);
		compiler.setVal(_refName + ".cutOff", _cutOff);
		compiler.setVal(_refName + ".outerCutOff", _outerCutOff);
	}
}

unsigned int OPENGL_LN::Light::rgba2hex(const glm::vec4 rgbaColor)
{
	unsigned int valR = (unsigned int)floor(rgbaColor.r * 255) << 24;
	unsigned int valG = (unsigned int)floor(rgbaColor.g * 255) << 16;
	unsigned int valB = (unsigned int)floor(rgbaColor.b * 255) << 8;
	unsigned int valA = (unsigned int)floor(rgbaColor.a * 255);
	return valR + valG + valB + valA;
}

glm::vec4 OPENGL_LN::Light::hex2rgba(const unsigned int hexColor)
{
	unsigned int red = (hexColor & 0xff000000) >> 24;
	unsigned int green = (hexColor & 0x00ff0000) >> 16;
	unsigned int blue = (hexColor & 0x0000ff00) >> 8;
	unsigned int alpha = (hexColor & 0x000000ff);

	return glm::vec4(red, green, blue, alpha) / 255.0f;
}



