#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

namespace OPENGL_LN 
{
	class Model
	{
	public:
		Model();
		Model(const Model& other) = delete;
		const Model& operator=(const Model& other) = delete;
	private:
		void processNode(aiNode* node, const aiScene* scene);
		Mesh&& processMesh(aiMesh* mesh, const aiScene* scene);
	private:
		unsigned int _id;
		std::vector<Mesh> _meshes;
	};
}
