#include "ModelLoader.h"
#include "Model.h"

OPENGL_LN::ModelMng::ModelMng()
{
}

OPENGL_LN::ModelMng::~ModelMng()
{
}

void OPENGL_LN::ModelMng::callBackHandleLoad(const void* any, unsigned int id)
{
	auto scene = static_cast<const aiScene*>(any);

	Model* pOperate = getObjbyKey(id);
	
	if (pOperate)
	{
		pOperate->flushSceneIntoModel(scene);
	}
}
