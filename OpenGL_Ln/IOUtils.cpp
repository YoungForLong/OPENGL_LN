#include "IOUtils.h"
#include "stb_image.h"
#include <assert.h>
#include "Texture.h"


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
				if (obj != nullptr)
				{
					this->_activeSignal = false;
					continue;
				}

				mut.unlock();
				if (obj->type == IOType::Model)
				{
					Assimp::Importer importer;

					const aiScene* scene = importer.ReadFile(this->getFilePath(obj->file_path.c_str()),
						aiProcess_Triangulate | aiProcess_FlipUVs);
					if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
					{
						auto errStr = importer.GetErrorString();
						assert(0 && "Err: ASSIMP::MODEL::LOAD::FAILED");
					}
					obj->callback(static_cast<const void*>(scene));
				}
				else if (obj->type == IOType::Texture)
				{
					auto real_path = getFilePath(obj->file_path.c_str());
					OPENGL_LN::ImageObj image;
					stbi_uc* data = stbi_load(real_path, &(image._width), &(image._height), &(image._channelNum), 0);
					if (!data)
					{
						assert(0 && "Err: STB::IMAGE::LOAD::FAILED");
					}
					TransTexture* t = new TransTexture();
					t->data = data;
					t->image = image;
					obj->callback(static_cast<const void*>(t));
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



const char * IOUtils::getFilePath(const char * filename)
{
	char real_path[MAX_PATH] = { 0 };
	strcpy_s(real_path, filename);
#ifdef WIN32
	memset(real_path, 0, MAX_PATH);
	const char* prefix = "../Resources/";
	strcpy_s(real_path, prefix);
	strcat_s(real_path, filename);
#endif
	return real_path;
}

void OPENGL_LN::IOUtils::asyncLoad(const char * filename, IOCallBack cb)
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
	int op = -1;
	string postfix;
	bool find = false;
	while (op < strlen(filename))
	{
		op++;
		if (filename[op] == '.')
		{
			find = true;
			continue;
		}
		if (find)
		{
			postfix.push_back(filename[op]);
		}
	}

	unordered_map<string, IOType> fileMap = 
	{
		{"png", Texture},
		{"jpg", Texture},
		{"obj", Model}
	};

	return fileMap[postfix];
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
