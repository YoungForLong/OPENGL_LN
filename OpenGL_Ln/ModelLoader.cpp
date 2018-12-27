#include "ModelLoader.h"
#include "Model.h"

OPENGL_LN::ModelMng::ModelMng()
{

}

unsigned int OPENGL_LN::ModelMng::initOneModel(const char * filename)
{
	return 0;
}

OPENGL_LN::Model * OPENGL_LN::ModelMng::getModelByKey(unsigned int key)
{
	return nullptr;
}

void OPENGL_LN::ModelMng::asyncLoad(const char * filename, unsigned int id)
{
	IOMNG->asyncLoad(filename, [this, id](const void* any)->void {
		this->callBackHandleLoad(any, id);
	});
}

void OPENGL_LN::ModelMng::callBackHandleLoad(const void* any, unsigned int id)
{
	auto scene = static_cast<const aiScene*>(any);

}
