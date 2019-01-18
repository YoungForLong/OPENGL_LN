#pragma once
#include <string>
#include <glad\glad.h>
#include <initializer_list>
#include <vector>

#define MAX_GL_TEXTURE_UNIT_NUM 32

namespace OPENGL_LN
{
	struct ImageObj
	{
		int _width;
		int _height;
		int _channelNum;
	};
	
	class Texture
	{
	public:
		Texture(const unsigned int id);
		explicit Texture(const unsigned int id, unsigned char* data, ImageObj* img);
		// Texture(const unsigned int id, const std::initializer_list<unsigned char*>& dataList);
		virtual ~Texture();
		// void loadImg(const char* path);
		
		void flushSingleImgIntoBuffer(unsigned char* data, const ImageObj* image);

		inline const GLuint getGLId() const { return _glId; }
	private:
		unsigned int _id;
		unsigned char* _data;
		ImageObj _image;
		GLuint _glId;
	};
}
