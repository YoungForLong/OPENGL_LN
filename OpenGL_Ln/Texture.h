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
		Texture() = delete;
		explicit Texture(unsigned char* data);
		Texture(const std::initializer_list<unsigned char*>& dataList);
		virtual ~Texture();
		// void loadImg(const char* path);
		void clearTextureCache();
		virtual void tick(float dt);
		unsigned int mixNum() const { return _textureArr.size(); }
	private:
		void flushSingleImgIntoBuffer(unsigned char* data);
		void flushMixImgIntoBuffer(const std::initializer_list<unsigned char*>& pathList);
	private:
		unsigned int _id;
		unsigned char* _data;
		ImageObj _image;
		std::vector<GLuint> _textureArr;
	};
}
