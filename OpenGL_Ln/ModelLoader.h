#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <functional>
#include <unordered_map>

#include "Singleton.h"

typedef unsigned int ModelKey;

#define MODELMNG ModelMng::instance()

namespace OPENGL_LN
{
	class Model;
	class Mesh;

	class ModelMng :public Singleton<ModelMng>
	{
	public:
		ModelKey initOneModel();
		Model* getModelByKey(ModelKey key);
	private:
		std::unordered_map<ModelKey, Model*> _modelContainer;
	};


	class ModelLoader
	{
	public:
		ModelLoader() = delete;
		~ModelLoader() = delete;
		void operator()(const char* path, const LoadCallBack& succCall)
		{
			char real_path[MAX_PATH] = { 0 };
			strcpy_s(real_path, path);
#ifdef WIN32
			memset(real_path, 0, MAX_PATH);
			const char* prefix = "../Resources/";
			strcpy_s(real_path, prefix);
			strcat_s(real_path, path);
#endif
			Assimp::Importer importer;
			auto pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		}

	};

}