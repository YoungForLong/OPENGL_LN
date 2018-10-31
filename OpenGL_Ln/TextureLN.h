#pragma once
#include <string>
#include <glad\glad.h>
#include <initializer_list>
#include <vector>

namespace OPENGL_LN
{
	typedef struct
	{
		int _width;
		int _height;
		int _channelNum;
	}ImageObj;
	
	class TextureLN
	{
	public:
		TextureLN();
		void loadImg(const char* path);
		void flushSingleImgIntoBuffer(const char* path);
		void flushMixImgIntoBuffer(const std::initializer_list<std::string> pathList);
		virtual void tick(float dt);
		void clearTextureCache();
		unsigned int mixNum() const { return _textureArr.size(); }
	private:
		unsigned char* _data;
		ImageObj _image;
		std::vector<GLuint> _textureArr;
	};
}
