#include "Model.h"
#include "TextureLoader.h"

OPENGL_LN::Model::Model(unsigned int id):
	_id(id)
{
}

void OPENGL_LN::Model::render(Shader * shader)
{
	for (auto iter = _meshes.begin(); iter != _meshes.end(); ++iter)
	{
		iter->render(shader);
	}
}

void OPENGL_LN::Model::flushSceneIntoModel(const aiScene * scene)
{
	processNode(scene->mRootNode, scene);
}

void OPENGL_LN::Model::processNode(aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(Mesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}