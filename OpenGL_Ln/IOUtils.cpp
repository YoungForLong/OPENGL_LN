#include "IOUtils.h"
#include "stb_image.h"
#include <assert.h>
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>


#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef MAX_POSTFIX
#define MAX_POSTFIX 20
#endif

using namespace OPENGL_LN;

OPENGL_LN::IOUtils::IOUtils()
{
	_queueMut.lock();
	_activeSignal = false;
	_doneSignal = false;
	_queueMut.unlock();

	for (int i = 0; i < THREAD_NUM; ++i)
	{
		_threadPool[i] = thread([this, i]() {
			while (!_doneSignal)
			{
				unique_lock<mutex> mut(this->_queueMut);
				while (!this->_activeSignal)
				{
					this->_cond.wait(mut);
				}

				auto* obj = this->dequeue();
				if (obj == nullptr)
				{
					this->_activeSignal = false;
					continue;
				}

				mut.unlock();
				if (obj->type == IOType::IO_MODEL)
				{
					Assimp::Importer importer;
					auto&& filepath = this->getFilePath(obj->file_path);
					const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
					if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
					{
						auto errStr = importer.GetErrorString();
						std::cout << errStr << endl;
						assert(0 && "Err: ASSIMP::MODEL::LOAD::FAILED");
					}
					obj->callback(static_cast<const void*>(scene));
				}
				else if (obj->type == IOType::IO_TEXTURE)
				{
					auto&& real_path = getFilePath(obj->file_path);
					OPENGL_LN::ImageObj* image = new OPENGL_LN::ImageObj;
					stbi_uc* data = stbi_load(real_path.c_str(), &(image->_width), &(image->_height), &(image->_channelNum), 0);
					if (!data)
					{
						std::cout << "filename is: " << real_path << endl;
						assert(0 && "Err: STB::IMAGE::LOAD::FAILED");
					}
					TransTexture* t = new TransTexture();
					t->data = data;
					t->image = image;
					obj->callback(static_cast<const void*>(t));
					stbi_image_free(data);
					delete image;
					image = NULL;
					t->image = NULL;
					delete t;
					t = NULL;
				}
			}
		});
	}
}

OPENGL_LN::IOUtils::~IOUtils()
{
	unique_lock<mutex> mut(_queueMut);
	_doneSignal = true;
	_cond.notify_all();
	for (auto& th : _threadPool)
		th.join();
}



string IOUtils::getFilePath(const char * filename)
{
	string ret = string("../Resources/") + filename;
	return ret;
}

void OPENGL_LN::IOUtils::asyncLoad(const char * filename, IOCallBack&& cb)
{
	IOType type = this->judgeFileType(filename);
	IOObject* obj = new IOObject;
	obj->type = type;
	obj->callback = cb;
	obj->file_path = filename;

	unique_lock<mutex> mut(this->_queueMut);
	if (_cachedObjMap.find(filename) == _cachedObjMap.end() || _cachedObjMap[filename] != true)
	{
		_cachedObjMap.insert(make_pair(filename, true));
		_ioQueue.push(obj);
		this->_activeSignal = true;
		_cond.notify_one();
	}
}

OPENGL_LN::IOType OPENGL_LN::IOUtils::judgeFileType(const char * filename)
{
	size_t op = 0;
	string postfix;
	bool find = false;
	size_t nameLen = strlen(filename);
	while (op < nameLen)
	{
		if (filename[++op] == '.')
		{
			find = true;
			continue;
		}
		if (find && filename[op] != '\0')
		{
			postfix.push_back(filename[op]);
		}
	}
	
	STRING_HASH_MAP(IOType) fileMap =
	{
		{"png", IO_TEXTURE},
		{"jpg", IO_TEXTURE},
		{"obj", IO_MODEL}
	};
	
	return fileMap[postfix.c_str()];
}

int OPENGL_LN::IOUtils::fileHash(const char * filename)
{
	int totalAscii = 0;
	for (int i = 0; i < strlen(filename); ++i)
	{
		if (*(filename + i) != '\0')
		{
			totalAscii += *(filename + i);
		}
	}

	return max(totalAscii % THREAD_NUM, 0);
}

IOObject * OPENGL_LN::IOUtils::dequeue()
{
	if (!_ioQueue.empty())
	{
		auto ret = _ioQueue.front();
		_ioQueue.pop();
		return ret;
	}

	return nullptr;
}
