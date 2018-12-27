#include "TextureLoader.h"
#include "Texture.h"

OPENGL_LN::TextureMng::~TextureMng()
{
}

unsigned int OPENGL_LN::TextureMng::initOneTexture(const char * filename)
{
	unsigned int textureId = genTextureId();
	Texture* texture = new Texture(textureId);

	{
		std::unique_lock<mutex> lock(_mut);
		_textureContainer.insert(std::make_pair(textureId, texture));
	}
	
	asyncLoad(filename, textureId);

	return textureId;
}

void OPENGL_LN::TextureMng::asyncLoad(const char * filename, const unsigned int id)
{
	IOMNG->asyncLoad(filename, [this, id](const void* any)->void {
		this->callBackHandleLoad(any, id);
	});
}

void OPENGL_LN::TextureMng::callBackHandleLoad(const void * any, const unsigned int id)
{
	const TransTexture* val = static_cast<const TransTexture*>(any);
	
	Texture* pOperate = NULL;

	{
		std::unique_lock<mutex> mut(this->_mut);
		auto iter = _textureContainer.find(id);
		if (iter != _textureContainer.end())
		{
			pOperate = _textureContainer[id];
		}
	}

	pOperate->flushSingleImgIntoBuffer(val->data, val->image);
}
