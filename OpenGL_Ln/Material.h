#pragma once
#include <glm\glm.hpp>
#include <string>

namespace OPENGL_LN
{
	class Shader;
	class Texture;
	class Material
	{
	public:
		Material(const std::string& refName, float shininess);
		virtual ~Material();
		void applyMaterial(Shader& sr);
		void bindDiffuseMap(const char* img);
		void bindSpecularMap(const char* img);
		virtual void tick(float dt);
	public:
		float _shininess;
	private:
		std::string _refName;
		Texture* _texture;
	};
}