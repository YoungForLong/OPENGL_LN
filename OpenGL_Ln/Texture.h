#pragma once
#include <string>
#include <glad\glad.h>
#include <initializer_list>
#include <vector>

namespace OPENGL_LN
{
	typedef struct ImageObj
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
		void clearTextureCache();
		virtual void tick(float dt);
		inline unsigned int mixNum() const { return _textureArr.size(); }
		void flushSingleImgIntoBuffer(unsigned char* data, const ImageObj* image);
	private:
		unsigned int _id;
		unsigned char* _data;
		ImageObj _image;
		std::vector<GLuint> _textureArr;
	};
}
