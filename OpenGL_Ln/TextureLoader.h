#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <map>

#include "Singleton.h"
#include "IOUtils.h"
#include "AsyncLoader.h"

#define TEXTUREMNG OPENGL_LN::TextureMng::instance()

namespace OPENGL_LN
{
	class Texture;

	class TextureMng : public Singleton<TextureMng>, public AsyncLoader<Texture>
	{
	private:
		TextureMng();
		~TextureMng();
	public:
		virtual void callBackHandleLoad(const void* any, const unsigned int id) override;
	};

}