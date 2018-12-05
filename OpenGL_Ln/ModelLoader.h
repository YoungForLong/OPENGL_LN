#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <functional>
#include <unordered_map>

#include "Singleton.h"

#define MODELMNG ModelMng::instance()

namespace OPENGL_LN
{
	class Model;
	class Mesh;

	class ModelMng :public Singleton<ModelMng>
	{
	public:
		unsigned int initOneModel(const char* filename);
		Model* getModelByKey(unsigned int key);
	private:
		std::unordered_map<unsigned int, Model*> _modelContainer;
	};

}