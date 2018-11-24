#pragma once
#include <thread>
#include <mutex>
#include <functional>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "Singleton.h"


namespace OPENGL_LN {
	class IOUtils :public Singleton<IOUtils>
	{
	public:
		const char* getFilePath(const char* filename);
		void asyncLoad()
		{

		}
	private:
		vector<mutex> _mutPool;
	};
}
