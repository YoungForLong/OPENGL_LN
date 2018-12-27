#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <mutex>

#include "Singleton.h"
#include "IOUtils.h"
#include "AsyncLoadObject.h"


namespace OPENGL_LN
{
	class Texture;

	class TextureMng : public Singleton<TextureMng>, public AsyncLoadObject
	{
	private:
		TextureMng();
		~TextureMng();
	public:
		unsigned int initOneTexture(const char* filename);
		virtual void asyncLoad(const char* filename, const unsigned int id) override;
		virtual void callBackHandleLoad(const void* any, const unsigned int id) override;
	protected:
		unsigned int genTextureId() { return _keyHash++; }
	private:
		unordered_map<unsigned int, Texture*> _textureContainer;
		mutex _mut;
		unsigned int _keyHash;
	};
}