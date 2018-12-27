#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Singleton.h"
#include "IOUtils.h"
#include "AsyncLoadObject.h"

#define MODELMNG ModelMng::instance()

namespace OPENGL_LN
{
	class Model;
	class Mesh;

	class ModelMng :public Singleton<ModelMng>, public AsyncLoadObject
	{
	private:
		ModelMng();
	public:
		unsigned int initOneModel(const char* filename);
		Model* getModelByKey(unsigned int key);
		virtual void asyncLoad(const char* filename, unsigned int id) override;
		virtual void callBackHandleLoad(const void* any, unsigned int id) override;
		unsigned int genTextureId() { return _keyHash++; }
	private:
		std::unordered_map<unsigned int, Model*> _modelContainer;
		unsigned int _keyHash;
	};

}