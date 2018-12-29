#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <mutex>

#include "Singleton.h"
#include "IOUtils.h"
#include "AsyncLoader.h"

#define MODELMNG ModelMng::instance()

namespace OPENGL_LN
{
	class Model;
	class Mesh;

	class ModelMng :public Singleton<ModelMng>, public AsyncLoader<Model>
	{
	private:
		ModelMng();
		~ModelMng();
	public:
		virtual void callBackHandleLoad(const void* any, unsigned int id) override;
	};

}