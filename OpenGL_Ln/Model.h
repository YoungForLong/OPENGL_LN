#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

namespace OPENGL_LN 
{
	// clone is illegal
	class Model
	{
	public:
		Model(unsigned int id);
		Model(const Model& other) = delete;
		const Model& operator=(const Model& other) = delete;

		void flushSceneIntoModel(const aiScene* scene);
	private:
		void processNode(aiNode* node, const aiScene* scene);
		Mesh&& processMesh(aiMesh* mesh, const aiScene* scene);
		void loadTextures(aiMaterial* mat, aiTextureType type);
	private:
		unsigned int _id;
		std::vector<Mesh> _meshes;
	};
}
