#pragma once
#include <glm\glm.hpp>
#include <unordered_map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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
		Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<unsigned int>&& diffuseMaps, std::vector<unsigned int>&& specularMaps, std::vector<unsigned int>&& heightMaps, std::vector<unsigned int>&& normalMaps);
		Mesh(aiMesh* mesh, const aiScene* scene);
		Mesh(const Mesh& other) = delete;
		const Mesh& operator=(const Mesh& other) = delete;
		Mesh(Mesh&& other);

		void render(Shader* shader);

		// virtual void clone(Mesh& dest, Mesh& src);
		
		void loadTextures(aiMaterial* mat, aiTextureType type, std::vector<unsigned int>& container);
	protected:
		virtual void init();
		unsigned int VAO, VBO, EBO;
	public:
		std::vector<Vertex> vertices_;
		std::vector<unsigned int> indices_;
		std::vector<unsigned int> diffuseMaps_;
		std::vector<unsigned int> specularMaps_;
		std::vector<unsigned int> heightMaps_;
		std::vector<unsigned int> normalMaps_;
	};
}