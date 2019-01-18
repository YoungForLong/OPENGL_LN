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
#include "Model.h"

namespace OPENGL_LN
{
	class ModelMng :public Singleton<ModelMng>, public AsyncLoader<Model>
	{
	public:
		ModelMng();
		~ModelMng();
		virtual void callBackHandleLoad(const void* any, unsigned int id) override;
	};

}

#define MODELMNG OPENGL_LN::ModelMng::instance()