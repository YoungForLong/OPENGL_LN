#pragma once
#include <glm\glm.hpp>
#include <unordered_map>
#include <vector>
#include "Shader.h"

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat4 mat4;

namespace OPENGL_LN
{
	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 texCoords;
	};

	class Texture;

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Texture>& textures);

		void render(Shader* shader);

	public:
		std::vector<Vertex> vertices_;
		std::vector<unsigned int> indices_;
		std::vector<Texture> textures_;
	};
}