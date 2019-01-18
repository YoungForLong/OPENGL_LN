#pragma once
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>
#include <condition_variable>

#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

#include "Singleton.h"
#include "stb_image.h"

#define THREAD_NUM 10

struct const_char_cmp
{
	bool operator()(const char *s1, const char *s2) const
	{
		return std::strcmp(s1, s2) == 0;
	}

};

struct const_char_hash_func
{
	size_t operator()(const char *str) const
	{
		int strLength = 0;

		for (; *(str + strLength) != '\0'; ++strLength);

		return std::_Hash_seq((unsigned char *)str, strLength);
	}
};

#define STRING_HASH_MAP(VAL_TYPE) std::unordered_map<const char*, VAL_TYPE, const_char_hash_func, const_char_cmp>

namespace OPENGL_LN {
	typedef function<void(const void*)> IOCallBack;

	enum IOType
	{
		IO_TEXTURE = 0x0001,
		IO_MODEL = 0x0002
	};

	struct ImageObj;
	struct TransTexture
	{
		stbi_uc* data;
		ImageObj* image;
	};

	struct IOObject
	{
		const char* file_path;
		IOType type;
		IOCallBack callback;
	};

	class IOUtils :public Singleton<IOUtils>
	{
	public:
		IOUtils();
		~IOUtils();
	public:
		string getFilePath(const char* filename);
		void asyncLoad(const char* filename, IOCallBack&& cb);
		IOType judgeFileType(const char* filename);
		int fileHash(const char* filename);
	protected:
		IOObject* dequeue();
	private:
		mutex _queueMut;
		condition_variable _cond;
		queue<IOObject*> _ioQueue;
		STRING_HASH_MAP(bool) _cachedObjMap;
		bool _activeSignal;
		bool _doneSignal;

		// put thread bottom
		thread _threadPool[THREAD_NUM];
	};
}

#define IOMNG OPENGL_LN::IOUtils::instance()