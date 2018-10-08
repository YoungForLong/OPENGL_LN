#pragma once
#include <string>
#include <glad\glad.h>

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
	private:
		unsigned char* _data;
		ImageObj _image;
		GLuint _texture;
	};
}
