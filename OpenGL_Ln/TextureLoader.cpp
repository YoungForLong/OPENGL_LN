#include "TextureLoader.h"
#include "Texture.h"

unsigned int OPENGL_LN::TextureMng::initOneTexture(const char * filename)
{
	return 0;
}

void OPENGL_LN::TextureMng::asyncLoad(const char * filename, const unsigned int id)
{
	IOMNG->asyncLoad(filename, [this, &id](const void* any)->void {
		this->callBackHandleLoad(any, id);
	});
}

void OPENGL_LN::TextureMng::callBackHandleLoad(const void * any, const unsigned int id)
{
	const TransTexture* val = static_cast<const TransTexture*>(any);
	
}
