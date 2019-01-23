#pragma once
#include <mutex>
#include <unordered_map>
#include <string>
#include "IOUtils.h"

namespace OPENGL_LN
{
	//thread-safe
	template<class __T>
	class AsyncLoader
	{
	public:
		AsyncLoader() { _loadFinished = true; _keyHash = 0; }
		
		virtual ~AsyncLoader() {}
		
		void modifyLoadState(bool finished);
		
		unsigned int initOneObj(const char* filename);

		__T* getObjbyKey(unsigned int key);

		virtual bool hasLoadFinished() const { return _loadFinished; }
		
		void asyncLoad(const char* filename, const unsigned int id);
		
		virtual void callBackHandleLoad(const void* any, const unsigned int id) = 0;
		
	protected:
		unsigned int genObjId() { return _keyHash++; }
	protected:
		std::unordered_map<unsigned int, __T*> _objContainer;
		STRING_HASH_MAP(unsigned int) _fileMap;
		std::atomic<bool> _loadFinished;
		std::mutex _mut;
		std::atomic<unsigned int> _keyHash;
	};

	template<class __T>
	inline void AsyncLoader<__T>::modifyLoadState(bool finished)
	{
		_loadFinished = finished;
	}

	template<class __T>
	inline unsigned int AsyncLoader<__T>::initOneObj(const char* filename)
	{
		std::unique_lock<mutex> lock(_mut);

		auto iter = _fileMap.find(filename);
		if (iter != _fileMap.end())
		{
			return iter->second;
		}
		unsigned int id = genObjId();
		__T* obj = new __T(id);

		_objContainer.insert(std::make_pair(id, obj));
		_fileMap.insert(std::make_pair(filename, id));

		lock.unlock();
		asyncLoad(filename, id);

		return id;
	}

	template<class __T>
	inline __T * AsyncLoader<__T>::getObjbyKey(unsigned int key)
	{
		std::unique_lock<std::mutex> lock(_mut);
		std::unordered_map<unsigned int, __T*>::iterator iter = _objContainer.find(key);
		if (iter != _objContainer.end())
		{
			return iter->second;
		}
	}

	template<class __T>
	inline void AsyncLoader<__T>::asyncLoad(const char * filename, const unsigned int id)
	{
		modifyLoadState(false);

		IOMNG->syncLoad(filename, [this, id](const void* any)->void {
			this->callBackHandleLoad(any, id);
		});
	}
}
