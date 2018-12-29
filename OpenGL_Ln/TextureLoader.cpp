#include "TextureLoader.h"
#include "Texture.h"

OPENGL_LN::TextureMng::TextureMng()
{
}

OPENGL_LN::TextureMng::~TextureMng()
{
}

void OPENGL_LN::TextureMng::callBackHandleLoad(const void * any, const unsigned int id)
{
	const TransTexture* val = static_cast<const TransTexture*>(any);
	
	Texture* pOperate = getObjbyKey(id);

	if (pOperate)
	{
		pOperate->flushSingleImgIntoBuffer(val->data, val->image);

		modifyLoadState(true);
	}
}
