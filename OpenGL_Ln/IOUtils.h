#pragma once
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>
#include <condition_variable>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

#include "Singleton.h"
#define THREAD_NUM 10

namespace OPENGL_LN {
	typedef function<void(const void*)> IOCallBack;

	enum IOType
	{
		Texture = 0x0001,
		Model = 0x0002
	};

	struct ImageObj;
	typedef struct
	{
		stbi_uc* data;
		ImageObj image;
	} TransTexture;

	typedef struct 
	{
		string file_path;
		IOType type;
		IOCallBack callback;
	} IOObject;

	class IOUtils :public Singleton<IOUtils>
	{
	private:
		IOUtils();
		~IOUtils();
	public:
		const char* getFilePath(const char* filename);
		void asyncLoad(const char* filename, IOCallBack cb);
		IOType judgeFileType(const char* filename);
		int fileHash(const char* filename);
	protected:
		IOObject* dequeue();
	private:
		mutex _queueMut;
		condition_variable _cond;
		queue<IOObject*> _ioQueue;
		unordered_map<string, bool> _cachedObjMap;
		bool _activeSignal;
		bool _doneSignal;

		// put thread bottom
		thread _threadPool[THREAD_NUM];
	};
}
