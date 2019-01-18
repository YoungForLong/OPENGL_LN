#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <map>

#include "Singleton.h"
#include "IOUtils.h"
#include "AsyncLoader.h"
#include "Texture.h"

namespace OPENGL_LN
{
	class TextureMng : public Singleton<TextureMng>, public AsyncLoader<Texture>
	{
	public:
		TextureMng();
		~TextureMng();
		virtual void callBackHandleLoad(const void* any, const unsigned int id) override;
	};

}

#define TEXTUREMNG OPENGL_LN::TextureMng::instance()