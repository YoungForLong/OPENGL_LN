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

void OPENGL_LN::ModelMng::asyncLoad(const char * filename)
{
	auto cb = std::bind(this, &OPENGL_LN::ModelMng::callBackHandleLoad);
	IOMNG->asyncLoad(filename, std::move(cb));
}

void OPENGL_LN::ModelMng::callBackHandleLoad(const void* any)
{
	auto scene = static_cast<const aiScene*>(any);

}
