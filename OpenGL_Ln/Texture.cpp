#include "Texture.h"
#include "stb_image.h"
#include <iostream>

OPENGL_LN::Texture::Texture(const unsigned int id):
	_id(id),
	_data(NULL)
{
}

OPENGL_LN::Texture::Texture(const unsigned int id, unsigned char * data, ImageObj* img):
	_id(id),
	_data(NULL)
{
	this->flushSingleImgIntoBuffer(data, img);
}

//OPENGL_LN::Texture::Texture(const unsigned int id, const std::initializer_list<unsigned char*>& dataList):
//	_id(id),
//	_data(NULL)
//{
//	this->flushMixImgIntoBuffer(dataList);
//}

OPENGL_LN::Texture::~Texture()
{
	stbi_image_free(_data);
}


void OPENGL_LN::Texture::flushSingleImgIntoBuffer(unsigned char* data, const ImageObj* image)
{
	_image._width = image->_width;
	_image._height = image->_height;
	_image._channelNum = image->_channelNum;

	GLuint texture;
	glGenTextures(1, &texture);

	_data = data;
	if (_data)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		auto color = _image._channelNum == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, color, _image._width, _image._height, 0, color, GL_UNSIGNED_BYTE, _data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	_glId = texture;
}
