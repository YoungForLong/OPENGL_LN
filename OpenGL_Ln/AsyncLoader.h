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
		AsyncLoader() { _loadFinished = true; }
		
		virtual ~AsyncLoader() {}
		
		void modifyLoadState(bool finished);
		
		unsigned int initOneModel(const string& filename);

		__T* getObjbyKey(unsigned int key);

		bool hasLoadFinished() const { return _loadFinished; }
		
		void asyncLoad(const char* filename, const unsigned int id);
		
		virtual void callBackHandleLoad(const void* any, const unsigned int id) = 0;
		
	protected:
		unsigned int genObjId() { return _keyHash++; }
	protected:
		std::unordered_map<unsigned int, __T*> _objContainer;
		std::unordered_map<std::string, unsigned int> _fileMap;
		bool _loadFinished;
		std::mutex _mut;
		std::atomic<unsigned int> _keyHash;
	};

	template<class __T>
	inline void AsyncLoader<__T>::modifyLoadState(bool finished)
	{
		std::unique_lock<mutex> lock(_mut);
		_loadFinished = true;
	}

	template<class __T>
	inline unsigned int AsyncLoader<__T>::initOneModel(const string & filename)
	{
		std::unique_lock<mutex> lock(_mut);

		std::unordered_map<unsigned int, __T*>::iterator  iter = _fileMap.find(filename);
		if (iter != _fileMap.end())
		{
			return iter->second;
		}
		unsigned int id = genObjId();
		__T* obj = new __T(id);

		_textureContainer.insert(std::make_pair(id, obj));
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

		IOMNG->asyncLoad(filename, [this, id](const void* any)->void {
			this->callBackHandleLoad(any, id);
		});
	}
}
